#include "libMRML/include/CXMLElement.h"
#include "libMRML/include/CXMLElementVisitor.h"

bool CXMLElementVisitor::startVisit(CXMLElement& inVisited){
  this->startVisit((const CXMLElement&)inVisited);
};
void CXMLElementVisitor::endVisit(CXMLElement& inVisited){
  this->endVisit((const CXMLElement&)inVisited);
};
