// -*- mode: c++ -*- 
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
/***************************************
*
* CDocumentFrequencyElement
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
* _IS_FLOAT_FEATURE_ features have float document frequencies
* _NO_PRINTOUT_WRITE_BINARY   we do not want to be kept posted about 
*                             the progress of the binary writing
*
****************************************/
#include "libGIFTAcInvertedFile/include/CDocumentFrequencyElement.h"
#include "FeatureExtraction/gift_features.h"
#include <cassert>
#include <fstream>

#define _IS_FLOAT_FEATURE_

#define _NO_PRINTOUT_WRITE_BINARY
/*   */
static unsigned const int sFREQ_MAX=(FREQ_MAX);



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
bool CDocumentFrequencyElement::input(istream& inStream){
  inStream >> mContent.mID 
	   >> mContent.mDocumentFrequency;
  return inStream;
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
bool CDocumentFrequencyElement::output(ostream& outStream)const{
  outStream << mContent.mID 
	    << " " 
	    << mContent.mDocumentFrequency
	    << endl;
  return outStream;
};



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
bool CDocumentFrequencyElement::writeBinary(ostream& outStream)const{
#ifndef _IS_FLOAT_FEATURE_
  outStream.write(&mContent.mID,
		  sizeof(mContent.mID));
  unsigned int lDF=(unsigned int)(mContent.mDocumentFrequency*sFREQ_MAX);

  outStream.write(&lDF,
		  sizeof(lDF));
#else
#ifndef _NO_PRINTOUT_WRITE_BINARY
  cout << "[" 
       << sizeof(*this) 
       << ";"
       << getID()
       << ";"
       << getDocumentFrequency()
       << "]";
#endif
  outStream.write((char*)this,
		  sizeof(*this));
#endif
  return outStream;
};



/***************************************
*
* Constructor
*
****************************************
*
* modification history
*
* 
*
****************************************/
CDocumentFrequencyElement::CDocumentFrequencyElement(istream& inStream){

  if(inStream){

#ifndef _IS_FLOAT_FEATURE_

#error "things here should not be compiled any more "

    //old, slow, ugly
    struct{
      TID mFeatureID;
      unsigned int mContent.mDocumentFrequency;
    }lVal;
    
    inStream.read(&lVal,
		  sizeof(lVal));



    //FIXME HACK FIXME for curing a misunderstanding between davids and my programs...
    //lVal.mContent.mDocumentFrequency&=sFREQ_MAX;

    mContent.mDocumentFrequency=float(lVal.mContent.mDocumentFrequency)/sFREQ_MAX;

    if(mContent.mDocumentFrequency>1.0001){

      cout << "DocumentFrequency is wrong:"
	   << mContent.mDocumentFrequency
	   << ">1.0001"
	   << "Better?"
	   << mContent.mDocumentFrequency/(1<<24)
	   << endl
	   << flush;
    }

    assert(mContent.mDocumentFrequency<1.0001);

    mContent.mID=lVal.mFeatureID;


#else
    //new fast beautiful
    inStream.read((char*)&mContent,
		  sizeof(mContent));

#ifdef PRINT
    cout << "[ID0x" 
	 << hex
	 << mContent.mID 
	 << dec
	 << ",DF" 
	 << mContent.mDocumentFrequency
	 << "--"
	 << sizeof(*this)
	 << "]"
	 << flush ;
#endif
#endif
  }
}




/***************************************
*
* Constructor
*
****************************************
*
* modification history
*
* 
*
****************************************/
CDocumentFrequencyElement::CDocumentFrequencyElement(TID inID,
						     float inDocumentFrequency){
  mContent.mDocumentFrequency=inDocumentFrequency;
  mContent.mID=inID;
}



/***************************************
*
* Constructor
*
****************************************
*
* modification history
*
* 
*
****************************************/
CDocumentFrequencyElement::CDocumentFrequencyElement(const CDocumentFrequencyElement& inElement){
  mContent.mDocumentFrequency=inElement.mContent.mDocumentFrequency;
  mContent.mID=inElement.mContent.mID;
}



/***************************************
*
* Constructor
*
****************************************
*
* modification history
*
* 
*
****************************************/
CDocumentFrequencyElement::CDocumentFrequencyElement(){
  mContent.mDocumentFrequency=0;
  mContent.mID=0;
}




/***************************************
*
* getID
*
****************************************
*
* modification history
*
* 
*
****************************************/
TID CDocumentFrequencyElement::getID()const{
  return mContent.mID;
};



/***************************************
*
* getDocumentFrequency
*
****************************************
*
* modification history
*
* 
*
****************************************/
float CDocumentFrequencyElement::getDocumentFrequency()const{
  return mContent.mDocumentFrequency;
};



/***************************************
*
* getIntegerDocumentFrequency
*
****************************************
*
* modification history
*
* 
*
****************************************/
unsigned int CDocumentFrequencyElement::getIntegerDocumentFrequency()const{
  return (unsigned int)(mContent.mDocumentFrequency*sFREQ_MAX);
};



/***************************************
*
* setDocumentFrequency
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
void CDocumentFrequencyElement::setDocumentFrequency(float inDF){
  mContent.mDocumentFrequency=inDF;
};



/***************************************
*
* divideDocumentFrequency
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CDocumentFrequencyElement::divideDocumentFrequency(float inDivisor){
  mContent.mDocumentFrequency/=inDivisor;
};





