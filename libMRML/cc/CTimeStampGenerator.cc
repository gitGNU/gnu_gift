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

#include "../include/CTimeStampGenerator.h"
#include "../include/CXMLElement.h"
#include "../include/mrml_const.h"
#include "libMRML/include/CMutex.h"
#include <ctime>
#include <string>
/** */

extern CMutex* gMutex;

CXMLElement* CTimeStampGenerator::generateTimeStamp()const{
  gMutex->lock();
  time_t lNow(time(0));
  
  CXMLElement* lReturnValue=new CXMLElement(mrml_const::cui_time_stamp,0);

  if(lNow>0){
    string lNowASCII=string(ctime(&lNow));
    lReturnValue->addAttribute(mrml_const::calendar_time,
			       lNowASCII);
  }
  gMutex->unlock();
  return lReturnValue;
}
