#include "libMRML/include/CAFPlugin.h"
#include "libMRML/include/CAccessorFactoryContainer.h"

/** makes a CAccessor object*/
CAccessor* CAFPlugin::makeAccessor(const CXMLElement& inXMLElement)const{
  return (*mMakeAccessor)(inXMLElement);
}
/** gets the name of the plugin*/
char* CAFPlugin::getName(){
  return (*mGetName)();
};

/** makes the CAFPlugin. All it needs is:
    @param inLibraryFileName the file name of the shared object 
    to be treated */
CAFPlugin::CAFPlugin(string inLibraryDirName,
				 string inLibraryFileName):
  mGetName(0),
  mMakeAccessor(0),
  mName("no name given"),
  mIsSane(0){
  
  const char* lError;
  string lLibraryPath=inLibraryDirName+"/"+inLibraryFileName;
  string lStringGetClassName(inLibraryFileName.substr(0,inLibraryFileName.size()-3)+"_getClassName");
  string lStringMakeAccessor(inLibraryFileName.substr(0,inLibraryFileName.size()-3)+"_makeAccessor");

  mDlOpenHandle = dlopen (lLibraryPath.c_str(), RTLD_LAZY);
  if (!mDlOpenHandle) {
    cerr << "Could not open library: " << endl
	 << dlerror() << endl;
    exit(1);
  }
  
  mGetName = (typeof(mGetName))dlsym(mDlOpenHandle, 
				     lStringGetClassName.c_str());
  if ((lError = dlerror()) != NULL)  {
    cerr << lError << endl;
  }else{
    mName=(*mGetName)();
    mMakeAccessor=(typeof(mMakeAccessor))dlsym(mDlOpenHandle,
				       lStringMakeAccessor.c_str());
    if ((lError = dlerror()) != NULL)  {
      cerr << lError << endl;
    }else{
      mIsSane=1;
    }
  }
}

CAFPlugin::CAFPlugin(CAFPlugin& inPlugin):
  mGetName(inPlugin.mGetName),
  mMakeAccessor(inPlugin.mMakeAccessor),
  mName(inPlugin.mName){
}

CAFPlugin::~CAFPlugin(){
  dlclose(mDlOpenHandle);
}

bool CAFPlugin::isSane()const{
  return mIsSane;
}

void CAFPlugin::registerFactory(CAccessorFactoryContainer& outContainer){
  outContainer.registerFactory(this,
			       getName());
};
