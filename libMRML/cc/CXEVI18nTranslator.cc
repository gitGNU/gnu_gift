
#include "libMRML/include/CXEVI18nTranslator.h"
#include "libMRML/include/CI18nTranslator.h"
#include <cassert>
/**
   
A visitor that visits XML elements in order to translate them

*/
CXEVI18nTranslator::CXEVI18nTranslator(const CI18nTranslator& inTranslator,
				       string inLanguageCode):mTranslator(inTranslator),
							      mLanguageCode(inLanguageCode){
};
/** actually carrying out the visit */
bool CXEVI18nTranslator::startVisit(const CXMLElement& inVisited){
  assert(0);
}
/** actually carrying out the visit */
void CXEVI18nTranslator::endVisit(const CXMLElement& inVisited){
  assert(0);
}


/** actually carrying out the visit */
bool CXEVI18nTranslator::startVisit(CXMLElement& inVisited){
  list<pair<string,string> >* lNamedValueList=inVisited.createNamedValueList();
  
  if(lNamedValueList){
    for(list<pair<string,string> >::const_iterator i=lNamedValueList->begin();
	i!=lNamedValueList->end();
	i++){
      if(mTranslator.shouldBeTranslatedAttribute(i->first)){
	inVisited.addAttribute(i->first,mTranslator.translateAttribute(mLanguageCode,
								       i->first,
								       i->second).second);
      }
    }
  }
};
/** actually carrying out the visit */
void CXEVI18nTranslator::endVisit(CXMLElement& inVisited){
}
