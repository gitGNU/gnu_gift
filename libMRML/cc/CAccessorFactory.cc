#include "CAccessorFactory.h"
#include "CAccessorFactoryContainer.h"

void CAccessorFactory::registerFactory(CAccessorFactoryContainer& outContainer){
  outContainer.registerFactory(this,
			       mName);
};
void CAccessorFactory::closeAccessor(){
  //does plain nothing in the base class
};
