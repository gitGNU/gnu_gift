/* -*- mode: c++ -*- 
*/
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, University of Geneva

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
#ifndef _CPERSISTENTTRANSLATEDINDEXMATRIX
#define _CPERSISTENTTRANSLATEDINDEXMATRIX
#include "assert.h"
#include <vector>
#include <map>
#include <string>
#include "libMRML/include/GIFTExceptions.h"
#include "libGIFTAcDistanceMatrix/include/CPersistentMatrix.h"
///#include "CIDToMatrixIndex.h"
//defining this activates printouts in this file
#undef CPTIM_DEBUG
#define HERE __FILE__ << ":" << __LINE__ << ":" 

template<class TTID,class TTContent>
class CPersistentTranslatedIndexMatrix{
  ///
  typedef vector<TTContent> CLineVector;
  ///
  typedef map<TTID,TTContent> CIDContentPairList;
  ///FIXME: taking a map here is a performance flaw
  typedef map<TTID,long> CTranslator;
  ///
  typedef vector<TTID> CIndexList;
  ///
  typedef CPersistentMatrix<TTContent> CMatrix;
  ///
  CTranslator mTranslator;
  ///
  CMatrix* mContent;
public:
  ///
  CPersistentTranslatedIndexMatrix();
  ///
  bool create(const char* inFileName,
	      const CIndexList& inIndexList);
  ///
  bool open(const char* inFileName);
  ///
  bool putLineVector(const TTID& inID,
		     const CLineVector& inVector)const;
  ///
  bool getLineVector(const TTID& inID,
		     CLineVector& outVector)const;
  ///
  bool putLineVector(const TTID& inID,
		     const CIDContentPairList& inList)const;
  ///
  bool getLineVector(const TTID& inI,
		     CIDContentPairList& outList)const;
  ///
  bool setValue(const TTID& inX,
		const TTID& inX,
		const TTContent& inValue)const;
  ///
  operator bool()const;
  ///
  long size()const;
};
///
template<class TTID,class TTContent>
CPersistentTranslatedIndexMatrix<TTID,TTContent>
::CPersistentTranslatedIndexMatrix():
  mContent(0)
{
  
};
///
template<class TTID,class TTContent>
bool CPersistentTranslatedIndexMatrix<TTID,TTContent>
::create(const char* inFileName,
	 const CIndexList& inIndexList){
  CIndexList lIndexList(inIndexList);
  sort(lIndexList.begin(),lIndexList.end());

  long lCount=0;
  for(CIndexList::const_iterator i=lIndexList.begin();
      i!=lIndexList.end();
      i++,lCount++){
    mTranslator.insert(make_pair(*i,lCount));
  }
  mContent=new CMatrix(lCount,lCount);
  if(mContent){
    string lStreamName=string(inFileName)+string(".trans");
    ofstream lStream(lStreamName.c_str());
    if(lStream){
      for(CTranslator::const_iterator i=mTranslator.begin();
	  i!=mTranslator.end();
	  i++){
	lStream << i->first 
		<< " " 
		<< i->second 
		<< endl;
      }
    }
    return lStream && mContent->create(inFileName) ;
  }
  return false;
};
///
template<class TTID,class TTContent>
bool CPersistentTranslatedIndexMatrix<TTID,TTContent>
::open(const char* inFileName){
  long lCount=0;

  delete mContent;
  
  mTranslator.clear();
  string lStreamName=string(inFileName)+string(".trans");
  ifstream lStream(lStreamName.c_str());
  if(lStream){
    cout << HERE
	 << "Opened translator stream" 
	 << endl;
  }else{
    cout << HERE << "FAILED" << endl;
    assert(0);
  }
  //reading pairs: left side is ID-element in ascii
  //right side is index in vector
  if(lStream){
    while(lStream){
      TTID lFirst;
      long lSecond;
      lStream >> lFirst 
	      >> lSecond;
      mTranslator.insert(make_pair(lFirst,lSecond));
    }
  }
  cout << HERE 
       << "After reading translator: " 
       << this->size()
       << " elements in translator"
       << endl;
  mContent=new CMatrix(size(),
		       size());
  mContent->open(inFileName);

  assert(mContent->operator bool());
  cout << HERE 
       << "OPEN3" 
       << endl;
  return lStream && mContent->operator bool();
}
///
template<class TTID,class TTContent>
bool CPersistentTranslatedIndexMatrix<TTID,TTContent>
::getLineVector(const TTID& inID,
		CLineVector& outVector)const{
  assert(mContent);

#ifdef CPTIM_DEBUG
  cout << "(" << inID << ")" << endl;
#endif

  if(mTranslator.find(inID)!=mTranslator.end()){
    long lLocalLine=mTranslator.find(inID)->second;
#ifdef CPTIM_DEBUG
    cout << "getting Line: " 
	 << lLocalLine 
	 << endl;
#endif
    return mContent->getLineVector(lLocalLine,outVector);
  }else{
    cout <<"<<dumping"<<endl;
    for(CTranslator::const_iterator i=mTranslator.begin();
	i!=mTranslator.end();
	i++){
      cout << i->first << "," << i->second << endl;
    }
    cout << "dumping>>"<<endl;
    assert(!"NotFound");
  }
  return false;
};
///
template<class TTID,class TTContent>
bool CPersistentTranslatedIndexMatrix<TTID,TTContent>
::getLineVector(const TTID& inI,
		CIDContentPairList& outList)const{
  assert(mContent);

  CLineVector lVector(size());
  bool lReturnValue=getLineVector(inI,
				  lVector);
  outList.clear();
  try{
    for(CTranslator::const_iterator i=mTranslator.begin();
	i!=mTranslator.end();
	i++){
      outList.insert(make_pair(i->first,
			       lVector[i->second]));
    }
  }catch(...){
    return false;
  }
  return lReturnValue;
};
///
template<class TTID,class TTContent>
bool CPersistentTranslatedIndexMatrix<TTID,TTContent>
::putLineVector(const TTID& inID,
		const CLineVector& inVector)const{

  if(mTranslator.find(inID)!=mTranslator.end()){
    long lLine=mTranslator.find(inID)->second;

    return mContent->putLineVector(lLine,inVector);
  }else{
    assert(!"There is no ID of this kind");
  }
  return 0;
};
///
template<class TTID,class TTContent>
bool CPersistentTranslatedIndexMatrix<TTID,TTContent>
::putLineVector(const TTID& inID,
		const CIDContentPairList& inList)const{
  CLineVector lVector(size());
  bool lReturnValue=getLineVector(inID,
				  lVector);
  cout << "inID" << inID << endl;
  assert(lReturnValue);

  for(CIDContentPairList::const_iterator i=inList.begin();
      i!=inList.end();
      i++){
    CTranslator::const_iterator lXPosition=mTranslator.find(i->first);
    if(lXPosition!=mTranslator.end()){
      lVector[lXPosition->second]=i->second;
    }
  }

  return lReturnValue && putLineVector(inID,
				       lVector);
};

///
template<class TTID,class TTContent>
long CPersistentTranslatedIndexMatrix<TTID,TTContent>
::size()const{
  return mTranslator.size();
}
///
template<class TTID,class TTContent>
CPersistentTranslatedIndexMatrix<TTID,TTContent>::operator bool()const{
  return mContent && bool(*mContent);
}

#endif
