// -*- mode: c++ -*- 
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
/***************************************
*
* CAdditionalDocumentInformation - Stores different 
*   informations about document frequencies
*
****************************************
*
* modification history:
*
* HM 090399 created the documentation
*
****************************************
*
* compiler defines used:
*
*
****************************************/

#include "libMRML/include/TID.h"
#include <string>
#include <functional>
#include "libGIFTAcInvertedFile/include/CAdditionalDocumentInformation.h"
#include <algorithm>
#include <assert.h>

/***************************************
*
* Constructor -
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
CAdditionalDocumentInformation::CAdditionalDocumentInformation(const string& inName){
  setFileNameBase(inName);
  resetDF();
  resetSquareDFLogICF();
};



/***************************************
*
* setFileNameBase
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
void CAdditionalDocumentInformation::setFileNameBase(const string& inName){
  mFileNameBase=inName;
  resetDF();
};



/***************************************
*
* resetDF
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
void CAdditionalDocumentInformation::resetDF(){
  mMaximumDF=0;
  mDFSquareSum=0;
};



/***************************************
*
* adjustDF
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
void CAdditionalDocumentInformation::adjustDF(double inDF){
  mDFSquareSum+=inDF*inDF;
  mMaximumDF=max(inDF,mMaximumDF);
};



/***************************************
*
* getMaximumDF
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
double CAdditionalDocumentInformation::getMaximumDF()const{
  assert(mMaximumDF);
  return mMaximumDF;
};



/***************************************
*
* getDFSquareSum
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
double CAdditionalDocumentInformation::getDFSquareSum()const{
  assert(mDFSquareSum);
  return mDFSquareSum;
};



/***************************************
*
* resetSquareDFLogICF
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
void CAdditionalDocumentInformation::resetSquareDFLogICF(){
  mSquareDFLogICFSum=0;
};



/***************************************
*
* adjustSquareDFLogICF
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
void CAdditionalDocumentInformation::adjustSquareDFLogICF(double inValue){
  mSquareDFLogICFSum+=inValue;
};



/***************************************
*
* getSquareDFLogICFSum
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
double CAdditionalDocumentInformation::getSquareDFLogICFSum()const{
  return mSquareDFLogICFSum;
};



/***************************************
*
* output
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
bool CAdditionalDocumentInformation::output()const{
  ofstream 
    lStream(string(mFileNameBase+".ADI").c_str());

  

  return output(lStream);
};



/***************************************
*
* 
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
bool CAdditionalDocumentInformation::output(ostream& outStream)const{
#ifdef PRINT
  cout << "[>mSquareDFLogICFSum" 
       << mMaximumDF
       << ",square"
       << mDFSquareSum
       << ","
       << mSquareDFLogICFSum
       << "]" 
       << flush 
       << endl;
#endif
  outStream << mMaximumDF
	    << " " 
	    << mDFSquareSum
	    << " " 
	    << mSquareDFLogICFSum;
 return outStream;
};



/***************************************
*
* input
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
bool CAdditionalDocumentInformation::input(istream& inStream){
  inStream >> mMaximumDF
	   >> mDFSquareSum
	   >> mSquareDFLogICFSum;
#ifdef PRINT
  cout << "[Max" 
       << mMaximumDF
       << ",square"
       << mDFSquareSum
       << ",IDFlog.."
       << mSquareDFLogICFSum
       << "]" 
       << flush 
       << endl;
#endif
  return inStream;
};



/***************************************
*
* input
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
bool CAdditionalDocumentInformation::input(){
  ifstream 
    lStream(string(mFileNameBase+".ADI").c_str());
  return input(lStream);
};




