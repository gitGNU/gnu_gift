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
#ifndef _CPERSISTENTMATRIX
#define _CPERSISTENTMATRIX
#include "libGIFTAcDistanceMatrix/include/uses-declarations.h"
#include "libGIFTAcDistanceMatrix/include/CPersistentVector.h"
#define CPM_debug

template<class T>
class CPersistentMatrix{
  ///This is for the users of CPersistentMatrix
  typedef vector<T> CLineVector;
  typedef CPersistentVector<T> CPersistentLineVector;

  long mXSize;
  long mYSize;

  mutable fstream* mContent;
public:
  CPersistentMatrix(long inXSize,
		    long inYSize);

  bool open(char const* inFileName);

  bool create(char const* inFileName)const;

  bool getLineVector(long inY,
		     CPersistentLineVector& outVector)const;

  bool getLineVector(long inY,
		     CLineVector& outVector)const;

  bool putLineVector(long inY,
		     CPersistentLineVector const& outVector);

  bool putLineVector(long inY,
		     CLineVector const& outVector);

  bool setValue(long inX,
		long inY,
		T const& inValue);

  operator bool()const;
};


template<class T>
CPersistentMatrix<T>::CPersistentMatrix(long inXSize,
					long inYSize):
  mXSize(inXSize),
  mYSize(inYSize){
}

template<class T>
bool CPersistentMatrix<T>::create(char const* inFileName)const{
  fstream* lFile=new fstream(inFileName,fstream::out+fstream::in);
  CPersistentVector<T> lLine(mXSize);
  if(lFile){
    for(long i=0;
	(i<mYSize)&&(*lFile);
	i++){
      lLine.write(*lFile);
    }
  }
  bool lResultValue= lFile && (*lFile);
  mContent=lFile;
  return lResultValue;
}

template<class T>
bool CPersistentMatrix<T>::open(char const* inFileName){
  mContent=new fstream(inFileName,
		       fstream::in+fstream::out);

  if(mContent && *mContent){
    cout << "PersistentMatrix: Opened File "
	 << inFileName
	 << endl
	 << flush;
  }else{
    cout << "PersistentMatrix: FAILED TO OPEN FILE "
	 << inFileName
	 << endl
	 << flush;

    throw VENotFound(string(string("CPersistentMatrix open")
			    +inFileName)
		     .c_str());

  }
  return(mContent 
	 && 
	 *mContent);
}

template<class T>
bool CPersistentMatrix<T>::getLineVector(long inY,
					 CPersistentLineVector& outVector)const{
#ifdef CPM_debug
  cout << "getLineVector"<<flush;
#endif
  if(!mContent)
    return false;
#ifdef CPM_debug
  cout << "(suite)"<<flush;
#endif

  mContent->seekg(inY * mXSize * sizeof(T));
  bool lReturnValue = outVector.read(*mContent);
  lReturnValue = lReturnValue && (*mContent); 
  assert(*mContent);
  assert(lReturnValue);
#ifdef CPM_debug
  cout << "returning"
       << flush;
#endif CPM_debug
  return lReturnValue;
}

template<class T>
bool CPersistentMatrix<T>::getLineVector(long inY,
					 CLineVector& outVector)const{
  assert(mContent);
#ifdef CPM_debug
  cout << "getLineVector"<<flush;
#endif CPM_debug

  CPersistentLineVector lVector(mXSize);
  bool lReturnValue;
  assert(lReturnValue=getLineVector(inY,
				    lVector));
  outVector.clear();
  copy(lVector.begin(),
       lVector.end(),
       back_inserter(outVector));
  return lReturnValue;
}

template<class T>
bool CPersistentMatrix<T>::putLineVector(long inY,
					 CPersistentLineVector const& 
					 inVector){
  if(!mContent)
    return false;
  
  //  cout << "Posish: " << inY * mXSize << "," << sizeof(T) << endl;

  mContent->seekp(inY * mXSize * sizeof(T));
  bool lReturnValue = inVector.write(*mContent);
  lReturnValue = lReturnValue && (*mContent); 
  return lReturnValue;
}

///FIXME: performance flaw. use vector swap for speed 

template<class T>
bool CPersistentMatrix<T>::putLineVector(long inY,
					 CLineVector const& 
					 inVector){
  CPersistentLineVector lVector(mXSize);
  CPersistentLineVector::iterator i=lVector.begin();
  CLineVector::const_iterator j=inVector.begin();
  while(i!=lVector.end() && j!=inVector.end()){
    *i++=*j++;
  }
  return putLineVector(inY,
		       lVector);
}

template<class T>
bool CPersistentMatrix<T>::setValue(long inX,
				    long inY,
				    T const& inValue){
  if(!mContent)
    return false;
  
  mContent->seekp((inY * mXSize + inX) * sizeof(T) );
  
  mContent->write(&inValue,sizeof(inValue));

  mContent->flush();

  return (*mContent);
}

///
template<class T>
CPersistentMatrix<T>::operator bool()const{
  return mContent && bool(*mContent);
}

#endif
