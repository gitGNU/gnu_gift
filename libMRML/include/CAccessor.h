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
#ifndef _CACCESSOR
#define _CACCESSOR
#include "libMRML/include/uses-declarations.h"
#include <string>
#include <list>
#include "libMRML/include/TID.h"

#ifdef GIFT_MULTI_THREAD
#include <pthread.h>
#endif

#include "libMRML/include/CMutex.h"
#include "libMRML/include/CAccessorElement.h"
class CXMLElement;

/**
*
* CAccessor - an abstract base class for everything
* accessing a GIFT database.
* in the future this might be an SQL database or Monet??
*
*
* modification history:
*
* WM 19990804 created file
*
*
*
* compiler defines used:
*/
class CAccessor{
protected:
  /**
     without this function things like
     upcasting etc. won't work
     (see stroustrup for this)
  */
  virtual void  dummy()const;

public:
  /**
   *
   * Is this accessor up and working?
   *
   */
  virtual operator bool()const=0;

  /**
   *
   * Translate a DocumentID to a URL (for output)
   *
   */
  virtual string IDToURL(TID inID)const=0;
  /**
   *
   * Translate a DocumentID to an accessor Element
   *
   */
  virtual pair<bool,CAccessorElement> IDToAccessorElement(TID inID)const=0;
  
  /**
   *
   * Translate an URL to its document ID
   *
   */
  virtual pair<bool,TID> URLToID(const string& inURL)const=0;
  
  /**
   *
   * This is useful for making lists of 
   * which images are present in a database
   * etc.
   *
   */
  /** List of the IDs of all documents present in the inverted file */
  virtual void getAllIDs(list<TID>&)const=0;
  /** List of triplets (ID,imageURL,thumbnailURL) of all
      the documents present in the inverted file */
  virtual void getAllAccessorElements(list<CAccessorElement>&)const=0;
  /** get a given number of random AccessorElement's 
      @param inoutResultList the list which will contain the result
      @param inSize          the desired size of the inoutResultList
  */
  virtual void getRandomIDs(list<TID>&,
			    list<TID>::size_type)const=0;
  /** For drawing random sets. Why is this part of an CAccessor?
      The way the accessor is organised might influence the way
      random sets can be drawn. At present everything happens in
      RAM, but we do not want to be fixed on that.

      @param inoutResultList the list which will contain the result
      @param inSize          the desired size of the inoutResultList
   */
  virtual void getRandomAccessorElements(list<CAccessorElement>& outResult,
					 list<CAccessorElement>::size_type inSize)const=0;
  /** The number of images in this accessor */
  virtual int size()const=0;
  //@}
  /** @name Database Construction */
  //@{
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
  virtual CXMLElement* prepareDatabase();
  /** 
      Is the database accessed by this accessor prepared?
      In other words: is there an index structure to access?
   */
  virtual bool isPreparedDatabase()const;
  //@}
  /** virtual accessor for clean destruction */
  virtual ~CAccessor();
};

#endif
