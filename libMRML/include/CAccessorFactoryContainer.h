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
#ifndef _CACCESSORFACTORYCONTAINER
#define _CACCESSORFACTORYCONTAINER
#include "libMRML/include/uses-declarations.h"
#include <map>
#include <string>
#include "libMRML/include/CSelfDestroyPointer.h"
class CXMLElement;
class CAccessor;
class CAccessorFactory;
/**
 * This is destined to hold a number of CAccessorFactories.
 * The use of this is to have an open-close-counter for 
 * all the factories which have been registered in this
 * 
 * New elements are only constructed if necessary.
 *
 *@author Wolfgang Müller
 */
class CAccessorFactoryContainer{
  /** 
      This tree contains the configuration data for this
  */
  CSelfDestroyPointer<CXMLElement> mConfigurationData;
  class CContentElement{
  public:
    /** Contains a pointer to the accessor, if already opened */
    CAccessorFactory* mFactory;
    /** Counts how often the accessor has been opened or closed */
    int mOpenCloseCounter;
  };
  /** 
      Typedef: The content is a map of string to content element 
      In this map is noted, which accessors are open for the
      collection with the given ID.
   */
public:
  typedef map<string,CContentElement> CContent;
protected:
  /** the content */
  CContent mContent;
     
 public:
  /** construct this. In the case of static linking,
      this means the construction of factorys
      for all accessors.
  */
  CAccessorFactoryContainer();
  /** configure this

      In the case of dynamic linking we will scan
      libdir for plugins
   */
  void configure(CXMLElement* inConfigurationData);
  /**
     The destructor destroys the CContentElements properly 
  */
  ~CAccessorFactoryContainer();
  /** 
      adding a factory to the content
   */
  void registerFactory(CAccessorFactory* inFactory,
		       string inName);
  /**
     opening an accessor:
     if the accessor is already constructed,
     then simply give the pointer,
     otherwhise construct a new one 
     using the appropriate factory

     @param name of the accessor to be created
     @param inConfigurationData the configuration data of 
            the CXMLElement to be created
  */
  CAccessor* openAccessor(string inName,
			  const CXMLElement& inCollectionElement);
  /**
     closing an accessor.
     If there have been as many openAccessor 
     as closeAccessor calls, the accessor will
     be delete-d.
  */
  void closeAccessor(string inName);
  /**
     Is a given factory present in the factory?
     Useful for building in order to avoid double registration
     and double dynamic linking.
  */
  bool isPresentFactory(string inName)const;
  
};

#endif
