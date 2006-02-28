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
#include "libMRML/include/CAccessor.h"
#include <iostream>
void CAccessor::dummy()const{
};
CAccessor::~CAccessor(){
  cout << __FILE__ << "calling destructor" << endl;
};

/** 
    If a new collection is created during runtime, this function prepares 
    the indexing structures such that they are able to accept new objects.
    
    The collection id and name must be given when constructing 
    <code>*this</code>. All other data are added to the configuration
    tag in this. The resulting configuration tag is output as result
    of prepareDatabase.
    
    @returns configuration element that contains new configuration data
    as result of prepareDatabase
*/
CXMLElement* CAccessor::prepareDatabase(){
  return 0;
};
/** 
    Here: do plain nothing. Return false, so
    the default is to initialise rather than not.
*/
bool CAccessor::isPreparedDatabase()const{
  return false;
};
//@}
