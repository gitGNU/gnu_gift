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
#include "libGIFTAcDistanceMatrix/include/CDrawer.h"

CDrawer* CDrawer::clone()const{
  if(this)
    return subClone();
  else
    return 0;
}

///
void CDrawer::setNumberOfImages(const int& inNumberOfImages){
 mNumberOfImages= inNumberOfImages;
};

///
int CDrawer::getNumberOfImages()const{
 return mNumberOfImages;
};


inline double CDUniform::operator()(double inProbability)const{
  return (inProbability);
}

CDrawer* CDUniform::subClone()const{
  return new CDUniform(*this);
}

inline double CDPreferMiddle::operator()(double inProbability)const{
  return inProbability + 1/mNumberOfImages;//ex sqrt(sqrt (5.7.)
  //ex NUMBER_OF_IMAGES (7.12.)
}

CDrawer* CDPreferMiddle::subClone()const{
  return new CDPreferMiddle(*this);
}
