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
// -*- mode: c++ -*-
/**
*
* CInvertedFileChunk
*
*
*
* modification history:
*
* HM 090399 created the documentation
*
*
*
* compiler defines used:
*
*
*/
#ifndef _CINVERTEDFILECHUNK
#define _CINVERTEDFILECHUNK
#include "libGIFTAcInvertedFile/include/uses-declarations.h"
#include <iostream>
#include <fstream>
#include "libMRML/include/TID.h"
#include <map>
#include "libGIFTAcInvertedFile/include/CDocumentFrequencyList.h"


/**  */
class CMapInvertedFileChunk:public map<TID,double>{};


/** A class which does all the necessary administrative
    work for generating inverted files. It contains the list of 
    documents which contain a given feature */
class CInvertedFileChunk:public CMapInvertedFileChunk{

public:

/**
 *
 * returns the percentage of documents which contain this one feature 
 *
 */
  
  double getCollectionFrequency(size_t inSize)const;

/**
 *
 * add one document of to the list for one feature 
 *
 */
  void CInvertedFileChunk::addElement(TID inDocumentID,
				      double inDocumentFrequency);

/**
 * 
 *  writes the binary part of the list for one feature 
 *
 */
  bool writeBinary(ostream&,
		   TID inFeatureID,
		   size_t inSize)const;
};

#endif
