// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, University of Geneva

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
* CRelevanceLevelList
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
#include "libMRML/include/CRelevanceLevelList.h"

/***************************************
*
* writeBinary
*
****************************************
*
* modification history
*
* 
*
****************************************/
// void CRelevanceLevelList::writeBinary(ostream& outStream)const{
//   for(const_iterator i=begin();
//       i!=end();
//       i++){
//     (*i).writeBinary(outStream);
//   }
// };
/***************************************
*
* writeBinary
*
****************************************
*
* modification history
*
* 
*
****************************************/
CRelevanceLevelList::CRelevanceLevelList(const list<string>& inList){
    for(list<string>::const_iterator i=inList.begin();
	i!=inList.end();
	i++){
	push_back(CRelevanceLevel(*i,0));
    }
};
CRelevanceLevelList::CRelevanceLevelList(){
};
string CRelevanceLevelList::toXML()const{
  string lReturnValue ="<sresultelementlist>\n";
  for(const_iterator i=begin();
      i!=end();
      i++){
    lReturnValue+=i->toXML();
  }
  lReturnValue+="</sresultelementlist>\n";
  return lReturnValue;
}
