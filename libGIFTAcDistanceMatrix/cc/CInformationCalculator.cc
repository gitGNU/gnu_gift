// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000 CUI, University of Geneva

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
#include "CInformationCalculator.h"
#include <algorithm>
#include <functional>
#include <math.h>
#include <assert.h>

CInformationCalculator* CInformationCalculator::clone()const{
  if(this)
    return subClone();
  else
    return 0;
};

bool CInformationCalculator::isOnePass()const{
  return true;
}
void CInformationCalculator::adjustInnerState(double){
  assert(!"This function should never be called: "
	 "It is non virtual for reasons to long to explain");
}
void CInformationCalculator::startFirstPass(){
  assert(!"This function should never be called: "
	 "It is non virtual for reasons to long to explain");
}


double CICEntropy::operator()(double inProbability)const{
  return 
    (inProbability>0)
    ? -log(inProbability)*inProbability
    : 0;
}

CInformationCalculator* CICEntropy::subClone()const{
  return new CICEntropy(*this);
}
//
///
CICEntropyWithThreshold::CICEntropyWithThreshold(double inThreshold){
  setThreshold(inThreshold);
}
///
CICEntropyWithThreshold::CICEntropyWithThreshold(const CICEntropyWithThreshold&
						 in){
  mThreshold = in.mThreshold;
  mMaximum = in.mMaximum;
}
///
void CICEntropyWithThreshold::setThreshold(double inThreshold){
  mThreshold=inThreshold;
};

///
bool CICEntropyWithThreshold::isOnePass()const{
  return false;
};
///
void CICEntropyWithThreshold::startFirstPass(){
  mMaximum=-1e30;
};
///
void CICEntropyWithThreshold::adjustInnerState(double inValue){
  mMaximum=max(mMaximum,
	       inValue);
};

double CICEntropyWithThreshold::operator()(double inProbability)const{
#ifdef _debug
  if(inProbability
     >
     mMaximum*mThreshold){
    cout << '+' ;
  }else{
    cout << '-';
  }
#endif

  return 
    (inProbability
     >
     mMaximum*mThreshold)
    ? -log(inProbability)*inProbability
    : 0;
}

CInformationCalculator* CICEntropyWithThreshold::subClone()const{
  return new CICEntropyWithThreshold(*this);
}

///
CICERelative::CICERelative(double inThreshold){
  setThreshold(inThreshold);
}
///
CICERelative::CICERelative(const CICERelative& in){
  mThreshold = in.mThreshold;
  mSum = in.mSum;
}
///
void CICERelative::setThreshold(double inThreshold){
  mThreshold=inThreshold;
};

///
bool CICERelative::isOnePass()const{
  return false;
};
///
void CICERelative::startFirstPass(){
  mSum=0;
};
///
void CICERelative::adjustInnerState(double inValue){
  mSum+=inValue;
};

double CICERelative::operator()(double inProbability)const{
  /*  if((inProbability
      >
      mThreshold) 
      && 
      (mSum>0))
      cout << "+";
    else
  cout << "-";*/

  return 
    ((inProbability
      >
      mThreshold) 
     && 
     (mSum>0))
    ? -log(inProbability/mSum)*(inProbability/mSum)
    : 0;
}

CInformationCalculator* CICERelative::subClone()const{
  return new CICERelative(*this);
}


///
double CICSum::operator()(double inProbability)const{
  return inProbability;
}
///
CInformationCalculator* CICSum::subClone()const{
  return new CICSum(*this);
}




