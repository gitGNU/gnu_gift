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
/***************************************
*
* 
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
#ifndef _CIFLISTSTART
#define _CIFLISTSTART
#include "libGIFTAcInvertedFile/include/uses-declarations.h"
#include "libMRML/include/TID.h"
#include <iostream>

/**The chunk which starts a list
  in the inverted file, consisting of
  FeatureID,
  CollectionFrequency of the Feature and
  Number of Elements in the List */
class CIFListStart{
  ///
  TID mFeatureID;
  ///
  float mCollectionFrequency;
  /** The number of elements */
  unsigned int mNumberOfElements;
  /** The total number of elements be 
      in this list before
      rearranging the file */
  unsigned int mNumberOfReservedElements;
public:
  /***************************************
   *
   * 
   *
   ***************************************/
  CIFListStart(istream& inStream);

 
  /***************************************
   *
   * the ID of the feature
   *
   ***************************************/
  TID getFeatureID()const;

  /***************************************
   *
   * The collection frequency of this
   *
   ***************************************/
  double getCollectionFrequency()const;

  /***************************************
   *
   * The number of elements in this list
   *
   ***************************************/
  unsigned int getNumberOfElements()const;
  /***************************************
   *
   * The total number of elements allowed in this list
   *
   ***************************************/
  unsigned int getNumberOfReservedElements()const;
};

#endif
