// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000 CUI, University of Geneva

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
 * Do we print the initialisation?
 *
 */

#define _NO_PRINT_INIT
#include <iostream>
#include "mrml_const.h"
#include "CAcURL2FTS.h"
#include "CXMLElement.h" // constructor
//the expat xml parser by J.Clark
#include "xmlparse.h"



#define USE_FLATFILE 0

void newStartURL2FTSElement(void *inUserData, 
			    const char *inElementName, 
			    const char **inAttributes){
  
  CAcURL2FTS* lAccessor=(CAcURL2FTS*)inUserData;
  
  //cout << inElementName << endl;

  string lElementName(inElementName);
  CAttributeList lAttributes(inAttributes);

  if(lElementName=="image-list"){
    lAccessor->mID=0;
    lAccessor->mURLPrefix=string("");
    lAccessor->mThumbnailURLPrefix=string("");
    if(lAttributes.stringReadAttribute("url-prefix").first){
      lAccessor->mURLPrefix=lAttributes.stringReadAttribute("url-prefix").second;
    }
    if(lAttributes.stringReadAttribute("thumbnail-url-prefix").first){
      lAccessor->mThumbnailURLPrefix=lAttributes.stringReadAttribute("thumbnail-url-prefix").second;
    }
  }
  if(lElementName=="image"){
    string lURL=lAccessor->mURLPrefix+lAttributes.stringReadAttribute("url-postfix").second;
    string lThumbnailURL=lAccessor->mThumbnailURLPrefix+lAttributes.stringReadAttribute("thumbnail-url-postfix").second;
    string lFeatureFileName=lAttributes.stringReadAttribute("feature-file-name").second;
    
    lAccessor->mURLToFFN.insert(make_pair(lURL,lFeatureFileName));
    lAccessor->mIDToFFN.insert(make_pair(lAccessor->mID,lFeatureFileName));
    lAccessor->mURLToID.insert(make_pair(lURL,lAccessor->mID));
    lAccessor->mIDToAccessorElement.insert(make_pair(lAccessor->mID,CAccessorElement(lAccessor->mID,
									  lURL,
									  lThumbnailURL,
									  lFeatureFileName)));
    lAccessor->mID++;

    //cout << lURL << endl;
    //cout << lThumbnailURL << endl;
    //cout << lFeatureFileName << endl;
  }
}
void newEndURL2FTSElement(void *inUserData, 
			  const char *inElementName){
}

CAcURL2FTS::CAcURL2FTS(const CXMLElement& inCollectionElement):
  mURLToFeatureFileName(inCollectionElement.stringReadAttribute(mrml_const::cui_base_dir).second
			+inCollectionElement.stringReadAttribute(mrml_const::cui_feature_file_location).second),
  mURLToFeatureFile((inCollectionElement.stringReadAttribute(mrml_const::cui_base_dir).second
                     +inCollectionElement.stringReadAttribute(mrml_const::cui_feature_file_location).second).c_str())
{
  mMutexURL2FTS.lock();

  cout << endl
       << "Opening "
       << mURLToFeatureFileName;
  if(!mURLToFeatureFile)
    cout <<" ...FAILED!";
  else
    cout << " ...success." 
	 << endl;
  
  mWellConstructed=mURLToFeatureFile;
  
  mURLToFFN.clear();
  mURLToID.clear();
  mIDToFFN.clear();
  mIDToAccessorElement.clear();
  
  
  int lID=0;
  if(USE_FLATFILE){
    
    /* as long as there is no end of file */
    while(mURLToFeatureFile){
      string lURL;
      string lThumbnailURL;
      string lFeatureFileName;
      
      /* read in the URLs of all the images with the corresponding feature file */



      mURLToFeatureFile >> lURL
			>> lThumbnailURL
			>> lFeatureFileName;
      

      if(lURL.size()>4){
#ifndef _NO_PRINT_INIT
	cout << lID 
	     << ": URL            " << lURL << flush << endl
	     << " Thumbnail       " << lThumbnailURL << flush << endl
	     << " FeatureFileName " << lThumbnailURL << flush << endl;
#endif

	mURLToFFN.insert(make_pair(lURL,lFeatureFileName));
	mIDToFFN.insert(make_pair(lID,lFeatureFileName));
	mURLToID.insert(make_pair(lURL,lID));
	mIDToAccessorElement.insert(make_pair(lID,CAccessorElement(lID,
								   lURL,
								   lThumbnailURL,
								   lFeatureFileName)));
	lID++;
      }
    }
  }else{// use an XML parser
    cout << "constructing parser" << endl;
    XML_Parser lParser = XML_ParserCreate(NULL);//default encoding
    XML_SetUserData(lParser,
		    this);
    XML_SetElementHandler(lParser, 
			  newStartURL2FTSElement,
			  newEndURL2FTSElement);

    bool lParseError=false;
    string lURL2FTS;

    cout << "Reading " << endl;

    mURLToFeatureFile.seekg(0,ios::end);
    int lFileSize(mURLToFeatureFile.tellg());
    mURLToFeatureFile.seekg(0,ios::beg);

    cout << lFileSize 
	 << " characters" 
	 << endl;

    char* lBuffer=new char[lFileSize+1];
    mURLToFeatureFile.read(lBuffer,lFileSize);
    lBuffer[lFileSize]=char(0);

    cout << "Read url2fts.xml" << endl;
    
    bool lDone=false;
    for(int i=0;
	i<lFileSize/10000;
	i++){
      if (!XML_Parse(lParser,
		     lBuffer+i*10000,
		     (lFileSize-i*10000<10000)
		     ?lFileSize-i*10000:10000,
		     lDone)
	  ) {
	cerr << "CAcURL2FTS:__LINE__: XML ERROR: "
	     << XML_ErrorString(XML_GetErrorCode(lParser))
	     << " at line "
	     << XML_GetCurrentLineNumber(lParser)
	     << endl;
	exit(1);
      }
    }
    cout << "Successfully processed" << endl;
    delete lBuffer;
    XML_ParserFree(lParser);
  }



  cout << "URLFile " 
       << mURLToFeatureFileName 
       << " processed."
       << endl;
  mURLToFeatureFile.close();
  mMutexURL2FTS.unlock();
};
CAcURL2FTS::operator bool()const{
  return mWellConstructed;
}
  /** gives back the content of mURLToFeatureFileName */
const string& CAcURL2FTS::getURLToFeatureFileName()const{
  return mURLToFeatureFileName;
};


int CAcURL2FTS::size()const{
  mMutexURL2FTS.lock();
  int lReturnValue(mIDToFFN.size());
  mMutexURL2FTS.unlock();
  return lReturnValue;
};


pair<bool,string> CAcURL2FTS::URLToFFN(const string& inURL)const{
  mMutexURL2FTS.lock();
  string_string_map::const_iterator lFound=mURLToFFN.find(inURL);

  if(lFound != mURLToFFN.end()){
    pair<bool,string> lReturnValue(make_pair(true,lFound->second));
    mMutexURL2FTS.unlock();
    return lReturnValue;
  }else{
    // should be false here??
    pair<bool,string> lReturnValue(make_pair(true,string(inURL+"does not exsist")));
    mMutexURL2FTS.unlock();
    return lReturnValue;
  }
};

pair<bool,string> CAcURL2FTS::IDToFFN(TID inID)const{
  mMutexURL2FTS.lock();
  TID_string_map::const_iterator lFound=mIDToFFN.find(inID);


  if(lFound != mURLToFFN.end()){
    pair<bool,string> lReturnValue(make_pair(true,lFound->second));
    mMutexURL2FTS.unlock();
    return lReturnValue;
  }else{
    pair<bool,string> lReturnValue(make_pair(true,string("the requested id does not exsist")));
    mMutexURL2FTS.unlock();
    return lReturnValue;
  }
};
