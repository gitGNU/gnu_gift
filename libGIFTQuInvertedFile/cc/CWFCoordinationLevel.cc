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
* CWFCoordinationLevel
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
#include "libGIFTQuInvertedFile/include/CWFCoordinationLevel.h"
#include <cmath>

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
CWFCoordinationLevel::
CWFCoordinationLevel(const CAcInvertedFile* inAccessor,
		     CQueryNormalizer*,
		     CQueryNormalizer* inThisNormalizer):
  CWeightingFunction(inAccessor,
		     0,
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
double CWFCoordinationLevel::subApply(const double inDocumentFrequency,
				      const double inNormalizingFactor)const{
#ifdef PRINT
  cout << "WFCoLevel" << endl;
#endif

  return 1      
    *
    CWeightingFunction::subApply(inDocumentFrequency,
				 inNormalizingFactor);
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
CWeightingFunction* CWFCoordinationLevel::clone()const{
  CWeightingFunction* lRetVal=new CWFCoordinationLevel(mAccessor,
						       mQueryNormalizer,
						       mThisNormalizer);
  return lRetVal;
};


