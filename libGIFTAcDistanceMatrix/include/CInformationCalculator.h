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
#ifndef _CINFORMATIONCALCULATOR
#define _CINFORMATIONCALCULATOR
#include "../include/uses-declarations.h"
#include <math.h>
#include <functional>

class CInformationCalculator:public unary_function<double,double>{
public:
  ///really applying this function
  virtual double operator()(double)const=0;
  ///
  virtual void startFirstPass();
  ///
  virtual bool isOnePass()const;
  ///
  virtual void adjustInnerState(double);

  ///Cloning
  CInformationCalculator* clone()const;
  ///
  virtual CInformationCalculator* subClone()const=0;
};

//
class CICEntropy:public CInformationCalculator{
public:
  ///
  virtual double operator()(double)const;
  ///
  virtual CInformationCalculator* subClone()const;
};
//
class CICEntropyWithThreshold:public CInformationCalculator{
  ///
  double mMaximum;
  ///
  double mThreshold;
public:
  ///
  CICEntropyWithThreshold(double inThreshold=1.0);
  ///
  CICEntropyWithThreshold(const CICEntropyWithThreshold&
		       in);
  ///
  void setThreshold(double inThreshold);
  ///
  virtual bool isOnePass()const;
  ///
  virtual void startFirstPass();
  ///
  virtual void adjustInnerState(double);
  ///
  virtual double operator()(double)const;
  ///
  virtual CInformationCalculator* subClone()const;
};

//
class CICERelative:public CInformationCalculator{
  ///
  double mSum;
  ///
  double mThreshold;
public:
  ///
  CICERelative(double inThreshold=1.0);
  ///
  CICERelative(const CICERelative&
		       in);
  ///
  void setThreshold(double inThreshold);
  ///
  virtual bool isOnePass()const;
  ///
  virtual void startFirstPass();
  ///
  virtual void adjustInnerState(double);
  ///
  virtual double operator()(double)const;
  ///
  virtual CInformationCalculator* subClone()const;
};

class CICSum:public CInformationCalculator{
public:
  virtual double operator()(double)const;
  virtual CInformationCalculator* subClone()const;
};
#endif


