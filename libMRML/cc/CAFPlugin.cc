#include "libMRML/include/CAFPlugin.h"
#include "libMRML/include/CAccessorFactoryContainer.h"
#include "libMRML/include/GIFTExceptions.h"

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
		     string inLibraryFileName,
		     string inLibraryName):
  mGetName(0),
  mMakeAccessor(0),
  mName("no name given"),
  mIsSane(0){
  
  const char* lError;
  string lLibraryPath=inLibraryDirName+"/"+inLibraryFileName;
  string lStringGetClassName(inLibraryName+"_getClassName");
  string lStringMakeAccessor(inLibraryName+"_makeAccessor");

  mDlOpenHandle = dlopen (lLibraryPath.c_str(), RTLD_LAZY);
  if (!mDlOpenHandle) {
    cerr << "Could not open library: " << endl
	 << dlerror() << endl;
    //throw VEConfigurationError(dlerror());
    mIsSane=0;
  }else{
  
    mGetName = (typeof(mGetName))dlsym(mDlOpenHandle, 
				       lStringGetClassName.c_str());
    if ((lError = dlerror()) != NULL)  {
      cerr << "Could not link to symbol " << lStringGetClassName << ":" << lError << endl;
    }else{
      mName=(*mGetName)();
      mMakeAccessor=(typeof(mMakeAccessor))dlsym(mDlOpenHandle,
						 lStringMakeAccessor.c_str());
      if ((lError = dlerror()) != NULL)  {
	cerr << "Could not link to symbol " << lStringMakeAccessor << ":" << lError << endl;
      }else{
	mIsSane=1;
      }
    }
  }
}

CAFPlugin::CAFPlugin(CAFPlugin& inPlugin):
  mGetName(inPlugin.mGetName),
  mMakeAccessor(inPlugin.mMakeAccessor),
  mName(inPlugin.mName){
}

CAFPlugin::~CAFPlugin(){
  if(mDlOpenHandle){
    dlclose(mDlOpenHandle);
  }
}

bool CAFPlugin::isSane()const{
  return mIsSane;
}

void CAFPlugin::registerFactory(CAccessorFactoryContainer& outContainer){
  outContainer.registerFactory(this,
			       getName());
};
