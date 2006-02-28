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
/***************************************
*
* CBitSetCollection
*
****************************************
*
* modification history:
*
* HM 090399 created the documentation
*
****************************************
*
* compiler defines used:
*
*
****************************************/
#include <iostream>
#include <algorithm>
#include <list>
#include "CBitVector.h"
#include "libGIFTAcInvertedFile/include/CBitSetCollection.h"
#include "libGIFTAcInvertedFile/include/CComparisonResults.h"

/***************************************
*
* addResult
*
****************************************
*
* modification history
*
* 
*
****************************************/
int CBitSetCollection::addResult(const CBitVector& inResult){
  push_back(inResult);

  cout << "The size of the inResult:"
       << inResult.size()
       << endl;

  copy(inResult.begin(),
       inResult.end(),
       inserter(mSetOfUsedElements,
		mSetOfUsedElements.begin()));

  return consistify();
}

/***************************************
*
* consistify
*
****************************************
*
* modification history
*
* 
*
****************************************/
int CBitSetCollection::consistify(){

  //we are interested in an algorithm which is robust 
  //and hopefully does not look at too many bitvectors


  CBitVector& lNewest(back());


  if(size()){

    cout << "Size=" << size() << endl;

    CBitVector lNewConsistentSet;
    
    if(mPresentlyConsistentSet.intersect(lNewest,
					 lNewConsistentSet)){
      mPresentlyConsistentSet=lNewConsistentSet;
    }else{
      //und hier geht jetzt der Punk ab...
      /* I am building a set of comparisons which meets the
	 following criterum:
	 
	 The younger -> the more belief,
	 
	 the set of comparisons is consistent.
	 */
      
      //build a new consistent set starting from the set of 
      //all used elements
      CBitVector lNewConsistentSet=mSetOfUsedElements;
      
      //delete the comparisons which lead to inconsistent results
      for(iterator i=begin();
	  i!=end();
	  ){
	if(!lNewConsistentSet.intersectIfConsistent(*i)){
	  erase(i++);
	}else{
	  i++;
	};
      }
      mPresentlyConsistentSet=
	lNewConsistentSet;
    }
  }
  cout << "{after" << mPresentlyConsistentSet.size() << "}" << flush;
  return mPresentlyConsistentSet.size();
}

CBitVector const& CBitSetCollection::getPresentlyConsistentSet()const{
  return mPresentlyConsistentSet;
};
void CBitSetCollection::setPresentlyConsistentSet(const list<TID>& inIDList){
  mPresentlyConsistentSet.clear();

  copy(inIDList.begin(),
       inIDList.end(),
       inserter(mPresentlyConsistentSet,
		mPresentlyConsistentSet.end()));
  cout << "****************************************Current PCS size " 
       << mPresentlyConsistentSet.size()
       << endl;
  

};



