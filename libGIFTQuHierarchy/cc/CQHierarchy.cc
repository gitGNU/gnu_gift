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
#include "mrml_const.h" // for parsing
#include "CQHierarchy.h"
#include "CAcHierarchy.h"
/** 

    There is no special initialisation to do,
    so this function is empty

 */

void CQHierarchy::init(){
  mCurrentPath.clear();
};

/**
 *
 * default constructor SHOULD NOT BE CALLED
 * 
 */
CQHierarchy::CQHierarchy(){
  assert(0);
};
/**
 * constructor
 * see CQuery
 */
CQHierarchy::CQHierarchy(CAccessorAdminCollection& inAccessorAdminCollection,
			 CAlgorithm&          inAlgorithm):
  CQuery(inAccessorAdminCollection,
	 inAlgorithm){
  {

    // mproxy has been filled in a reasonable way 
    // by CQuery::CQuery
    mAccessor=mAccessorAdmin->openAccessor("hierarchy");

    init();

    assert(mAccessor);
  }
};
    
/**
 *
 * destructor: at present empty
 * 
 */
CQHierarchy::~CQHierarchy(){

  cout << "destroying this "
       << __FILE__
       << __LINE__
       << flush
       << endl;

  //i thought i will need this, but at present I do not have this impression
  //it does not hurt, so we leave it in
};

/**  */
bool CQHierarchy::setAlgorithm(CAlgorithm & inAlgorithm){
  if(mAlgorithm && mAlgorithm->getCollectionID()==inAlgorithm.getCollectionID()){
    
    return true;
    
  }else{
    //close the old collection, if exsisting
    if(mAccessorAdmin)
      mAccessorAdmin->closeAccessor("hierarchy");
    //
    mAccessorAdmin=&mAccessorAdminCollection->getProxy(inAlgorithm.getCollectionID());
    mAccessor=mAccessorAdmin->openAccessor("hierarchy");
    
    assert(mAccessor);
    //
    return (CQuery::setAlgorithm(inAlgorithm) && mAccessor);
  }
};
/**
 *
 *
 */
CIDRelevanceLevelPairList* CQHierarchy::fastQuery(const CXMLElement& inQuery,
						 int inNumberOfInterestingImages,
						 double inDifferenceToBest){

  CIDRelevanceLevelPairList* lReturnValue(0);

  /*
    AS FOR ANY CALL TO FASTQUERY
    the inQuery parameter contains a "query-step" element.
    
    we are interested in children that contain
    a "relevance-level-element-list" or a 
    "cui-hierarchy-up" element.
   */
  for(list<CXMLElement*>::const_iterator i=inQuery.child_list_begin();
      i!=inQuery.child_list_end();
      i++){// iterating over all the children of the inQuery element.

     cout << "I:The name of this tree element: "
 	 << endl
 	 << (*i)->getName()
 	 << endl;
     if((*i)->getName()==mrml_const::cui_hierarchy_up){
       // this is when a cui-hierarchy-up element was found
       if(mCurrentPath.size()){
	 cout << "MOVING UP ";
	 copy(mCurrentPath.begin(),
	      mCurrentPath.end(),
	      ostream_iterator<TID>(cout,","));
	 cout << endl;       
	 mCurrentPath.pop_back();
	 copy(mCurrentPath.begin(),
	      mCurrentPath.end(),
	      ostream_iterator<TID>(cout,","));
	 return ((CAcHierarchy*)mAccessor)->getChildren(mCurrentPath);
       }
       return new CIDRelevanceLevelPairList();
     }
     if((*i)->getName()==mrml_const::user_relevance_element_list){
       // if an user-relevance-element-list was found       
       for(list<CXMLElement*>::const_iterator j=(*i)->child_list_begin();
	   j!=(*i)->child_list_end();
	   j++){//iterate over all the children
	 cout << "J:The name of this tree element: "
	      << endl
	      << (*j)->getName()
	      << endl;
	 if((*j)->getName()==mrml_const::user_relevance_element){
	   // if the current child is a user-relevance-element
	   if(((*j)->stringReadAttribute(mrml_const::image_location).first)
	      &&
	      ((*j)->stringReadAttribute(mrml_const::user_relevance).first)){
	     // get the image-location and the user-relevance attributes
	     if((*j)->doubleReadAttribute(mrml_const::user_relevance).second){
	       //the relevance of this element != 0
	       if(!lReturnValue){
		 // if the return value is still undefined
		 // (i.e. there was no previous element with (user-relevance != 0)
		 list<TID> lOldCurrentPath=mCurrentPath;
		 // add this element to the path
		 mCurrentPath.push_back(mAccessor->URLToID((*j)->stringReadAttribute(mrml_const::image_location).second).second);
		 
		 cout << "The new mCurrentPath: ";
		 copy(mCurrentPath.begin(),
		      mCurrentPath.end(),
		      ostream_iterator<TID>(cout,","));
		 cout << endl;
		 
		 // get the children of this node from the accessor
		 lReturnValue=((CAcHierarchy*)mAccessor)->getChildren(mCurrentPath);
		 if(lReturnValue){// if there are children:
		   // return them
		   return lReturnValue;
		 }else{
		   // if not:
		   // take the old path, get it's children, and return them
		   mCurrentPath=lOldCurrentPath;
		   lReturnValue=((CAcHierarchy*)mAccessor)->getChildren(mCurrentPath);
		   return lReturnValue;
		 }
	       }
	     }
	   }
	 }
       }
     }
  }
  // if nothing else was found,
  // display the first selection:
  mCurrentPath.clear();// clear the path
  return ((CAcHierarchy*)mAccessor)->getChildren(mCurrentPath);//children of root node
};

