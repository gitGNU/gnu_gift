#include <iostream>
#include "CDebuggingMemoryManager.h"
#include <assert.h>
#include <cstdlib>


CDebuggingMemoryManager::CDebuggingMemoryManager(const CDebuggingMemoryManagerSize inSize):
  mBuffer(new lTChunk[inSize / sizeof(lTChunk) + 2]),
  mFreeList(mBuffer),
  mUsedList(mBuffer+1),
  cMagic(0x1),
  cUnMagic(0x2)
{
  cVM=0x88414004;

  if(mBuffer)
    {
      mFreeList=mBuffer;
      mUsedList=mBuffer + 1;
      mFreeList->mPrev=0;
      mFreeList->mNext=mBuffer+2;
      mFreeList->mPreceding=0;
      mFreeList->mFollowing=mBuffer+1;
      mFreeList->mSize=0;
      mFreeList->mMagic=cUnMagic;

      mUsedList->mPrev=0;
      mUsedList->mNext=0;
      mUsedList->mPreceding=mBuffer;
      mUsedList->mFollowing=mBuffer+2;
      mUsedList->mSize=0;
      mUsedList->mMagic=cMagic;

      lTChunk* lFreeChunk=mBuffer+2;
      lTChunk* lLastNode=mBuffer+(inSize / sizeof(lTChunk));

      lFreeChunk->mPrev=mBuffer;
      lFreeChunk->mNext=0;
      lFreeChunk->mPreceding=mBuffer+1;
      lFreeChunk->mSize=inSize - sizeof(lTChunk)*3;
      lFreeChunk->mFollowing=lLastNode;
      lFreeChunk->mMagic=cUnMagic;

      ///A last node, which marks the end for the Chunklist
      ///The other lists need no end node
      lLastNode->mPrev=0;
      lLastNode->mNext=0;
      lLastNode->mPreceding=mBuffer+2;
      lLastNode->mSize=0;
      lLastNode->mFollowing=lLastNode;
      lLastNode->mMagic=cMagic;

    }
};


void* CDebuggingMemoryManager::getMem( CDebuggingMemoryManagerSize inSize){
  mMutex.lock();
  //inSize+=8;

  //Look for first free element of the right size;
  lTChunk* lCurrent= mFreeList;

  //For sake of cleanliness.
  //The number of Records for the strucure and the memblock
  CDebuggingMemoryManagerSize lNumRecords= (inSize + sizeof(lTChunk))/sizeof(lTChunk)+1;
  //the corresponding size
  CDebuggingMemoryManagerSize lSize= lNumRecords * sizeof(lTChunk) ;

  while(lCurrent && (lCurrent->mSize < lSize))
    lCurrent= lCurrent->mNext;

  if(lCurrent){

    assert(lCurrent->mMagic== cUnMagic);
    assert(lCurrent->mSize >= lSize);

    //Cut the chunk in two pieces

    lTChunk lNewChunk;

    //the preceding chunk of a free chunk is always occupied
    //and  free chunk has always a preceding node:
    lNewChunk.mPrev= lCurrent->mPreceding;
    lNewChunk.mNext= lNewChunk.mPrev->mNext;
    lNewChunk.mPreceding=lCurrent;
    lNewChunk.mFollowing=lCurrent->mFollowing;
    lNewChunk.mSize= lSize - sizeof(lTChunk);
    lNewChunk.mMagic=cMagic;

    CDebuggingMemoryManagerSize lOffset =(lCurrent->mSize/sizeof(lTChunk))- lNumRecords;

    assert(lOffset >= 0);

    ///Position of the new occupied chunk
    lTChunk* lPos= lCurrent
      + 1 //lCurrent will not be overwritten
      + lOffset;
      
    assert(lNewChunk.mPrev);

    //A previous node of lCurrent ALWAYS exists, but...
    lNewChunk.mPrev->mNext= lPos;
    if(lNewChunk.mNext)
      lNewChunk.mNext->mPrev= lPos;

    //A chunk here has always a following and preceding chunk
    assert(lNewChunk.mFollowing);
    lNewChunk.mFollowing->mPreceding= lPos;

    assert(lNewChunk.mPreceding);
    lNewChunk.mPreceding->mFollowing= lPos;

    *lPos= lNewChunk;

    lCurrent->mSize -= lSize;

    mMutex.unlock();   
    return (void*)(lPos+1);
  }else
    assert(0);
};

void CDebuggingMemoryManager::FreeChunk(lTChunk* inChunk){
  mMutex.lock();

  assert(inChunk);
  assert(inChunk->mPrev);
  assert(inChunk->mMagic==cMagic);


  if(inChunk->mNext)
    inChunk->mNext->mPrev=inChunk->mPrev;

  inChunk->mPrev->mNext=inChunk->mNext;

  inChunk->mMagic=cUnMagic;
  mMutex.unlock();
}

bool CDebuggingMemoryManager::freeMem(void* inChunk){
  mMutex.lock();

  lTChunk* lChunk= ((lTChunk*) inChunk);
  lChunk--;

  if(lChunk->mMagic != cMagic){
    //Deallocation of something free
    mMutex.unlock();
    return false;
  }else{
    //The previous node is free
    if(lChunk->mPreceding->mMagic == cUnMagic){
      if(lChunk->mFollowing->mMagic == cUnMagic)
	{
	  // both the preceding and the following node are free
	  // and BOTH EXIST! => Three free nodes -> 1 free node
	  lTChunk* lNew = lChunk->mPreceding;

	  lNew->mSize+= lChunk->mSize 
	    + lChunk->mFollowing->mSize 
	    + 2 *sizeof(lTChunk);

	  //New pointers to the successors
	  lNew->mNext=       lChunk->mFollowing->mNext;
	  lNew->mFollowing = lChunk->mFollowing->mFollowing;

	  //New pointers from successors to predecessor
	  // has to exist because of the termination of the PrecFollow-list
	  lTChunk* lNextOccupied= lChunk->mFollowing->mFollowing;
	  assert(lNextOccupied);
	  {
	    // The next occupied node exists.
	    lNextOccupied->mPreceding= lNew;
	  }
	  if(lNew->mNext)
	    lNew->mNext->mPrev=lNew;

	}else{
	  // only the preceding node is free (and exists)
	  // Changes of the FreeList and of the PF-list

	  lTChunk* lNew = lChunk->mPreceding;

	  
	  lNew->mSize+= lChunk->mSize 
	    + sizeof(lTChunk);

	  lNew->mFollowing = lChunk->mFollowing;
	  //lNew->mNext stays: The FreeList is not changed

	  // has to exist because of the termination of the PrecFollow-list
	  lTChunk* lNextOccupied= lChunk->mFollowing;
	  assert(lNextOccupied);
	  {
	    // The next occupied node exists.
	    lNextOccupied->mPreceding= lNew;
	  }
	}
      FreeChunk(lChunk);
    }else{
      if(lChunk->mFollowing->mMagic == cUnMagic){

	//only the following node is free (and exists)

	lTChunk* lFollowing = lChunk->mFollowing;
	lChunk->mSize+=     
	  lFollowing->mSize
	  +sizeof(lTChunk);
	lChunk->mFollowing= lFollowing->mFollowing;


	//lFollowing out of the PF-List
	lFollowing->mFollowing->mPreceding= lChunk;

	//Chunk out of the OccupiedList
	FreeChunk(lChunk);

	//Move lChunk into the FreeList
	lChunk->mPrev=lFollowing->mPrev;
	lFollowing->mPrev->mNext= lChunk;

	// lFollowing out of FreeList
	lChunk->mNext= lFollowing->mNext;
	if(lChunk->mNext)
	  lChunk->mNext->mPrev=lChunk;
      }else{
	//Looking for the next piece of free mem.
	lTChunk* lCurrent=lChunk;

	while(lCurrent && (lCurrent->mPreceding->mMagic == cMagic))
	  lCurrent = lCurrent -> mPreceding;

	//lCurrent has to exist: Either the FreeList-anchor
	//or some other piece of free mem.
	assert(lCurrent);

	//lChunk out of the Occupied list
	FreeChunk(lChunk);

	// According to initialization,
	// lCurrent->mPreceding must be a free node.
	// and it exsists

	lTChunk* lFree=lCurrent -> mPreceding;
	assert(lFree);

	//lChunk into the FreeList
	lChunk->mNext=lFree->mNext;
	lChunk->mPrev=lFree;
	lFree->mNext= lChunk;
	if(lChunk->mNext)
	  lChunk->mNext->mPrev= lChunk;
      }
    }
  }
  mMutex.unlock();
  return true;
};

bool CDebuggingMemoryManager::isValid()const{
  return cVM==0x88414004;
}



ostream& operator<<(ostream& outStream,const CDebuggingMemoryManager& inMem){
  outStream << endl << "List of free memory chunks" << endl;
  {
    lTChunk* i=inMem.mFreeList;
    while(i && i!=i->mNext){
      outStream << i
		<< ","
		<< i->mPrev
		<< ","
		<< i->mSize
		<< ","
		<< i->mMagic%16
		<< endl;
      i=i->mNext;
    }

    outStream<< endl << "List of occupied memory chunks" << endl;

    i=inMem.mUsedList;
    while(i && i!=i->mNext){
      outStream << i
		<< ","
		<< i->mPrev
		<< ","
		<< i->mSize
		<< ","
		<< i->mMagic%16
		<< endl;
      i=i->mNext;
    }

    outStream << endl << "PrecedingFollowList" << endl;

    i=inMem.mFreeList;
    while(i!=i->mFollowing){
      outStream << i
		<< ","
		<< i->mPreceding
		<< ","
		<< i->mSize
		<< ","
		<< i->mMagic%16
		<< endl;
      i=i->mFollowing;
    }
    {
      outStream << i
		<< ","
		<< i->mPreceding
		<< ","
		<< i->mSize
		<< ","
		<< i->mMagic%16
		<< endl;
      i=i->mFollowing;
    }

    outStream << "END" << endl;
    i=inMem.mBuffer+(50000 / sizeof(lTChunk));


    outStream << "Terminator" << endl;
    outStream << i 
	      << "," 
	      << i->mPreceding 
	      << ","
	      << i->mSize
	      << ","
	      << i->mMagic%16
	      << endl;
  }
  return outStream;
};





