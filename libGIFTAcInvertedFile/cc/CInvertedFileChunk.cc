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
* CInvertedFileChunk
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
* _CDocumentFrequencyListIsVector is the document frequency list a vector?
*
*
****************************************/

#include "FeatureExtraction/gift_features.h"
#include "libGIFTAcInvertedFile/include/CInvertedFileChunk.h"
#include <algorithm>


/***************************************
*
* addElement
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CInvertedFileChunk::addElement(TID inDocumentID,
				    double inDocumentFrequency){

  //cout << "a" << flush;

  insert(make_pair(inDocumentID,inDocumentFrequency));
};



/***************************************
*
* getCollectionFrequency - returns the percentage of documents which contain this one feature
*
****************************************
*
* modification history
*
* 
*
****************************************/
double CInvertedFileChunk::getCollectionFrequency(size_t inSize)const{
  return double(size())/inSize;
}



/***************************************
*
* writeBinary - writes the binary block for one feature into the file
*
****************************************
*
* modification history
*
* 
*
****************************************/
bool CInvertedFileChunk::writeBinary(ostream& outStream,
				     TID inFeatureID,
				     size_t inSize)const{
  
  bool lRetVal=true;

  {
    TID lFeatureID(inFeatureID);
    lRetVal = lRetVal && outStream.write((char*)&lFeatureID,
					 sizeof(lFeatureID));
  }
  
  {
    float lCollectionFrequency=(float(size())
				/inSize);
    if((lCollectionFrequency >1)
       ||
       (lCollectionFrequency < 0)){
      cout << "The collection frequency is: "
	   << lCollectionFrequency
	   << endl;
      assert(!"collection frequency out of range");
    }
    lRetVal = lRetVal && outStream.write((char*)&lCollectionFrequency,
				sizeof(lCollectionFrequency));
  }
  
  int lSize(size());
  {
    // writing this twice means: there is
    // no space left in this list
    lRetVal = lRetVal && outStream.write((char*)&lSize,
					 sizeof(lSize));
    // writing this twice means: there is
    // no space left in this list
    lRetVal = lRetVal && outStream.write((char*)&lSize,
					 sizeof(lSize));
    cout << "writing chunk of size " << lSize <<endl;
  }
  
  
  CDocumentFrequencyList lList(lSize);
  
#ifndef _CDocumentFrequencyListIsList
  CDocumentFrequencyList::iterator j=lList.begin();
#endif
  
  for(const_iterator i=begin();
      i!=end();
      i++){
#ifndef _CDocumentFrequencyListIsList
    *(j++)=CDocumentFrequencyElement((*i).first,
				     (*i).second);
#else
    lList.push_back(CDocumentFrequencyElement((*i).first,
					      (*i).second)
		    );
#endif
  }
#ifdef _CDocumentFrequencyListIsArray
  //lList.setEnd(j);// superfluous, because lList.begin()+lSize==j anyway
  assert(j==lList.begin()+lSize);
#endif
  
#ifndef _CDocumentFrequencyListIsList
  sort(lList.begin(),
       lList.end(),
       CSortByDocumentFrequency_DFE());
#else
  
  lList.sort(CSortByDocumentFrequency_DFE());
#endif
  
  for(CDocumentFrequencyList::const_iterator i=lList.begin();
      i!=lList.end();
      i++){
    lRetVal=lRetVal && i->writeBinary(outStream);
  }
  
  return lRetVal;
}
