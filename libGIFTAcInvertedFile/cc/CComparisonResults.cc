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
* CComparisonResults
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

#include <algorithm>
#include "CBitVector.h"
#include "libGIFTAcInvertedFile/include/CComparisonResults.h"
#include "CIFQPicHunter.h"
#include "libGIFTAcInvertedFile/include/CDocumentFrequencyList.h"



//FIXME HERE IS A MEMORY LEAK. SOMEHOW THE WEIGHTING FUNCTION POINTER LIST/HASH MUST BE CHANGED TO PREVENT
//FROM DOUBLE DESCTRUCTION. THIS COULD BE AVOIDED BY MAKING THE WHOLE THING A LIST/HASH OF AUTO POINTERS. (CHECK THAT!)

/***************************************
*
* compare
*
****************************************
*
* modification history
*
* 
*
****************************************/
CBitVector CComparisonResults::compare(const CComparisonResults& inResults)const{
  CBitVector lReturnSet;
  {
    lCScoreSet::const_iterator i=inResults.mScores.begin();
    lCScoreSet::const_iterator j=mScores.begin();
    for(;
	(i!=inResults.mScores.end()) && (i!=mScores.end());
	i++,j++){
      if((*j).getDocumentFrequency()>(*j).getDocumentFrequency())
	lReturnSet.addBit((*i).getID());
    }
  }

  return lReturnSet;
};


/***************************************
*
* getWeighters
*
****************************************
*
* modification history
*
* 
*
****************************************/
CWeightingFunctionPointerHash& CComparisonResults::getWeighters(){
  return mWeighters;
}


/***************************************
*
* calculateImageScore
*
****************************************
*
* modification history
*
* 
*
****************************************/
double CComparisonResults::calculateImageScore(TID inDID,
					       const CDocumentFrequencyList& inList)const{
  return mContainer.FeatureListToScore(inDID,
				       inList,
				       mWeighters);
}

/***************************************
*
* addImageScore
*
****************************************
*
* modification history
*
* 
*
****************************************/
double CComparisonResults::addImageScore(TID inDID,
					 const CDocumentFrequencyList& inList)const{
  mScores.insert(CDocumentFrequencyElement(inDID,
					   calculateImageScore(inDID,
							       inList)));
}

/***************************************
*
* Constructor
*
****************************************
*
* modification history
*
* 
*
****************************************/
CComparisonResults::CComparisonResults(lCContainerType& inContainer,
				       const CWeightingFunction& inPrototype,
				       const TID inID,
				       const CRelevanceLevel& inQuery):
  mContainer(inContainer),
  mWeighters(inPrototype),
  mQueryImage(inQuery),
  mID(inID){
    mContainer.buildNormalizedQueryHash(inQuery,
					mWeighters);
};

void CComparisonResults::setRelevanceLevel(double inRL){
  mQueryImage.setRelevanceLevel(inRL);
}

TID CComparisonResults::getID()const{
  return mID;
}

CRelevanceLevel const& CComparisonResults::getQuery()const{
  return mQueryImage;
}













