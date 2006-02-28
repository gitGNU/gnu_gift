// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

     Copyright (C) 2003, 2004 Bayreuth University
    Copyright (C) 2005,2006 Bamberg University
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
#include "libMRML/include/CQueryParadigmMatcher.h"
#include "libMRML/include/mrml_const.h" // mrml string constants
bool CQueryParadigmMatcher::matches(const CXMLElement& inQuery,const CXMLElement& inResult)const{
  const CXMLElement* lQuery(&inQuery);
  const CXMLElement* lResult(&inResult);
  
  //as the relation we are describing is symmetric,
  //we permit ourselves to swap things in a way, that
  //processing time is minimal
  if(lQuery->getNumberOfAttributes() > lResult->getNumberOfAttributes()){
    swap(lQuery,lResult);
  }

  list<pair<string,string> >* lQueryAttributes(lQuery->createNamedValueList());

  bool lMatches(true);

  for(list<pair<string,string> >::const_iterator i=lQueryAttributes->begin();
      lMatches && (i!=lQueryAttributes->end());
      i++){
    // get the query attribute in the result element
    pair<bool,string> lAttribute(lResult->stringReadAttribute(i->first));
    // they match, unless
    // the query attribute exsists in the result element
    // and the attribute values differ between query and result
    if(lAttribute.first 
       && (lAttribute.second != i->second)){
      lMatches=false;
    }
  }

  delete lQueryAttributes;

  return lMatches;
}

bool CQueryParadigmMatcher::operator()(const CXMLElement& inQuery,const CXMLElement& inResult)const{
  
  // nothing matches everything
  if(!(&inQuery) || !(&inResult)){
    return true;
  }
  if((inQuery.getName()==mrml_const::query_paradigm_list)
     &&
     (inQuery.getName()==mrml_const::query_paradigm_list)){
    for(list<CXMLElement*>::const_iterator i=inQuery.child_list_begin();
	i!=inQuery.child_list_end();
	i++){
      for(list<CXMLElement*>::const_iterator j=inResult.child_list_begin();
	j!=inResult.child_list_end();
	j++){
	if(matches(**i,**j)){
	  return true;
	}
      }
    };
  }
  return false;
};

