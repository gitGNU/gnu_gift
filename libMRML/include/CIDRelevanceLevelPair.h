/* -*- mode: c++ -*- 
*/
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

     Copyright (C) 2003, 2004 Bayreuth University
      2005 Bamberg University
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
* CIDRELEVANCELEVELPAIR
*
****************************************
*
* modification history:
*
*@Author Wolfgang Müller
*
****************************************
*
* compiler defines used:
*
*
****************************************/
#ifndef _CIDRELEVANCELEVELPAIR
#define _CIDRELEVANCELEVELPAIR
#include "libMRML/include/uses-declarations.h"
#include <iostream>
#include "libMRML/include/TID.h"
#include <functional>
#include <cmath>

/** Level of relevance given by the user to an image (URL) */
class CIDRelevanceLevelPair{
  /** / */
  int mID;
  /** / */
  double mRelevanceLevel;
public:
/***************************************
*
* 
*
***************************************/
  /**@name Constructors*/
  /** @{ */
  /** / */
  CIDRelevanceLevelPair();

/***************************************
*
* 
*
***************************************/
  /** / */
  CIDRelevanceLevelPair(const TID inID,
			double inRelevanceLevel);

/***************************************
*
* 
*
***************************************/
  /** / */
  CIDRelevanceLevelPair(const CIDRelevanceLevelPair& inElement);
  /** @} */

/***************************************
*
* 
*
***************************************/
  /**@name Accessors*/
  /** @{ */
  TID getID()const;

/***************************************
*
* 
*
***************************************/
  /** / */
  double getRelevanceLevel()const;

/***************************************
*
* 
*
***************************************/
  /** / */
  void setRelevanceLevel(const double inRelevanceLevel);

/***************************************
*
* 
*
***************************************/
  /** / */
  void adjustRelevanceLevel(const double inRelevanceLevel);

/***************************************
*
* 
*
***************************************/
  /** / */
  void divideRelevanceLevelBy(const double inDivisor);
  /** @} */
};

class CSortByID_IRLP:
  public binary_function<CIDRelevanceLevelPair,CIDRelevanceLevelPair,bool>{
public:
  /**  */
  inline bool operator()(const CIDRelevanceLevelPair& l,
			 const CIDRelevanceLevelPair& t){
    return 
      fabs(double(l.getID()))
      <
      fabs(double(t.getID()));
  };
};



/***************************************
*
* 
*
***************************************/
bool operator<(const CIDRelevanceLevelPair& l,
	       const CIDRelevanceLevelPair& t);

#endif
