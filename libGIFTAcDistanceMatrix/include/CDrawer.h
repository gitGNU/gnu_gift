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
#ifndef _CDRAWER
#define _CDRAWER
#include "../include/uses-declarations.h"
#include <assert.h>
#include <math.h>
#include <functional>

/***************************************
*
* CDrawer
*  a function for drawing from a CPSet 
*
****************************************
*
* modification history:
*
* WM 19990820 created file
*
****************************************
*
* compiler defines used:
*
*
****************************************/

class CDrawer:public unary_function<double,double>{
 protected:

  int mNumberOfImages;
 public:
  ///
  virtual double operator()(double)const=0;
  ///
  virtual CDrawer* subClone()const=0;
  ///
  void setNumberOfImages(const int& inNumberOfImages);
  ///
  int getNumberOfImages()const;
  ///
  CDrawer* clone()const;
};

class CDUniform:public CDrawer
{
public:
  virtual double operator()(double)const;
  virtual CDrawer* subClone()const;
};

class CDPreferMiddle:public CDrawer
{
public:
  virtual double operator()(double)const;
  virtual CDrawer* subClone()const;
};
#endif


