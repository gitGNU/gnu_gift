#include "libGIFTAcPerl/include/CAFPerl.h"

#ifdef __GIFT_WITH_PERL__

/**
 */
CAccessor* CAFPerl::makeAccessor(const CXMLElement& inConfiguration)const{
  return new CAcPerl(inConfiguration);
};

/**
 */
CAFPerl::CAFPerl(){
  mName="perl";
};

#endif
