/* -*- mode: c++ -*- 
*/
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
// -*- mode: c++ -*-
#ifndef _CXEVTRIPLETLIST
#define _CXEVTRIPLETLIST
#include "libMRML/include/uses-declarations.h"
#include <string> //mSessionID

#include "libGIFTQuPerl/include/CXMLTripletList.h"
#include "libMRML/include/CXMLElementVisitor.h" //base class

/**

   This visitor converts a CXMLTree into a list
   of (instruction,name,value) triplets, in order
   to pass them to Perl or CORBA.
   
*/
class CXEVTripletList:public CXMLElementVisitor{
  
protected:
  /**
     A list of (instruction,name,value) triplets. 
  */
  CXMLTripletList mTripletList;
  
public:
  /** constructor. 
      we do not need anything
  */
  CXEVTripletList();
  /** actually carrying out the visit */
  virtual bool startVisit(const CXMLElement& inVisited);
  /** actually carrying out the visit */
  virtual void endVisit(const CXMLElement& inVisited);
  /** getting the list which was built */
  const CXMLTripletList& getTripletList();
};


#endif
