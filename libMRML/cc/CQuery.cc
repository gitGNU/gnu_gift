// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, University of Geneva

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/
/***************************************
*
* CQuery.h 
* (abstract) parent of query structures.
* Goal of this structure is to process a query
* and to digest within--session feedback
* this tries not to contain anything application 
* specific
*
****************************************
*
* modification history:
*
* WM 08  99 creation
*
****************************************
*
* compiler defines used:
*
* _NO_FIDPRINT
* _TREE_BUILD_PRINTOUT in query(): building a result tree
*
****************************************/
#include "mrml_const.h" // for parsing
#include "gift_features.h"
#include <algorithm>
#include <functional>
#include <string>

#include <math.h>
#include <assert.h>

#include "map_to_list.h"

#include "CQuery.h"
#include "GIFTExceptions.h"

#include <time.h>
#define _NO_FIDPRINT    

int gQuery;

#include "CMutex.h"
extern CMutex* gMutex;//for debugging

/***************************************
*
* Constructor
*
****************************************
*
* modification history
* WM 19990804 created
* 
*
****************************************/
CQuery::CQuery(CAccessorAdminCollection& inAccessorAdminCollection,
	       CAlgorithm& inAlgorithm
	       ):
  mAccessorAdminCollection(&inAccessorAdminCollection),
  mAlgorithm(&inAlgorithm),
  mAccessorAdmin(0),
  mAccessor(0){
  cout << "\nGQOUERY" << gQuery << endl;
  gQuery++;

  //get proxy by CollectionID
  mAccessorAdmin=&mAccessorAdminCollection->getProxy(mAlgorithm->getCollectionID());
}
/***************************************
*
* Constructor
*
****************************************
*
* modification history
* WM 19990804 created
* 
*
****************************************/
CQuery::CQuery():
  mAccessorAdmin(0),
  mAlgorithm(0),
  mAccessor((CAccessor*)0),
  mAccessorAdminCollection(0){
  cout << "\nGQOUERY" << gQuery << endl;

  gQuery++;
}

CQuery::~CQuery(){
  gQuery--;
  cout << "\n/DELGQOUERY" << gQuery << endl;
}

/***************************************
*
* Getting random images as a seed
*
****************************************
*
* modification history
* WM 19990824 created
* 
*
****************************************/
CXMLElement* CQuery::getRandomImages(int inNumberOfInterestingImages)const{

  cout << "Getting random images "
       << endl;

  list<CAccessorElement> lURLList;
  
  mAccessor->getRandomAccessorElements(lURLList,
				       inNumberOfInterestingImages);

  CXMLElement* lResultList=new CXMLElement(mrml_const::query_result_element_list,
					 0);

  for(list<CAccessorElement>::const_iterator i=lURLList.begin();
      i!=lURLList.end();
      i++){

    CXMLElement* lResultElement=new CXMLElement(mrml_const::query_result_element,
					     0);

    lResultElement->addAttribute(mrml_const::image_location,
				 i->getURL());
    lResultElement->addAttribute(mrml_const::thumbnail_location,
				 i->getThumbnailURL());
    lResultElement->addAttribute(mrml_const::calculated_similarity,
				 double(1));

    lResultList->addChild(lResultElement);
    lResultList->moveUp();
  }

  CXMLElement* lReturnValue=new CXMLElement(mrml_const::query_result,
					    0);
  lReturnValue->addChild(lResultList);

  return lReturnValue;
};

/***************************************
*
* Getting random images as a seed
*
****************************************
*
* modification history
* WM 19990824 created
* 
*
****************************************/
CIDRelevanceLevelPairList* CQuery::getRandomIDs(int inNumberOfInterestingImages)const{
  list<CAccessorElement> lURLList;
  
  mAccessor->getRandomAccessorElements(lURLList,
				       inNumberOfInterestingImages);
  
  CIDRelevanceLevelPairList* lReturnValue(new CIDRelevanceLevelPairList());
  
  for(list<CAccessorElement>::const_iterator i=lURLList.begin();
      i!=lURLList.end();
      i++){
    lReturnValue->push_back(CIDRelevanceLevelPair(i->getID(),0));
  }
  return lReturnValue;
};




/***************************************
*
* Getting all images for making distance matrices 
* or similar stuff
*
****************************************
*
* modification history
* WM 19990824 created
* 
*
****************************************/
list<TID>* CQuery::getAllIDs()const{
  list<TID>* lIDList=new list<TID>();
  
  mAccessor->getAllIDs(*lIDList);

  return lIDList;
};



/***************************************
*
* Getting all images for making distance matrices 
* or similar stuff
*
****************************************
*
* modification history
* WM 19990824 created
* 
*
****************************************/
list<CAccessorElement>* CQuery::getAllAccessorElements()const{
  list<CAccessorElement>* lURLList=new list<CAccessorElement>();
  
  mAccessor->getAllAccessorElements(*lURLList);

  return lURLList;
};


/***************************************
*
* Setting the current algorithm
*
****************************************
*
* modification history
* WM 19990824 created
* 
*
****************************************/
bool CQuery::setAlgorithm(CAlgorithm& inAlgorithm){
  mAlgorithm=&inAlgorithm;
};

/***************************************
*
* getting the current algorithm
*
****************************************
*
* modification history
* WM 19990824 created
* 
*
****************************************/
const CAlgorithm& CQuery::getAlgorithm()const{
  return *mAlgorithm;
};


/** adding a child to this. We expect children to be 
    initialised when they are entered. *this assumes
    no destruction responsabilities for the entered data.
*/
void CQuery::addChild(CQuery* inChild,
		      double inWeight){
  
  mChildren.push_back(lCChild());
  mChildren.back().mQuery=inChild;
  mChildren.back().mWeight=inWeight;
};

/** 
    initialisation taking place when all children etc have
    been added
*/
void CQuery::finishInit(){
  // in the most general query nothing happens here
  // it is just a kind of container
};

/**
   Query: do a fast query, then translate the results.
 */
CXMLElement* CQuery::query(const CXMLElement& inQuery){

  pair<bool,long> lNumberOfInterestingImages=
    inQuery.longReadAttribute(mrml_const::result_size);
  
  int inNumberOfInterestingImages=
    lNumberOfInterestingImages.second;

  pair<bool,long> lCutoff=
    inQuery.longReadAttribute(mrml_const::result_cutoff);
  
  int inCutoff=
    lCutoff.second;
  
  //gMutex->lock();//debugging
  if(inQuery.child_list_begin()!=inQuery.child_list_end()){
    CSelfDestroyPointer<CIDRelevanceLevelPairList> lFastQueryResult(fastQuery(inQuery,
									      inNumberOfInterestingImages,
									      inCutoff));
    cout << "Assembling a query result tree " 
	 << endl;
    if(lFastQueryResult){
      cout << "from a result of size"
	   << lFastQueryResult->size()
	   << endl;
    }else{
      cout << "THERE IS NO RESULT"
	   << (CIDRelevanceLevelPairList*)lFastQueryResult
	   << endl;
    }
#ifdef _TREE_BUILD_PRINTOUT
#endif

    CXMLElement* lReturnValue(new CXMLElement(mrml_const::query_result,0));
    CXMLElement* lReturnList(new CXMLElement(mrml_const::query_result_element_list,0));
    lReturnValue->addChild(lReturnList);

    assert(mAccessor);

    if(lFastQueryResult && mAccessor){
      for(CIDRelevanceLevelPairList::const_iterator i=lFastQueryResult->begin();
	  i!=lFastQueryResult->end();
	  i++){

	CXMLElement* lReturnElement(new CXMLElement(mrml_const::query_result_element,
						    0));
	{
	  double lRelevanceLevel(i->getRelevanceLevel());
	  string lString(mrml_const::calculated_similarity);
	  lReturnElement->addAttribute(lString,
				       lRelevanceLevel);
	}
      

      
	{
	  string lURL(mAccessor->IDToAccessorElement(i->getID()).second.getURL());
	
	
	  string lString(mrml_const::image_location);

	  lReturnElement->addAttribute(lString,
				       lURL);
	}

      
	{
	  string lURL(mAccessor->IDToAccessorElement(i->getID()).second.getThumbnailURL());
	
	  string lString(mrml_const::thumbnail_location);
	  lReturnElement->addAttribute(lString,
				       lURL);
	}
      
	lReturnValue->addChild(lReturnElement);
      
	lReturnValue->moveUp();

      }
      //gMutex->unlock();//debugging
      return lReturnValue;
    }
  }else{
    //gMutex->unlock();//debugging
    return getRandomImages(inNumberOfInterestingImages);
  }

  //gMutex->unlock();//debugging
  return 0;
}
