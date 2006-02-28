/* -*- mode: c++ -*- 
*/
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
#ifndef _CIDTOMATRIXINDEX
#define _CIDTOMATRIXINDEX
#include "libGIFTAcDistanceMatrix/include/uses-declarations.h"
class CIDToMatrixIndex<T>{
  ///
  map<T,long> mContent;
public:
  ///
  CIDToMatrixIndex();
  ///
  void addID(const T& inID);
  ///
  long size()const;
  ///
  long operator[](const T& inID)const;
};

template<class T>
CIDToMatrixIndex<T>::CIDToMatrixIndex(){
}

template<class T>
long CIDToMatrixIndex<T>::size()const{
  return mContent.size();
}

template<class T>
void CIDToMatrixIndex<T>::addElement(T const& inID){
  long lCurrentSize =size();
  mContent[inID]    =lCurrentSize;
}

template<class T>
long CIDToMatrixIndex<T>::operator[](const T& inID)const{
  return *mContent.find(inID);
};
#endif
