// -*- mode: c++ -*- 
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
#include "libGIFTAcHierarchy/include/CAcHierarchy.h"
#include <assert.h>
#include "libMRML/include/GIFTExceptions.h"
#include "expat/xmlparse/xmlparse.h"
#include <fstream>
#include "libMRML/include/CXMLElement.h"
#include "libMRML/include/CAttributeList.h"

void startHierarchyElement(void *userData, 
				    const char *inElementName, 
				    const char **inAttributes)
{
  bool lSuccessful=true;
  //the calling CSessionManager
  CAcHierarchy* lCaller((CAcHierarchy*)userData);
  
  cout << "<"
       << inElementName
       << " WhileReading=\"true\" _"
       << flush;

  CAttributeList lAttributes(inAttributes);

  cout << "--"
       << flush;

  cout << lAttributes.size()
       << "."
       << flush;

  if(string(inElementName)
     ==
     string("element")){
    {
      pair<bool,int> lID(lAttributes.longReadAttribute("image-id"));
      pair<bool,string> lLocation(lAttributes.stringReadAttribute("image-location"));

      if(lID.first && lLocation.first){
	lCaller->addElement(lID.second,
			    lLocation.second,
			    lLocation.second);
      }
    }
  }
  if(string(inElementName)
     ==
     string("reference-tree")){
    {
      pair<bool,int> lID(lAttributes.longReadAttribute("image-id"));
      if(lID.first){
	lCaller->push(lID.second);
      }
    }
  }

  
}
  
void endHierarchyElement(void *userData, const char *inElementName){
  CAcHierarchy* lCaller((CAcHierarchy*)userData);
  if(string(inElementName)
     ==
     string("reference-tree")){
    {
      lCaller->popNodeAndRegisterEdge();
    }
  }
  
  if(string(inElementName)
     ==
     string("hierarchy-browser")){
    {
      
    }
  }
  
  cout << "</"
       << inElementName
       << " >"
       << endl;
}

CAcHierarchy::CAcHierarchy(const CXMLElement& inCollectionElement){
  mMutexHierarchy.lock();
  init(inCollectionElement
       .stringReadAttribute(mrml_const::cui_base_dir).second
       +inCollectionElement
       .stringReadAttribute(mrml_const::cui_hierarchy_file_location).second);
  mMutexHierarchy.unlock();
}

void CAcHierarchy::init(string inFileName){
  mMutexHierarchy.lock();

  ifstream lHierarchyFile(inFileName.c_str());
  if(lHierarchyFile){
      lHierarchyFile.seekg(0,ios::end);
      int lSize=lHierarchyFile.tellg();
      char lHierarchyFileContent[lSize+1];
      
      lHierarchyFile.seekg(0,ios::beg);
      lHierarchyFile.read(lHierarchyFileContent,
			  lSize);
      
      lHierarchyFileContent[lSize]=0;
      
      cout << "I did it: I read the HIERARCHY file "
	   << inFileName
	   << ": "
	   << lHierarchyFileContent
	   << endl
	   << flush;

      XML_Parser lParser = XML_ParserCreate(NULL);//default encoding
      XML_SetUserData(lParser,
		      this);
      XML_SetElementHandler(lParser, 
			    startHierarchyElement, 
			    endHierarchyElement);
      int lDone=true;

      do {
	if (!XML_Parse(lParser, 
		       lHierarchyFileContent, 
		       lSize, 
		       lDone)) {
	  cerr << "XML ERROR: "
	       << XML_ErrorString(XML_GetErrorCode(lParser))
	       << " at line "
	       << XML_GetCurrentLineNumber(lParser)
	       << endl;
	  exit(1);
	}
      } while (!lDone);

      XML_ParserFree(lParser);  
      mMutexHierarchy.unlock();
  }else{
    mMutexHierarchy.unlock();
    my_throw(string(string("could not open config file_")+inFileName+string("_")).c_str());
  }
};

void CAcHierarchy::addElement(TID inID, 
			      string inLocation, 
			      string inThumbnail){
  mMutexHierarchy.lock();

  mIDToAccessorElement.insert(make_pair(inID,
					CAccessorElement(inID,
							 inLocation,
							 inThumbnail,
							 string(""))));
  
  mURLToID[inLocation]=inID;
  mMutexHierarchy.unlock();
}
/**
   
   pushes the node onto the stack
   
*/
void CAcHierarchy::push(TID inNode){
  mMutexHierarchy.lock();
  mNodeStack.push_back(inNode);
  mMutexHierarchy.unlock();
};
/**
   
   pops the node and registers the edge from
   new top to old top and back
   
*/
void CAcHierarchy::popNodeAndRegisterEdge(){
  mMutexHierarchy.lock();

  if(mNodeStack.size()){
    TID lExTop=mNodeStack.back();
    
    mNodeStack.pop_back();
    
    addEdge(mNodeStack,lExTop);
  }
  mMutexHierarchy.unlock();
};
void CAcHierarchy::addEdge(const CPath& inPath,
			   TID inSon){
  mMutexHierarchy.lock();

  mEdges[inPath].push_back(inSon);
  cout << "SIZE addBackForthEdge: ("
       << inSon
       << ") "
       << mEdges[inPath].size()
       << " "
       << inPath.size()
       << " "
       << mEdges.size()
       << endl;
  mMutexHierarchy.unlock();
}

CIDRelevanceLevelPairList* CAcHierarchy::getChildren(const CPath& inPath)const{
  mMutexHierarchy.lock();

  CGraph::const_iterator lFound(mEdges.find(inPath));
  
  if(lFound!=mEdges.end()){
    CIDRelevanceLevelPairList* lList(new CIDRelevanceLevelPairList());
    
    for(list<TID>::const_iterator i=lFound->second.begin();
	i!=lFound->second.end();
	i++){
      lList->push_back(CIDRelevanceLevelPair(*i,1.0));
    }
    return lList;
  }

  cout << "NOTFOUND" << endl;

  mMutexHierarchy.unlock();
  return 0;
}

CAcHierarchy::operator bool()const{
  return true;
}
