/* -*- mode: c++ -*- 
*/
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
#ifndef _CPROBABILITYCOMBINER
#define _CPROBABILITYCOMBINER

#include <functional>
#include <assert.h>

class CProbabilityCombiner:public binary_function<double,double,double>{
  double mBelief;
  int    mNumberOfElements;
public:
  CProbabilityCombiner();
  virtual void setBelief(double inBelief=1);
  virtual void setNumberOfElements(int inNumberOfElements=1);
  virtual double perform(double,
			 double)const=0;
  virtual double operator()(double,
			    double)const;
  virtual CProbabilityCombiner* clone()const;// virtual is new as of 20010306
  virtual CProbabilityCombiner* subClone()const=0;
};

class CPCMaximum:public CProbabilityCombiner{
public:
  virtual double perform(double,double)const;
  virtual CProbabilityCombiner* subClone()const;
};

class CPCMinimum:public CProbabilityCombiner{
public:
  virtual double perform(double,double)const;
  virtual CProbabilityCombiner* subClone()const;
};

class CPCProduct:public CProbabilityCombiner{
 public:
  virtual double perform(double,double)const;
  virtual CProbabilityCombiner* subClone()const;
};

class CPCDiscrete:public CProbabilityCombiner{
  double lThreshold;
 public:
  virtual double perform(double,double)const;
  virtual CProbabilityCombiner* subClone()const;
};

#endif
