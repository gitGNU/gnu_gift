// -*- mode: c++ -*-
#ifndef _CQUERYTREEBUILDER
#define _CQUERYTREEBUILDER
#include "libMRML/include/uses-declarations.h"
class CAlgorithm;
class CAccessorAdminCollection;


#include "libMRML/include/CMagic.h" // checking CQueryTreeNode
#include "libMRML/include/CQueryTreeNode.h"
#include "libMRML/include/CSelfDestroyPointer.h"

class CStaticQueryFactory;
/** 
    A type helping in the translation from CAlgorithmID
    to CQuery*
*/
class CQueryContainer:public map<string,CQueryTreeNode* >,public CMagic{
public:
  /**
     Translate an CAlgorithmID to the corresponding CQueryTreeNode
  */
  CQueryTreeNode* getQueryByID(const string& inID)const;
};


/** 
    This class builds and maintains a query tree.
 */
class CQueryTreeBuilder{
public:
  /** */
  CQueryTreeBuilder();
  /** 
      Building a query tree out of the information we have gathered.
      true indicates success...
      
      
      We do a post-order traversal of the tree of algorithms:
      At each level we construct the query of the children
      taking them as an argument for constructing the query of
      this.
  */
  pair<CQueryContainer*,CQueryTreeNode*> buildQueryTree(CAlgorithm& inAlgorithm,
							CAccessorAdminCollection& inBaseConfiguration,
							CStaticQueryFactory& inBaseTypeFactory,
							CQueryContainer* inContainer=0)const;
  /** Getting a node of the tree by its
      ID */
  CQueryTreeNode* getQueryByID(const string& inID)const;

};

#endif
