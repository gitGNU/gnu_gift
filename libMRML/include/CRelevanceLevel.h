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
/**
*
* CRELEVANCELEVEL
*
*
*
* modification history:
* WM   1099 changed documentation fromat 
* HM 090399 created the documentation
* WM   1098 created the file
*
*
* compiler defines used:
*
* @author Wolfgang Mueller
*
*/
#ifndef _CRELEVANCELEVEL
#define _CRELEVANCELEVEL
#include "../include/uses-declarations.h"
#include <iostream.h>
#include <string>
#include "libMRML/include/TID.h"
#include <functional>

/** 
 * Level of relevance given by the user to an image (URL) 
 * This structure is used both for user given relevances as 
 * well as for calculated relevances (scores).
 *
 * What do you think, folx? I think, that the relevance level
 * as it is, is too inflexible. We need to reflect a bit more
 * what is given by MRML. to be FIXME d
 *
 */
class CRelevanceLevel{
  /**  */
  string mURL;
  /**  */
  double mRelevanceLevel;
public:
  /**
   *
   * just used to be able to construct emtpy elements in 
   * which to copy (STL)
   *
   */
  CRelevanceLevel();
  
  /**
   *
   * The constructor most frequently used.
   *
   */
  CRelevanceLevel(const string& inURL,
		  double inRelevanceLevel);
  
  /**
   *
   * copy constructor
   *
   */
  CRelevanceLevel(const CRelevanceLevel& inElement);
  
  /**
   *
   *
   * binary input
   *
   */
  bool input(istream& inStream);
  
  /**
   *
   * binary output
   *
   */
  bool output(ostream& outStream)const;
  
  /**
   *
   * output as HTML
   *
   */
  bool outputHTML(ostream& outStream)const;
  
  //@}
  
  /**
   *
   * Accessors
   *
   */
  string getURL()const;
  
  /**
   *
   * 
   *
   */
  double getRelevanceLevel()const;
  
  /**
   *
   * 
   *
   */
  void setRelevanceLevel(const double inRelevanceLevel);
  
  /**
   *
   * 
   *
   */
  void adjustRelevanceLevel(const double inRelevanceLevel);
  
  /**
   *
   * 
   *
   */
  void divideRelevanceLevelBy(const double inDivisor);
  /**  */
  string toXML()const;
};

/**
* Default less operator for the CRelevanceLevel class.
*  Sorts by mRelevancelevel. For sorting in the STL.
*
*/
bool operator<(const CRelevanceLevel& l,
	       const CRelevanceLevel& t);

/** sort CAccessorElements by their ID */
class CSortByURL_RL:public binary_function<CRelevanceLevel,CRelevanceLevel,bool>{
 public:
  /**                                          */
  bool operator()(const CRelevanceLevel& l,
		  const CRelevanceLevel& t)const;
};


#endif
