/***************************************
*
* CDocumentFrequencyList
*
****************************************
*
* modification history:
*
* HM 090399 created the documentation
* WM 10  98 created the file
****************************************
*
* compiler defines used:
*
*
****************************************/
#include "libGIFTAcInvertedFile/include/CDocumentFrequencyList.h"
#include <unistd.h> // debugging

#include "libMRML/include/CMutex.h"
extern CMutex* gMutex;

#ifdef __CDFL_MEM_DEBUG__
int gSize=0;
int gInSize=0;
#endif
/***************************************
*
* writeBinary
*
****************************************
*
* modification history
*
* 
*
****************************************/
CDocumentFrequencyList::CDocumentFrequencyList(size_t inSize):
#ifdef  _CDocumentFrequencyListIsArray
  mContent(new CDocumentFrequencyElement[inSize])
#else
#ifdef  _CDocumentFrequencyListIsVector
  vector<CDocumentFrequencyElement>(inSize)
#else
//otherwhise ignore the parameter
  list<CDocumentFrequencyElement>()
#endif
#endif 
{
  mSize=inSize;
#ifdef  _CDocumentFrequencyListIsArray
  mEnd=mContent+inSize;
#endif
#ifdef __CDFL_MEM_DEBUG__
  gSize+=size();
  gInSize+=inSize;
#endif
};
CDocumentFrequencyList::CDocumentFrequencyList():
#ifdef  _CDocumentFrequencyListIsVector
  vector<CDocumentFrequencyElement>()
#else
#ifdef  _CDocumentFrequencyListIsList
  //otherwhise ignore the parameter
  list<CDocumentFrequencyElement>()
#endif
#endif 
#ifdef  _CDocumentFrequencyListIsArray
  mContent(new CDocumentFrequencyElement[1])// will this work?
#endif
{
#ifdef  _CDocumentFrequencyListIsArray
  mSize=0;
  mEnd=mContent;
#endif
#ifdef __CDFL_MEM_DEBUG__
  gSize+=size();
  gInSize+=0;
#endif
};
CDocumentFrequencyList::~CDocumentFrequencyList(){
#ifdef  _CDocumentFrequencyListIsArray
  delete[] mContent;
#endif
#ifdef __CDFL_MEM_DEBUG__
  gSize-=size();
  gInSize-=mSize;
  cout << "[S" << gSize << "," << gInSize << "]" << flush;
#endif
}
bool CDocumentFrequencyList::writeBinary(ostream& outStream)const{
  bool lRetVal=true;

#ifdef _CDocumentFrequencyListIsArray
  outStream.write((char*)mContent,size()*sizeof(CDocumentFrequencyElement));
#else
  for(const_iterator i=begin();
      i!=end();
      i++){
    lRetVal=
      lRetVal && (*i).writeBinary(outStream);
  }
#endif
  return lRetVal;
};
bool CDocumentFrequencyList::readBinary(istream& inStream){
#ifdef _CDocumentFrequencyListIsList
  for(unsigned int i=0;
      i<mSize && inStream;
      i++){
    //neat: construct CDocumentFrequencyElement
    //      directly from file
    push_back(CDocumentFrequencyElement(inStream));
  }
#else
  if(inStream){
    //gMutex->lock();
    //    cout << "|[" << getpid() << flush;
    inStream.read((char*)begin(),size()*sizeof(CDocumentFrequencyElement));
    //cout << "]" << flush; <gMutex->unlock();
  }
#endif

  return bool(inStream);
};
#ifdef _CDocumentFrequencyListIsArray
CDocumentFrequencyList::iterator CDocumentFrequencyList::begin(){
  return mContent;
}
CDocumentFrequencyList::iterator CDocumentFrequencyList::end(){
  return mEnd;
}
CDocumentFrequencyList::const_iterator CDocumentFrequencyList::begin()const{
  return mContent;
}
CDocumentFrequencyList::const_iterator CDocumentFrequencyList::end()const{
  return mEnd;
}
size_t CDocumentFrequencyList::size()const{
  return mSize;
}

void CDocumentFrequencyList::setEnd(CDocumentFrequencyElement* inEnd){
  mEnd=inEnd;
  mSize=mEnd-mContent;
}
#endif
