#include "CQHierarchy.h"
#include "CAccessorAdminCollection.h"
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
