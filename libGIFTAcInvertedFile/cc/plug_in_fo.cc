#include "libGIFTAcInvertedFile/include/CAcIFFileSystem.h"
#include "libMRML/include/CAccessorAdminCollection.h"
#include <iostream>
extern "C" extern "C" char* libGIFTAcInvertedFile_getClassName(){
  return "inverted_file";
}

extern "C" extern "C" CAcIFFileSystem* libGIFTAcInvertedFile_makeAccessor(const CXMLElement& inElement){
  cout << endl
       << "CALLED: libGIFTAcInvertedFile_makeAccessor"
       << endl ;
  return new CAcIFFileSystem(inElement);
}
