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
* CWFStandardTF
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
#include "libGIFTQuInvertedFile/include/CWFStandardTF.h"
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
CWFStandardTF::
CWFStandardTF(const CAcInvertedFile* inAccessor,
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
double CWFStandardTF::subApply(const double inDocumentFrequency,
			       const double inEuclideanLengthSquare)const{
#ifdef PRINT
  cout << "WFStandardICF" << endl;
#endif

  double lEuclideanLength=sqrt(inEuclideanLengthSquare);

  double lTermFrequencyProduct=getTermFrequency()*inDocumentFrequency;

  //for histograms the term-frequency product is ignored
  if((mFeatureDescription==COL_HST)
     ||
     (mFeatureDescription==GABOR_HST))
    lTermFrequencyProduct=1;


  return 
    //document term weight
    lTermFrequencyProduct
    /lEuclideanLength
    //query term weight
    *
    CWeightingFunction::subApply(inDocumentFrequency,
				 1
				 //the normalizing factor 
				 //has already been used once, so this time 
				 //it is ignored -> 1
				 )
    ;

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
double CWFStandardTF::apply(const CDocumentFrequencyElement& inResultFeature)const{
  subApply(inResultFeature.getDocumentFrequency(),
	   mAccessor->DIDToDFSquareSum(inResultFeature.getID()));
};



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
CWeightingFunction* CWFStandardTF::clone()const{
  CWeightingFunction* lRetVal=new CWFStandardTF(mAccessor,
						mQueryNormalizer,
						mThisNormalizer);
  return lRetVal;
};
