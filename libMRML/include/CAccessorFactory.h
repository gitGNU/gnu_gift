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
#ifndef _CACCESSORFACTORY
#define _CACCESSORFACTORY
#include "libMRML/include/uses-declarations.h"
#include <string> //mName
class CAccessor;
class CAccessorFactory;
class CAccessorFactoryContainer;
class CXMLElement;
/**
 *
 * This is a common abstract base class for all specialised
 * CAccessorFactories
 *
 *@see    CAFSpecialised
 *@Author Wolfgang Müller
 */
class CAccessorFactory{
protected:
  /** This variable contains the name used for registration*/
  string mName;
public:
  /**
     (*this) has to register with a container.
     The container gets destruction responsability
  */
  virtual void registerFactory(CAccessorFactoryContainer& outContainer);
  /** As the name sais: this makes an accessor. 
      The parameter contains all attributes needes for
      a successful construction. 
  */
  virtual CAccessor* makeAccessor(const class CXMLElement& inConfiguration)const=0;
  /** This function does plain nothing in this class, however, it becomes
      interesting for plugins that have to free shared libraries 
      presently this is unused. All plugins will be loaded at load time,
      there will be no unloading of plugins during runtime.
  */
  virtual void closeAccessor();
};

#endif
