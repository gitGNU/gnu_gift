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
#include "libMRML/include/mrml_const.h" // for parsing
#include "libMRML/include/CXMLElement.h" // mCollectionElement 
#include "libMRML/include/CXMLHelper.h"
#include "libMRML/include/CMutex.h"
extern CMutex* gMutex;
#include <unistd.h> // printing process ID for debugging
#include "libMRML/include/CAccessorAdmin.h"

///
void CAccessorAdmin::setCollectionElement(CXMLElement* inElement){
  mCollectionElement=inElement;
};
///
const CXMLElement* CAccessorAdmin::getCollectionElement()const{
  return mCollectionElement;
};
///
CAccessorAdmin::CAccessorAdmin(CAccessorFactoryContainer& inFactoryContainer):
  mFactoryContainer(inFactoryContainer)
{
};
///
CAccessor* CAccessorAdmin::openAccessor(string inName){  
  CContent::const_iterator lFound=mContent.find(inName);
  if(lFound==mContent.end()){
    mContent[inName].mOpenCloseCounter=0;
    mContent[inName].mAccessor=0;
  }
  {
    if(!(mContent[inName].mOpenCloseCounter)){
      mContent[inName].mOpenCloseCounter++;
      mContent[inName].mAccessor=mFactoryContainer.openAccessor(inName,
								*mCollectionElement);
      cerr << endl
	   << "CAccessorAdmin::openAccessor:"
	   << inName << endl
	   << " NEWLY GENERATED." << endl;
    }else{
      mContent[inName].mOpenCloseCounter++;
      cerr << endl
	   << "CAccessorAdmin::openAccessor:"
	   << inName << endl
	   << " REUSED." << endl;
    }
    return mContent[inName].mAccessor;
  }
  assert(!"Execution should never arive here");
};
///
void CAccessorAdmin::closeAccessor(string inName){
  if(gMutex){
    gMutex->lock();
  }
  CContent::iterator lFound=mContent.find(inName);

  cout << __FILE__ << ":" << __LINE__ 
       << "PID:" << getpid()
       << "OpenCloseCounter["
       << inName
       << "]"
       << (mContent[inName].mOpenCloseCounter) << endl;
  
  //this is a hack. See the changelog for thisx
#ifdef __GIFT_FREE_UNUSED_ACCESSORS
  if((lFound!=mContent.end())
     &&
     !(--(mContent[inName].mOpenCloseCounter))){
    delete lFound->second.mAccessor;
    mContent.erase(lFound);

    mFactoryContainer.closeAccessor(inName);
  }
#endif
  if(gMutex){
    gMutex->unlock();
  }
};
///
string CAccessorAdmin::toXML(bool isPrivate)const{

  CXMLHelper lHelper;

  lHelper.setName(mrml_const::collection);
  lHelper.setEmpty();
  lHelper.addAttribute(mrml_const::collection_id,mCollectionElement->stringReadAttribute(mrml_const::collection_id).second);
  lHelper.addAttribute(mrml_const::collection_name,mCollectionElement->stringReadAttribute(mrml_const::collection_name).second);
  lHelper.addAttribute(mrml_const::cui_algorithm_id_list_id,getAlgorithmIDListID());
  

  string lReturnValue=lHelper.toString();
  return lReturnValue;
}

///
CXMLElement* CAccessorAdmin::toXMLElement()const{
  CXMLElement* lReturnValue(0);

  if(mCollectionElement){
    lReturnValue=new CXMLElement(*mCollectionElement);
  }

  return lReturnValue;
}

string CAccessorAdmin::getAlgorithmIDListID()const{
  return mCollectionElement->stringReadAttribute(mrml_const::cui_algorithm_id_list_id).second;
}
string CAccessorAdmin::getName()const{
  return mCollectionElement->stringReadAttribute(mrml_const::collection_name).second;
}
string CAccessorAdmin::getID()const{
  return mCollectionElement->stringReadAttribute(mrml_const::collection_id).second;
}

const CXMLElement* CAccessorAdmin::getQueryParadigmList()const{
  for(list<CXMLElement*>::const_iterator i(mCollectionElement->child_list_begin());
      i!=mCollectionElement->child_list_end();
      i++){
    if((*i)->getName()=="query-paradigm-list"){
      return *i;
    }
  }
  return 0;
};


/** */
list<string>* CAccessorAdmin::getAvailableIndices()const{
  list<string>* lReturnValue=new list<string>();

  lReturnValue->push_back("inverted_file");

  return lReturnValue;
};

///
bool CSortByName_pAP::operator()(const CAccessorAdmin* l,
				 const CAccessorAdmin* t){
    return 
      l->mCollectionElement->stringReadAttribute(mrml_const::collection_name).second
      <
      t->mCollectionElement->stringReadAttribute(mrml_const::collection_name).second;
}
			    
