/* -*- mode: c++ -*- 
*/
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
#ifndef _CDISTANCECALCULATOR
#define _CDISTANCECALCULATOR
#include "libGIFTAcDistanceMatrix/include/uses-declarations.h"
#include <functional>
#include "libGIFTAcDistanceMatrix/include/CPersistentTranslatedIndexMatrix.h"
#include <math.h>
#include "libMRML/include/CMagic.h"
#include "CProbabilisticSet.h"

#include "libGIFTAcDistanceMatrix/include/CSquasher.h"

template<class TTIterator,
  class TTInserter,
  class TTFunction>
void do_for_equal_first(const TTIterator& inBegin1,
			const TTIterator& inEnd1,
			const TTIterator& inBegin2,
			const TTIterator& inEnd2,
			TTInserter& inInserter,
			const TTFunction& f){
  TTIterator i=inBegin1;
  TTIterator j=inBegin2;

  //this algorithm assumes, that both the lists are sorted for their ID
  //otherwhise NOTHING is assumed
  while(i!=inEnd1 &&
	j!=inEnd2 ){
    if(i->first<j->first){
      i++;
    }else{
      // i->first >= j->first
      if(j->first<i->first){
	j++;
      }else{
	// i.e. i->first == j->first holds
	// output is squashing the difference of the distances, 
	// of elements with identical ID
	inInserter=(make_pair(i->first,
			      f(i->second,
				j->second)));
	
	i++;
	j++;
      }
    }
  }
}



// for testing: nice and radical
//#define TTSquash CDiscreteSquasher
//more adapted to the real user...
//#define TTSquash CStandardSquasher

//template<class TTID,class TTContent,class TTSquasher>
class CDistanceCalculator:public CMagic{
public: // public type definitions
  /** The content of this is a matrix from int to float */
  typedef  CPersistentTranslatedIndexMatrix<int,float> CContent;
  /** */
  typedef CContent::CIDContentPairList CIDtoDistanceList;
  /** */
  typedef CContent::CIndexList CIDList;
  /** */
  CContent mContent;
protected:
  /** squashing function */
  CSquasher* mSquasher;
  /** the ID type within this class */
  typedef int CID;
  /** a probabilistic set of IDs */
  typedef CProbabilisticSet<CID> CSet;
  /** Delete the pivot from a probabilistic set when digesting feedback? 
      See my thesis for an explanation
   */
  bool mDeletePivot;
public:
  ///
  void setDeletePivot(bool inDeletePivot);
  ///
  void setSquasher(CSquasher* inSquasher);
  ///
  CSquasher* getSquasher();
  ///
  CDistanceCalculator(const char* inFileName);
  ///
  bool calculateDistances(const CID& inID,
			  CIDtoDistanceList& outDistances,
			  bool inDeletePivot)const;
  ///
  bool calculateRelativeAndSquash(const CID& inPlus,
				  const CID& inMinus,
				  CIDtoDistanceList& outDistances,
				  bool inDeletePivot)const;
  ///
  bool calculateRelativeAndSquash(const CSquasher& inSquasher,
				  const CID& inPlus,
				  const CID& inMinus,
				  CIDtoDistanceList& outDistances,
				  bool inDeletePivot)const;
  ///
  bool calculateRelativeAndSquash(const CID& inPositiveFeedback,
				  const CID& inNegativeFeedback,
				  CSet& outProbabilities,
				  bool inDeletePivot)const;
  ///
  bool calculateRelativeAndSquash(const CSquasher& inSquasher,
				  const CID& inPositiveFeedback,
				  const CID& inNegativeFeedback,
				  CSet& outProbabilities,
				  bool inDeletePivot)const;

  bool digestFeedback(CIDList const& inPositiveList,
		      CIDList const& inNegativeList,
		      CSet& );
  ///
  operator bool()const;
};

#endif
