/* -*- mode: c++ -*- 
*/
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
#ifndef _CBITSETCOLLECTION
#define _CBITSETCOLLECTION
#include "libGIFTAcInvertedFile/include/uses-declarations.h"
*
* CBitSetSelection
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

#include <list>
#include "libMRML/include/TID.h"
#include "CBitVector.h"

/** 
  A class which is supposed to contain 
  the results of a comparison
  in the PicHunter sense of the word.

  In the present implementation each comparison result
  is represented by a bit vector. Each of these bitvectors
  contains the IDs of the documents which are consistent 
  with a comparison, i.e. which are after this comparison still
  considered to be a possible target.

  This class is now considered to 
  */
class CBitSetCollection:public list<CBitVector>{
  ///
  CBitVector mPresentlyConsistentSet;
  /**
    The set of all elements used up to now this is necessary for 
    inverting the bitset, because otherwise we do not have any
    information about which bits <em>could</em> have been set 
    */
  CBitVector mSetOfUsedElements;
public:
/***************************************
*
* 
*
***************************************/
  ///Adding a comparison results: push_back and consistify()
  int addResult(const CBitVector& inResult);
  /**
    The central function: 
    Consider the case where comparison results are inconsistent:
    It would mean that you have to drop some of your comparison results 
    and/or to modify the metric.
    
    Metric modifying comes later in the game, here we concentrate on dropping 
    the right comparison results.

    If you have to drop comparisons you would like to drop the older ones,
    because the newer are fresher, if the user has decided to move his/her
    target, the newer comparis will account for that.

    To cut a long story short: consistify will drop the comparison results
    which are considered as reasons for the inconsistence. The result
    is the number of comparisons which have been dropped.
    */
/***************************************
*
* 
*
***************************************/
  int consistify();

  ///giving back the set of all images presently consistent with the comparisons
  CBitVector const& getPresentlyConsistentSet()const;
  ///giving back the set of all images presently consistent with the comparisons
  void setPresentlyConsistentSet(const list<long>& inIDList);
};

#endif


