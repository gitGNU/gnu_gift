#include "my_throw.h"
#include "GIFTExceptions.h"
#include "CQueryTreeBuilder.h"
#include "CAlgorithm.h"
#include "mrml_const.h"
//----------------------------------------

CQueryTreeBuilder::CQueryTreeBuilder(){};

/** 
    Building a query tree out of the information we have gathered
    true indicates success...
*/
pair<CQueryContainer*,CQueryTreeNode*> CQueryTreeBuilder::buildQueryTree(CAlgorithm& inAlgorithm,
									 CAccessorAdminCollection& inBaseConfiguration,
									 CStaticQueryFactory& inBaseTypeFactory,
									 CQueryContainer* inoutContainer)const{

//   cout << __FILE__ << ":" << __LINE__ << ":WARNING returning 0,0 as query tree" << endl;
//   cerr << __FILE__ << ":" << __LINE__ << ":WARNING returning 0,0 as query tree" << endl;
//   return pair<CQueryContainer*,CQueryTreeNode*>(0,0);

  //
  // 1. generate a CQuery structure with the right type
  //
  cout << "[mQ:"
       << inAlgorithm.getBaseType()
       << endl;  
  CQueryTreeNode* lRoot=new CQueryTreeNode();
  
  if(!inoutContainer){
    inoutContainer=new CQueryContainer();
  }
  

  //  CSelfDestroyPointer<list< pair<string,string> > > lAttributes=inAlgorithm.createNamedValueList();
  cout << "mQ]"<<endl;

  //
  // 2. generate an entry for the new CQuery structure in the mQueryContainer
  //
  (*inoutContainer)[inAlgorithm.getID()]=lRoot;
  

  // 
  // 3. Then build the query trees of the children and 
  // add each child both to the query finding structure
  // as to the query tree.
  //
  for(CXMLElement::lCChildren::const_iterator i=inAlgorithm.child_list_begin();
      i!=inAlgorithm.child_list_end();
      i++) if((*i)->getTypeOfNode()==CXMLElement::cElementNode){
    CXMLElement* lDereferencedI=*i;
    
    CAlgorithm*  iAlgorithm=(CAlgorithm*)(lDereferencedI);
    
    cout << "Build subalgorithm"
	 << lDereferencedI
	 << " {"
	 << iAlgorithm->getName()
	 << "}:"
	 << iAlgorithm->getBaseType()
	 << endl;
    
    // check, that this XML element is really an algorithm
    if(iAlgorithm->getName()==mrml_const::algorithm){
	cout << "really doing it" << endl;
	pair<CQueryContainer*,CQueryTreeNode*> lNewNode=buildQueryTree(*iAlgorithm,
								       inBaseConfiguration,
								       inBaseTypeFactory,
								       inoutContainer);
	
	lRoot->addChild(lNewNode.second,
			iAlgorithm->getWeight());
    }
  }
  //
  // at the end, we tell the query structure
  // that it has recieved all the data it can
  // get, and that it better should get ready.
  //
  cout << "hier " 
       << flush
       << endl;
  
  cout << inAlgorithm.getID()
       << flush
       << endl;
  

  //
  // In the very end configure the root
  //
  lRoot->configure(inAlgorithm,
		   inBaseConfiguration,
		   inBaseTypeFactory);

  return make_pair(inoutContainer,lRoot);
}

//----------------------------------------
/** given an ID find the right item in the 
    query tree */
CQueryTreeNode* CQueryContainer::getQueryByID(const string& inID)const{
  
  const_iterator lFound=find(inID);
  if(lFound==end()){
    cout << "THROWING UP IN getQueryByID" 
	 << endl;

    string lThrowString=
      string("Current content of mQueryContainer \n");
    for(const_iterator i=begin();
	i!=end();
	i++){
      lThrowString+=i->first + string(",\n");
    }
    lThrowString+=string("\n and we were searching for ")+inID;
    if(size()){
      cerr << lThrowString << flush << endl; 
      return begin()->second;
    }    
    my_throw(VENotFound(lThrowString.c_str()));
    return 0;
  }else{
    return lFound->second;
  }
}
