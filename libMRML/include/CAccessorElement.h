// -*- mode: c++ -*-
#ifndef _CACCESSORELEMENT
#define _CACCESSORELEMENT
#include "libMRML/include/uses-declarations.h"
#include <string>
#include <functional>
#include <iostream>
#include "libMRML/include/TID.h"
/** Quadruplet of Image-ID, image-URL, thumbnail-URL, feature file
    name and assorted sorting functions
*/
class CAccessorElement{
  /** the ID of the image*/
  TID mID;
  /** the URL (of the image)*/
  string mURL;
  /** the URL (of the thumbnail)*/
  string mThumbnailURL;
  /** the Feature file name (of the image)*/
  string mFeatureFileName;
public:
  /** Constructor */
  CAccessorElement(const TID inID,
		   const string& inURL,
		   const string& inThumbnailURL,
		   const string& inFeatureFileName);
  /** get mURL */
  TID getID()const;
  /** get mURL */
  string getURL()const;
  /** */
  string getThumbnailURL()const;
  /** */
  string getFeatureFileName()const;

  /** sorting by ID (less<TID>)*/
  friend class CSortByID_CAE;
  /** sorting by the URL (less<string>) */
  friend class CSortByURL_CAE;
  /** sorting by the URL of the thumbnail (less<string>) */
  friend class CSortByThumbnailURL_CAE;
};

/** sort CAccessorElements by their ID */
class CSortByID_CAE:public binary_function<CAccessorElement,CAccessorElement,bool>{
public:
  /** */
  bool operator()(const CAccessorElement& l,
		  const CAccessorElement& t)const;
};
/** sort CAccessorElements by their URL */
class CSortByURL_CAE:public binary_function<CAccessorElement,CAccessorElement,bool>{
public:
  /** */
  bool operator()(const CAccessorElement& l,
		  const CAccessorElement& t)const;
};
/** sort CAccessorElements by their ThumbnailURL */
class CSortByThumbnailURL_CAE:public binary_function<CAccessorElement,CAccessorElement,bool>{
public:
  /** */
  bool operator()(const CAccessorElement& l,
		  const CAccessorElement& t)const;
};

ostream& operator<<(ostream& outStream,
		    const CAccessorElement& inElement);

#endif
