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
#ifndef _MERGE_SORT
#define _MERGE_SORT

#include <algorithm> // using swap
#include <fstream>   // file access


#define STREAMPOS_T long long int

/**
   A template for merging streams in the sense of mergesort
   @params in1,in2 are the streams to be merged
   @params out     is the stream which contains afterwards the two
                   merged streams
		   
   The stream is considered to contain only elements of type T

   This is the principal helper function of merge_sort_streams
 */
template<class T>
void merge_streams(istream& in1, const STREAMPOS_T inCount1,
		   istream& in2, const STREAMPOS_T inCount2,
		   ostream& out){

  {
//     cout << "Merging: " 
// 	 << inCount1
// 	 << ","
// 	 << inCount2
// 	 << endl;



    if(!inCount1){// if there is nothing to merge
      for(STREAMPOS_T i=0;//...copy
	  i<inCount2;
	  i++){
	T l2;
	in2.read((char*)&l2,
		 sizeof(l2));
	assert(in2);

	out.write((char*)&l2,
		  sizeof(l2));
	assert(out);
      }
      return;//and return
    }
    if(!inCount2){//if there is nothing to merge
      for(STREAMPOS_T i=0;//...copy
	  i<inCount1;
	  i++){
	T l1;
	in1.read((char*)&l1,
		 sizeof(l1));
	assert(in1);
	out.write((char*)&l1,
		  sizeof(l1));
	assert(out);
      }
      return;//and return
    }
    
    STREAMPOS_T lI1(0);
    STREAMPOS_T lI2(0);
    
    //read the first record from both files
    T l1;
    in1.read((char*)&l1,
	     sizeof(l1));
    assert(in1);
    T l2;
    in2.read((char*)&l2,
	     sizeof(l2));
    assert(in2);
    
    while((lI1<inCount1)
	  &&(lI2<inCount2)){
      if(l1<l2){
	//
	out.write((char*)&l1,
		  sizeof(l1));
	assert(out);

	//
	lI1++;
	if(lI1<inCount1){
	  in1.read((char*)&l1,
		   sizeof(l1));
	  assert(in1);
	}
      }else{
	//
	out.write((char*)&l2,
		  sizeof(l2));
	//
	lI2++;
	if(lI2<inCount2){
	  in2.read((char*)&l2,
		   sizeof(l2));
	  assert(in2);
	}
      }
    }
    while(lI1<inCount1){
      //
      out.write((char*)&l1,
		sizeof(l1));
      //
      lI1++;
      if(lI1<inCount1){
	in1.read((char*)&l1,
		 sizeof(l1));
	assert(in1);
      }
    }
    while(lI2<inCount2){
      //
      out.write((char*)&l2,
		sizeof(l2));
      assert(out);
      //
      lI2++;
      if(lI2<inCount2){
	in2.read((char*)&l2,
		 sizeof(l2));
	assert(in2);
      }
    }
  }
}
/**
   This function sorts a stream containing 
   elements of type T. The result of the merge sort
   is to be found inFileToBeSorted. The temporary file
   contains partly sorted garbage of the same size
   as the file of name inFileToBeSortedName

   @param inFileToBeSortedName the file to be sorted
   @param inFileToBeSortedName the name of a temporary file
 */
template<class T>
void merge_sort_streams(const char* inFileToBeSortedName,
			const char* inTemporaryName){

  const char* lFileToBeSortedName(inFileToBeSortedName);
  const char* lTemporaryName(inTemporaryName);

  STREAMPOS_T lFileSize(0);
  ifstream lToBeSorted1(inFileToBeSortedName);
  lToBeSorted1.seekg(0,
		    ios::end);
  lFileSize=lToBeSorted1.tellg();
  lToBeSorted1.close();
  
  ofstream lTemporary;
  ifstream lToBeSorted2;
    
  STREAMPOS_T lCount(0);
  for(STREAMPOS_T iMergeSize=sizeof(T);
      (iMergeSize < lFileSize)
	|| (lCount%2)
	// ||(lCount%2) makes sure that we will get 
	// the result in inFileToBeSorted
	;
      (iMergeSize = long(iMergeSize) << 1),
	(lCount++)){
    
    cout << "MERGESORT MergeSize " 
	 << iMergeSize 
	 << endl;
    
    
    lToBeSorted1.open(lFileToBeSortedName);
    lToBeSorted1.clear();

    lToBeSorted2.open(lFileToBeSortedName);
    lToBeSorted2.clear();

    lTemporary.open(lTemporaryName);
    lTemporary.clear();
    
    
    for(STREAMPOS_T i=0;
	i<lFileSize;
	i += iMergeSize*2){
      lToBeSorted1.seekg(i);

      if(!lToBeSorted1){
	cerr << "__FILE__:__LINE__: lToBeSorted false, after seekg("
	     << i
	     << ")"
	     << endl;
      }
      
      assert(lToBeSorted1);

      if(i+iMergeSize<lFileSize){
	lToBeSorted2.seekg(i+iMergeSize);
	assert(lToBeSorted2);
      }

      STREAMPOS_T lMergeSize1=lFileSize-i;
      STREAMPOS_T lMergeSize2=lFileSize-i-iMergeSize;
      
      // lToBeSorted1.clear();
      // lToBeSorted2.clear();
      
      if(lMergeSize1>iMergeSize){
	lMergeSize1=iMergeSize;
      }
      if(lMergeSize2>iMergeSize){
	lMergeSize2=iMergeSize;
      }
      if(lMergeSize1<0){
	lMergeSize1=0;
      }

      if(lMergeSize2<0){
	lMergeSize2=0;
      }
      merge_streams<T>(lToBeSorted1,
		       lMergeSize1/sizeof(T),
		       lToBeSorted2,
		       lMergeSize2/sizeof(T),
		       lTemporary);
    }
    
    lTemporary.close();
    lToBeSorted1.close();
    lToBeSorted2.close();
    swap(lFileToBeSortedName,
	 lTemporaryName);
  }
}
#endif
