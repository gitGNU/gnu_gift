#include "libGIFTQuHierarchy/include/CQHierarchy.h"
#include "libMRML/include/CAccessorAdminCollection.h"
extern "C" char* libGIFTQuHierarchy_getClassName(){
  return "hierarchy";
}

extern "C" CQuery* libGIFTQuHierarchy_makeQuery(CAccessorAdminCollection& inAccessorAdminCollection,
					CAlgorithm& inAlgorithm
					){
  return new CQHierarchy(inAccessorAdminCollection,
			 inAlgorithm
			 );
}
