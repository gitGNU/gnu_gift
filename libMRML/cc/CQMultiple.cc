// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000 CUI, University of Geneva

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/
#include "CQMultiple.h"
#ifdef HAVE_LIBPTHREAD
#include <pthread.h>
#endif
/** 

    There is no special initialisation to do,
    so this function is empty

 */
void CQMultiple::init(){
  
};

/**
 *
 * default constructor
 * 
 */
CQMultiple::CQMultiple(){
  assert(0);
};
/**
 * constructor
 * see CQuery
 */
CQMultiple::CQMultiple(CAccessorAdminCollection& inAccessorAdminCollection,
		       CAlgorithm&          inAlgorithm):
  CQuery(inAccessorAdminCollection,
	 inAlgorithm){
  {
    // mproxy has been filled in a reasonable way 
    // by CQuery::CQuery
    mAccessor=mAccessorAdmin->openAccessor("url2fts");
    assert(mAccessor);

  }
};
    
/**
 *
 * destructor: at present empty
 * 
 */
CQMultiple::~CQMultiple(){

  cout << "destroying this "
       << __FILE__
       << __LINE__
       << flush
       << endl;

  //i thought i will need this, but at present I do not have this impression
  //it does not hurt, so we leave it in
};

class CWeightedResult{
public:
  /** The weight of this */
  double mWeight;
  /** The weight of this */
  CIDRelevanceLevelPairList* mList;
  /** The weight of this */
  CIDRelevanceLevelPairList::iterator mI;
};
  
/** everything happening in the children */
bool CQMultiple::setAlgorithm(CAlgorithm & inAlgorithm){
  if(mAlgorithm && mAlgorithm->getCollectionID()==inAlgorithm.getCollectionID()){
    
    return true;
    
  }else{
    //close the old collection, if exsisting
    if(mAccessorAdmin)
      mAccessorAdmin->closeAccessor("url2fts");
    //
    mAccessorAdmin=&mAccessorAdminCollection->getProxy(inAlgorithm.getCollectionID());
    mAccessor=mAccessorAdmin->openAccessor("url2fts");

    assert(mAccessor);
    //
    return (CQuery::setAlgorithm(inAlgorithm) && mAccessor);
  }
};
/**
 *
 * calls fastQuery for every child, merges the results
 * and translates them back into URLs
 *

 NOTE THIS IS COMMENTED OUT. IT WAS TOO INEFFICIENT. IT WOULD BECOME A CHOICE
 IF WE WOULD OUTPUT THINGS RATHER SORTED BY THEIR URL THAN BY THEIR SCORE

CIDRelevanceLevelPairList* CQMultiple::fastQuery(const CXMLElement& inQuery,
						 int inNumberOfInterestingImages,
						 double inDifferenceToBest){

  cout << "CMultiple Number of children:"
       << mChildren.size()
       << endl;

  list<CWeightedResult> lTemporary;
  double lWeightSum(0);

  for(lCChildren::const_iterator i=mChildren.begin();
      i!=mChildren.end();
      i++){
    lWeightSum+=i->mWeight;

    lTemporary.push_back(CWeightedResult());
    
    cout << "this CMultiple:" << this 
	 << ", i->mQuery:" << i->mQuery 
	 << ", i->mWeight:" << i->mWeight 
	 << endl;


    lTemporary.back().mList=i->mQuery->fastQuery(inQuery,
						 mAccessor->size(),
						 inDifferenceToBest);
    
    if(lTemporary.back().mList){
      lTemporary.back().mList->sort(CSortByID_IRLP());
      lTemporary.back().mI=lTemporary.back().mList->begin();
            
      lTemporary.back().mWeight=i->mWeight;
    }else{
      lTemporary.pop_back();
    }
  }
  
  CIDRelevanceLevelPairList* lReturnValue=new CIDRelevanceLevelPairList();
  
  while(lTemporary.size()){
    
    list<CWeightedResult>::iterator lMinimumIterator=lTemporary.begin();
    for(list<CWeightedResult>::iterator i=++lTemporary.begin();
	i!=lTemporary.end();
	i++){
      //the ID pointed to by i is smaller lMinimumIterator
      if((i->mI)->getID()<(lMinimumIterator->mI)->getID()){
	lMinimumIterator=i;
      }
    }
    if((lReturnValue->size()) 
       && ((lMinimumIterator->mI)->getID()==lReturnValue->back().getID())){
      lReturnValue->back().setRelevanceLevel(
					     lReturnValue->back().getRelevanceLevel()
					     +
					     ((lMinimumIterator->mI)->getRelevanceLevel()
					      *
					      lMinimumIterator->mWeight)
					     );
    }else{
      lReturnValue->push_back(*lMinimumIterator->mI);
      lReturnValue->back().setRelevanceLevel(
					     ((lMinimumIterator->mI)->getRelevanceLevel()
					      *
					      lMinimumIterator->mWeight)
					     );
    }
    (lMinimumIterator->mI)++;
    //end of this list, so we delete it
    if(lMinimumIterator->mI==lMinimumIterator->mList->end()){
      lTemporary.erase(lMinimumIterator);
    }
  }
  //lReturnValue->sort(CSortByID_IRLP());// for testing, to see if there are any duplicates
  lReturnValue->sort();
  lReturnValue->reverse();
  cout << "Size of the result "
       << lReturnValue->size()
       << endl;

  {
    CIDRelevanceLevelPairList::iterator iSkip=lReturnValue->begin();
    for(int i=0;
	i<inNumberOfInterestingImages && i<lReturnValue->size();
	i++){
      iSkip->setRelevanceLevel(iSkip->getRelevanceLevel()/lWeightSum);
      iSkip++;
    }
    lReturnValue->erase(iSkip,lReturnValue->end());
  }

  return lReturnValue;
};

 */


/** all information about a query thread for a CQMultiple*/
class CQMThread{
  /** the thread administrator */
#ifdef HAVE_LIBPTHREAD
  pthread_t mThread;
#endif
  /** is this called as a thread or as a function */
  bool mIsThreaded;
public:
  /**  query processor used in this thread*/
  CQuery& mQueryProcessor;
  /** the query (an xml parse tree) */
  const CXMLElement& mQuery;
  /** the desired result size */
  int mResultSize;
  /** the cutoff */
  double mDifferenceToBest;
  /** the result of the query*/
  CIDRelevanceLevelPairList* mResult;
  /** constructor */
  CQMThread(CQuery& inQueryProcessor,
	   const CXMLElement& inQuery,
	   int inResultSize,
	   double inDifferenceToBest):
    mQueryProcessor(inQueryProcessor),
    mQuery(inQuery),
    mResultSize(inResultSize),
    mDifferenceToBest(inDifferenceToBest),
    mResult(0){
  };
  /** copy constructor */
  CQMThread(const CQMThread& in):
    mQueryProcessor(in.mQueryProcessor),
    mQuery(in.mQuery),
    mResultSize(in.mResultSize),
    mDifferenceToBest(in.mDifferenceToBest),
    mResult(0),
    mIsThreaded(0){
  };
  /** running the thread */
  void runThread(void){
#ifdef HAVE_LIBPTHREAD
    mIsThreaded=true;
    pthread_create(&mThread,0,CQMultiple::doQueryThread,this);
#else
    CQMultiple::doQueryThread(this);
#endif
  }
  /** running the function which would be called by the thread */
  void callFunction(void){
    CQMultiple::doQueryThread(this);
  }
  /** joining this thread with the caller */
  void join(void){
#ifdef HAVE_LIBPTHREAD
    void** lReturnValue(NULL);
    cout << "PREJOIN" << endl;
    if(mIsThreaded){
      pthread_join(mThread,lReturnValue);
    }else{
      cout << "This was an unthreaded join!" 
	   << endl;
    }
    mIsThreaded=false;
    cout << "POSTJOIN" << endl;
#endif
  }
};

void* CQMultiple::doQueryThread(void* inParameter){
  class CQMThread* lParam((CQMThread*) inParameter);
  lParam->mResult=lParam->mQueryProcessor.fastQuery(lParam->mQuery,
						    lParam->mResultSize,
						    lParam->mDifferenceToBest);
  cout << "I AM FINISHED HERE " << lParam << "result" << lParam->mResult << endl;
}

/**
 *
 * calls fastQuery for every child, merges the results
 * and translates them back into URLs
 *
 * NEW, MORE EFFICIENT VERSION
 */
CIDRelevanceLevelPairList* CQMultiple::fastQuery(const CXMLElement& inQuery,
						 int inNumberOfInterestingImages,
						 double inDifferenceToBest){

  cout << "CMultiple Number of children:"
       << mChildren.size()
       << endl;


  list<CWeightedResult> lTemporary;
  double lWeightSum(0);

  hash_map<TID,CIDRelevanceLevelPair> lResultMap;


  // no mutex protection needed as this is not called except by main thread

  //
  // rip this into two parts
  // in order to 
  // make it possible to run the querying in one thread
  // and do the merging after having waited for each thread
  //

  list<CQMThread> lListOfThreads;

  lCChildren::const_iterator lLast=mChildren.end();
  lLast--;
  for(lCChildren::const_iterator i=mChildren.begin();
      i!=mChildren.end();
      i++){
    lWeightSum+=i->mWeight;
    
    lTemporary.push_back(CWeightedResult());
    
    cout << "this CMultiple:" << this 
	 << ", i->mQuery:" << i->mQuery 
	 << ", i->mWeight:" << i->mWeight 
	 << endl;
    
    
    lListOfThreads.push_back(CQMThread(*(i->mQuery),          // The Query processor to choose
				       inQuery,            // the query to be processed
				       mAccessor->size(),  // the size of the accessor (to get all potential results)
				       inDifferenceToBest));// the difference to the best which is allowed for a result
    if(i==lLast){
      lListOfThreads.back().callFunction();//something to do for the main thread
    }else{
      cout << "Running thread" 
	   << endl;
      lListOfThreads.back().runThread();//run the thread
      cout << "loop" 
	   << endl;
    }
    cout << "endloop" 
	 << endl;
  }
  // here we would join all threads

  for(list<CQMThread>::iterator lThread=lListOfThreads.begin();
      lThread!=lListOfThreads.end();
      lThread++){

    cout << "joining..." << endl;

    lThread->join();

    cout << "before merging " << endl;

    if(!lThread->mResult){
      cerr << "The the result of this thread was nil " 
	   << endl;
    }
    if(lThread->mResult){
      for(CIDRelevanceLevelPairList::const_iterator i=lThread->mResult->begin();
	  i!=lThread->mResult->end();
	  i++){
	
	hash_map<TID,CIDRelevanceLevelPair>::const_iterator lFound=lResultMap.find(i->getID());
	
	if(lFound==lResultMap.end()){
	  
	  lResultMap.insert(make_pair(i->getID(),
				      *i));
	  
	}else{
	  
	  lResultMap[i->getID()].setRelevanceLevel(lResultMap[i->getID()].getRelevanceLevel()
						   +i->getRelevanceLevel());
	}
      }
      delete lThread->mResult;
    }

    
    cout << "after merging " << endl;
  }
  
  CIDRelevanceLevelPairList* lReturnValue=new CIDRelevanceLevelPairList();

  cout << "<pushing>"
       << endl;

  for(hash_map<TID,CIDRelevanceLevelPair>::const_iterator i=lResultMap.begin();
      i!=lResultMap.end();
      i++){
    lReturnValue->push_back(i->second);
  }

  cout << "</pushing>\n<sorting>"
       << endl;

  lReturnValue->sort();
  lReturnValue->reverse();
  cout << "Size of the result "
       << lReturnValue->size()
       << endl;
  cout << "</sorting>"
       << endl;

  cout << "<cutting>"
       << endl;
  {
    CIDRelevanceLevelPairList::iterator iSkip=lReturnValue->begin();
    for(int i=0;
	i<inNumberOfInterestingImages && i<lReturnValue->size();
	i++){
      iSkip->setRelevanceLevel(iSkip->getRelevanceLevel()/lWeightSum);
      iSkip++;
    }
    lReturnValue->erase(iSkip,lReturnValue->end());
  }
  cout << "</cutting>"
       << endl;
  return lReturnValue;
};

