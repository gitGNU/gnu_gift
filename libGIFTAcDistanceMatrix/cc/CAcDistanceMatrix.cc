// -*- mode: c++ -*- 
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
#include "mrml_const.h" // for parsing
#include "CXMLElement.h" // constructor
#include "CAcDistanceMatrix.h"

/***************************************
 *
 * is this structure working?
 *
 ***************************************/
bool CAcDistanceMatrix::operator()()const{
  return 
    bool((const CAcURL2FTS&)(*this)) 
    &&
    bool((const CDistanceCalculator&)(*this));
};

/***************************************
 *
 * Initialization
 *
 ***************************************/
CAcDistanceMatrix::CAcDistanceMatrix(const CXMLElement& inCollectionElement):
  CAcURL2FTS(inCollectionElement),
  CDistanceCalculator((inCollectionElement.stringReadAttribute(mrml_const::cui_base_dir).second
		      +inCollectionElement.stringReadAttribute(mrml_const::cui_distance_matrix_location).second).c_str()){
  
};
  
/***************************************
 *
 * Destructor 
 *
 ***************************************/
CAcDistanceMatrix::~CAcDistanceMatrix(){
  
};

