// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

     Copyright (C) 2003, 2004 Bayreuth University
      2005 Bamberg University
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
#include "libGIFTAcDistanceMatrix/include/CDistanceCalculator.h"

#define HERE __FILE__ << ":" << __LINE__ << ":" 

#define _CD_DEBUG_VECTOR

//template<class TTID,class TTContent/*,class TTSquash*/>
CDistanceCalculator//<TTID,TTContent/*,TTSquash*/>
::CDistanceCalculator(const char*
		      inFileName){
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

void CDistanceCalculator::setDeletePivot(bool inDeletePivot){
  mDeletePivot=inDeletePivot;
};
