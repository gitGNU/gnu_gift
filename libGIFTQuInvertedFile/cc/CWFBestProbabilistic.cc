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
* CWFBestProbabilistic
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
#include "libGIFTQuInvertedFile/include/CWFBestProbabilistic.h"
#include <math.h>
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
CWFBestProbabilistic::
CWFBestProbabilistic(const CAcInvertedFile* inAccessor=0,
		     CQueryNormalizer* inQueryNormalizer=0,
		     CQueryNormalizer* inThisNormalizer=0):
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
double CWFBestProbabilistic::subApply(const double inDocumentFrequency,
				      const double inMaxDocumentFrequency)const{

#ifdef PRINT
  cout << "WFFBestProbabilistic" << endl;
#endif
  TID lFeatureID=mID;
  double lCollectionFrequency=
    mAccessor->FeatureToCollectionFrequency(lFeatureID);

  double lCollectionFrequencyFactor=log(1/lCollectionFrequency-0.999);

  //  lCollectionFrequencyFactor=1;
  
  if((mFeatureDescription==COL_HST)
     ||
     (mFeatureDescription==GABOR_HST)){
    lCollectionFrequencyFactor=1;
  }
  
  assert(inMaxDocumentFrequency>0);

  return 
    //document term weight
    (0.5+
     0.5*inDocumentFrequency
     /inMaxDocumentFrequency
     )
    //query term weight
    *lCollectionFrequencyFactor
    *CWeightingFunction::subApply(inDocumentFrequency,
				  1);
}



/***************************************
*
* apply
*
****************************************
*
* modification history
*
* 
*
****************************************/
double CWFBestProbabilistic::apply(const CDocumentFrequencyElement& 
				   inResultFeature)const{
  
  TID lDocumentID=inResultFeature.getID();

  return subApply(inResultFeature.getDocumentFrequency(),
		  mAccessor->DIDToMaxDocumentFrequency(lDocumentID));
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
CWeightingFunction* CWFBestProbabilistic::clone()const{
  CWeightingFunction* lRetVal=new CWFBestProbabilistic(mAccessor,
						       mQueryNormalizer,
						       mThisNormalizer);
  return lRetVal;
};

