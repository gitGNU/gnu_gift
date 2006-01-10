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
* CQNNoNormalization
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
#include "libGIFTQuInvertedFile/include/CQNNoNormalization.h"
#include "libMRML/include/my_diagnose.h"
/***************************************
*
* CQNNoNormalization
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
CQNNoNormalization::CQNNoNormalization(const CAcInvertedFile* inAccessor):
  CQueryNormalizer(inAccessor)
{
};



/***************************************
*
* getValue
*
****************************************
*
* modification history
*
* 
*
****************************************/
double CQNNoNormalization::getValue()const{
  return 1;
}



/***************************************
*
* considerQueryFeature
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CQNNoNormalization::considerQueryFeature(const CWeightingFunction& 
					    inFeature){
  // deliberately left empty
};

/***************************************
*
* clone: This permits us to store
*     completely preconfiguered normalizers
*     and use them when needed
*
****************************************
*
* modification history
*
* 
*
****************************************/
CQueryNormalizer* CQNNoNormalization::clone()const{
  my_diagnose("in clone");
  return new CQNNoNormalization(*this);
}
