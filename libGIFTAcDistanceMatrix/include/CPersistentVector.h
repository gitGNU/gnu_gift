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
#ifndef _CPERSISTENTVECTOR
#define _CPERSISTENTVECTOR
#include "libGIFTAcDistanceMatrix/include/uses-declarations.h"
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>

template<class T>
void binary_writer(ostream& outStream,
		   const T& inToBeWritten){
  outStream.write(&inToBeWritten,
		  sizeof(inToBeWritten));
}

template<class T>
void binary_reader(istream& inStream,
		   T& inToBeRead){
  inStream.read((char*)&inToBeRead,
		sizeof(inToBeRead));
}

///
template<class T>
class CPersistentVector:public vector<T>{
  ///
  long mLength;
public:
  ///
  CPersistentVector(long inLength);
  ///
  bool read(istream& inStream);
  ///
  bool read(istream& inStream,
	    long inNumRecords);
  ///
  bool write(ostream& outStream)const;
  ///
  void init(T const& inDefaultValue);
};

template<class T>
CPersistentVector<T>::CPersistentVector(long inLength):
  mLength(inLength),
  vector<T>(inLength)
{
}

template<class T>
void CPersistentVector<T>::init(T const& inDefaultValue){
#ifdef V295
  constant_void_fun<T> f(inDefaultValue);

  this->clear();
  this->reserve(mLength);
  for(long i=0;
      i<mLength;
      i++){
    push_back(f());
  }
#else
  this->clear();
  this->resize(mLength);
  fill(this->begin(),this->end(),inDefaultValue);
#endif
}

template<class T>
bool CPersistentVector<T>::read(istream& inStream){
  return read(inStream,
       mLength);
}

template<class T>
bool CPersistentVector<T>::read(istream& inStream,
				long inLength){
  this->reserve(inLength);
  this->clear();
  for(long i=0;
      i<inLength;
      i++){
    T lToBeRead;
    binary_reader(inStream,
		  lToBeRead);

    assert(lToBeRead>=0 && lToBeRead<=1);

    push_back(lToBeRead);
  }
  mLength=inLength;
  return inStream;
}

template<class T>
bool CPersistentVector<T>::write(ostream& outStream)const{
  for(typename CPersistentVector<T>::const_iterator i=this->begin();
      i!=this->end();
      i++){
    binary_writer(outStream,
		  *i);
  }
  outStream << flush;
  return outStream;
}

#endif
