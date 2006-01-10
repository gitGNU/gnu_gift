/* -*- mode: c++ -*- 
*/
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
// -*- mode: c++ -*-
#ifndef _CNODELETE
#define _CNODELETE
#include "libMRML/include/uses-declarations.h"

#define INLINE

/** 
    The use of this class is to control where an element
    is destroyed. If an instantiation of this class is
    activated, it will bomb in the moment when the 
    destructor is called.

    @Author: Wolfgang Müller
*/
class CNoDelete{
 protected:
  /** is this activated*/
  bool mIsActivated;
 public:
  /** activate this.
      This means: destruction of this 
      will bomb the program using
      an assertion of !mIsActivated */
  INLINE void activate(){
    mIsActivated=1;
  }
  /** activate this.
      This means: destruction of *this 
      will bomb the program using
      a zero assertion */
  INLINE void deActivate(){
    mIsActivated=0;
  };
  /** activate this.
      This means: destruction of *this 
      will be possible
  */
  INLINE CNoDelete():
    mIsActivated(0){
  };
  /** activate this.
      This means: destruction of this 
      will bomb the program using
      a zero assertion */
  INLINE ~CNoDelete(){
    assert(!mIsActivated);
  };
  
};

#endif
