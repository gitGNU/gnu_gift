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
#ifndef _CACCESSORPROXY
#define _CACCESSORPROXY
#include "libMRML/include/uses-declarations.h"
#include "libMRML/include/CSelfDestroyPointer.h"
#include <string>
#include <functional>
#include "libMRML/include/CAccessor.h"
#include "libMRML/include/CAccessorFactory.h"
#include "libMRML/include/CAccessorFactoryContainer.h"
/**
  CAccessorAdmin: The goal of this is to make it possible
  to have a list containing accessors to all possible
  collections, without really opening the accessors.

  The accessor proxy is in fact an open/close counter
  for accessors. This permits multiple query structures
  to share just one accessor, and it avoids having to open 
  all kinds of accessors when starting the program.

*/
class CAccessorAdmin{
protected:
  /** 
      Factory for making all kinds of accessors
   */
  CAccessorFactoryContainer& mFactoryContainer;
  /** */
  CSelfDestroyPointer<CXMLElement> mCollectionElement;
  /**
     Content is pointer to a factory,
     a pointer to the actual Accessor (if already made)
     and the open-close-counter
   */
  class CContentElement{
  public:
    /** Contains a pointer to the accessor, if already opened */
    CAccessor* mAccessor;
    /** The factory to make an accessor, if this is needed */
    CAccessorFactory* mFactory;
    /** Counts how often the accessor has been opened or closed */
    int mOpenCloseCounter;
  };
  /** 
      Typedef: The content is a map of string to content element 
      In this map is noted, which accessors are open for the
      collection with the given ID.
   */
  typedef map<string,CContentElement> CContent;
  /** The content is a map of string to content element */
  CContent mContent;
public:
  //----------------------------------------
  /** open an accessor.

      This is the important part for the outside:
      Open or close inverted file accessors.
      Several queries can share one accessor,
      the accessor will be constructed when the first
      CQuery needs it, and it will be deleted, when 
      the last one does not need it any more:
  */
  CAccessor* openAccessor(string inType);
  /** closing an accessor.
   @see openAccessor*/
  void closeAccessor(string inType);
  //----------------------------------------
  /**  Once this is working this will replace
       about every set/get function which follows
  */
  void setCollectionElement(CXMLElement*);
  /**  */
  const CXMLElement* getCollectionElement()const;
  /**  */
  const CXMLElement* getQueryParadigmList()const;
  /** */
  list<string>* getAvailableIndices()const;
  //----------------------------------------
  /**  generate out put for shandshake message */
  string toXML(bool isPrivate=false)const;
  /**  pack the content of this into an CXMLElement 
       with elementname "collection"
   */
  CXMLElement* toXMLElement()const;
  //----------------------------------------
  /** Constructor */
  CAccessorAdmin(CAccessorFactoryContainer& inFactoryContainer);
  /** destructor */
  ~CAccessorAdmin();
  /**  */
  string CAccessorAdmin::getAlgorithmIDListID()const;
  /**  */
  string CAccessorAdmin::getName()const;
  /**  */
  string CAccessorAdmin::getID()const;

  /** for sorting */
  friend class CSortByName_pAP;
};


/* Sort pointers to Accessor Proxies by the name of what they point to */
class CSortByName_pAP:public binary_function<const CAccessorAdmin*,const CAccessorAdmin*,bool>{
public:
  /**  */
  bool operator()(const CAccessorAdmin* l,
		  const CAccessorAdmin* t);
};


#endif
