// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

     Copyright (C) 2003, 2004 Bayreuth University
    Copyright (C) 2005,2006 Bamberg University
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

/** Author: Wolfgang Müller */

#include "libGIFTQuInvertedFile/include/CQueryNormalizer.h"
#include "libGIFTQuInvertedFile/include/CWeightingFunction.h"
#include "libGIFTQuInvertedFile/include/CWeighter.h"
/** constructor */
CWeighter::CWeighter(CWeightingFunction* inWeightingFunction,
		     CQueryNormalizer* inDocumentNormalizer,
		     CQueryNormalizer* inQueryNormalizer):
  mWeightingFunction(inWeightingFunction),
  mDocumentNormalizer(inDocumentNormalizer),
  mQueryNormalizer(inQueryNormalizer){
};
/** destructor: deletes all content. */
CWeighter::~CWeighter(){
  delete mWeightingFunction;
  delete mDocumentNormalizer;
  delete mQueryNormalizer;
};
/** 
    initialise pointers 
*/
void CWeighter::init(){
  mWeightingFunction->setNormalizers(mQueryNormalizer,
				     mDocumentNormalizer); 
}

/** 
    initialise pointers 
*/
void CWeighter::setAccessor(CAcInvertedFile& inAccessor){
  mWeightingFunction->setAccessor(&inAccessor);
  mQueryNormalizer->setAccessor(&inAccessor);
  mDocumentNormalizer->setAccessor(&inAccessor);
  
  init();
}

/** initialise pointers */
CWeighter* CWeighter::clone()const{

  CWeightingFunction* lWeightingFunction =mWeightingFunction->clone();
  CQueryNormalizer*   lDocumentNormalizer=mDocumentNormalizer->clone();
  CQueryNormalizer*   lQueryNormalizer   =mQueryNormalizer->clone();

  CWeighter* lWeighter=new CWeighter(lWeightingFunction,
				     lQueryNormalizer,
				     lDocumentNormalizer);
  
  lWeighter->init();
  
  return lWeighter;
}

/** */
CWeightingFunction* CWeighter::getWeightingFunction()const{
  return mWeightingFunction;
};
/** */
CQueryNormalizer* CWeighter::getDocumentNormalizer()const{
  return mDocumentNormalizer;
};
/** */
CQueryNormalizer* CWeighter::getQueryNormalizer()const{
  return mQueryNormalizer;
};


