// -*- mode: c++ -*- 
#line 3 "CAccessorAdminCollection.pre-cc"
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
#include "libMRML/include/mrml_const.h"
#include "libMRML/include/my_throw.h"
#include "libMRML/include/my_assert.h"
#include <cassert>
#include "libMRML/include/GIFTExceptions.h"
#include "libMRML/include/CAccessorAdminCollection.h"
#include "libMRML/include/CAttributeList.h"
//#include "expat/xmlparse/xmlparse.h"
#include <expat.h>
#include <fstream>
#include "libMRML/include/CXMLElement.h" // for adding collections 
#include "libMRML/include/directory.h"

void startAccessorAdminCollectionElement(void *userData, 
				    const char *inElementName, 
				    const char **inAttributes)
{
  bool lSuccessful=true;
  //the calling CSessionManager
  CAccessorAdminCollection* lCaller((CAccessorAdminCollection*)userData);
  

#ifdef CAC_DEBUG_PRINTOUT
  cout << "<"
       << inElementName
       << " WhileReading=\"true\" _"
       << flush;
#endif
  CAttributeList lAttributes(inAttributes);
#ifdef CAC_DEBUG_PRINTOUT
  cout << "--"
       << flush;
  cout << lAttributes.size()
       << "."
       << flush;
#endif

  if(string(inElementName)
     ==
     string(mrml_const::collection)
     || (lCaller->getCollectionInConstruction())
     ){
      {
	if(!lCaller->getCollectionInConstruction()){
	  lCaller->startCollectionConstruction(new CXMLElement(inElementName,
							       inAttributes));
	}else{
	  lCaller->getCollectionInConstruction()->addChild(inElementName,
							   inAttributes);
	}
      }
  }
#ifdef CAC_DEBUG_PRINTOUT
  cout << " >"
       << endl
       << flush;
#endif
}

void endAccessorAdminCollectionElement(void *userData, const char *inElementName){
  CAccessorAdminCollection* lCaller((CAccessorAdminCollection*)userData);
  if((string(inElementName)
      ==
      string(mrml_const::collection))
     ||lCaller->getCollectionInConstruction()){
    {
      my_assert(lCaller->getCollectionInConstruction(),
		"Collection in construction not found");
      
      lCaller->getCollectionInConstruction()->moveUp();
      if(lCaller->getCollectionInConstruction()->isSubtreeFinished()){
	lCaller->addCollection(lCaller->getCollectionInConstruction());
	lCaller->startCollectionConstruction((CXMLElement*)0);
      }
    }
  }

#ifdef CAC_DEBUG_PRINTOUT
  cout << "</"
       << inElementName
       << " >"
       << endl;
#endif
}

CAccessorAdminCollection::const_iterator CAccessorAdminCollection::begin()const{
  return mIDToProxy.begin();
};
CAccessorAdminCollection::const_iterator CAccessorAdminCollection::end()const{
  return mIDToProxy.end();
};

CAccessorAdminCollection::CAccessorAdminCollection(string inFileName):
  mCollectionInConstruction(0),
  mFactory(new CAccessorFactoryContainer()){
  {
    cout << "-----" << endl
	 << "Configuring CAccessorAdminCollection" << endl;
    // an XMLElement specifying what directories to scan
    // presently this is just one directory,
    // the libdir
    CXMLElement* lFilesToScan(new CXMLElement("directories-to-scan",0));
    assert(lFilesToScan);
    cout << "." << flush;
    CXMLElement* lTemp(new CXMLElement("directory",0));
    assert(lTemp);
    cout << "." << __LIBDIR__ << "." << flush;
    lTemp->addAttribute("name",__LIBDIR__);
    cout << "." << flush;
    lFilesToScan->addChild(lTemp);
    cout << "." << flush;
    lFilesToScan->moveUp();
    cout << "." << flush;
    // configure the factory i.e. make it scan directories
    mFactory->configure(lFilesToScan);
    cout << "." << flush;
  }

  ifstream lConfigFile(inFileName.c_str());
  if(lConfigFile){
      lConfigFile.seekg(0,ios::end);
      int lSize=lConfigFile.tellg();
      char lConfigFileContent[lSize+1];
      
      lConfigFile.seekg(0,ios::beg);
      lConfigFile.read(lConfigFileContent,
			lSize);
      
      lConfigFileContent[lSize]=0;
      
      cout << "--" 
	   << "gift::CAccessorAdminCollection : I read the config file " << endl
	   << inFileName << endl
#ifdef CAC_DEBUG_PRINTOUT
	   << "This file contains the following string: " << endl
	   << lConfigFileContent 
	   << endl
#endif
	   << "Now we will parse the contents of this file using an XML parser "
	   << endl;

      XML_Parser lParser = XML_ParserCreate(NULL);//default encoding
      XML_SetUserData(lParser,
		      this);
      XML_SetElementHandler(lParser, 
			    startAccessorAdminCollectionElement, 
			    endAccessorAdminCollectionElement);
      int lDone=true;

      do {
	if (!XML_Parse(lParser, 
		       lConfigFileContent, 
		       lSize, 
		       lDone)) {
	  cerr << "CAccessorAdminCollection: XML ERROR: "
	       << XML_ErrorString(XML_GetErrorCode(lParser))
	       << " at line "
	       << XML_GetCurrentLineNumber(lParser)
	       << endl
	       << "file was:["
	       << endl
	       << lConfigFileContent
	       << "]"
	       << endl;
	  exit(1);// this happens only in the config file
	}
      } while (!lDone);

      cout << "The config file has been parsed " << endl
	   << "successfully maybe other parts" << endl
	   << "of the program will also parse" << endl
	   << "the same config file. " << endl
	   << "FINISHED configuring CAccessorAdminCollection." << endl;
      XML_ParserFree(lParser);  
  }else{
    my_throw(string(string("could not open config file_")+inFileName+string("_")).c_str());
  }
};
///
void CAccessorAdminCollection::addCollection(CXMLElement* inCollectionElement){
  pair<bool,string> lID=inCollectionElement->stringReadAttribute(mrml_const::collection_id);
  
  if(!lID.first){
    cout << "throwing: "
	 << VEProgrammingMistake("addAccessor")
	 << endl
	 << flush;
    my_throw(VEProgrammingMistake("addAccessor"));
  }

  if(mIDToProxy.find(lID.second)
     !=
     mIDToProxy.end()){
    cout << "The ID I am looking for is:"
	 << lID.second
	 << ", and I did not find it"
	 << endl;
    cout << "throwing: "
	 << VEDoubleInsert("addAccessor")
	 << endl
	 << flush;
    my_throw(VEDoubleInsert("addAccessor"));
  }else{
    mIDToProxy[lID.second] = (mCurrentProxy = new CAccessorAdmin(*mFactory));
    mIDToProxy[lID.second]->setCollectionElement(inCollectionElement);
    cout << "CAccessorAdminCollection: Adding collection " 
	 << inCollectionElement->stringReadAttribute("collection-name").second << endl
	 << "under the ID " << lID.second << endl;
	 
  }
};
///
CAccessorAdmin& CAccessorAdminCollection::getCurrentProxy(){
  return *mCurrentProxy;
};
///
CAccessorAdmin& CAccessorAdminCollection::getProxy(const string& inID){
  if(mIDToProxy.find(inID)!=mIDToProxy.end())
    return *mIDToProxy[inID];
  else{
    cerr << "The ID I am looking for is:"
	 << inID
	 << ", and I did not find it"
	 << endl;
    // my_throw(VENotFound(string("mIDToProxy _"+inID+"_").c_str()));
    // return the default collection

    //
    // quick fix by WM against people nuking our server... :-(
    //
    // 	BE WARNED: this can fail, if not every query 
    // 	processor works with every collection.
    return *mIDToProxy[this->getDefaultAccessorID()];
  }
};


//get some default accessor: simply take the first
string CAccessorAdminCollection::getDefaultAccessorID()const{
  return mIDToProxy.begin()->first;
};

//turn this into xml (simplissime)
CXMLElement* CAccessorAdminCollection::toXMLElement()const{

    cout << "x" 
	 << flush
	 << toXML(0)
	 << endl;

  CXMLElement* lReturnValue(new CXMLElement(mrml_const::collection_list,0));
  
  for(CContent::const_iterator i=mIDToProxy.begin();
      i!=mIDToProxy.end();
      i++){
    CXMLElement* lNewChild(i->second->toXMLElement());
    if(lNewChild){
      lReturnValue->addChild(lNewChild);
      lReturnValue->moveUp();
    }
  }
  return lReturnValue;
};
//turn this into xml (simplissime)
string CAccessorAdminCollection::toXML(bool isPrivate)const{
  string lRetVal=string("<")+mrml_const::collection_list+">\n";

  

  for(CContent::const_iterator i=mIDToProxy.begin();
      i!=mIDToProxy.end();
      i++){
    lRetVal+=i->second->toXML(isPrivate);
  }

  lRetVal+=string("</")+mrml_const::collection_list+">\n";
  return lRetVal;
};
CXMLElement* CAccessorAdminCollection::getCollectionInConstruction(){
  return mCollectionInConstruction;
}
void CAccessorAdminCollection::startCollectionConstruction(CXMLElement*
						 inCollection){
  mCollectionInConstruction=inCollection;
}

//destroying the content of this
CAccessorAdminCollection::CContent::~CContent(){
  for(const_iterator i=begin();
      i!=end();
      i++){
    delete i->second;
  }
};
/**
   get a collection configuration element by its ID
*/
const CXMLElement* CAccessorAdminCollection::getCollectionElement(string inID)const{
  CContent::const_iterator lFound(mIDToProxy.find(inID));

  if(lFound!=mIDToProxy.end()){
    return lFound->second->getCollectionElement();
  }
  return 0;
}
/**  
     get a query paradigm element by the ID of its collection
*/
const CXMLElement* CAccessorAdminCollection::getQueryParadimList(string inID)const{
  CContent::const_iterator lFound(mIDToProxy.find(inID));

  if(lFound!=mIDToProxy.end()){
    return lFound->second->getQueryParadigmList();
  }
  return 0;
}
