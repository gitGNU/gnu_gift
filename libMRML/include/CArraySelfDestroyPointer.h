#ifndef _CARRAYSELFDESTROYPOINTER
#define _CARRAYSELFDESTROYPOINTER
/*
#include "libMRML/include/uses-declarations.h"

  destroys the element it points to
*/

template<class T>
class CArraySelfDestroyPointer{
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
  ~CArraySelfDestroyPointer();
  ///
  CArraySelfDestroyPointer(T*,
		      bool = true);
  ///
  CArraySelfDestroyPointer(const CArraySelfDestroyPointer<T>& 
		      inSelfDestroyPointer);
  ///
  CArraySelfDestroyPointer();
  ///
  operator bool()const;
  ///
  operator T*()const;
};


///
template<class T>
void CArraySelfDestroyPointer<T>::resetWithoutDeleting(){
  mPointer=0;
}

template<class T>
T* CArraySelfDestroyPointer<T>::operator=(T* inPointer){

  if(mIsSelfDestroyer){
    delete[] mPointer;
  }
  return mPointer=inPointer;
}

template<class T>
T const& CArraySelfDestroyPointer<T>::operator *()const{
  return *mPointer;
}

template<class T>
T const* CArraySelfDestroyPointer<T>::operator ->()const{
  return mPointer;
}

template<class T>
T& CArraySelfDestroyPointer<T>::operator *(){
  return *mPointer;
}

template<class T>
T* CArraySelfDestroyPointer<T>::operator ->(){
  return mPointer;
}

template<class T>
CArraySelfDestroyPointer<T>::CArraySelfDestroyPointer(T* inPointer,
					    bool inIsSelfDestroyer):
  mPointer(inPointer),
  mIsSelfDestroyer(inIsSelfDestroyer)
{
}
///
template<class T>
CArraySelfDestroyPointer<T>::CArraySelfDestroyPointer(const CArraySelfDestroyPointer<T>& in):
  mPointer(in.mPointer),
  mIsSelfDestroyer(in.mIsSelfDestroyer)
{
};

template<class T>
CArraySelfDestroyPointer<T>::CArraySelfDestroyPointer():
  mPointer(0),
  mIsSelfDestroyer(true)
{
}

template<class T>
CArraySelfDestroyPointer<T>::~CArraySelfDestroyPointer()
{
  if(mIsSelfDestroyer){

    delete[] mPointer;
  }
}


template<class T>
void CArraySelfDestroyPointer<T>::setIsSelfDestroyer(bool inIsSelfDestroyer)const{
  mIsSelfDestroyer= inIsSelfDestroyer;
};

template<class T>
bool CArraySelfDestroyPointer<T>::isSelfDestroyer()const{
  return mIsSelfDestroyer;
};

template<class T>
void CArraySelfDestroyPointer<T>::unsetIsSelfDestroyer()const{
  mIsSelfDestroyer=0;
};

template<class T>
CArraySelfDestroyPointer<T>::operator bool()const{
  return mPointer;
};

template<class T>
CArraySelfDestroyPointer<T>::operator T*()const{
  return mPointer;
};
#endif
