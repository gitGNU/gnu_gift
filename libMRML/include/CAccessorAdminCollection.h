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
// -*- Mode: c++ -*-
#ifndef _CACCESSORCOLLECTION
#define _CACCESSORCOLLECTION

#include <map>
#include "libMRML/include/CAccessorAdmin.h"

class CXMLElement;

/** This is in fact a collection of collections.

    The accessorcollection contains CAccessorAdmins
    
*/  
class CAccessorAdminCollection{
  /** A helper for building up an CXMLElement which will
      later serve as configuration for the new CAccessorAdmin
  */
  CXMLElement* mCollectionInConstruction;
  

public:
  /** this is mainly a map from string to accessor proxy
      we would like to destroy this in a decent way */
  class CContent:public map<string,CAccessorAdmin*>{
  public:
    /** */
    ~CContent();
  };
  /**  */
  CContent mIDToProxy;
  /**  */
  CAccessorAdmin* mCurrentProxy;

public:
  /** for easier read access from the outside */
  typedef CContent::const_iterator const_iterator;
protected:
  /** The accessor factory container: Turn a name 
      into an accessor */
  CSelfDestroyPointer<CAccessorFactoryContainer> mFactory;
public:
  /** for reading all accessors*/
  CContent::const_iterator begin()const;
  /** for reading all accessors*/
  CContent::const_iterator end()const;
  /**  
       The Accessor collection gets its
       configuration out of the same
       file as the CSessionManager
  */
  CAccessorAdminCollection(string inFileName);
  /**  
       add an accessor to a collection to our list of proxies.
  */
  void addCollection(CXMLElement* inCollectionElement);
  /**  get the proxy on which is presently worked on
   */
  CAccessorAdmin& getCurrentProxy();
  /**  get a proxy for a given collectionID
       if this collection is unknown, give back the
       first collection in the stack
  */
  CAccessorAdmin& getProxy(const string& inID);
  /**  simply take the first.*/
  string getDefaultAccessorID()const;
  /**  some tags plus our content */
  string toXML(bool isPrivate=false)const;
  /**  The content of this as an CXMLElement */
  CXMLElement* toXMLElement()const;
  /** 
      get the XMLElement in construction
  */
  CXMLElement* getCollectionInConstruction();
  /** 
      set the element 
  */
  void startCollectionConstruction(CXMLElement* inRoot);
};

#endif
