#ifndef _CSELFDESTROYPOINTER
#define _CSELFDESTROYPOINTER
#include "../include/uses-declarations.h"/*A pointer which stores along with itself, if it
  destroys the element it points to
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
    delete mPointer;
  }
  return mPointer=inPointer;
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
