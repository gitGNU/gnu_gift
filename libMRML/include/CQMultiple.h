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
#ifndef _CQMULTIPLE
#define _CQMULTIPLE


/* -*- mode: c++ -*- 
*/
/**
 *
 * CQMultiple.h
 * A CQuery structure which has several children
 * queries them one after another,
 * weights them, and obtains a common result
 *
 *
 *
 * modification history:
 *
 * WM 20000205 creation
 *
 *
 *
 * compiler defines used:
 * _CQMULTIPLE avoids double inclusion
 *
 */
#include <memory>
#include <map>
#include <list>
#include "CQuery.h"
#include "CSelfDestroyPointer.h"
#include "CIDRelevanceLevelPairList.h"
#include "CRelevanceLevelList.h"

//#include "CWeightingFunctionPointerList.h"
//#include "CWeightingFunctionPointerHash.h"

#include "CAlgorithm.h"
#include "CAccessor.h"
#include "CAccessorAdminCollection.h"
#include "CAccessorAdmin.h"

#include <hash_map>

class CScoreBoard;
class CAccessor;


/** 

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

    [in the following I am talking about things I WANT to do,
    so the two modes stuff is not yet implemented]

    CQMultiple has two minor modes:

    Merge-by-ID or Merge-by-URL

    In the first case, we need information on how to translate image 
    IDs to image URLs. We dispatche a <code>fastQuery()</code> to each
    child node, and then we merge the results (by imageID). The resulting
    list of ID-relevancelevel pairs is translated back to URLs using 
    an URL2FTS accessor.

    <em>Please note that I am aware that this needs refactoring:</em> we should
    have an ULRToID accessor superclass, which provides the necessary translation
    services, without being fixed on a given data representation.
    
    In the second case, we do not need any additional information: 
    we dispatch a <code>query()</code> (as opposed to <code>fastQuery()</code>)  
    to the child nodes, and then we merge the results. This means we have to 
    merge plenty of XML.
    *
    @Author: Wolfgang Müller

 */
class CQMultiple:public CQuery{
protected:

  /** 
      Do we merge the results by their URL or by their 
      image ID? 
  */

  /** */
  void init();

 public:
  /**
   *
   * default constructor
   * 
   */
  CQMultiple();
  /**
   *
   * we need to unregister the accessors used
   * 
   */
  ~CQMultiple();
  /**
   * In fact, what we are doing here is to get ourselves
   * an accessor ACURL2FTS to do a proper fastQuery
   */
  CQMultiple(CAccessorAdminCollection& inAccessorAdminCollection,
	     CAlgorithm&          inAlgorithm);
  
  /** This function processes is in the inner loop of
      fastQuery. If multithreading is possible on the system
      on which GIFT was compiled, then this function will
      run in a thread, and fastQuery will wait for it
  */
  static void doFastQueryThread(void *);
  /**
   *
   * calls fastQuery for every child, merges the results
   *
   */
  virtual CIDRelevanceLevelPairList* fastQuery(const CXMLElement& inQuery,
					       int inNumberOfInterestingImages,
					       double inDifferenceToBest);

  /**
   *
   * set the Algorithm.
   * same scheme as in setCollection
   *
   */
  virtual bool setAlgorithm(CAlgorithm& inAlgorithm);
  /** does what should be done in a query thread */
  static void * doQueryThread(void* inParameter);
}; /* end of class */

#endif

