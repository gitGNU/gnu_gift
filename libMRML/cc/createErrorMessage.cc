#include "libMRML/include/CXMLElement.h"
#include "libMRML/include/GIFTExceptions.h"
#include "libMRML/include/createErrorMessage.h"
#include "libMRML/include/mrml_const.h"
CXMLElement* createErrorMessage(const char* inErrorMessage){
  CXMLElement* lReturnValue=new CXMLElement(mrml_const::error,0);
  assert(lReturnValue);
  lReturnValue->addAttribute(mrml_const::message,inErrorMessage);
  return lReturnValue;
}
CXMLElement* createErrorMessage(const GIFTException& inException){
  return createErrorMessage(inException.getMessage());
}
CXMLElement* createErrorMessage(const GIFTException* inException){
  return createErrorMessage(inException->getMessage());
}
