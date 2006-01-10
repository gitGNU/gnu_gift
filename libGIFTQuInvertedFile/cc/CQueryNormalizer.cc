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
* CQueryNormalizer
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
#include "libGIFTQuInvertedFile/include/CQueryNormalizer.h"

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
CQueryNormalizer::CQueryNormalizer(const CAcInvertedFile* inAccessor):
  mAccessor(inAccessor){
    reset();
};




/***************************************
*
* setAccessor
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CQueryNormalizer::setAccessor(const CAcInvertedFile* inAccessor){
  mAccessor=
    inAccessor;
};



/***************************************
*
* reset
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CQueryNormalizer::reset(){
  mValue=0;
}



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
double CQueryNormalizer::getValue()const{
  return mValue;
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
void CQueryNormalizer::considerQueryFeature(const CWeightingFunction& 
					    inFeature){
#define THIS_SHOULD_NEVER_BE_CALLED 0
  assert(THIS_SHOULD_NEVER_BE_CALLED);
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
CQueryNormalizer* CQueryNormalizer::clone()const{
  cout << "here in clone" << flush;
    return new CQueryNormalizer(*this);
}
