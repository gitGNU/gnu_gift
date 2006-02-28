/* -*- mode: c++ -*- 
*/
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
// -*- mode: c++ -*-
#ifndef _CXMLTRIPLET
#define _CXMLTRIPLET
#include "libGIFTQuPerl/include/uses-declarations.h"
#include <string>

/**
   This structure contains a triplet of
   (instruction,name,value). It is designed
   to help build an XML tree from a flat 
   structure.
 */
class CXMLTriplet{
 
 public:
  /** the instruction */
  int mInstruction;
  /** the name */
  string mName;
  /** the value */
  string mValue;


  /** a list of the possible instructions 
      beginning of an element */
  static const int element;
  /** end of an element, up in the tree */
  static const int up;
  /** an attribute */
  static const int attribute;
  /** a text node */
  static const int text;
};

#endif
