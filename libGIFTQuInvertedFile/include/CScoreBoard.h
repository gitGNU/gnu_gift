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
#ifndef _SCOREBOARD
#define _SCOREBOARD

#include <functional>
#include <map>
#ifdef HAS_HASH_MAP
#include <hash_map>
#else
#define hash_map map
#endif
#include <iostream.h>
#include "libMRML/include/CRelevanceLevel.h"
#include "libMRML/include/CRelevanceLevelList.h"
#include "libGIFTQuInvertedFile/include/CWeightingFunction.h"
#include "libGIFTAcInvertedFile/include/CDocumentFrequencyElement.h"
class CAcInvertedFile;

/**
 *
 * @name SCOREBOARD
 *
 * For counting the scores when processing the query
 * @author Wolfgang Mueller, changes for pruning Henning Mueller
 *
 *
 * modification history:
 * WM   1099 filled the lacking documentation
 * HM 090399 created the documentation 
 * WM   1199 created the file
 *
 *
* compiler defines used:
*
*
*/
class CScoreBoard:public hash_map<TID,double>,public binary_function<CWeightingFunction,CDocumentFrequencyElement,void>{
  /* this variable blocks the adding of new documents to the scoreboard */
  bool mIgnoreNewValues;

public:
  /**
   *
   * needs an accessor for initialization 
   *
   */
  CScoreBoard();

  /**
   *
   * 
   *
   */
  void operator()(const CWeightingFunction&,
		  const CDocumentFrequencyElement&);

  /**
   *
   * prints out the Scoreboard on the screen
   *
   */
  void output(ostream&,const CAccessor&)const;

  /**
   *
   * sets the variable to avoid adding new images to the Scoreboard
   *
   */
  void setIgnore();

  /**
   *
   *  allows the adding of new members to the scoreboard again
   *
   */
  void releaseIgnore();

  /**
   *
   * limits the scoreboard to the best n results
   *
   */
  void limitNumberTo(int maximumNumber);

};


#endif
