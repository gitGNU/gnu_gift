// -*- mode: c++ -*-
#ifndef _CQUERYTREENODE
#define _CQUERYTREENODE

#include "CQuery.h"

#ifdef LINK_DYNAMICALLY
#include "CDynamicQueryFactory.h"
#else
#include "CStaticQueryFactory.h"
#endif

#include "CSelfDestroyPointer.h"
#include "CMagic.h"

class CXMLElement;
class CAlgorithm;
class CStaticQueryFactory;

/** This class does nothing but wrap the 
    current query interface in a way, that 
    it can be used with CORBA
*/
class CQueryTreeNode:public CMagic{
  /**
     type of the children of this
   */
  typedef list<pair<CQueryTreeNode*,double> > CChildren;
  /** The children of this */
  CChildren mChildren;
  /**
     The content of this. It is constructed, when
     CQueryTreeNode::configure is called
  */
  CQuery* mContent;
  /** 
      the current Algorithm of this query 
  */
  CAlgorithm* mAlgorithm;
public:
  /** Construct this */
  CQueryTreeNode();
  /** Destruct this: delete all the children of this */
  ~CQueryTreeNode();
  /** Configure this. This means:
   create an object of a subclass of CQuery, 
   and set its children*/
  void configure(CXMLElement& inAlgorithm,
		 CAccessorAdminCollection& inAccessors,
		 CStaticQueryFactory& inBaseTypeFactory);
  /** 
   *
   * do a query 
   *
   */
  virtual CXMLElement* query(const CXMLElement& inQuery);
  /**
   *
   * @short a query which returns ID/RelevanceLevel pairs instead of
   * instead of URL/RelevanceLevel pairs
   *
   */
  virtual CIDRelevanceLevelPairList* fastQuery(const CXMLElement& inQuery,
					       int inNumberOfInterestingImages,
					       double inDifferenceToBest);
  /**
   *
   * @short a query which returns ID/RelevanceLevel pairs instead of
   * instead of URL/RelevanceLevel pairs
   *
   */
  void addChild(CQueryTreeNode* inChild,
		double inWeight=1);

};

#endif
