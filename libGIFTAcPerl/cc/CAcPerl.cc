// -*- mode: c++ -*- 
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
/**
 *
 * Do we print the initialisation?
 *
 */

#ifdef __GIFT_WITH_PERL__ 
#define _NO_PRINT_INIT
#include "libMRML/include/mrml_const.h"
#include "libGIFTAcPerl/include/CAcPerl.h"
#include "libMRML/include/CXMLElement.h" // constructor

/**
 */
CAcPerl::CAcPerl(const CXMLElement& inCollectionElement):
  mCollectionElement(inCollectionElement.clone(true)){
  
};
/**
 */
CAcPerl::~CAcPerl(){
  cerr << "deleting perl accessor" << endl;
};
/**
 */
CXMLElement* CAcPerl::cloneCollectionElement()const{
  return mCollectionElement->clone(true);
};
/**
 */
CAcPerl::operator bool()const{
  return mCollectionElement;
}
/**
 *
 */
string CAcPerl::IDToURL(long int) const{
  assert(0);
  return "this function should never be called";
};
/**
 *
 */
struct pair<bool,CAccessorElement> CAcPerl::IDToAccessorElement(long int) const{
  return(make_pair(bool(0),CAccessorElement(0,"null","null","null")));
};
/**
 *
 */
struct pair<bool,long int> CAcPerl::URLToID(const string &) const{
  return(make_pair(bool(0),0));
};
/**
 *
 */
void CAcPerl::getAllIDs(list<long int > &) const{
    assert(0);
};
/**
 *
 */
void CAcPerl::getAllAccessorElements(list<CAccessorElement > &) const{
  assert(0);
};
/**
 *
 */
void CAcPerl::getRandomIDs(list<long int > &, unsigned int) const{
  assert(0);
};
/**
 *
 */
void CAcPerl::getRandomAccessorElements(list<CAccessorElement > &, unsigned int) const{
  assert(0);
};
/**
 *
 */
int CAcPerl::size() const{
  return 0;
}
#endif
