// -*- mode: c++ -*- 
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
/***************************************
*
* CWFClassicalIDF
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
#include "CWFClassicalIDF.h"
#include "gift_features.h"
#include <math.h>

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
CWFClassicalIDF::
CWFClassicalIDF(const CAcInvertedFile* inAccessor=0,
		CQueryNormalizer* inQueryNormalizer=0,
		CQueryNormalizer* inThisNormalizer=0):
  CWeightingFunction(inAccessor,
		     inQueryNormalizer,
		     inThisNormalizer)
{
}



/***************************************
*
* preCalculate
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CWFClassicalIDF::preCalculate(){

  CWeightingFunction::preCalculate();

  mQueryFactor=
    mQueryFactor
    *
    log(mAccessor->FeatureToCollectionFrequency(mID))
    *
    log(mAccessor->FeatureToCollectionFrequency(mID));
    
}



/***************************************
*
* subApply
*
****************************************
*
* modification history
*
* 
*
****************************************/
double CWFClassicalIDF::subApply(const double inDocumentFrequency,
				 const double inNormalizingFactor)const{
#ifdef PRINT
  cout << "WFClassic" << endl;
#endif
  TID lFeatureID=mID;
  
  /* to test the behaviour before the code optimization */
  /*  double lQueryFactor=log(mAccessor->FeatureToCollectionFrequency(mID))*
                      log(mAccessor->FeatureToCollectionFrequency(mID));*/


  if((mFeatureDescription==COL_HST)
     ||
     (mFeatureDescription==GABOR_HST))
    return CWeightingFunction::subApply(inDocumentFrequency,
					inNormalizingFactor);
  else
    return mQueryFactor;
}



/***************************************
*
* clone
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
CWeightingFunction* CWFClassicalIDF::clone()const{
  CWeightingFunction* lRetVal=new CWFClassicalIDF(mAccessor,
						  mQueryNormalizer,
						  mThisNormalizer);
  return lRetVal;
};

