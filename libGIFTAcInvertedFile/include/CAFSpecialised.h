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
// -*- mode: c++ -*-
#ifndef _CAFSPECIALISED
#define _CAFSPECIALISED
#include "libGIFTAcInvertedFile/include/uses-declarations.h"
#include "react_to_perl.h"
#include "libMRML/include/CAccessorFactory.h"

/**
 *
 * This template is designed to make easy use
 * of the CAccessorFactory: Tem
 *
 *@author: Wolfgang M�ller
 */
template<class TAccessorSubType,char* TName>
class CAFSpecialised:public CAccessorFactory{
 public:
  /** Construct this.
      The template parameter TName is used for
      registering in the right way.
  */
  CAFSpecialised():mName(TName){};
  /** Make a new */
  CAccessor* makeAccessor(const CAttributeList& inAttributes){
    return new TAccessorSubType(inAttributes);
  };
};

#endif
