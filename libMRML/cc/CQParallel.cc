// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

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
#include "libMRML/include/CQParallel.h"
/** 

    FIXME: The name WRONGLY suggests that this actually does parallel
    processing. It should be relatively quick to add, it is intendend,
    but it is not yet done.

    This is going to be one of our main building blocks.
    It is a structure which contains a couple of 
    CQuery structures, hands a query through to them,
    and then unifies the result. In fact this is the center of 
    all this query tree business.

    Probably we will put another layer into the class tree:
    The CQTreeNode, but let's first start.

    Important: The basic assumption here is, that all children 
    operate on the same collections. If this is not the case we 
    have to be more careful, and most of all: we have to operate using
    URLs.

 */

/**
 *
 * Initializer, used by both construcors
 *
 */
void CQParallel::init(){
  //we assume that all the children have been initialised.
  //so there is nothing to do here
};
    
/**
 * constructor
 * see CQuery
 */
CQParallel::CQParallel(CAccessorAdminCollection& inAccessorAdminCollection,
		       CAlgorithm&          inAlgorithm):
  mAccessorAdminCollection(inAccessorAdminCollection)
{
  

  // nothing we could do here at present...
 

};
    
/**
 *
 * destructor: at present empty
 * 
 */
CQParallel::~CQParallel(){

  cout << "destroying this "
       << __FILE__
       << __LINE__
       << flush
       << endl;

  //i thought i will need this, but at present I do not have this impression
  //it does not hurt, so we leave it in
};
/**
 *
 * calls fastQuery for every child, merges the results
 * and translates them back into URLs
 *
 */
CXMLElement* CQParallel::query(const CXMLElement& inQuery,
				       int inNumberOfInterestingImages,
				       double inDifferenceToBest){

  //
  double lWeightNormaliser=0;

  //
  cout << endl
       << "I made it till here " 
       << endl
       << flush;

  CXMLElement* lReturnValue=new CXMLElement();

  //
  assert(lReturnValue);

  //this will be the receptacle of the final result
  map<string,double> lURLToRelevanceLevel;
  

  //for all collections
  for(lCLocalAccessorMap::iterator i=mAccessors.begin();
      i!=mAccessors.end();
      i++){
    lWeightNormaliser+=i->second.mWeightedSum;
    i->second.mUnifiedResults=new map<TID,double>();


    //for all query structures using each collection
    for(list<lCChildren::iterator>::iterator j=i->second.mUsedBy.begin();
	j!=i->second.mUsedBy.end();
	j++){


      CIDRelevanceLevelPairList* ltempResultList=
	(*j)->mQuery->fastQuery(inQuery,
				inNumberOfInterestingImages,
				inDifferenceToBest);


      //
      CSelfDestroyPointer<CIDRelevanceLevelPairList> lResultList=
	ltempResultList;
      
      
      cout << endl
	   << "Subquery: current result size "
	   << lResultList->size()
	   << endl;

      if(lResultList){
	for(CIDRelevanceLevelPairList::iterator k=lResultList->begin();
	    k!=lResultList->end();
	    k++){
	  map<TID,double>::iterator lFound=
	    (i->second.mUnifiedResults)->find(k->getID());

	  if(lFound==i->second.mUnifiedResults->end()){
	    i->second.mUnifiedResults
	      ->insert(make_pair(k->getID(),
				 k->getRelevanceLevel()));
	  }else{
	    i->second.mUnifiedResults
	      ->insert(make_pair(k->getID(),
				 k->getRelevanceLevel()
				 +lFound->second * (*j)->mWeight));

	  }
	}
      }
    }

    //now we have unified lists of IDs which belong
    //to one collection
    //Now turn them into URLs and sum them all up
    //note that it is not guaranteed that 
    //the maps we unite are sorted the same way, so
    //we do it again with maps.
    for(map<TID,double>::const_iterator j=i->second.mUnifiedResults->begin();
	j!=i->second.mUnifiedResults->end();
	j++){
      assert((i->second).mAccessor);
      for(map<TID,double>::const_iterator 
	    k=(i->second).mUnifiedResults->begin();
	  k!=(i->second).mUnifiedResults->end();
	  k++){
	string lURL=i->second.mAccessor->IDToURL(k->first);

	map<string,double>::iterator lFound=
	  lURLToRelevanceLevel.find(lURL);
	if(lFound!=lURLToRelevanceLevel.end()){
	  lURLToRelevanceLevel[lURL]+=k->second;
	}else{
	  lURLToRelevanceLevel[lURL] =k->second;
	}
      }
    }
    i->second.mUnifiedResults=0;
  }
  //
  //
  //
  for(map<string,double>::iterator i=lURLToRelevanceLevel.begin();
      i!=lURLToRelevanceLevel.end();
      i++){
    lReturnValue->push_back(CRelevanceLevel(i->first,
					    i->second/lWeightNormaliser));
    
  }
  //this is quite ugly
  //should be one sort with the right op
  lReturnValue->sort();
  lReturnValue->reverse();
  return lReturnValue;
};

/**
 *
 * using IDs instead of CQParallel::URLs (for keeping communication
 * overhead low, when doing things like CORBA).
 * 
 */
CIDRelevanceLevelPairList* CQParallel::fastQuery(const CXMLElement& inQuery,
						 int inNumberOfInterestingImages,
						 double inDifferenceToBest){
  assert(0);
};

/**
 *
 * get some random images 
 *
 */
CXMLElement* CQParallel::getRandomImages(int inNumberOfInterestingImages)const{};
/**
 *
 * set the Algorithm.
 * same scheme as in setCollection
 *
 */
bool CQParallel::setAlgorithm(CAlgorithm& inAlgorithm){};

/**
   it might be necessary to wait until all the children
   are added before ending the initialisation phase.
   
   This function is called by CAlgorithm.
   
*/
void CQParallel::finishInit(){
  cout << endl
       << "Finish init in CQParallel, size of mChildren"
       << mChildren.size()
       << endl;
  
  for(lCChildren::iterator i=mChildren.begin();
      i!=mChildren.end();
      i++){
    string lCollection=i->mQuery->getAlgorithm().getCollectionID();

    //FIXME fails if not all the parents say which collections 
    //unfixable. need to catch this case somewhere
    //
    //they contain included in this instruction is:
    //make a new item if there is already one
    lCLocalAccessorMap::iterator lFound=mAccessors.find(lCollection);

    if(lFound==mAccessors.end()){
      mAccessors.insert(make_pair(lCollection,
				  lCLocalAccessor()));
      //do i need this: look up in specs
      lFound=mAccessors.find(lCollection);
    }
    //
    lFound->second.mUsedBy.push_back(i);
    //FIXME: actually make the accessor.
    //relies on mAccessor initially being 0
    if(!lFound->second.mAccessor){
      //
      CAccessorAdmin* lProxy=&(mAccessorAdminCollection
			       .getProxy(i->mQuery->getAlgorithm()
					 .getCollectionID()));
      
      lFound->second.mAccessor=lProxy->openURL2FTS();
      lFound->second.mAccessorAdmin=lProxy;
      
    }

    //this enables you to put relative weight on algorithms
    lFound->second.mWeightedSum+=i->mWeight;
  }
  cout << "finishInitFinished inCQParallel"
       << endl
       << endl;
};
     

CQParallel::lCLocalAccessor::lCLocalAccessor():
  mWeightedSum(0),
  mAccessor(0),
  mAccessorAdmin(0),
  mUsedBy(),
  mUnifiedResults(0){
}
CQParallel::lCLocalAccessor::~lCLocalAccessor(){
  if(mAccessorAdmin){
    mAccessorAdmin->closeURL2FTS();
  }
}
