#include "libMRML/include/CQueryTreeNode.h"
#include "libMRML/include/CAlgorithm.h"
#include "libMRML/include/mrml_const.h"

/** Construct this */
CQueryTreeNode::CQueryTreeNode():
  mContent(0),
  mAlgorithm(0),
  mChildren(){
  
};
/** destruct this */
CQueryTreeNode::~CQueryTreeNode(){
  cout << "DELETING THIS" 
       << endl;
  for(CChildren::iterator i=mChildren.begin();
      i!=mChildren.end();
      i++){
    delete i->first;
    cout << "DELETING" 
	 << endl;
  }
  delete mContent;
  delete mAlgorithm;
};
/** Configure this */
void CQueryTreeNode::configure(CXMLElement& inAttributes,
			       CAccessorAdminCollection& inAccessors,
			       CStaticQueryFactory& inFactory){

  mAlgorithm=new CAlgorithm(inAttributes);
  cout << "Deleting mContent";
  mContent=0;
  cout << "done";
  mContent=inFactory.makeQuery(mAlgorithm->getBaseType(),
			       inAccessors,
			       *mAlgorithm);
  cout << "MAKING " << this << "." << static_cast<CQuery*>(mContent) << endl;
  mContent->checkNPrint();
  // Build a local query tree
  for(CChildren::iterator i=mChildren.begin();
      i!=mChildren.end();
      i++){
    cout << "ADDING " << this << "." << static_cast<CQuery*>(mContent) << " <- " << i->first << "." << static_cast<CQuery*>(i->first->mContent) << ", " << i->second << endl;
    cout << "i->first:          " << flush ;i->first->checkNPrint();
    cout << "i->first->Content: " << flush ;i->first->mContent->checkNPrint();
    mContent->addChild(i->first->mContent,
		       i->second);
  }
};
/** add a child to this */
void CQueryTreeNode::addChild(CQueryTreeNode* inChild,
			      double inWeight=1){

  cout << "this " << flush ;checkNPrint();
  cout << " CQueryTreeNode::addChild(inChild,1) " << inChild << flush ; inChild->checkNPrint();
  mChildren.push_back(make_pair(inChild,inWeight));
  cout << "AFTER CQueryTreeNode::addChild(inChild,1) " << inChild << flush ; mChildren.back().first->checkNPrint();
};
/** 
 *
 * do a query 
 *
 */
CXMLElement*  CQueryTreeNode::query(const CXMLElement& inQuery){
  if(mContent)
    return mContent->query(inQuery);
  return 0;
};

/**
 *
 * @short a query which returns ID/RelevanceLevel pairs instead of
 * instead of URL/RelevanceLevel pairs
 *
 */
CIDRelevanceLevelPairList* CQueryTreeNode::fastQuery(const CXMLElement& inQuery,
						     int inNumberOfInterestingImages,
						     double inDifferenceToBest){
  if(mContent)
    return mContent->fastQuery(inQuery,
			       inNumberOfInterestingImages,
			       inDifferenceToBest);
  return 0;
}


