/* -*- mode: c++ -*- 
*/
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
#ifndef _CATTRIBUTELIST
#define _CATTRIBUTELIST
#include <map>
#include <list>
#include <string>
#include "CMagic.h"
//#define _PRINTOUTS_AL

/** a simple class destined to contain attribute-value pairs in an 
    easily accessible fashion */
class CAttributeList:public map<string,pair<char*,char*> >,public CMagic{
public:
  /** */
  void check()const;
  /** */
  typedef map<string,pair<char*,char*> > CBase;
  /** from expat */
  CAttributeList(const char* const * const inAttributeList);
  /** from other c++ */
  CAttributeList(const list<pair <string,string> >& inAttributes);
  /** copy constructor */
  CAttributeList(const CAttributeList& inList);
  /** finding in this*/
  const_iterator find(string inString)const;
  /** adding an attribute */
  void add(const string& inFirst,
	   const string& inSecond);
  /** adding an attribute for integers using the function for strings */
  void add(const string& inAttribute,long inValue);
  /** adding an attribute for doubles using using the function for strings */
  void add(const string& inAttribute,double inValue);
  /** reading an attribute for booleans using stringReadAttribute
      it supports several pairs:
      yes/no
      true/false
      y/n
  */
  pair<bool,bool> boolReadAttribute(const string& inAttribute)const;
  /** reading an attribute for integers using stringReadAttribute */
  pair<bool,long> longReadAttribute(const string& inAttribute)const;
  /** reading an attribute for doubles using stringReadAttribute */
  pair<bool,double> doubleReadAttribute(const string& inAttribute)const;
  /** reading an attribute for strings 
      
      All the attribute functions implement a primitive version
      of inheritance: We look also at the parents of this, if we
      do not find a given attribute in this.
      
      this is virtual in order to plug in other methods of inheritance

  */
  virtual pair<bool,string> stringReadAttribute(const string& inAttribute)const;
  /** Outputting a string to XML 
   */
  void CAttributeList::toXML(string& outString)const;
  /** the destructor 
      in the case of char* as content it deletes the arrays of char.
   */
  ~CAttributeList();
};
#endif
