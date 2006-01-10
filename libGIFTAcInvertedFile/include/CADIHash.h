/* -*- mode: c++ -*- 
*/
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
#ifndef _CADIHash
#define _CADIHash
#include "libGIFTAcInvertedFile/include/uses-declarations.h"
#include <map>
#include "libMRML/include/TID.h"

#include "libGIFTAcInvertedFile/include/CAdditionalDocumentInformation.h"
#include <fstream>

/**
*
* CADIHash
*
*
*
* modification history:
*
*@Author: Wolfgang Müller
*
* Changes:
* HM 090399 created the documentation
*
* compiler defines used:
*
* A hash for additional document information (hashed by document ID) 
* 
*
*/
class CADIHash:public map<TID,
		CAdditionalDocumentInformation,
		less<TID> >{
public:
  
/**
 *
 * Writing ADI for each document to its own
 *
 */
  inline bool output()const{
    bool lRetVal=true;

    for(const_iterator i=begin();
	i!=end();
	i++){
      lRetVal= lRetVal && (*i).second.output();
    }

    return lRetVal;
  }
};
#endif
