// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

     Copyright (C) 2003, 2004 Bayreuth University
    Copyright (C) 2005,2006 Bamberg University
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
* CIDRelevanceLevelPair
*
****************************************
*
* modification history:
*
* HM 090399 created the documentation
* WM   1198 created file
*
* @Author: Wolfgang Müller
****************************************
*
* compiler defines used:
*
*
****************************************/
#include <cstdio>
#include "libMRML/include/CIDRelevanceLevelPair.h"



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
/* Constructor with no inputs */
CIDRelevanceLevelPair::CIDRelevanceLevelPair():
  mID(0),
  mRelevanceLevel(0)
{};



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
/* constructor with input */
CIDRelevanceLevelPair::CIDRelevanceLevelPair(const TID inID,
					     double inRelevanceLevel):
  mID(inID),
  mRelevanceLevel(inRelevanceLevel){
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
/* constructur with input */
CIDRelevanceLevelPair::CIDRelevanceLevelPair(const CIDRelevanceLevelPair& inElement):
  mID(inElement.mID),
  mRelevanceLevel(inElement.mRelevanceLevel)
{
};



/***************************************
*
* getURL
*
****************************************
*
* modification history
*
* 
*
****************************************/
/* returns the URL */
TID CIDRelevanceLevelPair::getID()const{
  return mID;
};



/***************************************
*
* getRelevanceLevel
*
****************************************
*
* modification history
*
* 
*
****************************************/
/* returns only the relevance level */
double CIDRelevanceLevelPair::getRelevanceLevel()const{
  return mRelevanceLevel;
};



/***************************************
*
* setRelevanceLevel
*
****************************************
*
* modification history
*
* 
*
****************************************/
/* sets the relevance level directly */
void CIDRelevanceLevelPair::setRelevanceLevel(const double inRelevanceLevel){
  mRelevanceLevel=
    inRelevanceLevel;
};



/***************************************
*
* adjustRelevanceLevel - adds another relevance level to the current
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CIDRelevanceLevelPair::adjustRelevanceLevel(const double inRelevanceLevel){
  mRelevanceLevel+=
    inRelevanceLevel;
};



/***************************************
*
* divideRelevanceLevelBy
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CIDRelevanceLevelPair::divideRelevanceLevelBy(const double inDivisor)
{
  mRelevanceLevel/=
    inDivisor;
};

/***************************************
*
* <
*
****************************************
*
* modification history
*
* 
*
****************************************/
bool operator<(const CIDRelevanceLevelPair& l,const CIDRelevanceLevelPair& t)
{
  return l.getRelevanceLevel()<t.getRelevanceLevel();
};
