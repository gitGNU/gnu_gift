// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

     Copyright (C) 2003, 2004 Bayreuth University
    Copyright (C) 2005,2006 Bamberg University
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
* CAcIFMeta - An accessor containing multiple inverted file accessors
*
****************************************
*
* modification history:
* 
* WM 20070703 created
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


#include <cstring>
#include <cerrno> //show which error makes reads fail!

#include <unistd.h>     // for getpid
#include "libMRML/include/mrml_const.h" // for parsing
#include <unistd.h>
#include <cstdio>       // for sprintf
#include <sstream>
#include <string>
#include <set> 
#include <algorithm>
#include <functional>

#include "libMRML/include/my_assert.h"

#include "libGIFTAcInvertedFile/include/merge_sort_streams.h"    //newGenerateInvertedFile
#include "libGIFTAcInvertedFile/include/CIFBuilderTriplet.h"     //newGenerateInvertedFile
#include "libGIFTAcInvertedFile/include/CDocumentFrequencyList.h"
#include "libGIFTAcInvertedFile/include/CAcIFFileSystem.h"
#include "libGIFTAcInvertedFile/include/CAcIFMeta.h"
#include "libGIFTAcInvertedFile/include/CInvertedFileChunk.h"
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include "libGIFTAcInvertedFile/include/CIFListStart.h"
#include "libMRML/include/CXMLElement.h" // constructor
#include "libMRML/include/directory.h"   // the install locations etc. as determined by ./configure
#include <time.h> // for adding unknown images.
#define _NO_PRINT_OFFSET_CHECK
#define _NO_CHECK_OFFSET_FILE
#define _NO_CHECK_CONSISTENCY
#define _NO_CONSISTENCYPRINT
#define _NO_FIDPRINT
#define _NO_DIDPRINT
#define	_NO_PRINT_ID
#define	_NO_PRINT_INIT

// needed for stat-ing
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



extern class CMutex* gMutex;

/** used for reading the offset file */
typedef struct{
  unsigned int mUInt1;
  unsigned int mUInt2;
} SUIntUInt;

/** used for reading the offset file */
typedef struct{
  unsigned int mUInt1;
  //unsigned int mUInt2;
  streampos mStreampos;
} SUIntStreampos;


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
bool CAcIFMeta::operator()()const{
};

 
/***************************************
*
* Generate an inverted file <b>if necessary</b>
*
****************************************/
bool CAcIFMeta::generateInvertedFile(){
}

/***************************************
*
* Generate an inverted file <b>if necessary</b>
*
****************************************
*
* modification history
*
* 
*
****************************************/
bool CAcIFMeta::newGenerateInvertedFile(){
}




/***************************************
*
* Constructor
*
****************************************/
CAcIFMeta::CAcIFMeta(const CXMLElement& inCollectionElement){
}


/***************************************
*
* init - initialization needed for the inverted file
*
****************************************/
bool CAcIFMeta::init(bool inMemory){
};



/***************************************
*
* FeatureToCollectionFrequency - Returns the collection frequency for a given feature
*
****************************************/
double CAcIFMeta::
FeatureToCollectionFrequency(TFeatureID inFeatureID)const{
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
double CAcIFMeta::DIDToMaxDocumentFrequency(TID inID)const{
};



/***************************************
*
* DIDToDFSquareSum - returns the Document frequency squaresum for a given document ID
*
****************************************/
/// returns the Document frequency squaresum for a given document ID 
double CAcIFMeta::DIDToDFSquareSum(TID inID)const{
}



/***************************************
*
* DIDToSquareDFLogICFSum - returns this function for a given document ID
*
****************************************/
double CAcIFMeta::DIDToSquareDFLogICFSum(TID inID)const{
};
  


/***************************************
*
* FeatureToList - returns a list of document frequencies (documents) for a given feature ID
*
****************************************/
CDocumentFrequencyList* CAcIFMeta::
FeatureToList(TFeatureID inFeatureID)const{
};




/***************************************
*
* findWithinStream - needed for checking the consistency
*
****************************************/
///needed for checking the consistency
bool CAcIFMeta::findWithinStream(TID inFeatureID,
				       TID inDocumentID,
				       double inDocumentFrequency)const{
};




/***************************************
*
* checkConsistency - checks if the data in the inverted file is the same as in the single files
*
*
****************************************/
bool CAcIFMeta::checkConsistency(){

}





/***************************************
*
* URLToFeatureList -  
* returns the feature list (with document IDs) for a given URL
*
****************************************/
CDocumentFrequencyList* 
CAcIFMeta::URLToFeatureList(string inURL)const{};



CDocumentFrequencyList* CAcIFMeta::getFeatureFile(string inFileName)const{
}

/***************************************
*
* DIDToFeatureList - returns the feature list for a special image with a document ID
*
****************************************/
CDocumentFrequencyList* 
CAcIFMeta::DIDToFeatureList(TID inDID)const{

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
string CAcIFMeta::IDToURL(TID inID)const{};



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
unsigned int CAcIFMeta::getFeatureDescription(TID inID)const{

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
CAcIFMeta::~CAcIFMeta(){
  cout << "CAcIFMeta::~CAcIFMeta() called "
       << endl
       << flush;
  /**/gMutex->lock();
  /**/gMutex->unlock();
};

TID CAcIFMeta::getMaximumFeatureID()const{

};

/** 
    Getting a list of all features contained in this
*/
list<TID>* CAcIFMeta::getAllFeatureIDs()const{
}

/** List of the IDs of all documents present in the inverted file */
void CAcIFMeta::getAllIDs(list<TID>& outIDList)const{

};
/** List of triplets (ID,imageURL,thumbnailURL) of all
    the documents present in the inverted file */
void CAcIFMeta::getAllAccessorElements(list<CAccessorElement>& outAccessorElementList)const{
};
/** get a given number of random AccessorElement's 
    @param inoutResultList the list which will contain the result
    @param inSize          the desired size of the inoutResultList
*/
void CAcIFMeta::getRandomIDs(list<TID>& outRandomIDList,
				   list<TID>::size_type inSize)const{
};
/** For drawing random sets. Why is this part of an CAccessorImplementation?
    The way the accessor is organised might influence the way
    random sets can be drawn. At present everything happens in
    RAM, but we do not want to be fixed on that.
    
      @param inoutResultList the list which will contain the result
      @param inSize          the desired size of the inoutResultList
*/
void CAcIFMeta::getRandomAccessorElements(list<CAccessorElement>& outResult,
						list<CAccessorElement>::size_type inSize)const{
};
/**
 *
 * Translate a DocumentID to an accessor Element
 *
 */
pair<bool,CAccessorElement> CAcIFMeta::IDToAccessorElement(TID inID)const{
};
/** The number of images in this accessor */
int CAcIFMeta::size()const{};
/** The number of images in this accessor */
pair<bool,TID> CAcIFMeta::URLToID(const string& inURL)const{};

CAcIFMeta::operator bool() const{

}
