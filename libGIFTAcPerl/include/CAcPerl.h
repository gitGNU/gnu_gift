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
#include "../../gift-config.h"
#ifdef __GIFT_WITH_PERL__
// -*- mode: c++ -*-
#ifndef _CACPERL
#define _CACPERL
/**
*
* CAcPerl
* a class which makes the configuration of a collection
* accessible to some perl linked to gift
*
* modification history:
*
* WM 20000626 created file
*
*
*
* compiler defines used:
*
*/

#include <string>
#include "libMRML/include/TID.h"
#include <iostream.h>
#include <fstream.h>
#include <list>
#include <map>
#include <hash_map>
#include "libMRML/include/CAccessor.h"
#include "libMRML/include/CSelfDestroyPointer.h"

class CXMLElement;//constructor

/** 
    This accessor is a base class for accessors which use an Perl file to 
    implement the interface of the CAccessor base class.


    If we do not want to get very deeply into perl, we have
    to make this accessor some kind of pseudo accessor
    
 */
class CAcPerl:public CAccessor{
 private:
  /**
     An XMLElement containing an "collection" element 
     from the configuration file
  */
  CSelfDestroyPointer<CXMLElement> mCollectionElement;

public:
  
  /**
   * Unlike the other accessors, this accessor does not treat any
   * data. Its only reason of existence is to pass through the input
   * parameter inContentElement when needed. This means, it
   * <em>enables</em> perl scripts to configure themselves
   * in the same way as other query processors do,
   * pertaining the usual structure for configuration files.
   *
   * As a consequence which parts of the <collection/> element 
   * that is given as inContentElement, is entirely in the 
   * responsibility of the query processor using this accessor.
   */
  CAcPerl(const CXMLElement& inContentElement);
  /**
   * Destructor: Write amessage and return
   */
  ~CAcPerl();
  
  /**
   *
   * Is this accessor up and working?
   *
   */
  virtual operator bool()const;

  /**
   *
   * Get the mCollectionElement
   
   *
   */
  CXMLElement* cloneCollectionElement()const;
  /**
   *
   */
  string IDToURL(long int) const;
  /**
   *
   */
  struct pair<bool,CAccessorElement> IDToAccessorElement(long int) const;
  /**
   *
   */
  struct pair<bool,long int> URLToID(const string &) const;
  /**
   *
   */
  void getAllIDs(list<TID> &) const;
  /**
   *
   */
  void getAllAccessorElements(list<CAccessorElement> &) const;
  /**
   *
   */
  void getRandomIDs(list<long int > &, unsigned int) const;
  /**
   *
   */
  void getRandomAccessorElements(list<CAccessorElement > &, unsigned int) const;
  /**
   *
   */
  int size() const;


};

#endif
#endif
