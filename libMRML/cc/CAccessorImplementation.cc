// -*- Mode: c++ -*- 
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
* CAccessorImplementation - a base class for everything
* accessing a GIFT database.
* at present this will be either an inverted file
* or a TrackingGIFT accessor.
*
****************************************
*
* modification history:
* 
* WM 19990804 created 
*
****************************************
*
* compiler defines used:
*
* _NO_PRINT_OFFSET_CHECK ?
* _NO_CHECK_OFFSET_FILE  ?
* _NO_CHECK_CONSISTENCY  ?
* _NO_FIDPRINT           ?
* _NO_DIDPRINT           ?
* _NO_PRINT_ID           ?
*
****************************************/


#include <string>
#include <set> 
#include "libMRML/include/CAccessorImplementation.h"
#include <iostream>
#include <cassert>
#include <cmath>

#define _NO_PRINT_OFFSET_CHECK
#define _NO_CHECK_OFFSET_FILE
#define _NO_CHECK_CONSISTENCY
#define _NO_FIDPRINT
#define _NO_DIDPRINT
#define	_NO_PRINT_ID


/***************************************
*
* getAllIDs - make a list of all IDs
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CAccessorImplementation::getAllIDs(list<TID>& outReturnList)const{
  
  for(TID_CAccessorElement_map::const_iterator i=mIDToAccessorElement.begin();
      i!=mIDToAccessorElement.end();
      i++){
    outReturnList.push_back(i->first);
  }
}

/***************************************
*
* getAllAccessorElements
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CAccessorImplementation::getAllAccessorElements(list<CAccessorElement>& outReturnList)const{
  
  for(TID_CAccessorElement_map::const_iterator i=mIDToAccessorElement.begin();
      i!=mIDToAccessorElement.end();
      i++){
    outReturnList.push_back(i->second);
  }
}

/***************************************
*
* getRandomIDs - make a list of distinct random IDs
*
****************************************
*
* modification history
* WM created 19990520
* 
*
****************************************/
void CAccessorImplementation::getRandomIDs(list<TID>& outReturnList,
			     list<TID>::size_type inRequestedSize)const{
  
  const list<TID>::size_type lModulo=mIDToAccessorElement.size();
  
  inRequestedSize=min(lModulo,
		      inRequestedSize);
  
  set<list<TID>::size_type> lPositions;
  
  ///
  for(int i=0;
      i<inRequestedSize;
      i++){
    lPositions.insert(rand()%lModulo);
  }
  while(lPositions.size()<inRequestedSize){
    lPositions.insert(rand()%lModulo);
  }

  cout << "Size of the drawn set" 
       << lPositions.size()
       << endl;

  ///
  TID_CAccessorElement_map::const_iterator lCurrentID=mIDToAccessorElement.begin();
  ///
  TID_CAccessorElement_map::size_type lCurrentPosition=0;
  ///
  for(set<list<TID>::size_type>::const_iterator i=lPositions.begin();
      i!=lPositions.end();
      i++){
    for(;
	lCurrentPosition<*i;
	lCurrentPosition++){
      lCurrentID++;
    }
    outReturnList.push_back(lCurrentID->first);
  }
}


/***************************************
*
* getRandomAccessorElements - make a list of distinct random IDs
*
****************************************
*
* modification history
* WM created 19990520
* 
*
****************************************/
void CAccessorImplementation::getRandomAccessorElements(list<CAccessorElement>& outReturnList,
					  list<CAccessorElement>::size_type inRequestedSize)const{

  if(!inRequestedSize){
    return;
  }

  outReturnList.clear();

  const list<CAccessorElement>::size_type lModulo=mIDToAccessorElement.size();

  inRequestedSize=min(lModulo,
		      inRequestedSize);
  
  set<list<CAccessorElement>::size_type> lPositions;
  
  ///
  for(int i=0;
      i<inRequestedSize;
      i++){
    lPositions.insert(rand()%lModulo);
  }

  while(lPositions.size()<inRequestedSize){
    lPositions.insert(rand()%lModulo);
  }
  
  cout << "Size of the drawn set" 
       << lPositions.size()
       << ","
       << inRequestedSize
       << endl;
  ///
  list<CAccessorElement>::size_type lCurrentPosition(0);
  ///
  TID_CAccessorElement_map::const_iterator lCurrentID=mIDToAccessorElement.begin();
  ///
  for(set<list<CAccessorElement>::size_type>::const_iterator i=lPositions.begin();
      i!=lPositions.end();
      i++){
//     cout << "Before lCurrentID" 
// 	 << lCurrentID->first
// 	 << endl;
    for(/* keep lCurrentPosition */;
	lCurrentPosition<*i;
	lCurrentPosition++){
      lCurrentID++;
    }
//     cout << "lCurrentID" 
// 	 << lCurrentID->first
// 	 << endl;

    outReturnList.push_back(lCurrentID->second);
  }
  //  cout << "still ariving here " << endl;
}


/***************************************
*
* IDToAccessorElement - return the AccessorElement for an image for a given image ID
*
****************************************
*
* modification history
*
* 
*
****************************************/
pair<bool,CAccessorElement> CAccessorImplementation::IDToAccessorElement(TID inID)const
{

  TID_CAccessorElement_map::const_iterator lI=mIDToAccessorElement.find(inID);
  
  if(lI!=mIDToAccessorElement.end())
    {
      return make_pair(true,(*lI).second);
    }
  else
    {
      cerr << "Error in Conversion from ID "
	   << inID 
	   << " to AccessorElement."
	   << endl;
      return make_pair(false,CAccessorElement(-1,
					      "error",
					      "error",
					      "error"));
    }
}


/***************************************
*
* IDToURL - return the URL for an image for a given image ID
*
****************************************
*
* modification history
*
* 
*
****************************************/
string CAccessorImplementation::IDToURL(TID inID)const
{

  TID_CAccessorElement_map::const_iterator lI=mIDToAccessorElement.find(inID);
  
  if(lI!=mIDToAccessorElement.end())
    {
      return (*lI).second.getURL();
    }
  else
    {
      cerr << "Error in Conversion from ID "
	   << inID 
	   << " to URL."
	   << endl;
      return "error";
    }
}



/***************************************
*
* URLToID - return the ID for an image for a given image URL
*
****************************************
*
* modification history
*
* 
*
****************************************/
pair<bool,TID> CAccessorImplementation::URLToID(const string& inURL)const
{

  string_TID_map::const_iterator lI=mURLToID.find(inURL);
  
  if(lI!=mURLToID.end())
    {
      return make_pair<bool,TID>(true,(*lI).second);
    }
  else
    {
      cerr << "Error in Conversion from URL "
	   << inURL 
	   << " to ID."
	   << endl;
      return make_pair<bool,TID>(false,-1);
    }
}

/***************************************
*
* URLToID - return the ID for an image for a given image URL
*
****************************************
*
* modification history
*
* 
*
****************************************/
int CAccessorImplementation::size()const
{
  return mURLToID.size();
}


