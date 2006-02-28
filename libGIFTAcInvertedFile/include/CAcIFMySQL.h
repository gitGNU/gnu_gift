/* -*- mode: c++ -*- 
*/
#ifdef __GIFT_WITH_MYSQL__
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
* WM 150600 created the file
*
*
*
* compiler defines used:
*
*
*/

#ifndef _CACIFMYSQL
#define _CACIFMYSQL
#include "libGIFTAcInvertedFile/include/uses-declarations.h"
class CXMLElement;

#include "libGIFTAcInvertedFile/include/CAcInvertedFile.h"
#include "libGIFTAcURL2FTS/include/CAcURL2FTS.h"
#include <string>
#include "libMRML/include/TID.h"
#include "libMRML/include/CSelfDestroyPointer.h"
#include "libMRML/include/CArraySelfDestroyPointer.h"
#include "libGIFTAcInvertedFile/include/CDocumentFrequencyList.h"

#include "libGIFTAcInvertedFile/include/CADIHash.h"
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

#include <sqlplus.hh>
#include <custom.hh>
#include "libMRML/include/CMagic.h"


typedef TID TFeatureID ;

/**
   An accessor to an inverted file. This access is done
   "by hand" at present this not really efficient, however
   we plan to move to memory mapped files. 

 */

class documentFeatureConnection;

class CAcIFMySQL:public CAccessor{  
  /** In order to have just one parent,
      I have to limit on single inheritance.
      I cannot use virtual base classes, because then I
      cannot downcast
  */
  CSelfDestroyPointer<CAcURL2FTS> mURL2FTS;
  /** our connection to the document-feature database */
  documentFeatureConnection* mConnection;

  string GdbName;
  string GdbUser;
  string GdbHost;
  string GdbPassword;

  bool connection2dbOK;

  Connection *cnx;

  /**  */
  //vector <documentFeatureConnection> queryReturn;
  /**  */
  vector <documentFeatureConnection>::iterator It;
  /** this function was not asked for */
  CDocumentFrequencyList* featureList2docList(CDocumentFrequencyList inFeatureList);
  /** dto. */
  void showDocFreqList(CDocumentFrequencyList docFreqList);
  /* neither this */
  CDocumentFrequencyList* blob2docFreqList(string inBlobFile)const;

protected:
  int CAcIFMySQL::recordCount(string ltableName)const;
  bool CAcIFMySQL::selectDB( string);
  bool CAcIFMySQL::createDB( string);
  bool CAcIFMySQL::dropDB( string);
  void CAcIFMySQL::initialiseGIFTTables(bool, bool,string);


public:
  /** for testing if the inverted file is correctly constructed*/
  virtual bool operator()()const;

  /**  This opens an exsisting inverted file, and then 
       inits this structure. After that it is fully
       usable 

       As a paramter it takes an XMLElement which contains
       a "collection" element and its content.

       If the attribute vi-generate-inverted-file is true,
       then a new inverted file will be generated using
       the parameters given in inCollectionElement. you will
       NOT be able to use *this afterwards.

       The REAL constructor.
  */
  CAcIFMySQL(const CXMLElement& inCollectionElement);
  /** Destructor */
  ~CAcIFMySQL();

  /** <HIER-WIRDS-INTERESSANT-> */
  
  /** Translate a DocumentID to a URL (for output) */
  virtual string IDToURL(TID inID)const;

  /** Translate an URL to its document ID */
  virtual pair<bool,TID> URLToID(const string& inURL)const;
  
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
  virtual CDocumentFrequencyList* FeatureToList(TFeatureID inFID)const;

  /** List of features contained by a document with URL inURL*/
  virtual CDocumentFrequencyList* URLToFeatureList(string inURL)const;

  /** List of features contained by a document with ID inDID */
  virtual CDocumentFrequencyList* DIDToFeatureList(TID inDID)const;

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
  virtual double FeatureToCollectionFrequency(TFeatureID)const;

  /** What kind of feature is the feature with ID inFeatureID? */
  virtual unsigned int getFeatureDescription(TID inFeatureID)const;
  //@}

  /**@name Accessing additional document information*/
  //@{
  /**  returns the maximum document frequency for one document ID */
  virtual double DIDToMaxDocumentFrequency(TID)const;

  /**  Returns the document-frequency square sum for a given document ID */
  virtual double DIDToDFSquareSum(TID)const;

  /**  Returns this function for a given document ID */
  virtual double DIDToSquareDFLogICFSum(TID)const;
  //@}

  /*@name Inverted File Generation and Consistency Checking*/
  //@{

  /** Generating an inverted File, if there is none.
  virtual bool generateInvertedFile();
   */


  //@}

  /** Getting a list of all features contained in this.
      This function is necessary, because in the present 
      system only about 50 percent of the features are 
      really used.

      A feature is considered used if it arises in at least one image
   */
  void CAcIFMySQL::getAllIDs( list<TID > &) const;
  void CAcIFMySQL::getAllAccessorElements( list<CAccessorElement> &) const;
  void CAcIFMySQL::getRandomIDs(list<TID > &, size_t) const;
  void CAcIFMySQL::getRandomAccessorElements(list<CAccessorElement > &, size_t) const;
  virtual list<TID>* getAllFeatureIDs()const;
  /**
   *
   * Translate a DocumentID to an accessor Element
   *
   */
  virtual pair<bool,CAccessorElement> IDToAccessorElement(TID inID)const;

  int CAcIFMySQL::size() const;
  CAcIFMySQL::operator bool()const;

};


#endif
#endif
