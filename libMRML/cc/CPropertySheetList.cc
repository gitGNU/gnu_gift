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
#include "CPropertySheetList.h"
#include "CXMLElement.h"
#include "mrml_const.h"
/** */
CPropertySheetList::CPropertySheetList(CXMLElement* inUnprocessedContent):
  mUnprocessedContent(inUnprocessedContent){
  //now there comes a small indexing party
  //we are looking at the children of the "vi-property-list" ...
  for(CXMLElement::lCChildren::iterator i=
	mUnprocessedContent->child_list_begin();
      i!=mUnprocessedContent->child_list_end();
      i++){
    //which should all be vi-property-elements so each of them
    //SHOULD have an ID. We assert this.
    assert((*i)->stringReadAttribute(mrml_const::cui_property_item_id).first);
    //the children of each element should be properties
    //if not, that's not so cool.
    mContent.insert(make_pair((*i)->stringReadAttribute(mrml_const::cui_property_item_id).second,
			      *((*i)->child_list_begin())));
  }
}

/** 
    ~CPropertySheetList
*/ 
CPropertySheetList::~CPropertySheetList(){
//   for(CContent::iterator i=mContent.begin();
//       i!=mContent.end();
//       i++){
//     delete i->second;
//   }
}


/** 
    ...handing it through to the map
*/ 
void CPropertySheetList::insertPropertySheet(const string& inID,
					     CXMLElement* inXMLElement){
  mContent.insert(make_pair(inID,
			    inXMLElement));
  
}
/** 
    ...handing it through to the map
*/ 
CXMLElement& CPropertySheetList::getPropertySheet(const string& inID){
  CContent::const_iterator lFound=mContent.find(inID);

  if(lFound!=mContent.end()){
    return *lFound->second;
  }else{
    return (*((CXMLElement*)0));
  }
}
/** 
    ...handing it through to the map
*/ 
CXMLElement* CPropertySheetList::newPropertySheet(const string& inID){
  CContent::const_iterator lFound=mContent.find(inID);

  if(lFound!=mContent.end()){
    return new CXMLElement(*lFound->second);
  }else{
    return 0;
  }
}
