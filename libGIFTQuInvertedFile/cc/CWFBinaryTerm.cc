// -*- mode: c++ -*- 
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
/***************************************
*
* CWFBinaryTerm
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

#include "libGIFTQuInvertedFile/include/CWFBinaryTerm.h"
#include <cmath>
#include "FeatureExtraction/gift_features.h"


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
CWFBinaryTerm::
CWFBinaryTerm(const CAcInvertedFile* inAccessor,
	      CQueryNormalizer* inQueryNormalizer,
	      CQueryNormalizer* inThisNormalizer):
  CWeightingFunction(inAccessor,
		     inQueryNormalizer,
		     inThisNormalizer)
{
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
double CWFBinaryTerm::subApply(const double inDocumentFrequency,
			       const double inNormalizationFactor)const{
#ifdef PRINT
cout << "CWFBinaryTerm" <<endl;
#endif
  TID lFeatureID=mID;
  
  double lCollectionFrequency=
    mAccessor->FeatureToCollectionFrequency(lFeatureID);

  if((mFeatureDescription==COL_HST)
     ||
     (mFeatureDescription==GABOR_HST)){
    return CWeightingFunction::subApply(inDocumentFrequency,
					inNormalizationFactor);
  }else{
    return 
      CWeightingFunction::subApply(inDocumentFrequency,
				inNormalizationFactor)
      *
      log(1/lCollectionFrequency-0.9999);
  }
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
CWeightingFunction* CWFBinaryTerm::clone()const{
  CWeightingFunction* lRetVal=new CWFBinaryTerm(mAccessor,
						mQueryNormalizer,
						mThisNormalizer);
  return lRetVal;
};


