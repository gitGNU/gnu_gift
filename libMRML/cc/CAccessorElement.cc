#include "libMRML/include/CAccessorElement.h"

/** creating clean output */
ostream& operator<<(ostream& outStream,
		    const CAccessorElement& inElement){
  outStream << string("CAccessorElement(");
  outStream.operator<<(long(inElement.getID()));
  outStream << string(" ")
	    << inElement.getURL()
	    << string(" ")
	    << inElement.getThumbnailURL()
	    << string(" ")
	    << inElement.getFeatureFileName()
	    << string(")--")
	    << endl;
  return outStream;
};
/** Constructor */
CAccessorElement::CAccessorElement(const TID inID,
				   const string& inURL,
				   const string& inThumbnailURL,
				   const string& inFeatureFileName):
  mID(inID),
  mURL(inURL),
  mThumbnailURL(inThumbnailURL),
  mFeatureFileName(inFeatureFileName){
  
};
/** get mURL */
string CAccessorElement::getURL()const{
  return mURL;
};
/** get mID */
TID CAccessorElement::getID()const{
  return mID;
};
/** */
string CAccessorElement::getThumbnailURL()const{
  return mThumbnailURL;
};
/** */
string CAccessorElement::getFeatureFileName()const{
  return mFeatureFileName;
};

/** sort CAccessorElements by their ID */
bool CSortByID_CAE::operator()(const CAccessorElement& l,
			       const CAccessorElement& t)const{
  return l.mID<t.mID;
}

/** sort CAccessorElements by their URL */
bool CSortByURL_CAE::operator()(const CAccessorElement& l,
				const CAccessorElement& t)const{
  return l.mURL<t.mURL;
};

/** sort CAccessorElements by their ThumbnailURL */
bool CSortByThumbnailURL_CAE::operator()(const CAccessorElement& l,
					 const CAccessorElement& t)const{
  return l.mThumbnailURL<t.mThumbnailURL;
}

