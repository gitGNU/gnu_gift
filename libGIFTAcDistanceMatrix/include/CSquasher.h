/* -*- mode: c++ -*- 
*/
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

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
#ifndef _CSQUASHER
#define _CSQUASHER
#include "libGIFTAcDistanceMatrix/include/uses-declarations.h"
#include <functional>

//
// in general for squashing: for the case that 
// FARTHER much farther away than CLOSER
// the probability is supposed to be one
//

//the base
class CSquasher:public unary_function<double,double>{
 protected:
  double mSigma;
 public:
  ///the real squashing function
  virtual double operator()(double inCloser,
			   double inFarther)const=0;
  ///setting a parameter how much to believe the user
  inline void setSigma(double inSigma){
    mSigma=inSigma;
  }
};

class CStandardSquasher:public CSquasher{
 public:
  ///
  virtual double operator()(double inCloser,
			    double inFarther)const;
};

class CDiscreteSquasher:public CSquasher{
 public:
  virtual double operator()(double inCloser,
			    double inFarther)const;
};

class CGaussianSquasher:public CSquasher{
 public:
  virtual double operator()(double inCloser,
			    double inFarther)const;
};

class CFlexibleSquasher:public CSquasher{
 public:
  virtual double operator()(double inCloser,
			    double inFarther)const;
};


#endif
