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
#include "mrml_const.h" // for parsing
#include "my_throw.h"
#include "my_assert.h"
#include "GIFTExceptions.h"
#include "CAlgorithmCollection.h"

#include "xmlparse.h" // for reading the xml

#include "CXMLHelper.h"

#include <vector>

#include "CQInvertedFile.h"
#ifdef __GIFT_BAYESIAN__
#include "CIFQuickAndDirtyHunter.h"
#endif
#include "CQParallel.h"

void startAlgorithmCollectionElement(void *userData, 
				     const char *inElementName, 
				     const char **inAttributes){
  bool lSuccessful=true;
  //the calling CSessionManager
  CAlgorithmCollection* lCaller((CAlgorithmCollection*)userData);
  
#ifdef CAlC_DEBUG_PRINTOUT
  cout << "<"
       << inElementName
       << " WhileReading=\"true\" "
       << flush;
#endif
  CAttributeList lAttributes(inAttributes);

//   if(string(inElementName)==
//      string(mrml_const::cui_property_list)){
//     lCaller->startPropertySheetSubtree();
//   }

//   //if we have started a property sheet list
//   //everything goes there
//   if(lCaller->getPropertySheetSubtree()){
//     if((string(inElementName)==
// 	string(mrml_const::cui_property_item))
// 	||
//        (string(inElementName)==
// 	string(mrml_const::property_sheet))){
// 	lCaller->getPropertySheetSubtree()
// 	  ->addChild(inElementName,
// 		     inAttributes);
//     }
//   }
  if(string(inElementName)
     ==
     string(mrml_const::algorithm)
     || (lCaller->getAlgorithmInConstruction())
     ){
    {
      if(!lCaller->getAlgorithmInConstruction()){
	lCaller->startAlgorithmConstruction(new CAlgorithm(inElementName,
							   inAttributes));
      }else{
	lCaller->getAlgorithmInConstruction()->addChild(inElementName,
							inAttributes);
      }      
    }
  }

#ifdef CAlC_DEBUG_PRINTOUT
  cout << " >"
       << endl
       << flush;
#endif
}
void endAlgorithmCollectionElement(void *userData, const char *inElementName){
  CAlgorithmCollection* lCaller((CAlgorithmCollection*)userData);

  if((string(inElementName)
     ==
     string(mrml_const::algorithm))
     ||lCaller->getAlgorithmInConstruction()){
    {
      my_assert(lCaller->getAlgorithmInConstruction(),
		"Algorithm in construction not found");

      lCaller->getAlgorithmInConstruction()->moveUp();
      if(lCaller->getAlgorithmInConstruction()->isSubtreeFinished()){
	lCaller->addAlgorithm(lCaller->getAlgorithmInConstruction());
	lCaller->startAlgorithmConstruction((CAlgorithm*)0);
      }
    }
  }
  //if we have started a property sheet list
  //everything goes there
 //  if(lCaller->getPropertySheetSubtree()){
//     if((string(inElementName)==
// 	string(mrml_const::cui_property_item))
// 	||
//        (string(inElementName)==
// 	string(mrml_const::property_sheet))){
//       lCaller->getPropertySheetSubtree()
// 	->moveUp();
//     }
//   }
//   if(string(inElementName)==
//      string(mrml_const::cui_property_list)){
//     lCaller->endPropertySheetSubtree();
//   }
#ifdef CAlC_DEBUG_PRINTOUT
  cout << "</"
       << inElementName
       << " >"
       << endl;
#endif
}

CAlgorithmCollection::CAlgorithmCollection(string inConfigFileName):
  mAlgorithmInConstruction(0)
{

  ifstream lConfigFile(inConfigFileName.c_str());

  if(lConfigFile){


    lConfigFile.seekg(0,ios::end);
    int lSize=lConfigFile.tellg();
    char lConfigFileContent[lSize+1];

    lConfigFile.seekg(0,ios::beg);
    lConfigFile.read(lConfigFileContent,
		     lSize);
  

    lConfigFileContent[lSize]=0;
  
    cout << "gift: CAlgorithmCollection: The following is the config file " << endl
	 << "located at \"" << inConfigFileName << "\" as read by this program: " 
	 << endl;
    cout << lConfigFileContent
	 << endl
	 << "This config file content will now be parsed by an XML parser " << endl
	 << "If error messages occur, these are related to the content shown above." << endl;
  
    XML_Parser lParser = XML_ParserCreate(NULL);//default encoding
    XML_SetUserData(lParser,
		    this);
    XML_SetElementHandler(lParser, 
			  startAlgorithmCollectionElement, 
			  endAlgorithmCollectionElement);
    int lDone=true;
    do {
      if (!XML_Parse(lParser, 
		     lConfigFileContent, 
		     lSize, 
		     lDone)) {
	cerr << "XML ERROR: "
	     << XML_ErrorString(XML_GetErrorCode(lParser))
	     << " at line "
	     << XML_GetCurrentLineNumber(lParser)
	     << endl
	     << "file was:["
	     << endl
	     << lConfigFileContent
	     << "]"
	     << endl;

	exit(1);
      }
    } while (!lDone);


    XML_ParserFree(lParser);    
    cout << "gift: CAlgorithmCollection: The config file has " << endl
	 << "been parsed successfully by CAlgorithmCollection" << endl
	 << "maybe other parts of the program will also parse" << endl
	 << "the same config file. " << endl;
       
  }else{
    //here I get an internal compiler error with egcs
    // my_throw(VENotFound(inConfigFileName.c_str()));
    cerr << "gift: I could not find config file: " 
	 << inConfigFileName 
	 << endl;
    exit(1);
  }

}

CAlgorithmCollection::~CAlgorithmCollection(){
  for(CContent::iterator i=mContent.begin();
      i!=mContent.end();
      i++){
    delete i->second;
  }
};
/** for being able to read all the content */
CAlgorithmCollection
::CContent::const_iterator CAlgorithmCollection::begin()const{
  return mContent.begin();
}
/** for being able to read all the content */
CAlgorithmCollection
::CContent::const_iterator CAlgorithmCollection::end()const{
  return mContent.end();
}

///adding an algorithm with a given ID/name and a given base
void CAlgorithmCollection::addAlgorithm(CAlgorithm* inAlgorithm){
  mContent.insert(make_pair(inAlgorithm->getType().second,
			    inAlgorithm));
};

bool CAlgorithmCollection::containsAlgorithmByType(const string& inType)const{
  CContent::const_iterator lFound(mContent.find(inType));
  return(lFound!=mContent.end());
};
CAlgorithm& CAlgorithmCollection::getAlgorithmByType(const string& inType)const{
  CContent::const_iterator lFound(mContent.find(inType));
  if(lFound!=mContent.end()){
    return *lFound->second;
  }else{
    // return a default algorithm, if none was found.
    
    cout << "returning a default algorithm:"
	 << mContent.begin()->first
	 << ": "
	 << mContent.begin()->second
	 << endl;

    if(mContent.find("adefault")!=mContent.end()){
      return *mContent.find("adefault")->second;
    }
    assert(mContent.size());
    return *mContent.begin()->second;
  }
}
CAlgorithm* CAlgorithmCollection::makeAlgorithmByType(const string& inType)const{
  CContent::const_iterator lFound(mContent.find(inType));
  if(lFound!=mContent.end()){
    return lFound->second->clone();
  }else{
    return 0;
  }
};
CAlgorithm* CAlgorithmCollection::getDefaultAlgorithm()const{
  //at present, take simply the first element
  //it might be an xml tag in the future

  cout << "We have " 
       << mContent.size() 
       << " algorithms in the algorithm collection."
       << endl;


  CContent::const_iterator lFound=mContent.find(string("adefault"));

  my_assert(lFound!=mContent.end(),
	    "There SHOULD be a 'adefault' algorithm in the config file!");

  

  return lFound->second;
};
CAlgorithm* CAlgorithmCollection::makeDefaultAlgorithm()const{
  //at present, take simply the first element
  //it might be an xml tag in the future
  return new CAlgorithm(*getDefaultAlgorithm());
};

string CAlgorithmCollection::toXML(bool inIsPrivate)const{
  CXMLHelper lHelper;
  lHelper.setName(mrml_const::algorithm_list);
  string lReturnValue=lHelper.toString()+"\n";

  for(CContent::const_iterator i=mContent.begin();
      i!=mContent.end();
      i++){
    i->second->toXML(lReturnValue);
  }
  lHelper.setEnd();
  lReturnValue+=lHelper.toString()+"\n";
  return lReturnValue;
}


CXMLElement* CAlgorithmCollection::toXMLElement()const{
  CXMLElement* lReturnValue(new CXMLElement(mrml_const::algorithm_list,0));

  for(CContent::const_iterator i=mContent.begin();
      i!=end();
      i++){
    CXMLElement* lNewChild=new CXMLElement(*i->second);
    lReturnValue->addChild(lNewChild);
    lReturnValue->moveUp();
  }
  return lReturnValue;
}


CXMLElement* CAlgorithmCollection::getAlgorithmList(const CXMLElement& inGetAlgorithms)const{
  

  return 0;
};
//--------------------------------------------------
CAlgorithm* CAlgorithmCollection::getAlgorithmInConstruction(){
  return mAlgorithmInConstruction;
}
//--------------------------------------------------
void CAlgorithmCollection::startAlgorithmConstruction(CAlgorithm*
						      inAlgorithm){
  mAlgorithmInConstruction=inAlgorithm;
}
//--------------------------------------------------
CQuery* CAlgorithmCollection::makeQuery(const string & inBaseType, 
					CAccessorAdminCollection & inAccessorAdminCollection,
					CAlgorithm & inAlgorithm,
					CStaticQueryFactory & inFactory){

  assert(!"this should not be called");
  return inFactory.makeQuery(inBaseType, 
			     inAccessorAdminCollection,
			     inAlgorithm);
}
