// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

     Copyright (C) 2003, 2004 Bayreuth University
    Copyright (C) 2005,2006 Bamberg University
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
#include "libMRML/include/mrml_const.h"
#include "libMRML/include/my_throw.h"
#include <algorithm>
#include "libMRML/include/CQuery.h"
#include "libMRML/include/CAlgorithm.h"
#include "libMRML/include/CXMLHelper.h"
#include "libMRML/include/CQueryTreeBuilder.h"
#include "libMRML/include/CAlgorithmCollection.h"
/** clone this*/
CAlgorithm* CAlgorithm::clone()const{
  return new CAlgorithm(*this);
};
/** clone this*/
CAlgorithm::CAlgorithm(const CAlgorithm& inAlgorithm):
  CXMLElement(inAlgorithm){
  
};
/** clone this*/
CAlgorithm::CAlgorithm(const CXMLElement& inAlgorithm):
  CXMLElement(inAlgorithm){
  
};
/**
   Add a child with the given attributes
   to the current node, and MAKE 
   THIS CHILD THE NEW CURRENT NODE.
*/
void CAlgorithm::addChild(const string& inName,
			   const char* const* const inAttributeList){
  //initialize new child
  CXMLElement::addChild(static_cast
			<CXMLElement*>(new CAlgorithm(inName.c_str(),
						      inAttributeList)));
}
/**
   Add a child with the given attributes
   to the current node, and MAKE 
   THIS CHILD THE NEW CURRENT NODE.
*/
void CAlgorithm::addChild(CXMLElement* inChild){
    //initialize new child
    CXMLElement::addChild(inChild);
}
/**
*/
CAlgorithm::CAlgorithm(const char* inName,
		       const char* const* const inAttributeList):
  CXMLElement(inName,inAttributeList){
};
/**
*/
CAlgorithm::CAlgorithm(const string& inName,
		       const list<pair<string,string> >& inAttributeList):
  CXMLElement(inName,inAttributeList){
};
//----------------------------------------
//sending back the name of the collection treated by this
string CAlgorithm::getCollectionID()const{
  return stringReadAttribute(mrml_const::collection_id).second;
};
//----------------------------------------
//sending back the name of the algorithm
string CAlgorithm::getAlgorithmName()const{
  return stringReadAttribute(mrml_const::algorithm_name).second;
};
//----------------------------------------
//sending back the ID of the algorithm
string CAlgorithm::getID()const{
  return stringReadAttribute(mrml_const::algorithm_id).second;
};
//----------------------------------------
//sending back the relative weight of the algorithm
double CAlgorithm::getWeight()const{
  if(doubleReadAttribute(mrml_const::cui_weight).first){
    return (doubleReadAttribute(mrml_const::cui_weight).second);
  }else{
    return 1;
  }
};
//----------------------------------------
//sending back the Type of the algorithm
pair<bool,string> CAlgorithm::getType()const{
  return stringReadAttribute(mrml_const::algorithm_type);
};
//----------------------------------------
//sending back the Type of the algorithm
pair<bool,string> CAlgorithm::getInheritsFrom()const{
  return stringReadAttribute("cui-inherits-from");
};
//----------------------------------------
//sending back the BaseType of the algorithm
string CAlgorithm::getBaseType()const{
  return stringReadAttribute(mrml_const::cui_base_type).second;
};
//----------------------------------------
//making some old MRML for speaking with zoran
string CAlgorithm::toOldMRML()const{
  return string("<"
		+ mrml_const::algorithm 
		+ " " 
		+ mrml_const::algorithm_name
		+ "=\"")
    + getName()
    + "\" " 
    + mrml_const::algorithm_id
    + "=\""
    + getID()
    + "\"/>\n";
};


// //----------------------------------------
// /** reading attributes with defaults
//  */
// pair<bool,string> CAlgorithm::stringReadAttribute(const string& 
// 						  inAttribute)const{


//   return CXMLElement::stringReadAttribute(inAttribute);

//   ///code ignored from here ******************************

//   cout << "StringReadAttribute:"
//        << inAttribute
//        << endl;

//   CAttributeList::const_iterator lFoundPosition=mAttributes.find(inAttribute);
  
//   if(lFoundPosition != mAttributes.end()){
//     return make_pair(bool(1),
// 		     lFoundPosition->second);
//   }
//   if(mDefaultAlgorithm){
//     pair<bool,string> lResult=mDefaultAlgorithm->stringReadAttribute(inAttribute);

//     if(lResult.first){
//       return lResult;
//     }
//   }
//   if(getFather()){
//     return getFather()->stringReadAttribute(inAttribute);
//   }
//   cout << "..failed: the ID of the element: "
//        << getID()
//        << endl;
//   return make_pair(0,
// 		   string(""));
// };
//----------------------------------------
/** 
    Building a query tree out of the information we have gathered
    true indicates success...

    
*/
bool CAlgorithm::configure(CAlgorithmCollection& inBaseConfiguration){
  //
  // merge this with the default algorithm of this type
  //
  cout << this << endl;
  cout << "1" << flush;
  if((this->getType().first)
     &&(inBaseConfiguration.containsAlgorithmByType(this->getType().second))){
    mergeAttributes(inBaseConfiguration.getAlgorithmByType(this->getType().second));

    CAlgorithm& lDefault(inBaseConfiguration.getAlgorithmByType(this->getType().second));

    //HACK clone children from default if this childlist is empty or contains 
    //only text nodes.
    int lSize=0;
    for(lCChildren::const_iterator i=mChildren.begin();
	i!=mChildren.end();
	i++){
      if((*i)->getName()==mrml_const::algorithm){
	lSize++;
      }
    }
    if(!lSize){
	cout << "°°"
	     << endl;

	//	mCurrentChild=this;
	for(lCChildren::const_iterator i=lDefault.child_list_begin();
	    i!=lDefault.child_list_end();
	    i++){
	  cout << " || "
	       << mChildren.size()
	       << ((CAlgorithm*)(*i))->getBaseType()
	       << endl;
	  addChild((*i)->clone(true));
	  moveUp();
	}
    }
    cout << "NUMBER OF CHILDREN that are ALGORITHMS " 
	 << lSize
	 << endl;
    //mergeChildLists(inBaseConfiguration.getAlgorithmByType(this->getType().second));
  }
  cout << "2" << flush;
  // merge with the configuration of the father
  if(getFather()){
    mergeAttributes(*(CAlgorithm*)(getFather()));
  }
  cout << "3" << flush;


  // 
  // 3. Then build the query trees of the children and 
  // add each child both to the query finding structure
  // as to the query tree.
  //
  for(CXMLElement::lCChildren::const_iterator i=child_list_begin();
      i!=child_list_end();
      i++){
    CXMLElement* lDereferencedI=*i;
    
    if(lDereferencedI->getName()==mrml_const::algorithm){
      CAlgorithm*  iAlgorithm=(CAlgorithm*)(lDereferencedI);
      cout << "." << flush;
      iAlgorithm->configure(inBaseConfiguration);
    }
  }
  if(getFather()){
    //mergeChildLists(*(CAlgorithm*)(getFather()));
  }
  cout << "4" << flush;
}
/** merging two algorithms.

    general rule: if something is contained already in *this
    the value of *this will be kept.
    
    
    first: default values.
*/
void CAlgorithm::mergeAttributes(const CAlgorithm& inAlgorithm){
  if(&inAlgorithm){
    //STOOPID way of merging, takes n log n time instead of n
    for(CAttributeList::const_iterator
	  i=inAlgorithm.mAttributes.begin();
	i!=inAlgorithm.mAttributes.end();
	i++){
      //add attributes which are not yet found
      if(mAttributes.find(i->first)==mAttributes.end()){
#ifdef SINGLE
	mAttributes.add(i->first,i->second);
#else
	mAttributes.check();
	mAttributes.add(i->first,i->second.first);
#endif
      }
    }
  }
}

void CAlgorithm::mergeChildLists(const CAlgorithm& inAlgorithm){
  if(&inAlgorithm){
    //merge child lists
    copy(inAlgorithm.child_list_begin(),
	 inAlgorithm.child_list_end(),
	 inserter(mChildren,
		  child_list_end()));
  }
}

//----------------------------------------
//Sorting algorithms by their ID (coming from a config file)
bool CSortByID_CA::operator()(const CAlgorithm& l,
			      const CAlgorithm& t)const{
  return l.stringReadAttribute(mrml_const::algorithm_id)<l.stringReadAttribute(mrml_const::algorithm_id);
};

//----------------------------------------
//Sorting algorithms by their base (an integer defined in this source file)
bool CSortByBase_CA::operator()(const CAlgorithm& l,
				const CAlgorithm& t)const{
  return l.stringReadAttribute(mrml_const::cui_base_type)<l.stringReadAttribute(mrml_const::cui_base_type);
};


