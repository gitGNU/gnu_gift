/* -*- mode: c++ -*- 
*/
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

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

/**
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

#ifndef _CINVERTEDFILEACCESSOR
#define _CINVERTEDFILEACCESSOR
#include "libGIFTAcInvertedFile/include/uses-declarations.h"
#include <string>
#include "libMRML/include/TID.h"
#include "libMRML/include/CAccessor.h"
#include "libGIFTAcInvertedFile/include/CDocumentFrequencyList.h"
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#ifdef HAS_HASH_MAP
#include <hash_map>
#else
#define hash_map map
#endif

#include <functional>
#include <algorithm>

#include "libMRML/include/CMagic.h"


typedef TID TFeatureID ;

/**
   An accessor to an inverted file. This access is done
   "by hand" at present this not really efficient, however
   we plan to move to memory mapped files. 

 */
class CAcInvertedFile:public CAccessor{  

public:
  /** for testing if the inverted file is correctly constructed*/
  virtual bool operator()()const =0;

  /** <HIER-WIRDS-INTERESSANT-> */
  
  /** Translate a DocumentID to a URL (for output) */
  virtual string IDToURL(TID inID)const =0;

  /** Translate an URL to its document ID */
  virtual pair<bool,TID> URLToID(const string& inURL)const =0;
  
  /**@name The proper inverted file access*/
  //@{
  /** Give the List of documents containing the feature inFID 


      CORNELIA:
      CDocumentFrequencyList ist nichts anderes als eine liste von
      
      int,float paaren:

      struct{
      int mID,
      float mFrequency;
      }

   */
  virtual CDocumentFrequencyList* FeatureToList(TFeatureID inFID)const =0;

  /** List of features contained by a document with URL inURL*/
  virtual CDocumentFrequencyList* URLToFeatureList(string inURL)const =0;

  /** List of features contained by a document with ID inDID */
  virtual CDocumentFrequencyList* DIDToFeatureList(TID inDID)const =0;

  /** </HIER-WIRDS-INTERESSANT-> */

  /**  For drawing random sets mainly a translation of  
       getRandomURLs

       CORNELIA:
       IM MOMENT KOENNEN WIR DIESE FUNKTION VERGESSEN

  virtual void getRandomRLLs(list<string>&,
			     list<string>::size_type)const;
  */
  //@}


  /**@name Accessing information about features*/
  //@{
  /** Collection frequency for a given feature */
  virtual double FeatureToCollectionFrequency(TFeatureID)const =0;

  /** What kind of feature is the feature with ID inFeatureID? */
  virtual unsigned int getFeatureDescription(TID inFeatureID)const =0;
  //@}

  /**@name Accessing additional document information*/
  //@{
  /**  returns the maximum document frequency for one document ID */
  virtual double DIDToMaxDocumentFrequency(TID)const =0;

  /**  Returns the document-frequency square sum for a given document ID */
  virtual double DIDToDFSquareSum(TID)const =0;

  /**  Returns this function for a given document ID */
  virtual double DIDToSquareDFLogICFSum(TID)const =0;
  //@}

  /*@name Inverted File Generation and Consistency Checking*/
  //@{

  /** Generating an inverted File, if there is none.
   */
  virtual bool generateInvertedFile() =0;

  /**Check the consistency of the inverted file system accessed
     by this accessor.*/
  virtual bool checkConsistency() =0;

  //@}

  /** Getting a list of all features contained in this.
      This function is necessary, because in the present 
      system only about 50 percent of the features are 
      really used.

      A feature is considered used if it arises in at least one image
   */
  virtual list<TID>* getAllFeatureIDs()const =0;
};

#endif
