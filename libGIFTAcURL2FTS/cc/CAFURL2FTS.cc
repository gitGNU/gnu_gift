#include "CAFURL2FTS.h"
#include "CAcURL2FTS.h"

CAccessor* CAFURL2FTS::makeAccessor(const CXMLElement& inConfiguration)const{
  return new CAcURL2FTS(inConfiguration);
};

CAFURL2FTS::CAFURL2FTS(){
  mName="url2fts";
};

