#include "CAFHierarchy.h"


CAccessor* CAFHierarchy::makeAccessor(const CXMLElement& inConfiguration)const{
  return new CAcHierarchy(inConfiguration);
};

CAFHierarchy::CAFHierarchy(){
  mName="hierarchy";
};

