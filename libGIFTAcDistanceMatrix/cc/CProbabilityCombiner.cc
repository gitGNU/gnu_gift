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
#include <functional>
#include <algorithm>

#include "CProbabilityCombiner.h"

CProbabilityCombiner::CProbabilityCombiner(){
  setBelief();
  mNumberOfElements=1;
};

void CProbabilityCombiner::setBelief(double inBelief){
  mBelief=inBelief;
};

CProbabilityCombiner* CProbabilityCombiner::clone()const{
  if(this)
    return subClone();
  else
    return 0;
};

void CProbabilityCombiner::setNumberOfElements(int inNumberOfElements){
  mNumberOfElements=inNumberOfElements;
};

double CProbabilityCombiner::operator()(double inFirst,double inSecond)const{
  return 
    (perform(inFirst,
	     inSecond)*mBelief)+(1-mBelief)/mNumberOfElements;
};


CProbabilityCombiner* CPCMaximum::subClone()const{
  return new CPCMaximum(*this);
};


double CPCMaximum::perform(double inFirst,double inSecond)const{
  return max(inFirst,inSecond);
};


double CPCMinimum::perform(double inFirst,double inSecond)const{
  return min(inFirst,inSecond);
};

CProbabilityCombiner* CPCMinimum::subClone()const{
  return new CPCMinimum(*this);
};



double CPCProduct::perform(double inFirst,double inSecond)const{
  return inFirst*inSecond;
};

CProbabilityCombiner* CPCProduct::subClone()const{
  return new CPCProduct(*this);
};



double CPCDiscrete::perform(double inFirst,double inSecond)const{
  if((inFirst*inSecond)>lThreshold){
    return inFirst*inSecond;
  }else{
    return 0;
  }
};

CProbabilityCombiner* CPCDiscrete::subClone()const{
  return new CPCDiscrete(*this);
};

