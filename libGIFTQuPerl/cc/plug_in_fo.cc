#ifdef __GIFT_WITH_PERL__

#include "libGIFTQuPerl/include/CQPerl.h"
#include "libMRML/include/CAccessorAdminCollection.h"



extern "C" char* libGIFTQuPerl_getClassName(){
  return "perl";
}

extern "C" CQuery* libGIFTQuPerl_makeQuery(CAccessorAdminCollection& inAccessorAdminCollection,
				CAlgorithm& inAlgorithm
				){
  return new CQPerl(inAccessorAdminCollection,
		    inAlgorithm
		    );
}
#endif
