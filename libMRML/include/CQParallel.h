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
#ifndef _CQPARALLEL
#define _CQPARALLEL

#include "libMRML/include/CQuery.h"
#include "libMRML/include/CSelfDestroyPointer.h"
// -*- mode: c++ -*- 
/**
 *
 * CQParallel.h
 * A CQuery structure which has several children
 * and uses them in parallel
 * PARALLEL THIS IS NOT YET DONE
 *
 *
 *
 * modification history:
 *
 * WM 19991125 creation
 *
 *
 *
 * compiler defines used:
 * _CQPARALLEL avoids double inclusion
 *
 */
#include <memory>
#include <map>
#include <list>
#include "libMRML/include/CSelfDestroyPointer.h"
#include "libMRML/include/CIDRelevanceLevelPairList.h"
#include "libMRML/include/CRelevanceLevelList.h"

//#include "CWeightingFunctionPointerList.h"
//#include "CWeightingFunctionPointerHash.h"

#include "libMRML/include/CAlgorithm.h"
#include "libMRML/include/CAccessor.h"
#include "libMRML/include/CAccessorAdminCollection.h"
#include "libMRML/include/CAccessorAdmin.h"


class CScoreBoard;
class CAccessor;


/** 

    FIXME: The name WRONGLY suggests that this actually does parallel
    processing. It should be relatively quick to add, it is intendend,
    but it is not yet done.

    This is going to be one of our main building blocks.
    It is a structure which contains a couple of 
    CQuery structures, hands a query through to them,
    and then unifies the result. In fact this is the center of 
    all this query tree business.

    Probably we will put another layer into the class tree:
    The CQTreeNode, but let's first start.

    Important: The basic assumption here is, that all children 
    operate on the same collections. If this is not the case we 
    have to be more careful, and most of all: we have to operate using
    URLs.

    @Author: Wolfgang Müller

 */
class CQParallel:public CQuery{

  /** We need this because we cannot use this just when
      constructing, but we have to use it in finishInit 
  */
  CAccessorAdminCollection& mAccessorAdminCollection;

  /** used in CLocalAccessorMap */
  class lCLocalAccessor{
  public:
    /** */
    CAccessor* mAccessor;
    /** */
    CAccessorAdmin* mAccessorAdmin;
    /** */
    list<lCChildren::iterator> mUsedBy;
    /** */
    CSelfDestroyPointer<map<TID,double> > mUnifiedResults;
    /** */
    double mWeightedSum;
    /** the constructor initialises everything 
	in the simplest possible way */
    lCLocalAccessor();
    /** the destructor just frees the accessor 
	by calling close on the proxy.
	*/
    ~lCLocalAccessor();
  }; 


  /** 

      Main idea of this class is to translate URLs from each 
      collection involved into IDs.
      For this I need an accessor for each collection. Some of these
      collections might have the same accessors.

      So I look at the algorithm of each child
      to find out which accessor to use 
      for which fast query.

      In the end I can iterate over collections, unify each result,
      and then have a unification of results based on URLs in the end.
  */
  class lCLocalAccessorMap:public map<string,lCLocalAccessor>{
  public:
  };
  /** do not confuse with mAccessor.
      This is the accessor list we talked about before */
  lCLocalAccessorMap mAccessors;
  

protected:
  /**
   *
   * Initializer, used by both construcors
   *
   */
  virtual void init();
    
public:
  /**
   *
   * we need to unregister the accessors used
   * 
   */
  ~CQParallel();
  /**
   * In fact, what we are doing here is to get ourselves
   * an accessor ACURL2FTS to do a proper fastQuery
   */
  CQParallel(CAccessorAdminCollection& inAccessorAdminCollection,
	     CAlgorithm&          inAlgorithm);
    
  /**
   *
   * calls fastQuery for every child, merges the results
   * and translates them back into URLs
   * difficulty: take into account that each child might operate
   * on a different collection.
   *
   */
  virtual CXMLElement* query(const CXMLElement& inQuery,
			     int inNumberOfInterestingImages,
			     double inDifferenceToBest);

  /**
   * FIXME: useless in current scenario.
   *
   * Generally using IDs instead of URLs (for keeping communication
   * overhead low, when doing things like CORBA).
   *
   * in this case we simply hand things through.
   * this might be some kind of "take it and translate it" thing.
   * however, at present we save the work.
   * 
   */
  virtual CIDRelevanceLevelPairList* fastQuery(const CXMLElement& inQuery,
					       int inNumberOfInterestingImages,
					       double inDifferenceToBest);

  /**
   *
   * get some random images 
   *
   */
  virtual CXMLElement* getRandomImages(int inNumberOfInterestingImages)const;
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
  virtual list<string>* getAllURLs()const;
  /**
   *
   * get the IDs of all images (handed through to accessor)
   *
   */
  virtual list<pair<TID,string> >* getAllIDURLPairs()const;
    
  /**
   *
   * set the Algorithm.
   * same scheme as in setCollection
   *
   */
  virtual bool setAlgorithm(CAlgorithm& inAlgorithm);

  /**
     it might be necessary to wait until all the children
     are added before ending the initialisation phase.

     This function is called by CAlgorithm.

     What we are doing here is to find out the necessary 
     accessor information for each child, and construct an
     accessor for it.

   */
   void finishInit();
     
}; /* end of class */

#endif



