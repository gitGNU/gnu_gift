// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, University of Geneva

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
/***************************************
*
* CAcIFFileSystem - class for the basic access to 
* the inverted file and the generation of the inverted file
*
****************************************
*
* modification history:
* 
* WM 260600 renamed file and class to reflect new architecture
* HM        removed some screen output
* HM 010499 changed to find error
* HM 090399 created the documentation
* WM 10  98 created most of the content
*
****************************************
*
* compiler defines used:
*
* _NO_PRINT_OFFSET_CHECK ?
* _NO_CHECK_OFFSET_FILE  ?
* _NO_CHECK_CONSISTENCY  do NOT check consistency after
*                        generating inverted file
* _NO_FIDPRINT           ?
* _NO_DIDPRINT           ?
* _NO_PRINT_ID           ?
*
* _NO_PRINT_INIT
*
* IGNORE_UNKNOWN_URLS    unknown images are considered to have an empty feature list
*
****************************************/


#include <string.h>
#include <errno.h> //show which error makes reads fail!

#include <unistd.h>     // for getpid
#include "libMRML/include/mrml_const.h" // for parsing
#include <unistd.h>
#include <cstdio>       // for sprintf
#include <strstream>
#include <string>
#include <set> 
#include <algorithm>
#include <functional>

#include "libMRML/include/my_assert.h"

#include "libGIFTAcInvertedFile/include/merge_sort_streams.h"    //newGenerateInvertedFile
#include "libGIFTAcInvertedFile/include/CIFBuilderTriplet.h"     //newGenerateInvertedFile
#include "libGIFTAcInvertedFile/include/CDocumentFrequencyList.h"
#include "libGIFTAcInvertedFile/include/CAcIFFileSystem.h"
#include "libGIFTAcInvertedFile/include/CInvertedFileChunk.h"
#include <iostream>
#include <fstream>
#include <assert.h>
#include <math.h>
#include "libGIFTAcInvertedFile/include/CIFListStart.h"
#include "libMRML/include/CXMLElement.h" // constructor
#include "libMRML/include/directory.h"   // the install locations etc. as determined by ./configure
#define _NO_PRINT_OFFSET_CHECK
#define _NO_CHECK_OFFSET_FILE
#define _NO_CHECK_CONSISTENCY
#define _NO_CONSISTENCYPRINT
#define _NO_FIDPRINT
#define _NO_DIDPRINT
#define	_NO_PRINT_ID
#define	_NO_PRINT_INIT

extern class CMutex* gMutex;

/** used for reading the offset file */
typedef struct{
  unsigned int mUInt1;
  unsigned int mUInt2;
} SUIntUInt;


/***************************************
*
* ()
*
****************************************
*
* modification history
*
* 
*
****************************************/
/* to test if the inverted file accessor is OK */
bool CAcIFFileSystem::operator()()const{
  /**/gMutex->lock();
  bool lReturnValue(mURL2FTS
		    && *mInvertedFile
		    && mOffsetFile
		    && mURL2FTS->operator bool());
  /**/gMutex->unlock();
  return lReturnValue;
};

 
/***************************************
*
* generateInvertedFile - the inverted file is generated here
*
****************************************
*
* modification history
*
* 
*
****************************************/
bool CAcIFFileSystem::generateInvertedFile(){
  /**/gMutex->lock();
  // open the feature description file
  //
  cout << "I want to use/generate the files:"
       << mInvertedFileName << endl
       << mOffsetFileName << endl
       << mURL2FTS->getURLToFeatureFileName() << endl
       << mFeatureDescriptionFileName << endl;

  ofstream lNewInvertedFile(mInvertedFileName.c_str());
  ofstream lNewOffsetFile(mOffsetFileName.c_str());

  /* if one of the files does not open correctly */
  if(!(mURL2FTS->operator bool()
       && 
       mFeatureDescriptionFile
       &&
       lNewInvertedFile
       &&
       lNewOffsetFile)){
      cerr << "I could not open the necessary files for" 
	   << "generating an inverted file";
      /**/gMutex->unlock();
      return false;
  }
  cout << "files successfully opened" 
       << flush
       << endl;
  
  
  bool lError=false;
  //Local: A hash of Inverted File Chunks by the feature ID
  map<TID,CInvertedFileChunk> lInvertedFileHash;
  
  
  /// Meaning additional document information
  CADIHash lADI;
  
  list<CAccessorElement> lAllAccessorElements;

  mURL2FTS->getAllAccessorElements(lAllAccessorElements);

  /* process this for all the images in the file URL to Feature file name */
  for(list<CAccessorElement>::const_iterator i=lAllAccessorElements.begin();
      i!=lAllAccessorElements.end();
      i++){
    
    //This variable is used in the for loop to translate positions in the map into IDs
    int lDocumentID=i->getID();


    cout << endl
	 << "Processing File: "
	 << lDocumentID
	 << flush;
    cout << " "
	 << *i
	 << "..."
	 << flush
	 << endl;
    cout << "h" 
	 << flush
	 << endl;

    /* use the second part of the map as the 
       file name for the next feature file */
    ifstream lFeatureFile(i->getFeatureFileName().c_str());
    lADI[lDocumentID]=CAdditionalDocumentInformation(i->getFeatureFileName().c_str());
    
    unsigned int lNumberOfFeatures=0;
    /* reads the number of features out of the file */
    lFeatureFile.read((char*)&lNumberOfFeatures,
		      sizeof(lNumberOfFeatures));
    
    double lMaxDocumentFrequency=0;
    double lDocumentFrequencySquareSum=0;
    
    //Read features for one image
    if(lFeatureFile)
      {
	/* for each feature in the file */
	for(unsigned int j=0;
	    j<lNumberOfFeatures && lFeatureFile;
	    j++)
	  {
	    /* does this already read in the element from the file ? yes*/
	    struct{
	      TID first;
	      float second;
	    } lInElement;

	    lFeatureFile.read((char*)&lInElement,
			      sizeof(lInElement));


	    CDocumentFrequencyElement lElement(lInElement.first,
					       lInElement.second);
	    
	    // 	    cout << "RSIZE" << sizeof(lInElement) 
	    // 		 << " " << lElement.getID()
	    // 		 << " " << lInElement.first
	    // 		 << " " << lElement.getDocumentFrequency()
	    // 		 << "== 0x" << hex << lInElement.second
	    // 		 << endl;
	    
	    //if this assertion fails, there has been a misunderstanding
	    //about the data format of the feature file 
	    //(padding: where and how?) WM
	    assert(lElement.getDocumentFrequency()>1/1000000000);
	    assert(lElement.getDocumentFrequency()<=1);

	    //Adjust values which depend on the document Frequency
	    //in the Additional Document Information
	    lADI[lDocumentID].
	      adjustDF(lElement.getDocumentFrequency());
	
	    /* this adds a feature into the inverted file list of features */
	    lInvertedFileHash[lElement.getID()].
	      addElement(lDocumentID,
			 lElement.getDocumentFrequency());
	  }
      }

    cout << "...finished" 
	 << flush
	 << endl;


    //one or more errors...
    if(!lFeatureFile){
      lError=true;
      cout << "Error reading file "
	   << i->getFeatureFileName()
	   << "!"
	   << endl;
    }
  }

  // For each Inverted file chunk: this means for each feature 
  // including the documents containing this feature
  // Write it and its offset
  for(map<TID,CInvertedFileChunk>::const_iterator i=lInvertedFileHash.begin();
      i!=lInvertedFileHash.end() && lNewInvertedFile;
      i++)
    {

      assert((*i).second.size());
      
      //find out the ID of the feature
      TID lFeatureID=(*i).first;
      //writing the offset
      {
	// this is the position of the next feature in the inverted file
	unsigned int lPos=lNewInvertedFile.tellp();
	
	/* updating the offset file */

	writeOffsetFileElement(lFeatureID,
			       lPos,
			       lNewOffsetFile);
			       

	cout << endl
	     << "Writing Chunk for Feature ID "
	     << hex
	     << lFeatureID
	     << ". The Offset is 0x"
	     << hex
	     << lPos
	     << dec
	     << "="
	     << lPos
	     << endl;
      }
      // Writing the next piece of the inverted file
      {
	/* this writes the actual data for the feature, 
	   meaning the list of all the documents containg 
	   this feature */
	bool lSuccessfullyWritten(((*i).second).writeBinary(lNewInvertedFile,
							    lFeatureID,
							    mURL2FTS->size()));
	if(!lSuccessfullyWritten)
	  cout << "!!!!!!!!!!!Error in writing Inverted File at Feature"
	       << lFeatureID
	       << flush
	       << endl;
      }
      
    } /* end of the loop for all the features */
  if(!lNewInvertedFile){
    cout << "ERROR in writing INVERTED FILE" << endl;
  }

  
  lNewInvertedFile.close();
  lNewOffsetFile.close();
 
  {
    //maybe calculate the ADI for each type of feature independently
    //could be useful


    //the value for the document frequency is known only 
    //when one is ready to write. This has just now been the case, 
    //and so...
    
    //For each feature
    for(map<TID,CInvertedFileChunk>::iterator iChunk=lInvertedFileHash.begin();
	iChunk!=lInvertedFileHash.end();
	iChunk++) {
	/* for each document frequency (each document) for this feature */
	for(CInvertedFileChunk::iterator iDocumentFrequency=(*iChunk).second.begin();
	    iDocumentFrequency!=(*iChunk).second.end();
	    iDocumentFrequency++)
	  {
	    
	    //The document for which we want to calculate the sum
	    TID lDID=(*iDocumentFrequency).first;
	    
	    /* if the document frequency does exist bigger than zero */
	    if((*iDocumentFrequency).second!=0)
	      {
       		double lDFLogICF=
		  (*iDocumentFrequency).second
		  *
		  (-log((*iChunk).second.getCollectionFrequency(mURL2FTS->size())));
		
		assert(((*iChunk).second).size()!=0);
		assert((*iChunk).second.getCollectionFrequency(mURL2FTS->size()));

		lADI[lDID].adjustSquareDFLogICF(lDFLogICF*lDFLogICF);
	      } /* end of the if */
	  }
      }
  } /* bracket with no meaning */

  //as last thing give as output the Additional Document Information
  /* writes the additional document information */
  assert(lADI.output());
  cout << "ADI WRITTEN ****************************************"
       << endl
       << endl;


#ifndef _NO_CHECK_CONSISTENCY
  cout << "BEFORE checking inverted file consistency" << flush;
  assert(checkConsistency());
  cout << "AFTER checking inverted file consistency" << endl
       << "The check was successful" << endl;
#endif
  
  /**/gMutex->unlock();
  return(!lError);
}

/** 
    add a pair of FeatureID,Offset to the open offset file 
    (helper function for inverted file construction)
*/
void CAcIFFileSystem::writeOffsetFileElement(TID inFeatureID,
					     int inPosition,
					     ostream& inOpenOffsetFile){
  /**/gMutex->lock();
  inOpenOffsetFile.write((char*)&inFeatureID,
			 sizeof(inFeatureID));
  inOpenOffsetFile.write((char*)&inPosition,
			 sizeof(inPosition));
  /**/gMutex->unlock();
  
};
/***************************************
*
* newGenerateInvertedFile - the inverted file is generated here
*
****************************************
*
* modification history
*
* 
*
****************************************/
bool CAcIFFileSystem::newGenerateInvertedFile(){
  /**/gMutex->lock();
  // open the feature description file
  //
  cout << "I want to use/generate the files:"
       << mInvertedFileName << endl
       << mOffsetFileName << endl
       << mURL2FTS->getURLToFeatureFileName() << endl
       << mFeatureDescriptionFileName << endl;

  ofstream lNewInvertedFile(mInvertedFileName.c_str());
  ofstream lNewOffsetFile(mOffsetFileName.c_str());

  /* if one of the files does not open correctly */
  if(!(mURL2FTS->operator bool()
       && 
       mFeatureDescriptionFile
       &&
       lNewInvertedFile
       &&
       lNewOffsetFile)){
    cerr << "I could not open the necessary files for" 
	 << "generating an inverted file";
    /**/gMutex->unlock();
    return false;
  }
  cout << "files successfully opened" 
       << flush
       << endl;
  
  bool lError=false;
  
  /// Meaning additional document information
  CADIHash lADI;
  
  ofstream lAuxOutputStream("gift-auxiliary-1");
  
  list<CAccessorElement> lAllAccessorElements;

  mURL2FTS->getAllAccessorElements(lAllAccessorElements);
  /* process this for all the images in the file URL to Feature file name */
  for(list<CAccessorElement>::const_iterator i=lAllAccessorElements.begin();
      i!=lAllAccessorElements.end();
      i++){
    //This variable is used in the for loop to translate positions in the map into IDs
    int lDocumentID=i->getID();


    cout << endl
	 << "Processing File: "
	 << lDocumentID
	 << flush;
    cout << " "
	 << *i
	 << "..."
	 << flush
	 << endl;
    cout << "h" 
	 << flush
	 << endl;

    /* use the second part of the map as the 
       file name for the next feature file */
    ifstream lFeatureFile(i->getFeatureFileName().c_str());
    lADI[lDocumentID]=CAdditionalDocumentInformation(i->getFeatureFileName().c_str());
    
    unsigned int lNumberOfFeatures=0;
    /* reads the number of features out of the file */
    lFeatureFile.read((char*)&lNumberOfFeatures,
		      sizeof(lNumberOfFeatures));
    
    double lMaxDocumentFrequency=0;
    double lDocumentFrequencySquareSum=0;

    //Read features for one image
    if(lFeatureFile){
      /* for each feature in the file */
      for(unsigned int j=0;
	  j<lNumberOfFeatures && lFeatureFile;
	  j++)
	{
	  /* does this already read in the element from the file ? yes*/
	  struct{
	    TID mFeatureID;
	    float mDocumentFrequency;
	  } lInElement;
	    
	    lFeatureFile.read((char*)&lInElement,
			      sizeof(lInElement));

	    CIFBuilderTriplet lBuilderTriplet(lInElement.mFeatureID,
					      lDocumentID,
					      lInElement.mDocumentFrequency);
	    
	    
	    
	    lAuxOutputStream.write((char*)&lBuilderTriplet,
				   sizeof(lBuilderTriplet));
	}
    }

    cout << "...finished" 
	 << endl
	 << flush;


    //one or more errors...
    if(!lFeatureFile){
      lError=true;
      cout << "Error reading file "
	   << i->getFeatureFileName()
	   << "!"
	   << endl;
    }
  }

  

  if(!lAuxOutputStream){
    cout << "ERROR in writing AUXILIARY FILE" << endl;
  }
  lAuxOutputStream.close();
  

  /* now sort the file of the triplets which we just have created 
     the first name is the name of the auxiliary file which we just
     wrote, the second name is the one of another auxiliary file which
     we need because we do not do any in-place-merging.

     the result of this is supposed to be in gift-auxiliary-1.
  */
  cout << "before mergesort " 
       << flush
       << endl;
    
  merge_sort_streams<CIFBuilderTriplet>("gift-auxiliary-1",
					"gift-auxiliary-2");


  cout << "after mergesort " 
       << flush
       << endl;
  
  
  /* now read the file  buildertriplet by buildertriplet.
     each time the ID changes, we are finished with one list */
  ifstream lInAuxiliaryFile("gift-auxiliary-1");

  cout << "Opening sorted stream for reading"
       << lInAuxiliaryFile
       << endl;
  
  if(lInAuxiliaryFile){
    // the right starting conditions for 
    // the following do-loop.
    CIFBuilderTriplet lOldTriplet(0,0,0);
    CIFBuilderTriplet lTriplet(0,0,0);
    // read the first IFBuilder Triplet from the sorted
    // triplet file
    lInAuxiliaryFile.read((char*)&lTriplet,
			  sizeof(lTriplet));
    do{
      CInvertedFileChunk lChunk;      
      TID lFeatureID;

      //Read now the list for one feature from
      //the auxiliary file
      {    
	do{
	  lOldTriplet=lTriplet;
	  lFeatureID=lOldTriplet.mFeatureID;
	  lChunk.addElement(lOldTriplet.mDocumentID,
			    lOldTriplet.mDocumentFrequency);
	  
	  lInAuxiliaryFile.read((char*)&lTriplet,
				sizeof(lTriplet));
	}while(lInAuxiliaryFile 
	       &&
	       (lTriplet.mFeatureID
		==lOldTriplet.mFeatureID));
	
	if((!lInAuxiliaryFile) 
	   &&
	   (!lTriplet.isIdentical(lOldTriplet))){
	  lChunk.addElement(lTriplet.mDocumentID,
			    lTriplet.mDocumentFrequency);
	  lFeatureID=lTriplet.mFeatureID;
	}else{
	  lFeatureID=lOldTriplet.mFeatureID;
	}
      }
      // Write this chunk just read as the next piece of the inverted file
      {
	/* 
	   this writes the current position and the current feature ID
	   into the offset file
	*/
	writeOffsetFileElement(lFeatureID,
			       lNewInvertedFile.tellp(), 
			       lNewOffsetFile); 
	/* this writes the actual data for the feature, 
	   meaning the list of all the documents containg 
	   this feature */
	cout << endl
	     << "Writing Chunk for Feature ID "
	     << hex
	     << lFeatureID
	     << ". The Offset is 0x"
	     << hex
	     << lNewInvertedFile.tellp()
	     << dec
	     << "="
	     << lNewInvertedFile.tellp()
	     << endl;
	bool lSuccessfullyWritten(lChunk.writeBinary(lNewInvertedFile,
						     lFeatureID,
						     mURL2FTS->size()));
      
	if(!lSuccessfullyWritten)
	  cout << "!!!!!!!!!!!Error in writing Inverted File at Feature"
	       << lFeatureID
	       << flush
	       << endl;
      }
    
    

      /* Now use this data also for creating additional document information
       
	 for each document frequency (each document) for this feature */
      for(CInvertedFileChunk::iterator iDocumentFrequency=lChunk.begin();
	  iDocumentFrequency!=lChunk.end();
	  iDocumentFrequency++){
      
	//The document for which we want to calculate the sum
	TID lDID=(*iDocumentFrequency).first;
      
	/* if the document frequency does exist bigger than zero */
	if((*iDocumentFrequency).second!=0){
	  double lDFLogICF=
	    (*iDocumentFrequency).second
	    *
	    (-log(lChunk.getCollectionFrequency(mURL2FTS->size())));
	  
	  assert((lChunk).size()!=0);
	  assert(lChunk.getCollectionFrequency(mURL2FTS->size()));
	  
	  //Adjust values which depend on the document Frequency
	  //in the Additional Document Information
	  lADI[lDID].
	    adjustDF(iDocumentFrequency->second);
	  lADI[lDID].adjustSquareDFLogICF(lDFLogICF*lDFLogICF);
	} /* end of the if */
      }// for each document in the list for one feature
    }while(lInAuxiliaryFile);//while there is still data in the auxiliary file
  }else{
    cout << "ERROR in reading sorted auxiliary file"
	 << endl;
  }

  if(!lNewInvertedFile){
    cout << "ERROR in writing INVERTED FILE" << endl;
    /**/gMutex->unlock();
    return false;
  }
  
  
  
  lNewInvertedFile.close();
  lNewOffsetFile.close();
  
  
  
  //as last thing give as output the Additional Document Information
  /* writes the additional document information */
  cout << "WRITING ADI ****************************************"
       << endl;
  assert(lADI.output());
  cout << "ADI WRITTEN ****************************************"
       << endl
       << endl;
  
  
#ifndef _NO_CHECK_CONSISTENCY
  cout << "BEFORE checking inverted file consistency" << flush;
  assert(checkConsistency());
  cout << "AFTER checking inverted file consistency" << endl
       << "The check was successful"
       << endl;
#endif
  
  // delete the helper files created by merge_sort_streams
 
  unlink("gift-auxiliary-1");
  unlink("gift-auxiliary-2");
  /**/gMutex->unlock();
  return(!lError);
}




/***************************************
*
* Constructor
*
****************************************
*
* modification history
*
* 
*
****************************************/
CAcIFFileSystem::CAcIFFileSystem(const CXMLElement& inCollectionElement):
  mURL2FTS(new CAcURL2FTS(inCollectionElement)),
  mOffsetFileName(inCollectionElement.stringReadAttribute(mrml_const::cui_base_dir).second
		  +inCollectionElement.stringReadAttribute(mrml_const::cui_offset_file_location).second),
  mInvertedFileName(inCollectionElement.stringReadAttribute(mrml_const::cui_base_dir).second
		    +inCollectionElement.stringReadAttribute(mrml_const::cui_inverted_file_location).second),
  mFeatureDescriptionFileName(inCollectionElement.stringReadAttribute(mrml_const::cui_base_dir).second
			      +inCollectionElement.stringReadAttribute(mrml_const::cui_feature_description_location).second),
  mInvertedFile(0),
  mInvertedFileBuffer(0),
  mMaximumFeatureID(0){
  /**/gMutex->lock();

  if(inCollectionElement.stringReadAttribute(mrml_const::cui_generate_inverted_file).first
     && inCollectionElement.boolReadAttribute(mrml_const::cui_generate_inverted_file).second){
    newGenerateInvertedFile();
  }else{

    
  bool lSuccessfulStart(inCollectionElement.stringReadAttribute(mrml_const::cui_base_dir).first
			&& inCollectionElement.stringReadAttribute(mrml_const::cui_feature_file_location).first
			&& inCollectionElement.stringReadAttribute(mrml_const::cui_offset_file_location).first
			&& inCollectionElement.stringReadAttribute(mrml_const::cui_inverted_file_location).first);
  
  if(lSuccessfulStart 
     && init(inCollectionElement.boolReadAttribute(mrml_const::cui_in_memory).first
	     && inCollectionElement.boolReadAttribute(mrml_const::cui_in_memory).second)){
    cout << "CInvertedFile succuessfully initialised. Parameters:"
	 << mInvertedFileName << endl
	 << mOffsetFileName << endl
	 << mURL2FTS->getURLToFeatureFileName() << endl
	 << mFeatureDescriptionFileName << endl
	 << endl;

#ifndef _NO_PRINT_INIT
    {
      cout << "DIAGNOSE" << flush << endl;
      cout << "The current size of mURLToID is " << mURLToID.size() << endl;
      cout << "All elements:" 
	   << endl;
      int lCount=1;
      for(string_TID_map::const_iterator i=mURLToID.begin();
	  i!=mURLToID.end();
	  i++){
	cout << ","
	     << lCount++
	     << flush;
      }
      lCount=1;
      for(string_TID_map::const_iterator i=mURLToID.begin();
	  i!=mURLToID.end();
	  i++){
	cout << lCount++
	     << ":"
	     << flush
	     << i->first
	     << "->"
	     << i->second
	     << flush
	     << endl 
	     << flush;
      }
    }
#endif


  };
#ifndef _NO_PRINT_INIT
  checkNPrint();
  cout << "Constructor left"
       << endl;
#endif
}
  /**/gMutex->unlock();
}


/***************************************
*
* init - initialization needed for the inverted file
*
****************************************
*
* modification history
*
* 
*
****************************************/
bool CAcIFFileSystem::init(bool inMemory)
{
  /**/gMutex->lock();

  mMaximumFeatureID=0;
  cout << "Opening _"
       << mInvertedFileName        
       << "_";
  if(inMemory){
    ifstream lInvertedFile(mInvertedFileName.c_str());
    if(lInvertedFile){

      cout << endl 
	   << "(TRYING TO READ THE WHOLE FILE INTO MEMORY"
	   << endl;
      
      lInvertedFile.seekg(0,ios::end);
      size_t lFileSize=lInvertedFile.tellg();
      mInvertedFileBuffer=new char[lFileSize];
      lInvertedFile.seekg(0,ios::beg);
      lInvertedFile.read(mInvertedFileBuffer,
			 lFileSize);

      mInvertedFile=new istrstream(mInvertedFileBuffer,
				   lFileSize);
      cout << "DONE)"
	   << endl;
    }else{
      mInvertedFile=0;
    }
  }

  mInvertedFile=new ifstream(mInvertedFileName.c_str());
  if(!(*mInvertedFile)){
    cout << " ...FAILED:" << strerror(errno) << endl;
  }else{
    cout << " ...success. " << endl;
  }

  cout << "Opening _"
       << mOffsetFileName 
       << "_";
  mOffsetFile.close();
  mOffsetFile.clear();
  mOffsetFile.open(mOffsetFileName.c_str());
  if(!mOffsetFile){
    cout << " FAILED!" << strerror(errno) << endl;
  }else{
    cout << " ...success. " << endl;
  }


  cout << "Opening _"
       << mFeatureDescriptionFileName
       << "_";
  mFeatureDescriptionFile.close();
  mFeatureDescriptionFile.clear();
  mFeatureDescriptionFile.open(mFeatureDescriptionFileName.c_str());
  if(!mFeatureDescriptionFile){
    cout << " ...FAILED!" << strerror(errno) << endl;
  }else{
    cout << " ...success. " << endl;
  }



  bool lRetVal=(mURL2FTS->operator bool()
		//was the superclass well constructed?
		&&
		mFeatureDescriptionFile
		&&
		mOffsetFile
		&&
		*mInvertedFile);

  assert(mURL2FTS->size());

  cout << endl
       << "Current success status"
       << lRetVal
       << endl;

  {
    list<CAccessorElement> lAllAccessorElements;
    
    mURL2FTS->getAllAccessorElements(lAllAccessorElements);

    //for each element in the database
    for(list<CAccessorElement>::const_iterator i=lAllAccessorElements.begin();
	i!=lAllAccessorElements.end();
	i++){
      TID    lID =i->getID();
      string lURL=i->getURL();

      pair<bool,string> lFeatureFileName=mURL2FTS->URLToFFN(lURL);

      assert(lFeatureFileName.first);
      
      mDocumentInformation
	.insert(make_pair(lID,
			  CAdditionalDocumentInformation(lFeatureFileName.second)));
      
      lRetVal = 
	lRetVal && mDocumentInformation[lID].input();
      
    }
  }

  cout << "URLFile " 
       << mURL2FTS->getURLToFeatureFileName() 
       << " processed. Current success status"
       << lRetVal
       << endl;

  /* erase the offset file */
  mIDToOffset.erase(mIDToOffset.begin(),
		    mIDToOffset.end());

  mOffsetFile.seekg(0);

  /* while there is no end of file for the offsets */
  while(mOffsetFile){
    SUIntUInt lVal;

    /* read a value into the offset file */
    mOffsetFile.read((char*)&lVal,sizeof(lVal));


    unsigned int lFeatureID=lVal.mUInt1;
    unsigned int lOffset=lVal.mUInt2;

    if(mOffsetFile){
      mMaximumFeatureID=(mMaximumFeatureID < lFeatureID)?lFeatureID:mMaximumFeatureID;
      
      mIDToOffset[lFeatureID]=lOffset;
    }

#ifndef _NO_PRINT_OFFSET_CHECK
    cout << "[lVal" << flush
	 << hex
	 << lFeatureID
	 << ","
	 << hex
	 << lOffset
	 << dec
	 << "]" << flush;
#endif
    //Reading the offsetfile
    {
      //move to the right position
      mInvertedFile->seekg(streampos(lOffset));

      //read the list start chunk 
      //(by constructing an instance of the list start cunk)
      CIFListStart lListStart(*mInvertedFile);

      //checking it
      if(lListStart.getFeatureID()!=lFeatureID){
	cout << "[ERROR" << flush
	     << hex
	     << lFeatureID
	     << ","
	     << hex
	     << lListStart.getFeatureID()
	     << dec
	     << "]" << flush;
	assert(0);
      }
#ifndef _NO_PRINT_OFFSET_CHECK
      else{
	cout << "-" << flush;
      }
#endif

      //And setting up the translation table from feature 
      //ID to collection frequency
      mFeatureToCollectionFrequency[lListStart.getFeatureID()]=
	lListStart.getCollectionFrequency();
      
      /*      assert(mFeatureToCollectionFrequency[lListStart.getFeatureID()]);*/

    }
#ifndef _NO_CHECK_OFFSET_FILE
#endif
    
  }

  cout << "OffsetFile " 
       << mOffsetFileName
       << " processed. Current success status: "
       << lRetVal
       << endl;

  {
    /* as long as there is no end of file */
    while(mFeatureDescriptionFile){
      TID lFeatureID;
      unsigned int lType;

      
      mFeatureDescriptionFile >> lFeatureID 
			      >> lType;

      mFeatureDescription[lFeatureID]=lType;
    } /* end of while */

  }

  cout << "FeatureDescriptionFile " 
       << mFeatureDescriptionFileName
       << " processed: "
       << mFeatureDescription.size()
       << " elements in hash."
       << endl
       << "Initialisation successful? Returning "
       << lRetVal
       << endl;

#ifndef _NO_PRINT_INIT
  checkNPrint();
#endif
  /**/gMutex->unlock();
  return lRetVal;
};



/***************************************
*
* FeatureToCollectionFrequency - Returns the collection frequency for a given feature
*
****************************************
*
* modification history
*
* 
*
****************************************/
double CAcIFFileSystem::
FeatureToCollectionFrequency(TFeatureID inFeatureID)const{
  /**/gMutex->lock();

  if((mFeatureToCollectionFrequency.find(inFeatureID))!=
     mFeatureToCollectionFrequency.end())
    {
      assert(0<(*(mFeatureToCollectionFrequency.find(inFeatureID))).second);
      
      double lReturnValue((*(mFeatureToCollectionFrequency.find(inFeatureID))).second);
      /**/gMutex->unlock();
      return lReturnValue;
    }
  else
    {
      /**/gMutex->unlock();
      return 1;
    }
}



/***************************************
*
* DIDToMaxDocumentFrequency - return the maximum document frequency for a given document ID
*
****************************************
*
* modification history
*
* 
*
****************************************/
double CAcIFFileSystem::DIDToMaxDocumentFrequency(TID inID)const{
  /**/gMutex->lock();

  if(mDocumentInformation.find(inID)!=mDocumentInformation.end())
    {
      double lReturnValue((*mDocumentInformation.find(inID)).second.getMaximumDF());
      /**/gMutex->unlock();
      return lReturnValue;
    }
  else
    {
      assert(1==0);
      /**/gMutex->unlock();
      return 1;
    }
};



/***************************************
*
* DIDToDFSquareSum - returns the Document frequency squaresum for a given document ID
*
****************************************
*
* modification history
*
* 
*
****************************************/
/// returns the Document frequency squaresum for a given document ID 
double CAcIFFileSystem::DIDToDFSquareSum(TID inID)const{
  /**/gMutex->lock();
  if(mDocumentInformation.find(inID)!=mDocumentInformation.end()){
    double lReturnValue(mDocumentInformation.find(inID)->second.getDFSquareSum());
      
    /**/gMutex->unlock();
    return lReturnValue;
  }else{
    assert(1+1==0);
    /**/gMutex->unlock();
    return 1;
  }
}



/***************************************
*
* DIDToSquareDFLogICFSum - returns this function for a given document ID
*
****************************************
*
* modification history
*
* 
*
****************************************/
double CAcIFFileSystem::DIDToSquareDFLogICFSum(TID inID)const{
  if(mDocumentInformation.find(inID)!=mDocumentInformation.end()){
    /**/gMutex->lock();
    double lReturnValue((*mDocumentInformation.find(inID))
			.second.getSquareDFLogICFSum());
    /**/gMutex->unlock();
    return lReturnValue;
  }else{
    assert(1+1+1==0);
    /**/gMutex->unlock();
    return 1;
  }
};
  


/***************************************
*
* FeatureToList - returns a list of document frequencies (documents) for a given feature ID
*
****************************************
*
* modification history
*
* 
*
****************************************/
CDocumentFrequencyList* CAcIFFileSystem::
FeatureToList(TFeatureID inFeatureID)const
{
  /**/gMutex->lock();
    CDocumentFrequencyList* lRetVal=0;

    
    {
      mInvertedFile->clear();


      //Find the list of URL-IDs for the feature
      if(mIDToOffset.find(inFeatureID)!=mIDToOffset.end()){
	mInvertedFile->seekg(streampos((*mIDToOffset.find(inFeatureID)).second));
      	assert(*mInvertedFile);
	/* if the inverted file has been able to be opened */
	if(*mInvertedFile){

	  //read the beginning chunk of the list;
	  CIFListStart lListStart(*mInvertedFile);

	  //
	  lRetVal=new CDocumentFrequencyList(lListStart.getNumberOfElements());

	  if(lListStart.getFeatureID()!=inFeatureID){
	    cerr << "Feature "
		 << hex
		 << inFeatureID
		 << " not found.";
	    /**/gMutex->unlock();
	    return 0;
	  }
	  
#ifndef _NO_DIDPRINT
	  cout << endl;
#endif


	  lRetVal->readBinary(*mInvertedFile);


#ifndef _NO_DIDPRINT
	  cout << endl;
#endif
	}
      }else{
	cerr << "II:Feature "
	     << hex
	     << inFeatureID
	     << " not found."
	     << mIDToOffset.size()
	     << endl;
	/**/gMutex->unlock();
	return 0;
      }
    }
    /**/gMutex->unlock();
    return lRetVal;
};




/***************************************
*
* findWithinStream - needed for checking the consistency
*
****************************************
*
* modification history
*
* 
*
****************************************/
///needed for checking the consistency
bool CAcIFFileSystem::findWithinStream(TID inFeatureID,
				       TID inDocumentID,
				       double inDocumentFrequency)const{
  /**/gMutex->lock();
  bool lRetVal=false;

  //Find the list of URL-IDs for the feature
  if(mIDToOffset.find(inFeatureID)!=mIDToOffset.end()) /* end points to the element 
							  after the last one for a list */
    {
      /* seekg searches the file position to read (get) */
      mInvertedFile->seekg(streampos((*mIDToOffset.find(inFeatureID)).second));
      
      /* if the file is correctly opened */
      if(*mInvertedFile){
	//read the beginning chunk of the list of documents
	//which contain a given feature
	CIFListStart lStart(*mInvertedFile);
	int    lFeatureID          = lStart.getFeatureID();
	double lCollectionFrequency= lStart.getCollectionFrequency();
	int    lEndFor             = lStart.getNumberOfElements();
	
	cout << endl
	     << "Feature:"
	     << inFeatureID
	     << "Size of CIFListStart:"
	     << sizeof(lStart)
	     << endl
	     << "Position in inverted file: "
	     << hex
	     << mInvertedFile->tellg()
	     << endl;

	
	if(lFeatureID!=inFeatureID){
	  cerr << "Feature "
	       << hex
	       << inFeatureID
	       << " not found."
	       << flush;
	  assert(0==1);
	  /**/gMutex->unlock();
	  return 0;
	}else{
	  
	  cout << "ENDFOR=" 
	       << lEndFor
	       << "!"
	       << flush
	       << endl;
	  
	  /* checks the document list for one feature */
	  for(int i=0;
	      i<lEndFor;
	      i++){
	    
	    CDocumentFrequencyElement lDFE(*mInvertedFile);
	    //	    cout << "[DID" ;
	    if((lDFE.getID()==inDocumentID)
	       &&
	       (lDFE.getDocumentFrequency()==inDocumentFrequency)
	       ){
	      lRetVal=true;
//  	      cout << "==" 
// 		   << inDocumentID
// 		   << " "
// 		   << flush;
	    }
// 	    cout << lDFE.getID()
// 		 << ".."
// 		 << lDFE.getDocumentFrequency()
// 		 << "]" 
// 		 << flush;
// 	    if(!(i%20)) cout << endl;
	  }
	}
      }
    }else{
      cout << "OFFSET "
	   << inFeatureID
	   << " NOT FOUND! Size of the container: " 
	   << mIDToOffset.size()	 
	   << "."
	   << endl;
      assert(0);
    }
  cout << "[Document ID:"
       << hex
       << inDocumentID
       << ",FeatureID"
       << inFeatureID
       << ","
       << inDocumentFrequency
       << "]" 
       << dec
       << flush;

  /**/gMutex->unlock();
  return lRetVal;
};




/***************************************
*
* checkConsistency - checks if the data in the inverted file is the same as in the single files
*
****************************************
*
* modification history
*
* 
*
****************************************/
bool CAcIFFileSystem::checkConsistency()
{
  /**/gMutex->lock();
#ifndef _NO_CONSISTENCYPRINT

  cout << "I am now checking the consistency between" << endl
       << "Feature files and the Inverted File"
       << endl
       << "Initialising...";
  
  
  init(false);

  cout << "...done";
  
  bool lRetVal=true;
  
  //for each image (URL)
  for(string_string_map::const_iterator i=mURL2FTS->mURLToFFN.begin();
      i!=mURL2FTS->mURLToFFN.end();
      i++)
    {
      CDocumentFrequencyList* lFeatures=URLToFeatureList((*i).first);
      
      cout << "FeatureFilename:-" 
	   << (*i).first
	   << "-"
	   << endl
	   << flush;
      
      assert(lFeatures);
      
      TID lDocumentID=(*mURL2FTS->mURLToID.find((*i).first)).second;
      
      /* for every feature of one image */
      for(CDocumentFrequencyList::const_iterator j=lFeatures->begin();
	  j!=lFeatures->end();
	  j++)
	{
	  /* looks if it really in the inverted file */
	  lRetVal &= findWithinStream((*j).getID(),
				      lDocumentID,
				      (*j).getDocumentFrequency());
	  assert(lRetVal);
	} /* end for each feature of the image */
    } /* end for each image */
  /**/gMutex->unlock();
  return lRetVal;
#else
  /**/gMutex->unlock();
  return true;
#endif
}




/***************************************
*
* URLToFeatureList -  
* returns the feature list (with document IDs) for a given URL
*
****************************************
*
* modification history
*
* 
*
****************************************/
CDocumentFrequencyList* 
CAcIFFileSystem::URLToFeatureList(string inURL)const
{
  /**/gMutex->lock();

#ifdef PRINT_ADI
  cout <<inURL
       << "(ADI:" ;
#endif

  pair<bool,TID> lID=URLToID(inURL);

  if(!lID.first){//i.e. the URL is not part of the collection

#ifdef IGNORE_UNKNOWN_URLS
    CDocumentFrequencyList* lReturnValue(0);
#else
    pid_t lPID= getpid();
    
    char lFeatureFileName[30];
    sprintf(lFeatureFileName,"/tmp/testFTS-%d.fts",int(lPID));

    system(string(string(__PERL_LOCATION__)+" "+string(__BINDIR__)+"/gift-url-to-fts.pl "+inURL+" "+lFeatureFileName).c_str());
    
    CDocumentFrequencyList* lReturnValue(this->getFeatureFile(lFeatureFileName));
 
    if(!lReturnValue){
      lReturnValue=new CDocumentFrequencyList();
    }
#endif
    /**/gMutex->unlock();
    return lReturnValue;
    // {
    //       cout << endl << "this= " << this << endl;
    //       checkNPrint();
    //       cout << "could not find ID for URL "
    // 	   << inURL << endl;
    //       cout << "The current size of mURLToID is " << mURLToID.size() << endl;
    //       cout << "All elements:" 
    // 	   << endl;
    //       int lCount=1;
    //       for(string_TID_map::const_iterator i=mURLToID.begin();
    // 	  i!=mURLToID.end();
    // 	  i++){
    // 	cout << ","
    // 	     << lCount++
    // 	     << flush;
    //       }
    //       lCount=1;
    //       for(string_TID_map::const_iterator i=mURLToID.begin();
    // 	  i!=mURLToID.end();
    // 	  i++){
    // cout << lCount++
    // 	     << ":"
    // 	     << flush
    // 	     << i->first
    // 	     << "->"
    // 	     << i->second
    // 	     << flush
    // 	     << endl 
    // 	     << flush;
    //       }
    //     }
  }
  my_assert(lID.first,inURL.c_str());   

  /**/gMutex->unlock();
  return DIDToFeatureList(lID.second);
};



CDocumentFrequencyList* CAcIFFileSystem::getFeatureFile(string inFileName)const{
  /**/gMutex->lock();
  CDocumentFrequencyList* lRetVal(0);
  /* if the filename has a size bigger than one, meaning it is defined */
  if(inFileName.size())
    {
      /* a file with the given file name */
      ifstream lFile(inFileName.c_str());
      unsigned int lNumberOfFeatures(0);
      if(lFile){
	lFile.read((char*)&lNumberOfFeatures,
		   sizeof(lNumberOfFeatures));
      
	if(lFile && (lRetVal=new CDocumentFrequencyList(lNumberOfFeatures))){
	
	  lRetVal->readBinary(lFile);
	  
	}
      }
    } /* end of if the URL was proper */
  /**/gMutex->unlock();
  return lRetVal;
}

/***************************************
*
* DIDToFeatureList - returns the feature list for a special image with a document ID
*
****************************************
*
* modification history
*
* 
*
****************************************/
CDocumentFrequencyList* 
CAcIFFileSystem::DIDToFeatureList(TID inDID)const{
  /**/gMutex->lock();

#ifdef PRINT_ADI
  cout <<inURL
       << "(ADI:" ;
#endif

  CADIHash::const_iterator iADI=mDocumentInformation.find(inDID);

  assert(iADI!=mDocumentInformation.end());

#ifdef PRINT_ADI
  iADI->second.output(cout);

  cout << ":ADI)"
       << endl;
#endif


  CDocumentFrequencyList* lRetVal=0;
  
  /* tests if URL is found */
  pair<bool,string> lFeatureFileName=mURL2FTS->IDToFFN(inDID);
  if(lFeatureFileName.first){
      
    lRetVal=this->getFeatureFile(lFeatureFileName.second);
    

  } /* end of the if statement */

  if(!lRetVal){
    lRetVal=new CDocumentFrequencyList(0);
  }
  /**/gMutex->unlock();
  return lRetVal;
};



 
/***************************************
*
* IDToURL - return the URL for an image for a given image ID
*
****************************************
*
* modification history
*
* 
*
****************************************/
string CAcIFFileSystem::IDToURL(TID inID)const
{
  /**/gMutex->lock();

  pair<bool,CAccessorElement> lElement=mURL2FTS->IDToAccessorElement(inID);
  
  if(lElement.first){
    /**/gMutex->unlock();
    return lElement.second.getURL();
  } else {
    cerr << "Error in Conversion from ID "
	 << inID 
	 << " to URL."
	 << endl;
    /**/gMutex->unlock();
    return mrml_const::error;
  }
}


/***************************************
*
* getFeatureDescription - returns the feature description for a given feature ID
*
****************************************
*
* modification history
*
* 
*
****************************************/
unsigned int CAcIFFileSystem::getFeatureDescription(TID inID)const{
  /**/gMutex->lock();
    
  if(mFeatureDescription.find(inID)!=mFeatureDescription.end()){
    unsigned int lReturnValue((*mFeatureDescription.find(inID)).second);
    /**/gMutex->unlock();  
    return lReturnValue;
  }else{
    cout << "[UF: "
	 << inID
	 << "]"
	 << flush;

    /** debugging code */
    cout << mFeatureDescription.size() << flush;
    /** /debugging code */

    /**/gMutex->unlock();
    return 0;
  }
}



/***************************************
*
* Destructor
*
****************************************
*
* modification history
*
* 
*
****************************************/
CAcIFFileSystem::~CAcIFFileSystem(){
  cout << "CAcIFFileSystem::~CAcIFFileSystem() called "
       << endl
       << flush;
  /**/gMutex->lock();
  /**/gMutex->unlock();
};

TID CAcIFFileSystem::getMaximumFeatureID()const{
  return mMaximumFeatureID;
};

/** 
    Getting a list of all features contained in this
*/
list<TID>* CAcIFFileSystem::getAllFeatureIDs()const{
  /**/gMutex->lock();
  list<TID>* lReturnValue=new list<TID>();

  if(lReturnValue){
    for(CIDToOffset::const_iterator i=mIDToOffset.begin();
	i!=mIDToOffset.end();
	i++){
      lReturnValue->push_back(i->first);
    }
  }
  lReturnValue->sort();

  /**/gMutex->unlock();
  return lReturnValue;
}

/** List of the IDs of all documents present in the inverted file */
void CAcIFFileSystem::getAllIDs(list<TID>& outIDList)const{
  /**/gMutex->lock();

  mURL2FTS->getAllIDs(outIDList);
  /**/gMutex->unlock();
};
/** List of triplets (ID,imageURL,thumbnailURL) of all
    the documents present in the inverted file */
void CAcIFFileSystem::getAllAccessorElements(list<CAccessorElement>& outAccessorElementList)const{
  /**/gMutex->lock();
  mURL2FTS->getAllAccessorElements(outAccessorElementList);
  /**/gMutex->unlock();
};
/** get a given number of random AccessorElement's 
    @param inoutResultList the list which will contain the result
    @param inSize          the desired size of the inoutResultList
*/
void CAcIFFileSystem::getRandomIDs(list<TID>& outRandomIDList,
				   list<TID>::size_type inSize)const{
  /**/gMutex->lock();
   mURL2FTS->getRandomIDs(outRandomIDList,
			  inSize);
  /**/gMutex->unlock();
};
/** For drawing random sets. Why is this part of an CAccessorImplementation?
    The way the accessor is organised might influence the way
    random sets can be drawn. At present everything happens in
    RAM, but we do not want to be fixed on that.
    
      @param inoutResultList the list which will contain the result
      @param inSize          the desired size of the inoutResultList
*/
void CAcIFFileSystem::getRandomAccessorElements(list<CAccessorElement>& outResult,
						list<CAccessorElement>::size_type inSize)const{
  /**/gMutex->lock();
  mURL2FTS->getRandomAccessorElements(outResult,
				      inSize);
  /**/gMutex->unlock();
};
/**
 *
 * Translate a DocumentID to an accessor Element
 *
 */
pair<bool,CAccessorElement> CAcIFFileSystem::IDToAccessorElement(TID inID)const{
  /**/gMutex->lock();
  pair<bool,CAccessorElement> lReturnValue(mURL2FTS->IDToAccessorElement(inID));
  /**/gMutex->unlock();
  return lReturnValue;
};
/** The number of images in this accessor */
int CAcIFFileSystem::size()const{
  /**/gMutex->lock();
  int lReturnValue(mURL2FTS->size());
  /**/gMutex->unlock();
  return lReturnValue;
};
/** The number of images in this accessor */
pair<bool,TID> CAcIFFileSystem::URLToID(const string& inURL)const{
  /**/gMutex->lock();
  pair<bool,TID> lReturnValue(mURL2FTS->URLToID(inURL));
  /**/gMutex->unlock();
  return lReturnValue;
};

CAcIFFileSystem::operator bool() const{
  /**/gMutex->lock();
  bool lReturnValue(this->operator()());
  /**/gMutex->unlock();
  return lReturnValue;
}
