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
/**
*
* CAccessor - a base class for everything
* accessing a GIFT database.
* at present this will be either an inverted file
* or a TrackingGIFT accessor.
* in the future this might be an SQL database or Monet??
*
*
*
* modification history:
*
* WM 19990804 created file
*
*
*
* compiler defines used:
* _NO_PRINT_INIT print info about initialisation
* USE_FLAT_FILE  use a flat url2fts file (instead of an XML file)
*/
#ifndef _CACURL2FTS
#define _CACURL2FTS
#include "../include/uses-declarations.h"
#include <string>
#include "libMRML/include/TID.h"
#include <iostream.h>
#include <fstream.h>
#include <map>
#include <hash_map>
#include "libMRML/include/CAccessorImplementation.h"
#include "libMRML/include/CMutex.h" // multithreading
class CXMLElement;  // parameter of constructor

/** 
    This accessor is a base class for accessors which use an URL2FTS file to 
    implement the interface of the CAccessor base class.
 */
class CAcURL2FTS:public CAccessorImplementation{
private:
  /** is this well constructed */
  bool mWellConstructed;
protected:
  /**
     the ID of the next element
   */
  TID mID;
  /**
     the url-prefix for the image list
   */
  string mURLPrefix;
  /**
     the thumbnail-url-prefix for the image list
   */
  string mThumbnailURLPrefix;
  /** 
      the mutex for multithreading 
      the name is intended to be unique
      and immune against inheritance...
  */
  CMutex mMutexURL2FTS;
  /** map from the url of an image to the name of the feature file for this image */
  string_string_map mURLToFFN;
  /** map from the id of an image to the name of the feature file for this image */
  TID_string_map mIDToFFN;
  /** URL -> FeatureFileName */
  mutable ifstream mURLToFeatureFile;
  /** 
      Name of the file that contains pairs of  URL and the Feature file that belongs to the URL 
   */
  string mURLToFeatureFileName;
  // this seems to be necessary
  friend class CAcIFFileSystem;
public:
  /** gives back the content of mURLToFeatureFileName */
  const string& getURLToFeatureFileName()const;

  /**
   *
   * Constructor: slurp in an url2fts file 
   * and fill the maps.
   * 

       Like every accessor, this accessor takes a <collection />
       MRML element as input (@see CXMLElement for how to access 
       the attributes of this element). Currently this accessor
       understands  the following attributes

       cui-base-dir:      the directory containing the following files
       cui-feature-file-location:  the location of the "url2fts" file
                                   which translates urls to feature
				   file names.

   */
  CAcURL2FTS(const CXMLElement& inContentElement);
  
  /**
   *
   * Is this accessor up and working?
   *
   */
  virtual operator bool()const;
  /**
   *
   * Give the number of elements stored in this accessor
   *
   */
  virtual int size()const;
  /**
   *
   * gives the feature file name which corresponds to a given URL
   * return value: pair of bool   (does the feature file exsist)
                           string (the feature file name)
   */
  pair<bool,string> URLToFFN(const string& inURL)const;
  /**
   *
   * gives the feature file name which corresponds to a given URL
   * return value: pair of bool   (does the feature file exsist)
                           string (the feature file name)
   */
  pair<bool,string> IDToFFN(TID inID)const;
  /** we construct this with a little help from this friend */
  friend void newStartURL2FTSElement(void *inUserData, 
				     const char *inElementName, 
				     const char **inAttributes);
  /** we construct this with a little help from this friend */
  friend void newEndURL2FTSElement(void *inUserData, 
				   const char *inElementName);
};

#endif
