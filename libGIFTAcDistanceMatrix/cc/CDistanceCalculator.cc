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
#include "CDistanceCalculator.h"

#define HERE __FILE__ << ":" << __LINE__ << ":" 

#define _CD_DEBUG_VECTOR

//template<class TTID,class TTContent/*,class TTSquash*/>
CDistanceCalculator//<TTID,TTContent/*,TTSquash*/>
::CDistanceCalculator(const char*
		      inFileName):
  mSquasher(new CStandardSquasher())//was CDiscreteSquasher
{
  // in fact, this squasher should be just for 
  // preventing crashes. It will normally be reset
  // by the system in function of the user's wishes.
  mSquasher->setSigma(0.05);// was .05

  cout << HERE 
       << "BEGINconstructing CDistanceCalculator"
       << endl
       << "opening "
       << inFileName
       << endl 
       << flush;
  mContent.open(inFileName);
  assert(mContent);
  cout << "ENDconstructing CDistanceCalculator" << flush;
}

//template<class TTID,class TTContent/*,class TTSquash*/>
CDistanceCalculator//<TTID,TTContent/*,TTSquash*/>
::operator bool()const{
  return bool(mContent);
};

//template<class TTID,class TTContent/*,class TTSquash*/>
bool CDistanceCalculator//<TTID,TTContent/*,TTSquash*/>
::calculateDistances(const CID& inPivot,
		     CIDtoDistanceList& outDistances,
		     bool inDeletePivot)const{
  assert(mContent);
  if(mContent){
    bool lReturnValue=mContent.getLineVector(inPivot,
					     outDistances);

    /// if inDeletePivot has been given as true:
    ///delete the pivot from the set: Distance to self is considered
    ///uninteresting.
    if(outDistances.find(inPivot)!=outDistances.end()
       && inDeletePivot){
      outDistances.erase(outDistances.find(inPivot));
    }

#ifdef _CD_DEBUG_VECTOR
    cout <<  "Calculate Distances: " 
	 << outDistances.size()
	 << " size of vector"
	 << endl;
#endif
    return lReturnValue && mContent;
  };
  cout << HERE
       << "real shit looking for" 
       << inPivot
       << endl;
  return false;
}

/** one of the main things when doing picHunting
    this algorithm assumes, that both the lists are sorted for their ID
*/  
//template<class TTID,class TTContent/*,class TTSquash*/>
bool CDistanceCalculator//<TTID,TTContent/*,TTSquash*/>
::calculateRelativeAndSquash(const CID& inPositiveFeedback,
			     const CID& inNegativeFeedback,
			     CIDtoDistanceList& outProbabilities,
			     bool inDeletePivot)const{
  return this->calculateRelativeAndSquash(*mSquasher,
					  inPositiveFeedback,
					  inNegativeFeedback,
					  outProbabilities,
					  inDeletePivot);
};

//template<class TTID,class TTContent/*,class TTSquash*/>
bool CDistanceCalculator//<TTID,TTContent/*,TTSquash*/>
::calculateRelativeAndSquash(const CSquasher& inSquasher,
			     const CID& inPositiveFeedback,
			     const CID& inNegativeFeedback,
			     CIDtoDistanceList& outProbabilities,
			     bool inDeletePivot)const{
  assert(mContent);
  //
  //IMPORTANT: this starts always with the complete set.
  //
  CIDtoDistanceList lPositiveDistances;
  CIDtoDistanceList lNegativeDistances;

  bool lReturnValue=true;
  
  lReturnValue = lReturnValue && calculateDistances(inPositiveFeedback,
						    lPositiveDistances,
						    inDeletePivot);

  lReturnValue = lReturnValue && calculateDistances(inNegativeFeedback,
						    lNegativeDistances,
						    inDeletePivot);

  //intersects and squashes
  outProbabilities.clear();

  //I do not know if i can get around these ugly lines
  //gcc is simply too picky now
  insert_iterator<CIDtoDistanceList> 
    lInserter(inserter(outProbabilities,
		       outProbabilities.begin()));
  do_for_equal_first(lPositiveDistances.begin(),
		     lPositiveDistances.end(),
		     lNegativeDistances.begin(),
		     lNegativeDistances.end(),
		     lInserter,
		     inSquasher);
  
  //which means that the resulting set should be smaller
  //than each of the origin sets.
  
  assert(outProbabilities.size()<=lNegativeDistances.size());
  assert(outProbabilities.size()<=lPositiveDistances.size());

  return lReturnValue;
};

bool CDistanceCalculator//<TTID,TTContent/*,TTSquash*/>
::calculateRelativeAndSquash(const CID& inPositiveFeedback,
			     const CID& inNegativeFeedback,
			     CSet& outProbabilities,
			     bool inDeletePivot)const{

  return this->calculateRelativeAndSquash(*mSquasher,
					  inPositiveFeedback,
					  inNegativeFeedback,
					  outProbabilities,
					  inDeletePivot);
};

bool CDistanceCalculator//<TTID,TTContent/*,TTSquash*/>
::calculateRelativeAndSquash(const CSquasher& inSquasher,
			     const CID& inPositiveFeedback,
			     const CID& inNegativeFeedback,
			     CSet& outProbabilities,
			     bool inDeletePivot)const{
  assert(mContent);

  CIDtoDistanceList lPositiveDistances;
  CIDtoDistanceList lNegativeDistances;
  bool lReturnValue=true;
  
  //intersects and squashes
  lReturnValue = lReturnValue && calculateDistances(inPositiveFeedback,
						    lPositiveDistances,
						    inDeletePivot);

  lReturnValue = lReturnValue && calculateDistances(inNegativeFeedback,
						    lNegativeDistances,
						    inDeletePivot);


  outProbabilities.clear();
  do_for_equal_first(lPositiveDistances.begin(),
		     lPositiveDistances.end(),
		     lNegativeDistances.begin(),
		     lNegativeDistances.end(),
		     outProbabilities,//outProbabilities is its own inserter
		     inSquasher);

  //which means that the resulting set should be smaller
  //than each of the origin sets.
  assert(outProbabilities.size()<=lNegativeDistances.size());
  assert(outProbabilities.size()<=lPositiveDistances.size());

  return lReturnValue;
};


bool CDistanceCalculator//<TTID,TTContent/*,TTSquash*/>
::digestFeedback(CIDList const& inPositiveList,
		 CIDList const& inNegativeList,
		 CSet& outSet){
  this->checkNPrint();
  assert(mContent);

  bool lReturnValue=true;
  for(CIDList::const_iterator i=inPositiveList.begin();
      i!=inPositiveList.end();
      i++){
    CIDList::const_iterator j=inNegativeList.begin();
    calculateRelativeAndSquash(*i,
			       *j,
			       outSet,
			       mDeletePivot);
    outSet.normalize();//FIXME why? what does this do?
    
    for(++j;
	j!=inNegativeList.end();
	j++){
      CSet lSet(outSet,
		false);
      calculateRelativeAndSquash(*i,
				 *j,
				 lSet,
				 mDeletePivot);
      cout << "." << flush;
      outSet.intersect(lSet);
    }
  }
}

void CDistanceCalculator::setSquasher(CSquasher* inSquasher){
  mSquasher=inSquasher;
};
void CDistanceCalculator::setDeletePivot(bool inDeletePivot){
  mDeletePivot=inDeletePivot;
};
