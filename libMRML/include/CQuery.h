/* -*- mode: c++ -*- 
*/
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
// -*- mode: c++ -*- 

#include "CAccessorElement.h"
#include "CXMLElement.h"
/**
 *
 * CQuery.h 
 * (abstract) parent of query structures.
 * Goal of this structure is to process a query
 * and to digest within--session feedback
 * this tries not to contain anything application 
 * specific
 *
*
*
* modification history:
*
* WM 19990805 creation
*
*
*
* compiler defines used:
* _CQUERY avoids double inclusion
*
*/
#ifndef _CQUERY
#define _CQUERY
#include <memory>
#include <map>
#include "CSelfDestroyPointer.h"
#include "CIDRelevanceLevelPairList.h"
#include "CXMLElement.h"

//#include "CWeightingFunctionPointerList.h"
//#include "CWeightingFunctionPointerHash.h"

#include "CAlgorithm.h"
#include "CAccessor.h"
#include "CAccessorAdminCollection.h"
#include "CAccessorAdmin.h"
#include "CMagic.h" //public CMagic

class CScoreBoard;
class CAccessor;


/** The Query manager for Queries on inverted Files */
class CQuery:public CMagic{

protected:
  /**  
       needed to translate URLs to IDs 
       this is a pointer, only because we cannot change references 
       it does not imply any destruction responsability 
  */
  CAccessor* mAccessor;
  /** 
      This is where the the Accessor comes from.
      Also the AccessorAdmin is not to be deleted by this.
  */
  CAccessorAdmin* mAccessorAdmin;
  /**  
       Where to get CAccessorAdmins from.
	 
  */
  CAccessorAdminCollection* mAccessorAdminCollection;


  /**  the structure containing everything we know about the algorithm 
       used 
       do not destroy this
  */
  CAlgorithm* mAlgorithm;
  
  /** A child of this.
      We want to store the child together with a weight.
  */
  class lCChild{
  public:
    /** the child itself */
    CQuery* mQuery;
    /** the weight */
    double  mWeight;
  };
  /** type for children of this */
  typedef list<lCChild> lCChildren;
  /** The children of this.
   */
  lCChildren mChildren;
  
  

protected:
  /**
   *
   * Initializer, used by both construcors
   *
   */
  virtual void init()=0;
    
public:
  /**
   *
   * default constructor
   * 
   */
  CQuery();
  /**
   *
   * destructor
   * 
   */
  virtual ~CQuery();
  /**
   *
   * this constructor takes all the information ANY CQuery needs
   * to configure itself.
   *
   * inAccessorAdminCollection The CQuery needs to know where to get the right
   *                      accessor from.
   * inAlgorithm          This structure contains all the information
   *                      about the algorithm, including which accessor
   *                      to get.
   * 
   */
  CQuery(CAccessorAdminCollection& inAccessorAdminCollection,
	 CAlgorithm&          inAlgorithm);
    
  /** adding a child to this. We expect children to be 
      nonzero and initialised when they are entered. 
      *this assumes no destruction responsabilities for the entered
      data. 
   */
  void addChild(CQuery* inChild,
		double inWeight=1);


  /**
   *
   * do a query
   *
   */
  virtual CXMLElement* query(const CXMLElement& inQuery);
  
  /**
   *
   * @short a query which returns ID/RelevanceLevel pairs instead of
   * instead of URL/RelevanceLevel pairs
   *
   */
  virtual CIDRelevanceLevelPairList* fastQuery(const CXMLElement& inQuery,
					       int inNumberOfInterestingImages,
					       double inDifferenceToBest)=0;

  /**
   *
   * get some random images (handed through to accessor)
   *
   */
  virtual CXMLElement* getRandomImages(int inNumberOfInterestingImages)const;
  /**
   *
   * get some random images (handed through to accessor)
   *
   */
  virtual CIDRelevanceLevelPairList* getRandomIDs(int inNumberOfInterestingImages)const;
  /**
   *
   * get the IDs of all images (handed through to accessor)
   *
   */
  virtual list<TID>* getAllIDs()const;
  /**
   *
   * get the IDs of all images (handed through to accessor)
   *
   */
  virtual list<CAccessorElement>* getAllAccessorElements()const;
  /**
   *
   * set the Algorithm.
   * set a new algorithm. Build a new query if necessary.
   *
   */
  virtual bool setAlgorithm(CAlgorithm& inAlgorithm);
  /**
   *
   * get the Algorithm.
   * same scheme as in setCollection
   *
   */
  const CAlgorithm& getAlgorithm()const;

  /**
     it might be necessary to wait until all the children
     are added before ending the initialisation phase.

     This function is called by CAlgorithm.

   */
   virtual void finishInit();
     
}; /* end of class */

#endif

