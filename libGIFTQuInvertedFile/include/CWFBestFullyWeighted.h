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
/***************************************
*
* CWFBESTFULLYWEIGHTED
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
#ifndef _CWFBESTFULLYWEIGHTED
#define _CWFBESTFULLYWEIGHTED
#include "libGIFTQuInvertedFile/include/uses-declarations.h"
#include "libGIFTQuInvertedFile/include/CWeightingFunction.h"

///Weighting function for method 1
class CWFBestFullyWeighted:public CWeightingFunction{
protected:
public: 
/***************************************
*
* 
*
***************************************/
  ///
  CWFBestFullyWeighted(const CAcInvertedFile* inAccessor,
		       CQueryNormalizer* inQueryNormalizer,
		       CQueryNormalizer* inThisNormalizer);

/***************************************
*
* 
*
***************************************/
  ///Apply this on a document frequency
  virtual double subApply(const double inDocumentFrequency,
			  const double inNormalizingFactor)const;

/***************************************
*
* 
*
***************************************/
  ///Preparing the right parameters for subApply: Apply this on another feature
  virtual double apply(const CDocumentFrequencyElement& inResultFeature)const;

/***************************************
*
* 
*
***************************************/
  /**For the "prototype pattern": 
    Kinda virtual copy constructor: 
    Gamma/Helm/Johnson/Vlissides p. 107*/
  CWeightingFunction* clone()const;

};

#endif
