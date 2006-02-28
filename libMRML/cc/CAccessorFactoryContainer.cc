// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

     Copyright (C) 2003, 2004 Bayreuth University
    Copyright (C) 2005,2006 Bamberg University
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
#include "libMRML/include/my_throw.h"
#include "libMRML/include/GIFTExceptions.h"
#include "libMRML/include/CAccessor.h"
#include "libMRML/include/CAccessorFactory.h"
#include "libMRML/include/CAccessorFactoryContainer.h"

#include <sys/types.h> // reading a directory
#include <dirent.h>    // reading a directory

// classes to include into the CAccessorFactoryContainer
#include "libGIFTAcInvertedFile/include/CAFInvertedFile.h"
#include "libGIFTAcInvertedFile/include/CAFIFMySQL.h"
#include "libGIFTAcPerl/include/CAFPerl.h"
#include "libGIFTAcURL2FTS/include/CAFURL2FTS.h"
#include "libGIFTAcDistanceMatrix/include/CAFDistanceMatrix.h"
#include "libGIFTAcHierarchy/include/CAFHierarchy.h"
#include "libMRML/include/CXMLElement.h"
#include "libMRML/include/CAFPlugin.h"
#include <string>
#include <iostream>
#include <set>

#include "libMRML/include/getLibNameFromFileName.h"


CAccessorFactoryContainer::CAccessorFactoryContainer():
  mConfigurationData(0){
  //fill the accessor factory container in a sensible way
  //  (new CAFInvertedFile())->registerFactory(*this);
  // #ifdef __GIFT_BAYESIAN__
  //   (new CAFDistanceMatrix())->registerFactory(*this);
  // #endif
  // #ifdef __GIFT_WITH_MYSQL__
  //   (new CAFIFMySQL())->registerFactory(*this);
  // #endif

#ifdef __GIFT_WITH_PERL__
   (new CAFPerl())->registerFactory(*this);
#endif
  //(new CAFURL2FTS())->registerFactory(*this);
  //(new CAFHierarchy())->registerFactory(*this);
}

/** configure this */
void CAccessorFactoryContainer::configure(CXMLElement* inConfigurationData){
  mConfigurationData=inConfigurationData;
#ifdef LINK_DYNAMICALLY
  cout << "--" << endl
       << "Configuring the CAccessorFactoryContainer:" << endl
       << "I am going to scan the following directorys for plugins "<< endl;
       
  set<string> lSeenLibs;

  if(inConfigurationData){
    for(list<CXMLElement*>::const_iterator i=inConfigurationData->child_list_begin();
	i!=inConfigurationData->child_list_end();
	i++
	){
      if((*i)->getName()=="directory"){
	cout << (*i)->stringReadAttribute("name").second
	     << endl;
	{
	  pair<bool,string> lDirectoryName((*i)->stringReadAttribute("name"));
	  DIR* lDirectory(0);
	  if(lDirectoryName.first){
	    lDirectory=opendir(lDirectoryName.second.c_str());
	  }  
	  if(!lDirectory){
	    cerr << "FATAL ERROR, cannot find " << flush;
	    if(lDirectoryName.first){
	      cerr << lDirectoryName.second ;
	    }
	    cerr << endl;
	    exit(20);
	  }else{
	    dirent* lDirectoryEntry;
	    while(lDirectoryEntry=readdir(lDirectory)){
	      string lFileName(lDirectoryEntry->d_name);

	      pair<bool,string> lIsLibAndLibName(getLibNameFromFileName("libGIFTAc",lFileName));
	      bool lIsLib(lIsLibAndLibName.first);
	      string lLibName(lIsLibAndLibName.second);

	      if(lIsLib && (lSeenLibs.find(lLibName)==lSeenLibs.end())){
		CAFPlugin* lPlugin(new CAFPlugin(lDirectoryName.second.c_str(),
						 lFileName,
						 lLibName));
		if(lPlugin->isSane()){
		  cout << lFileName << " contains a sane GIFT Accessor plugin: " << lPlugin->getName() << endl;
		  registerFactory(lPlugin,string(lPlugin->getName()));
		  lSeenLibs.insert(lLibName);
		}else{
		  cout << lFileName << " tested. Test FAILED. " << endl;
		  delete lPlugin;
		}
	      }
	      else{
		if(!lIsLib){
		  cout << "Not testing file:" << lFileName << " (File name does not match plugin name) " << endl;
		}else{
		  cout << "Lib:" << lLibName << ", to be linked from " << lFileName << " already registered! " << endl;
		}
 	      }
	    }
	  }
	}
      }
    }
  }
  cout << "FINISHED configuring the CAccessorFactoryContainer."
       << endl;
#endif
};
/**
     The destructor destroys the CContentElements properly 
  */
CAccessorFactoryContainer::~CAccessorFactoryContainer(){
  for(CContent::iterator i=mContent.begin();
      i!=mContent.end();
      i++){
    delete i->second.mFactory;
  }
};
/** 
      adding a factory to the content
   */
void CAccessorFactoryContainer::registerFactory(CAccessorFactory* inFactory,
						string inName){
  CContent::const_iterator lFound=mContent.find(inName);
  if(lFound!=mContent.end()){
    string lMessage("Accessor ");
    lMessage+=inName;
    lMessage+=" tried to register twice!";
    my_throw(VEProgrammingMistake(lMessage.c_str()));
  }else{
    mContent[inName].mFactory=inFactory;
    mContent[inName].mOpenCloseCounter=0;
  }
};

bool CAccessorFactoryContainer::isPresentFactory(string inName)const{
  return(mContent.find(inName)!=mContent.end());
}

/**
     opening an accessor:
     if the accessor is already constructed,
     then simply give the pointer,
     otherwhise construct a new one 
     using the appropriate factory

  */
CAccessor* CAccessorFactoryContainer::openAccessor(string inName,
						   const CXMLElement& inCollectionElement){
  CContent::const_iterator lFound=mContent.find(inName);
  if(lFound==mContent.end()){
    cerr << endl
	 << "CAccessorFactoryContainer::openAccessor:"
	 << inName << endl
	 << " NOT FOUND." << endl;
    return 0;
  }else{
    if(!(mContent[inName].mOpenCloseCounter)){
      mContent[inName].mOpenCloseCounter++;
      cerr << endl
	   << "CAccessorFactoryContainer::openAccessor:"
	   << inName << endl
	   << " NEWLY GENERATED." << endl;
    }else{
      cerr << endl
	   << "CAccessorFactoryContainer::openAccessor:"
	   << inName << endl
	   << " REUSED." << endl;
    }
    return mContent[inName].mFactory->makeAccessor(inCollectionElement);
  }
};
/**
   closing an accessor.
   If there have been as many openAccessor 
   as closeAccessor calls, the accessor will
   be delete-d.
*/
void CAccessorFactoryContainer::closeAccessor(string inName){
  CContent::iterator lFound=mContent.find(inName);
  if((lFound!=mContent.end())
     &&
     !(--(lFound->second.mOpenCloseCounter))){
    lFound->second.mFactory->closeAccessor();
  }
};
  
    

  
