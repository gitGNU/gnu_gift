/* -*- mode: c++ -*- 
*/
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
// -*- mode: c++ -*-


class CXMLElement;

/*
*
* 
*  This class manages the access to the inverted file as well 
*    as its generation
*
*
*
* modification history:
*
* WM   1099 changed documentation format
*           completed documentation
* HM 090399 created the documentation
* WM   1098 created the file
*
*
*
* compiler defines used:
*
*
*/

#ifndef _CACIFFILESYSTEM
#define _CACIFFILESYSTEM

#include <string>
#include "TID.h"
#include "CSelfDestroyPointer.h"
#include "CArraySelfDestroyPointer.h"
#include "CDocumentFrequencyList.h"
#include "CMutex.h" // multi threading
//#include "CCollectionFrequencyList.h"
#include "CADIHash.h"
#include "CAcURL2FTS.h"
#include "CAcInvertedFile.h"
#include <iostream.h>
#include <fstream.h>
#include <map>
#include <vector>
#include <hash_map>
#include <functional>
#include <algorithm>

#include "CMagic.h"


typedef TID TFeatureID ;

/**
   An accessor to an inverted file. This access is done
   "by hand".

   For a long time we wanted to move to memory mapped files (like SWISH++)
   but currently I think this is not the best idea. 

   

 */
class CAcIFFileSystem:public CAcInvertedFile{  

protected:
  /** the mutex for multi threading */
  CMutex mMutex;
  /** In order to have just one parent,
      I have to limit on single inheritance.
      I cannot use virtual base classes, because then I
      cannot downcast
  */
  CSelfDestroyPointer<CAcURL2FTS> mURL2FTS;
  /** the maximum feature ID arising in this file */
  TID mMaximumFeatureID;
  /** A buffer, if the inverted file is to be 
      held in ram */
  CArraySelfDestroyPointer<char> mInvertedFileBuffer;
  /** The inverted file */
  mutable CSelfDestroyPointer<istream> mInvertedFile;

  /** Feature -> Offset in inverted file */
  mutable ifstream mOffsetFile;

  /** File of feature descriptions */
  ifstream mFeatureDescriptionFile;

  /** Name of the inverted file */
  string mInvertedFileName;

  /** Name of the Offset file */
  string mOffsetFileName;

  /** Name for the file with the feature description */
  string mFeatureDescriptionFileName;

  /** map from feature id to the offset for this feature */
  typedef hash_map<TID,unsigned int> CIDToOffset;//new hash
  /** map from feature id to the offset for this feature */
  CIDToOffset mIDToOffset;

  /** map from feature to the collection frequency */
  mutable hash_map<TID,double> mFeatureToCollectionFrequency;//new hash

  /**@name for fast access...*/
  //@{
  /**  map from the feature ID to the feature description */
  hash_map<TID,unsigned int> mFeatureDescription;//new hash_

  /**  additional information about the document like, e.g.
       the euclidean length of the feature list.
   */
  CADIHash mDocumentInformation;
  //@}
  /** add a pair of FeatureID,Offset to the open offset file 
      (helper function for inverted file construction)
   */
  void writeOffsetFileElement(TID inFeatureID,
			      int inPosition,
			      ostream& inOpenOffsetFile);
  /** loads a *.fts file. and returns the feature list*/
  CDocumentFrequencyList* getFeatureFile(string inFileName)const;
public:
  /** for testing if the inverted file is correctly constructed*/
  bool operator()()const;

  /**  This opens an exsisting inverted file, and then 
       inits this structure. After that it is fully
       usable 

       As a paramter it takes an XMLElement which contains
       a "collection" element and its content.

       If the attribute cui-generate-inverted-file is true,
       then a new inverted file will be generated using
       the parameters given in inCollectionElement. you will
       NOT be able to use *this afterwards.

       Like every accessor, this accessor takes a <collection />
       MRML element as input (@see CXMLElement for how to access 
       the attributes of this element). Currently this accessor
       understands  the following attributes

       cui-base-dir:      the directory containing the following files
       cui-inverted-file-location: the location of the inverted file
       cui-offset-file-location:   a file containing offsets into the
                                   inverted file
       cui-feature-file-location:  the location of the "url2fts" file
                                   which translates urls to feature
				   file names.

  */
  CAcIFFileSystem(const CXMLElement& inCollectionElement);
  /**  called by constructors */
  bool init(bool);

  /** Destructor */
  ~CAcIFFileSystem();
  
  /** Translate a DocumentID to a URL (for output) */
  string IDToURL(TID inID)const;

  /**@name The proper inverted file access*/
  //@{
  /** List of documents containing the feature */
  CDocumentFrequencyList* FeatureToList(TFeatureID)const;

  /** List of features contained by a document */
  CDocumentFrequencyList* URLToFeatureList(string inURL)const;

  /** List of features contained by a document with ID inDID */
  CDocumentFrequencyList* DIDToFeatureList(TID inDID)const;

  //@}


  /**@name Accessing information about features*/
  //@{
  /** Collection frequency for a given feature */
  double FeatureToCollectionFrequency(TFeatureID)const;

  /** What kind of feature is the feature with ID inFeatureID? */
  unsigned int getFeatureDescription(TID inFeatureID)const;
  //@}

  /**@name Accessing additional document information*/
  //@{
  /**  returns the maximum document frequency for one document ID */
  double DIDToMaxDocumentFrequency(TID)const;

  /**  Returns the document-frequency square sum for a given document ID */
  double DIDToDFSquareSum(TID)const;

  /**  Returns this function for a given document ID */
  double DIDToSquareDFLogICFSum(TID)const;
  //@}

  /*@name Inverted File Generation and Consistency Checking*/
  //@{

  /** Generating an inverted File, if there is none.
      Fast but stupid in-memory method. This method is 
      very fast, if all the inverted file (and a bit more)
      can be kept in memory at runtime. If this is not the 
      case, extensive swapping is the result, virtually halting
      the inverted file creation.
   */
  bool generateInvertedFile();

  /** Generating an inverted File, if there is none.
      
      Employing the two-way-merge method described
      in "managing gigabytes", chapter 5.2. Sort-based
      inversion. (Page 181)

   */
  bool newGenerateInvertedFile();

  /**Check the consistency of the inverted file system accessed
     by this accessor.*/
  bool checkConsistency();

  /**Is the Document with inDocumentID contained in the 
    document frequency list of the feature inFeatureID and
    is the associated document frequency the same?
    
    @param inFeature<id the 
  */
  bool findWithinStream(TID inFeatureID,
			TID inDocumentID,
			double inDocumentFrequency)const;
  
  //@}

  /**
   *
   * Translate an URL to its document ID
   *
   */
  virtual pair<bool,TID> URLToID(const string& inURL)const;
  
  /** List of the IDs of all documents present in the inverted file */
  void getAllIDs(list<TID>&)const;
  /** List of triplets (ID,imageURL,thumbnailURL) of all
      the documents present in the inverted file */
  void getAllAccessorElements(list<CAccessorElement>&)const;
  /** get a given number of random C-AccessorElement-s 
      @param inoutResultList the list which will contain the result
      @param inSize          the desired size of the inoutResultList
  */
  void getRandomIDs(list<TID>&,
		    list<TID>::size_type)const;
  /** For drawing random sets. Why is this part of an CAccessorImplementation?
      The way the accessor is organised might influence the way
      random sets can be drawn. At present everything happens in
      RAM, but we do not want to be fixed on that.

      @param inoutResultList the list which will contain the result
      @param inSize          the desired size of the inoutResultList
   */
  void getRandomAccessorElements(list<CAccessorElement>& outResult,
				  list<CAccessorElement>::size_type inSize)const;
  /** The number of images in this accessor */
  int size()const;
  //@}
  /** This is interesting for browsing*/
  TID getMaximumFeatureID()const;
  /** Getting a list of all features contained in this.
      This function is necessary, because in the present 
      system only about 50 percent of the features are 
      really used.

      A feature is considered used if it arises in mIDToOffset.
   */
  list<TID>* getAllFeatureIDs()const;
  /**
   *
   * Translate a DocumentID to an accessor Element
   *
   */
  virtual pair<bool,CAccessorElement> IDToAccessorElement(TID inID)const;
  /** is this well constructed? */
  operator bool()const;

};

#endif
