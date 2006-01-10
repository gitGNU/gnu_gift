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
#ifndef _CACCESSORIMPLEMENTATION
#define _CACCESSORIMPLEMENTATION
#include "libMRML/include/uses-declarations.h"
#include "libMRML/include/CMagic.h"
#include <string>
#include "libMRML/include/TID.h"
#include <iostream>
#include <fstream>
#include <map>

#ifdef HAS_HASH_MAP
#include <hash_map>
#else
#define hash_map map
#endif

#include "libMRML/include/CAccessor.h"
#include "libMRML/include/CAccessorElement.h"

/**
*
* convenience typedef 
*
*/
class string_string_map:public map<string,string>{
};

/**
*
* convenience typedef 
*
*/
class string_TID_map:public map<string,TID>{
};

/**
*
* convenience typedef 
*
*/
class TID_string_map:public map<TID,string>{
};
/**
*
* convenience typedef 
*
*/
class TID_CAccessorElement_map:public map<TID,CAccessorElement>{
};

/**
*
* CAccessorImplementation - a base class for everything
* accessing a GIFT database.
* at present this will be either an inverted file
* or a TrackingGIFT accessor.
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
class CAccessorImplementation:public CAccessor{

protected:

  /**@name for fast access...*/
  //@{
  /**  map the url of an image to the id of this image */
  string_TID_map mURLToID;

  /**  maps the ID of an image to the URL of this image */
  TID_CAccessorElement_map mIDToAccessorElement;
  //@}

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
  virtual string IDToURL(TID inID)const;
  /**
   *
   * Translate a DocumentID to an accessor Element
   *
   */
  virtual pair<bool,CAccessorElement> IDToAccessorElement(TID inID)const;
  
  /**
   *
   * Translate an URL to its document ID
   *
   */
  virtual pair<bool,TID> URLToID(const string& inURL)const;
  
  /**
   *
   * This is useful for making lists of 
   * which images are present in a database
   * etc.
   *
   */
  /** List of the IDs of all documents present in the accessor file */
  void getAllIDs(list<TID>&)const;
  /** List of triplets (ID,imageURL,thumbnailURL) of all
      the documents present in the inverted file */
  void getAllAccessorElements(list<CAccessorElement>&)const;
  /** get a given number of random AccessorElement's 
      @param inoutResultList the list which will contain the result
      @param inSize          the desired size of the inoutResultList
  */
  void getRandomIDs(list<TID>&,
		    list<TID>::size_type)const;
  /** For drawing random sets. Why is this part of an CAccessorImplementation?
      The way the accessor is organised might influence the way
      random sets can be drawn. At present everything happens in
      RAM, but we do not want to be fixed on that.

      @param inoutResultList the list which will contain the result
      @param inSize          the desired size of the inoutResultList
   */
  void getRandomAccessorElements(list<CAccessorElement>& outResult,
				  list<CAccessorElement>::size_type inSize)const;
  /** The number of images in this accessor */
  int size()const;
  //@}

};

#endif
