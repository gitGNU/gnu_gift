#include "CAcDistanceMatrix.h"
#include "CAccessorAdminCollection.h"
#include <iostream>
extern "C" char* libGIFTAcDistanceMatrix_getClassName(){
  return "distance_matrix";
}

extern "C" CAcDistanceMatrix* libGIFTAcDistanceMatrix_makeAccessor(const CXMLElement& inElement){
  CAcDistanceMatrix* lReturnValue(new CAcDistanceMatrix(inElement));
  lReturnValue->checkNPrint();
  cout << __FILE__ << ":" << __LINE__ << "made " << lReturnValue << endl;
  return lReturnValue;
}
