/* -*- mode: c++ -*- 
*/
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
// -*- mode: c++ -*-
#ifndef _CPROPERTYSHEETMATCHER
#define _CPROPERTYSHEETMATCHER
#include "libMRML/include/uses-declarations.h"
#include <functional>
#include "libMRML/include/CXMLElement.h" // the type of the operands of the function we are creating


/** A class for checking if the content of two query-paradigm
    tags matches */
class CQueryParadigmMatcher:public binary_function<bool,CXMLElement,CXMLElement>{
protected:
  /** Two XMLElements of type query-paradigm match,
      if the intersection of their attribute sets
      is identical.
      
      I.e.: If inQuery contains <query-paradigm a1="x" a2=y" />,
      inResult with <query-paradigm a1="x" a3="z" /> would match,
      because the intersection {a1,a2} \cap {a1,a2} of the attribute
      sets is 
      <query-paradigm a1="x" /> and <query-paradigm a1="x"/> for both 
      inQuery and inResult.

      <query-paradigm a1="x"/>
      <query-paradigm a2="x"/>

      would also match (the intersection of {a1} and {a2} is empty)

      <query-paradigm a1="x" a3="x" />
      <query-paradigm a1="y" />

      would not (the intersection of {a1,a3} and {a1} is non-empty, but 
      the values in the two tags are differen)

  */
  bool matches(const CXMLElement& inQuery,const CXMLElement& inResult)const;
 public:
  /** 
      This function gives back, if 
      the inQuery matches inResult.
      
      Both parameters are elements of type query-paradigm-list

      inQuery matches inResult, if one of the children (a query paradigm)
      of inQuery matches one of the children (also a query paradigm) of 
      inResult.
       
   */
  bool operator()(const CXMLElement& inQuery,const CXMLElement& inResult)const;
};

#endif
