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
#ifndef _CXMLHELPER
#define _CXMLHELPER

#include <string>
#include <map>

/** a class which helps with xml output */
class CXMLHelper{
  /**  */
  string mName;
  /**  */
  typedef map<string,string> CContent;
  /**  */
  CContent mAttributes;
  /**  */
  bool mIsEmpty;
  /**  */
  bool mIsEnd;
 public:
  /**  */
  CXMLHelper();
  /**  */
  string toString()const;
  /**  */
  void setName(const string& inName);
  /**  */
  void setEmpty();
  /**  */
  void setEnd();
  /**  */
  void addAttribute(const string& inAttribute,
		    const string& inValue);
  /**  */
  void addAttribute(const string& inAttribute,
		    int inValue);
  /**  */
  void addAttribute(const string& inAttribute,
		    double inValue);
};

#endif
