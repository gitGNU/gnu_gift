#include "libGIFTAcHierarchy/include/CAcHierarchy.h"
#include "libMRML/include/CAccessorAdminCollection.h"
extern "C" char* libGIFTAcHierarchy_getClassName(){
  return "hierarchy";
}

extern "C" CAcHierarchy* libGIFTAcHierarchy_makeAccessor(const CXMLElement& inElement){
  return new CAcHierarchy(inElement);
}
