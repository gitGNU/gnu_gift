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
#include "libGIFTAcDistanceMatrix/include/CSquasher.h"
#include <math.h>
#include <iostream>
double CStandardSquasher::operator()(double inCloser,
				     double inFarther)const{
  double lReturnValue(1.0/(1+exp((inCloser-inFarther)
				 /mSigma
				 )));
  //cout << "µ" << lReturnValue << ":" << mSigma << ":  " << inFarther << ">" << inCloser<< endl;
  return lReturnValue;
};

double CDiscreteSquasher::operator()(double inCloser,
				     double inFarther)const{
#ifdef VISTEX
  return (inCloser 
	  < 
	  inFarther+mSigma) 
    ? 1:0.3;
#else
  return (inCloser 
	  < 
	  inFarther+mSigma) 
    ? 1:0.1;
#endif
}

double gauss(double x,double inSigma){
  inSigma+=1e-11;
  return exp(-x*x
	     /
	     (inSigma*inSigma));
}

double CGaussianSquasher::operator()(double inCloser,
				     double inFarther)const{
  return 0.5+(-0.5*gauss(inFarther,mSigma)
	      +0.5*gauss(inCloser,mSigma));//sic!
}
double CFlexibleSquasher::operator()(double inCloser,
				     double inFarther)const{
  return 0.5+(-0.5*gauss(inFarther,inFarther-inCloser)
	      +0.5*gauss(inCloser,inFarther-inCloser));
}



