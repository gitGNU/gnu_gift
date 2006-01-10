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
#ifndef _CWEIGHTER
#define _CWEIGHTER
#include "libGIFTQuInvertedFile/include/uses-declarations.h"
class CWeightingFunction;
class CQueryNormalizer;
class CAcInvertedFile;
/**
   A weighter is a set of WeightingFunction and 
   normalizers which are going to be used together.

   Also:
   The content of the weighting function factory

   @author Wolfgang Müller
*/
class CWeighter{
  /** */
  CWeightingFunction* mWeightingFunction;
  /** */
  CQueryNormalizer* mDocumentNormalizer;
  /** */
  CQueryNormalizer* mQueryNormalizer;
public:
  /** constructor */
  CWeighter(CWeightingFunction* inWeightingFunction=0,
	    CQueryNormalizer* inDocumentNormalizer=0,
	    CQueryNormalizer* inQueryNormalizer=0);
  /** 
      make the weighting function know who its normalizers are
  */
  void init();
  /** 
      Set for all content the accessor properly and then call init.
  */
  void setAccessor(CAcInvertedFile& inAccessor);
  /** 
      clone: clone an init-ed copy of this
  */
  CWeighter* clone()const;
  
  /** destructor: deletes all content. */
  ~CWeighter();

  /** */
  CWeightingFunction* getWeightingFunction()const;
  /** */
  CQueryNormalizer* getDocumentNormalizer()const;
  /** */
  CQueryNormalizer* getQueryNormalizer()const;
};

#endif
