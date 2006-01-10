// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

     Copyright (C) 2003, 2004 Bayreuth University
      2005 Bamberg University
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

#include "libMRML/include/CQueryPlugin.h"

/** makes a CQuery object*/
CQuery* CQueryPlugin::makeQuery(CAccessorAdminCollection& inAccessorAdminCollection,
			       CAlgorithm& inAlgorithm){
  return (*mMakeQuery)(inAccessorAdminCollection,
		       inAlgorithm);
}
/** gets the name of the plugin*/
char* CQueryPlugin::getName(){
  return (*mGetName)();
};

/** makes the CQueryPlugin. All it needs is:
    @param inLibraryFileName the file name of the shared object 
    to be treated */
CQueryPlugin::CQueryPlugin(string inLibraryDirName,
			   string inLibraryFileName,
			   string inLibraryName):
  mGetName(0),
  mMakeQuery(0),
  mName("no name given"),
  mIsSane(0){

  char* lError;
  string lLibraryPath=inLibraryDirName+"/"+inLibraryFileName;
  string lStringGetClassName(inLibraryName+"_getClassName");
  string lStringMakeQuery(inLibraryName+"_makeQuery");

  mDlOpenHandle = dlopen (lLibraryPath.c_str(), RTLD_LAZY);
  if (!mDlOpenHandle) {
    cerr << "..Could not open library: " << endl
	 << dlerror() << endl;
    //exit(1);
    mIsSane=0;
  }else{
    mGetName = (typeof(mGetName))dlsym(mDlOpenHandle, 
				       lStringGetClassName.c_str());
    if ((lError = dlerror()) != NULL)  {
      cerr << lError << endl;
    }else{
      mName=(*mGetName)();
      mMakeQuery=(typeof(mMakeQuery))dlsym(mDlOpenHandle,
					   lStringMakeQuery.c_str());
      if ((lError = dlerror()) != NULL)  {
	cerr << lError << endl;
      }else{
	mIsSane=1;
      }
    }
  }
}

CQueryPlugin::CQueryPlugin(CQueryPlugin& inPlugin):
  mGetName(inPlugin.mGetName),
  mMakeQuery(inPlugin.mMakeQuery),
  mName(inPlugin.mName){
}

CQueryPlugin::~CQueryPlugin(){
  if(mDlOpenHandle){
    dlclose(mDlOpenHandle);
  }
}

bool CQueryPlugin::isSane()const{
  return mIsSane;
}
