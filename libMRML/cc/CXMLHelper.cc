// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000 CUI, University of Geneva

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
#include "CXMLHelper.h"
#include <cstdio>
#include <string>

//a class which helps with xml 
CXMLHelper::CXMLHelper(){
  mIsEmpty=false;
  mIsEnd=false;
};
///
string CXMLHelper::toString()const{
  if(mIsEnd){
    return string("</")+mName+string(">");
  }
  string lReturnValue=string("<")+mName;

  for(CContent::const_iterator i=mAttributes.begin();
      i!=mAttributes.end();
      i++){
    lReturnValue+=string(" ")+i->first+string("=\"")+i->second+string("\"");
  }
  if(mIsEmpty){
     lReturnValue+=" />\n";
  }else{
    lReturnValue+=">\n";
  }
  return lReturnValue;
};

///
void CXMLHelper::setName(const string& inName){
  mName=inName;
};

///
void CXMLHelper::setEmpty(){
  mIsEmpty=true;
};
///
void CXMLHelper::setEnd(){
  mIsEnd=true;
};
///
void CXMLHelper::addAttribute(const string& inAttribute,
			      const string& inValue){
  mAttributes[inAttribute]=inValue;
};
///
void CXMLHelper::addAttribute(const string& inAttribute,
			      int inValue){
  char lValue[20];
  sprintf(lValue,"%d",inValue);
  mAttributes[inAttribute]=string(lValue);

};
///
void CXMLHelper::addAttribute(const string& inAttribute,
			      double inValue){
  char lValue[20];
  sprintf(lValue,"%lf",inValue);
  mAttributes[inAttribute]=string(lValue);
};

