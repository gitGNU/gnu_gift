#include "CAFDistanceMatrix.h"
#include "CAcDistanceMatrix.h"


CAccessor* CAFDistanceMatrix::makeAccessor(const CXMLElement& inConfiguration)const{
  return new CAcDistanceMatrix(inConfiguration);
};

CAFDistanceMatrix::CAFDistanceMatrix(){
  mName="distance_matrix";
};

