/* -*- mode: c++ -*- 
*/
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000 CUI, University of Geneva

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
* 
*
*
*
* modification history:
*
* WM 211099 filled documentation templates
*           answered some comments that were questions
* WM   0599 file created
*
* @author Wolfgang Mueller
*
* compiler defines used:
*
*
*/
#ifndef _CDISTANCEMATRIXACCESSOR
#define _CDISTANCEMATRIXACCESSOR

#include <string>
#include "TID.h"
#include "CAcURL2FTS.h"
#include <iostream.h>
#include <fstream.h>
#include <map>
#include <hash_map>

class CXMLElement;
#include "CDistanceCalculator.h"

/** In addition to basic CAccessor functionality, this 
    accessor permits the lookup of distances between elements 
    in the database. The table for this lookup the Distance
    Matrix has to have been calculated in advance, using a series
    of QBE queries
 */
class CAcDistanceMatrix:public CAcURL2FTS, 
			public CDistanceCalculator{

protected:
  
public:

  /**
   *
   * Is this accessor up and running?
   *
   */
  bool operator()()const;

  /**
   *
   * The construction
   *
   */
  CAcDistanceMatrix(const CXMLElement& inCollectionElement);
  
  /**
   *
   * Destructor 
   *
   */
  ~CAcDistanceMatrix();
};

#endif
