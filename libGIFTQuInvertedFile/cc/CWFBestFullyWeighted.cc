// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

     Copyright (C) 2003, 2004 Bayreuth University
      2005 Bamberg University
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
* CWFBestFullyWeighted
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
#include "libGIFTQuInvertedFile/include/CWFBestFullyWeighted.h"
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
CWFBestFullyWeighted::
CWFBestFullyWeighted(const CAcInvertedFile* inAccessor,
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
double CWFBestFullyWeighted::subApply(double inDocumentFrequency,
				      double inSquareDFLogICFSum)const{
  TID lFeatureID=mID;
  
  double lCollectionFrequency=
    mAccessor->FeatureToCollectionFrequency(lFeatureID);

  double lLogCF=-log(lCollectionFrequency);
  
  if((mFeatureDescription==COL_HST)
     ||
     (mFeatureDescription==GABOR_HST))
    lLogCF=
      1
      /
      inDocumentFrequency;
  
  return 
    //document term weight
    (inDocumentFrequency*lLogCF)
    /
    sqrt(inSquareDFLogICFSum)
    //query term weight
      *(0.5+
	(0.5*getTermFrequency()
	 /mQueryNormalizer->getValue())
	)
      *
      lLogCF
      *
      CWeightingFunction::subApply(inDocumentFrequency,
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
double CWFBestFullyWeighted::apply(const CDocumentFrequencyElement& inResultFeature)const{
  return subApply(inResultFeature.getDocumentFrequency(),
		  mAccessor
		  ->
		  DIDToSquareDFLogICFSum(inResultFeature.getID()));
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
CWeightingFunction* CWFBestFullyWeighted::clone()const{
  CWeightingFunction* lRetVal=new CWFBestFullyWeighted(mAccessor,
						       mQueryNormalizer,
						       mThisNormalizer);
  return lRetVal;
};
