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
* CWeightingFunctionPointerHash
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
#include "libGIFTQuInvertedFile/include/CWeightingFunctionPointerHash.h"

/***************************************
*
* Constructor
*
****************************************
*
* modification history
*
* 
*
****************************************/
CWeightingFunctionPointerHash::
CWeightingFunctionPointerHash(const CWeightingFunction& inWF):
  mWeightingFunction(inWF)
{
}

/***************************************
*
* size
*
****************************************
*
* modification history
*
* 
*
****************************************/
CWeightingFunctionPointerHash::CFather::size_type CWeightingFunctionPointerHash::size()const{
  return CFather::size();
}



/***************************************
*
* addFeature
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CWeightingFunctionPointerHash
::addFeature(const double inRelevanceLevel,
	     const CDocumentFrequencyElement& inElement){

#ifdef PRINT
  cout << "CWFPHop()"
       << size()
       << ","
       << inElement.getID()
       << ","
       << mRelevanceLevel
       << flush;
#endif

  iterator lHashEntry=
    find(inElement.getID());
  
  if(lHashEntry==end()){
    // insert a new instance of the weighting function, 
    // copied from the prototype in mWeightingFunction
    insert(make_pair(inElement.getID(),
		     mWeightingFunction
		     .constructNew(inElement.getID())));
    lHashEntry=find(inElement.getID());
    (*lHashEntry).second->addQueryFeature(inRelevanceLevel,
					  inElement);
  }else{
    (*lHashEntry).second->addQueryFeature(inRelevanceLevel,
					  inElement);
  }
}
void CWeightingFunctionPointerHash::clearFeatures(){
  for(CWeightingFunctionPointerHash::iterator i=begin();
      i!=end();
      i++){
    delete i->second;
  }
  clear();
};
/** */
CWeightingFunctionPointerHash::~CWeightingFunctionPointerHash(){
  clearFeatures();
};
