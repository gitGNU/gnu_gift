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
#ifndef _CSELFDESTROYPOINTER
#define _CSELFDESTROYPOINTER
#include "libMRML/include/uses-declarations.h"
/*
  destroys the element it points to or not.
*/

template<class T>
class CSelfDestroyPointer{
  ///
  mutable bool mIsSelfDestroyer;
  ///
 protected:
  T* mPointer;
  ///
public:
  ///
  void resetWithoutDeleting();
  ///
  inline bool isSelfDestroyer()const;
  ///
  inline void setIsSelfDestroyer(bool inisSelfDestroyer=true)const;
  ///
  void unsetIsSelfDestroyer()const;
  ///
  T* operator= (T* inPointer);
  ///
  T& operator*();
  ///
  T const& operator*()const;
  ///
  T* operator -> ();
  ///
  T const* operator -> ()const;
  ///
  ~CSelfDestroyPointer();
  ///
  CSelfDestroyPointer(T*,
		      bool = true);
  ///
  CSelfDestroyPointer(const CSelfDestroyPointer<T>& 
		      inSelfDestroyPointer);
  ///
  CSelfDestroyPointer();
  ///
  operator bool()const;
  ///
  operator T*()const;
};


///
template<class T>
void CSelfDestroyPointer<T>::resetWithoutDeleting(){
  mPointer=0;
}

template<class T>
T* CSelfDestroyPointer<T>::operator=(T* inPointer){

  if(mIsSelfDestroyer){
#ifdef _DEBUG_SELF_DESTROY_
    cout <<"£"<<flush;
#endif
    delete mPointer;
  }
  mPointer=inPointer;
}

template<class T>
T const& CSelfDestroyPointer<T>::operator *()const{
  return *mPointer;
}

template<class T>
T const* CSelfDestroyPointer<T>::operator ->()const{
  return mPointer;
}

template<class T>
T& CSelfDestroyPointer<T>::operator *(){
  return *mPointer;
}

template<class T>
T* CSelfDestroyPointer<T>::operator ->(){
  return mPointer;
}

template<class T>
CSelfDestroyPointer<T>::CSelfDestroyPointer(T* inPointer,
					    bool inIsSelfDestroyer):
  mPointer(inPointer),
  mIsSelfDestroyer(inIsSelfDestroyer)
{
}
///
template<class T>
CSelfDestroyPointer<T>::CSelfDestroyPointer(const CSelfDestroyPointer<T>& in):
  mPointer(in.mPointer),
  mIsSelfDestroyer(in.mIsSelfDestroyer)
{
};

template<class T>
CSelfDestroyPointer<T>::CSelfDestroyPointer():
  mPointer(0),
  mIsSelfDestroyer(true)
{
}

template<class T>
CSelfDestroyPointer<T>::~CSelfDestroyPointer()
{
  if(mIsSelfDestroyer){

    delete mPointer;
  }
}


template<class T>
void CSelfDestroyPointer<T>::setIsSelfDestroyer(bool inIsSelfDestroyer)const{
  mIsSelfDestroyer= inIsSelfDestroyer;
};

template<class T>
bool CSelfDestroyPointer<T>::isSelfDestroyer()const{
  return mIsSelfDestroyer;
};

template<class T>
void CSelfDestroyPointer<T>::unsetIsSelfDestroyer()const{
  mIsSelfDestroyer=0;
};

template<class T>
CSelfDestroyPointer<T>::operator bool()const{
  return mPointer;
};

template<class T>
CSelfDestroyPointer<T>::operator T*()const{
  return mPointer;
};

template<class T>
class CSelfClonePointer: public CSelfDestroyPointer<T>{
  ///
  mutable bool mIsSelfCloner;
  ///
public:
  ///
  CSelfClonePointer(T*,
		    bool = true);
  ///
  CSelfClonePointer<T>& operator= (T* in);
  ///
  CSelfClonePointer<T>& operator= (const CSelfClonePointer<T>& in);
  ///
  CSelfClonePointer(const CSelfClonePointer<T>&);
  ///
  CSelfClonePointer();
  ///
  operator bool()const;
  ///
  operator T*()const;
};


template<class T>
CSelfClonePointer<T>& CSelfClonePointer<T>::operator=(T* in){


  CSelfDestroyPointer<T>::operator=(in);
  return *this;
};

template<class T>
CSelfClonePointer<T>& CSelfClonePointer<T>::operator= (const CSelfClonePointer<T>& in){

  mPointer=in.mPointer;
  setIsSelfDestroyer(in.isSelfDestroyer());
  return *this;
};

template<class T>
CSelfClonePointer<T>::CSelfClonePointer(T* inPointer,bool inIsSelfCloner):
  CSelfDestroyPointer<T>(inPointer,
		      inIsSelfCloner)
{
}
template<class T>
CSelfClonePointer<T>::CSelfClonePointer():
  CSelfDestroyPointer<T>(0,
		      true)
{
}
template<class T>
CSelfClonePointer<T>::CSelfClonePointer(const CSelfClonePointer<T>& in):
  CSelfDestroyPointer<T>(in)
{
  if(in.mPointer && in.isSelfDestroyer()){
    mPointer=in.mPointer->clone();
  }else{
    mPointer=in.mPointer;
  }  
}

#endif
