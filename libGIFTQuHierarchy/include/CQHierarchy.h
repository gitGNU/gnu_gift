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
#ifndef _CQHIERARCHY
#define _CQHIERARCHY
#include "../include/uses-declarations.h"

/* -*- mode: c++ -*- 
*/
/**
 *
 * CQHierarchy.h
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
 * _CQHIERARCHY avoids double inclusion
 *
 */
#include <memory>
#include <map>
#include <list>
#include "libMRML/include/CQuery.h"
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

    This is a simple hierarchy based browser.
    

    @Author: Wolfgang Müller

 */
class CQHierarchy:public CQuery{
protected:

  /** The current position in the hierarchy */
  list<TID> mCurrentPath;
  /** sets mCurrentPosition to 0*/
  void init();

 public:
  /**
   *
   * default constructor
   * 
   */
  CQHierarchy();
  /**
   *
   * we need to unregister the accessors used
   * 
   */
  ~CQHierarchy();
  /**
   * In fact, what we are doing here is to get ourselves
   * an accessor ACURL2FTS to do a proper fastQuery
   */
  CQHierarchy(CAccessorAdminCollection& inAccessorAdminCollection,
	     CAlgorithm&          inAlgorithm);
    
  /**
   *
   * enables moving in the hierarchy. You can either move down 
   * (i.e. you select an image) or up in the hierarchy (by getting
   * a cui-hierarchy-move-up signal as query).
   * RICHLY COMMENTED IN THE *.cc FILE.
   *
   *@param inQuery an CXMLElement containing a query-step element
   *@param inNumberOfInterestingImages preferred number of result images (ignored)
   *@param inDifferenceToBest preferred relevance cutoff (ignored)
   */
  virtual CIDRelevanceLevelPairList* fastQuery(const CXMLElement& inQuery,
					       int inNumberOfInterestingImages,
					       double inDifferenceToBest);

  /**
   *
   * set the Algorithm.
   *
   */
  virtual bool setAlgorithm(CAlgorithm& inAlgorithm);

}; /* end of class */

#endif

