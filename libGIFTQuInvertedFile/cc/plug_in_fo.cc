#include "libGIFTQuInvertedFile/include/CQInvertedFile.h"
#include "libMRML/include/CAccessorAdminCollection.h"
extern "C" char* libGIFTQuInvertedFile_getClassName(){
  return "inverted_file";
}

extern "C" CQuery* libGIFTQuInvertedFile_makeQuery(CAccessorAdminCollection& inAccessorAdminCollection,
					CAlgorithm& inAlgorithm
					){
  return new CQInvertedFile(inAccessorAdminCollection,
			    inAlgorithm
			    );
}
