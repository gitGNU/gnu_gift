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
// -*- mode: c++ -*-
#ifndef _CIFBUILDERTRIPLET
#define _CIFBUILDERTRIPLET
#include "libGIFTAcInvertedFile/include/uses-declarations.h"
#include "libMRML/include/TID.h"

/** When building an inverted file by successive merging
    we need first to create a file containing triplets of
    FeatureID, DocumentID and DocumentFrequency

    These elements need to be sorted by FeatureID and inverse
    Document Frequency.
*/
class CIFBuilderTriplet{
public:
  /** the default constructor */
  CIFBuilderTriplet();
  /** convenience constructor */
  CIFBuilderTriplet(TID inFeatureID,
		    TID inDocumentID,
		    float inDocumentFrequency);
  /** testing if all fields are equal */
  bool isIdentical(const CIFBuilderTriplet& inTriplet)const;
  /** the feature id of this feature */
  int mFeatureID;
  /** the document id of the document containing 
      this feature */
  int mDocumentID;
  /** the frequency of the feature within the document */
  float mDocumentFrequency;
};


bool operator<(
	       const CIFBuilderTriplet& l,// less
	       const CIFBuilderTriplet& t // than
	       );

#endif
