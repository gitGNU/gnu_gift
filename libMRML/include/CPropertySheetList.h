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
// -*- mode: c++ -*-
#ifndef _CPROPERTYSHEETLIST
#define _CPROPERTYSHEETLIST

#include <map>
#include <string>
#include "CSelfDestroyPointer.h"
class CXMLElement;

/** takes the information contained in the list of
    properties, which is given in the config.mrml file.

    basically, this is a map from string to CXMLElement.

    @Author: Wolfgang Müller
*/
class CPropertySheetList{
  /** the content of this 
      NO DESTRUCTION RESPONSIBILITY HERE.
   */
  typedef map<string,CXMLElement*> CContent;
  /** */
  CContent mContent;
  /** The unprocessed content.
      mContent will index into that structure
      we have destruction responsibility
  */
  CSelfDestroyPointer<CXMLElement> mUnprocessedContent;
  
 public:
  /** We construct *this from a subtree of the document.
      @param inUnprocessedContent is a subtree of the document
      which will be put in mUnprocessedContent. The children of 
      mUnprocessedContent will then be indexed by CContent.
   */
  CPropertySheetList(CXMLElement* inUnprocessedContent);
  /** presently empty, because the destruction will
      be done by the structure contained in 
      mUnprocessedContent.
   */
  ~CPropertySheetList();
  /** insert an element 
      for this element which is designed by  a pointer 
      WE TAKE THE DESTRUCTION RESPONSABILITY
   */
  void insertPropertySheet(const string& inID,
			   CXMLElement* inXMLElement);
  /** Get a property sheet by its ID*/
  CXMLElement& getPropertySheet(const string& inID);
  /** clone a property sheet by its ID*/
  CXMLElement* newPropertySheet(const string& inID);
};



#endif
