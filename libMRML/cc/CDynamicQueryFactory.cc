// -*- mode: c++ -*- 
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
#include "CDynamicQueryFactory.h"
#include "CQMultiple.h"
#include "CQPerl.h"

CQuery* CDynamicQueryFactory::makeQuery(const string & inBaseType, 
					   CAccessorAdminCollection & inAccessorAdminCollection,
					   CAlgorithm & inAlgorithm){
  if(inBaseType=="multiple"){
    return(new CQMultiple(inAccessorAdminCollection,
			  inAlgorithm));
  }
#ifdef __GIFT_WITH_PERL__
  if(inBaseType=="perl"){
    return(new CQPerl(inAccessorAdminCollection,
		      inAlgorithm));
  }
#endif

  const_iterator lFound(find(inBaseType));
  if(lFound!=end()){
    cout << "CDynamicQueryFactory" << endl;
    return lFound->second->makeQuery(inAccessorAdminCollection,
				     inAlgorithm);
  }else{
    cerr << "CDynamicQueryFactory: " << inBaseType << " was not found";
    exit(20);
  }
}

CDynamicQueryFactory::CDynamicQueryFactory(string inDirectoryName){
  cout << "--" << endl
       << "Configuring CDynamicQueryFactory " << endl
       << "Looking for libGIFTQu*.so in directory " << endl
       << inDirectoryName << endl;
  DIR* lDirectory(opendir(inDirectoryName.c_str()));  
  if(!lDirectory){
    cerr << "FATAL ERROR, cannot find " << inDirectoryName << endl;
    exit(20);
  }else{
    dirent* lDirectoryEntry;
    while(lDirectoryEntry=readdir(lDirectory)){
      string lFileName(lDirectoryEntry->d_name);

      if(!strncmp(".so",lFileName.c_str()+lFileName.size()-3,3)){
	if(lFileName.find("libGIFTQu")==0){
	  CQueryPlugin* lPlugin(new CQueryPlugin(inDirectoryName,
						 lFileName));
	  if(lPlugin->isSane()){
	    cout << lFileName << " contains a sane GIFT Query plugin: " << lPlugin->getName() << endl;
	    insert(make_pair(string(lPlugin->getName()),
			     lPlugin));
	  }else{
	    delete lPlugin;
	  }
	}
      }
    }
  }
  cout << "FINISHED Configuring CDynamicQueryFactory " << endl;
}

