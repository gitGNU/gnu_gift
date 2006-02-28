/* -*- mode: c++ -*- 
*/
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
* CCOMPARISONRESULTS
*
****************************************
*
* modification history:
*
* HM 090399 created the documentation
*
****************************************
*
* compiler defines used:
*
*
****************************************/
#ifndef _CCOMPARISONRESULTS
#define _CCOMPARISONRESULTS
#include "libGIFTAcInvertedFile/include/uses-declarations.h"
#include <list>
#include <set>
#include "libMRML/include/CRelevanceLevel.h"
#include "libGIFTQuInvertedFile/include/CWeightingFunctionPointerHash.h"
#include "libGIFTAcInvertedFile/include/CDocumentFrequencyElement.h"
//#include "CBitVector.h"

class CIFQPicHunter;
class CRelevanceLevel;
class CDocumentFrequencyList;
/** 

  The general scheme of implementing a PicHunter like algorithm
  in GIFT uses the following scheme:

  If you want a suggestion of <em>n<\em> elements
  get <em>2n<\em> elements. Call them the s_i.

  For the same <em>dbSize/10 (or something)</em> elements (which we will
  call the e_i) calculate the distance (s_i,d_j) for all i and j possible.
  (Leading to something like 500 comparisons in our case).

  For each s_i  the delta_{ij} will be stored in a structure of the type
  CComparisonResults

  */
class CComparisonResults{
  ///
  typedef CIFQPicHunter lCContainerType;
  ///
  CWeightingFunctionPointerHash mWeighters;
  ///
  lCContainerType& mContainer;
  ///
  typedef set<CDocumentFrequencyElement,CSortByID_DFE> lCScoreSet;
  ///
  mutable lCScoreSet mScores;
  ///
  CRelevanceLevel mQueryImage;
  ///
  TID mID;
  /**For giving non-temporary references back: I think this is a workaround, 
    but maybe also due to bad understanding*/
  mutable CBitVector mReturnSet;
public:
/***************************************
*
* 
*
***************************************/
  ///
  CComparisonResults(lCContainerType& inContainer,
		     const CWeightingFunction& inPrototype,
		     TID inID,
		     const CRelevanceLevel& inQuery);

/***************************************
*
* 
*
***************************************/
  ///
  double calculateImageScore(TID inDID,
			      const CDocumentFrequencyList& inList)const;

/***************************************
*
* 
*
***************************************/
  ///
  double addImageScore(TID inDID,
		       const CDocumentFrequencyList& inList)const;

/***************************************
*
* 
*
***************************************/
  ///
  CBitVector compare(const CComparisonResults& inResults)const;

  ///
  double getAsymmetry(const CComparisonResults& inResults)const;
  ///
  CWeightingFunctionPointerHash& getWeighters();
  ///
  void setRelevanceLevel(double);
  ///
  TID getID()const;
  ///
  CRelevanceLevel const& getQuery()const;
};

#endif


