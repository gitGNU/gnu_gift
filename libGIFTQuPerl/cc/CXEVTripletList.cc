
#include "libMRML/include/CSelfDestroyPointer.h"
#include "libMRML/include/CXEVTripletList.h"

/** Constructor */
CXEVTripletList::CXEVTripletList(){

};
/** actually carrying out the visit */
bool CXEVTripletList::startVisit(const CXMLElement& inVisited){

    if(inVisited.getTypeOfNode()==CXMLElement::cElementNode){
	mTripletList.push_back(CXMLTriplet());
	mTripletList.back().mInstruction=CXMLTriplet::element;
	mTripletList.back().mName=inVisited.getName();
    }else if(inVisited.getTypeOfNode()==CXMLElement::cTextNode){
	mTripletList.push_back(CXMLTriplet());
	mTripletList.back().mInstruction=CXMLTriplet::text;
	mTripletList.back().mName=inVisited.getText();
    }



  /** 
      now insert all attributes 
  */
  CSelfDestroyPointer<list<pair<string,string> > > 
    lAttributeValuePairs(inVisited.createNamedValueList());
  /** FIXME:
      slow */
  if(lAttributeValuePairs){
    for(list<pair<string,string> >::const_iterator i=lAttributeValuePairs->begin();
	i!=lAttributeValuePairs->end();
	i++){
      mTripletList.push_back(CXMLTriplet());
      mTripletList.back().mInstruction=CXMLTriplet::attribute;
      mTripletList.back().mName=i->first;
      mTripletList.back().mValue=i->second;
    }
  }
};
/** actually carrying out the visit */
void CXEVTripletList::endVisit(const CXMLElement& inVisited){
  mTripletList.push_back(CXMLTriplet());
  mTripletList.back().mInstruction=CXMLTriplet::up;
};


/** getting the list which was built */
const CXMLTripletList& CXEVTripletList::getTripletList(){
  return mTripletList;
};
