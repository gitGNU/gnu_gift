#ifdef __GIFT_WITH_MYSQL__

#include "libGIFTAcInvertedFile/include/CAFIFMySQL.h"
#include "libGIFTAcInvertedFile/include/CAcIFMySQL.h"

/**
 */
CAccessor* CAFIFMySQL::makeAccessor(const CXMLElement& inConfiguration)const{
  //presently there's no choice

  cout << "éééMAKING MYSQL" << endl;

  return new CAcIFMySQL(inConfiguration);
};

/**
 */
CAFIFMySQL::CAFIFMySQL(){
  mName="if_mysql";
};

#endif
