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
* CInitializedDouble
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
#ifndef _CINITIALIZEDDOUBLE
#define _CINITIALIZEDDOUBLE
#include "libGIFTAcInvertedFile/include/uses-declarations.h"
/** A class of doubles which are initialized with a zero
 (unnecessary, it seems) */
class CInitializedDouble{
protected:
  ///The double
  double mDouble;
public:

/***************************************
*
* 
*
***************************************/
  ///Conversion (for double=CInitializedDouble)
  inline operator double()const{
    return mDouble;
  };

/***************************************
*
* 
*
***************************************/
  ///
  inline CInitializedDouble(const double inDouble=0):mDouble(inDouble){};

/***************************************
*
* 
*
***************************************/
  ///
  inline CInitializedDouble(const CInitializedDouble& in):
    mDouble(in.mDouble){};

/***************************************
*
* 
*
***************************************/
  ///
  inline CInitializedDouble operator=(const CInitializedDouble& in)
    {mDouble=(in.mDouble);};
};

#endif
