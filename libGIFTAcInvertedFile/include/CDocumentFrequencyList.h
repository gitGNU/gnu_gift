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
* 
*
*
*
* modification history:
* WM 19990913 
* HM 090399   created the documentation
* WM          created
*
*
*
* compiler defines used:
*
*
*/
#ifndef _CDOCUMENTFREQUENCYLIST
#define _CDOCUMENTFREQUENCYLIST
#include "libGIFTAcInvertedFile/include/CDocumentFrequencyElement.h"

//#define _CDocumentFrequencyListIsVector
//#define _CDocumentFrequencyListIsList
#define _CDocumentFrequencyListIsArray

#ifdef _CDocumentFrequencyListIsVector
#include <vector>
#else
#include <list>
#endif

/** A list of Document Frequency Elements (the main part of an inverted file) */
class CDocumentFrequencyList
#ifdef  _CDocumentFrequencyListIsVector
:public vector<CDocumentFrequencyElement>
#endif
#ifdef  _CDocumentFrequencyListIsList
:public list<CDocumentFrequencyElement>
#endif
{
  /** The (maximum) size of this */
  int mSize;
  /** The end of this */
  CDocumentFrequencyElement* mEnd;
  
#ifdef _CDocumentFrequencyListIsArray
  CDocumentFrequencyElement* mContent;
#endif
public:
  /**
   *
   * Constructor
   *
   */
  CDocumentFrequencyList(size_t inSize);
  /**
   *
   * Constructor
   *
   */
  CDocumentFrequencyList();
  /**
   *
   * Destructor
   *
   */
  ~CDocumentFrequencyList();
  /**
   *
   * A nice function for writing the whole list as binary 
   * (as opposed to human readable ascii) into a file
   * 
   */
  bool writeBinary(ostream& outStream)const;
  /**
   *
   * A nice function for reading the whole list as binary 
   * (as opposed to human readable ascii) from a file
   * 
   */
  bool readBinary(istream& inStream);
#ifdef _CDocumentFrequencyListIsArray
  /** mimicking STL iterator is simple pointer*/
  typedef CDocumentFrequencyElement* iterator;
  /** mimicking STL const_iterator is simple const pointer*/
  typedef CDocumentFrequencyElement const* const_iterator;
  /** mimicking STL */
  iterator begin();
  /** mimicking STL */
  iterator end();
  /** mimicking STL */
  const_iterator begin()const;
  /** mimicking STL */
  const_iterator end()const;
  /** mimicking STL */
  size_t size()const;
  /** to set the end of this */
  void setEnd(CDocumentFrequencyElement*);
#endif
};

#endif
