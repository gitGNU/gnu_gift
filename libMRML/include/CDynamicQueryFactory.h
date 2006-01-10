/* -*- mode: c++ -*- 
*/
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

     Copyright (C) 2003, 2004 Bayreuth University
      2005 Bamberg University
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
#ifndef _CDYNAMICBASETYPEFACTORY
#define _CDYNAMICBASETYPEFACTORY
#include "libMRML/include/uses-declarations.h"

#include <sys/types.h> // reading a directory
#include <dirent.h>    // reading a directory
#include <dlfcn.h>     // dynamic linking 

#include <iostream>    // screen output
#include <string>
#include <map>

#include "libMRML/include/CStaticQueryFactory.h"
#include "libMRML/include/CQueryPlugin.h"

class CDynamicQueryFactory:public CStaticQueryFactory,protected map<string,CQueryPlugin*>{
  
public:
  /** makes a query of a given BaseType (i.e. name) */
  CQuery* makeQuery(const string & inBaseType, 
		    CAccessorAdminCollection & inAccessorAdminCollection,
		    CAlgorithm & inAlgorithm);
  /** constructor: scans a given directory for plugins*/
  CDynamicQueryFactory(string inDirectoryName);
};

#endif
