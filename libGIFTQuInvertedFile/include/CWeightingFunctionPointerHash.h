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
/**
*
* 
*
*
*
* modification history:
*
* HM 090399 created the documentation
*
*
*
* compiler defines used:
*
*
*/
#ifndef _CWEIGHTINGFUNCTIONPOINTERHASH
#define _CWEIGHTINGFUNCTIONPOINTERHASH
#include "libGIFTQuInvertedFile/include/uses-declarations.h"
#include <map>
#include <algorithm>
#ifdef HAS_HASH_MAP
#include <hash_map>
#else
#define hash_map map
#endif

#include "libMRML/include/TID.h"
#include "libGIFTQuInvertedFile/include/CWeightingFunction.h"
#include "libGIFTQuInvertedFile/include/CWeightingFunctionPointerList.h"

/** 
    A hash of weighting function pointers
*/
class CWeightingFunctionPointerHash:public hash_map<TID,CWeightingFunctionPointer>{
protected:
  /** For making the change between different map types easier */
  typedef hash_map<TID,CWeightingFunctionPointer> CFather;
  /**  */
  const CWeightingFunction& mWeightingFunction;
public:
  /**
   *
   * 
   *
   */
  CWeightingFunctionPointerHash(const CWeightingFunction& inWF);
  
  /**
   *
   * 
   *
   */
  CWeightingFunctionPointerHash::CFather::size_type size()const;

  /** clears features before destroying this */
  ~CWeightingFunctionPointerHash();
  /**
   *
   */
  void clearFeatures();

  /**
   *
   * 
   *
   */
  void addFeature(const double inRelevanceLevel,
		  const CDocumentFrequencyElement& inElement);

};


#endif


