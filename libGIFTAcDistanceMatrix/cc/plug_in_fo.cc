#include "CAcDistanceMatrix.h"
#include "CAccessorAdminCollection.h"
extern "C" char* libGIFTAcDistanceMatrix_getClassName(){
  return "distance_matrix";
}

extern "C" CAcDistanceMatrix* libGIFTAcDistanceMatrix_makeAccessor(const CXMLElement& inElement){
  return new CAcDistanceMatrix(inElement);
}
