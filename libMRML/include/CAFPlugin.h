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
#ifndef _CACCESSORPLUGIN
#define _CACCESSORPLUGIN

#include <sys/types.h> // reading a directory
#include <dirent.h>    // reading a directory
#include <dlfcn.h>     // dynamic linking 

#include <iostream>    // screen output
#include <string>
#include <map>

#include "libMRML/include/CAccessor.h"
#include "libMRML/include/CXMLElement.h"
#include "libMRML/include/CAccessorFactory.h"

class CAFPlugin:public CAccessorFactory{
protected:
  /** the handle which enables us to open*/
  void*   mDlOpenHandle;
  /** the name of the plugin */
  string  mName;
  /** the function for making a CAccessor object */
  CAccessor* (*mMakeAccessor)(const CXMLElement&);
  /** this function gives us the name of the plugin */
  char*  (*mGetName)(void);
  /** is this well constructed? */
  bool mIsSane;
public:

  /** 
      Add some 
      register function here 
      
      the rest can stay the same
  */


  /** makes a CAccessor object*/
  CAccessor* makeAccessor(const CXMLElement&)const;
  /** gets the name of the plugin */
  char* getName();
  /** opens the library and sets the make query etc. functions */
  CAFPlugin(string,string);
  /** copy all the content */
  CAFPlugin(CAFPlugin&);
  /** close the DlOpenHandle of the library*/
  ~CAFPlugin();
  /** is this well constructed */
  bool isSane()const;
  /** register the factory */
  void registerFactory(CAccessorFactoryContainer& outContainer);

};

#endif
