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
#include "libMRML/include/getLibNameFromFileName.h"
std::pair<bool,std::string> getLibNameFromFileName(std::string inPrefix,std::string inName){
  std::string lReturnString(inName);

  bool lSuccess(inName.find(inPrefix)==0);

  if(lSuccess){
    std::string::size_type lFirstPointPosition=inName.find_first_of(std::string("."));
    lReturnString=inName.substr(0,lFirstPointPosition);
  }
  return make_pair(lSuccess,lReturnString);  
}
