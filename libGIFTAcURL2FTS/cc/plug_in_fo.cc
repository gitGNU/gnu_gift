#include "libGIFTAcURL2FTS/include/CAcURL2FTS.h"
#include "libMRML/include/CAccessorAdminCollection.h"
extern "C" char* libGIFTAcURL2FTS_getClassName(){
  return "url2fts";
}

extern "C" CAcURL2FTS* libGIFTAcURL2FTS_makeAccessor(const CXMLElement& inElement){
  return new CAcURL2FTS(inElement);
}
