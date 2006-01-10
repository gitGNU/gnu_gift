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
/**
*
* CDOCUMENTFREQUENCYELEMENT
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
#ifndef _CDOCUMENTFREQUENCYELEMENT
#define _CDOCUMENTFREQUENCYELEMENT
#include "libGIFTAcInvertedFile/include/uses-declarations.h"
#include <iostream>
#include <functional>
#include "libMRML/include/TID.h"

/** A pair of Document Frequency ID: Can be used for FeatureIDs and DocumentIDs */
class CDocumentFrequencyElement{
  struct{
    /** The ID of the document or featue */
    TID mID;
    /** The document frequency of the feature */
    float mDocumentFrequency;
  } mContent;
public:
  /**
   *
   *  @name Constructors
   *
   */
  //@{
  /** */
  CDocumentFrequencyElement();

  /**
   *
   * constructor
   * fills mContent with inID,inDocumentFrequency
   *
   */

  CDocumentFrequencyElement(TID inID,
			    float inDocumentFrequency);

  /**
   *
   * 
   *
   */
  CDocumentFrequencyElement(istream& inStream);

  /**
   *
   * 
   *
   */
  CDocumentFrequencyElement(const CDocumentFrequencyElement& inElement);

  /**
   *
   * 
   *
   */
  //@}

  /**@name input/output*/
  //@{

  /**  */
  bool input(istream& inStream);
  
  /**
   *
   * 
   *
   */
  bool output(ostream& outStream)const;

  /**
   *
   * 
   *
   */
  bool writeBinary(ostream& outStream)const;

  //@}

  /**@name Accessors*/
  //@{
  /**
   *
   * 
   *
   */
  TID getID()const;

  /**
   *
   * 
   *
   */
  unsigned int getIntegerDocumentFrequency()const;

  /**
   *
   * 
   *
   */
  float getDocumentFrequency()const;

  /**
   *
   * 
   *
   */
  void setDocumentFrequency(float);

  /**
   *
   * 
   *
   */
  void divideDocumentFrequency(float);

  //@}

};


/**
 *
 * 
 * CDocumentFrequencyElement:Sorting function class for sorting by ID  
 *
 */

 
class CSortByID_DFE:
  public binary_function
<CDocumentFrequencyElement,CDocumentFrequencyElement,bool>{
 public:
  /** Inlined for speed */
  inline bool operator()(const CDocumentFrequencyElement& l,
			 const CDocumentFrequencyElement& t){
    return l.getID()<t.getID();
  }
};



/**
 *
 * 
 * CDocumentFrequencyElement:ASCENDING Sort for the document frequency 
 *
 */

class CSortByDocumentFrequency_DFE:
  public binary_function
<CDocumentFrequencyElement,CDocumentFrequencyElement,bool>{
public:
  /**  */
  inline bool operator()(const CDocumentFrequencyElement& l,
			 const CDocumentFrequencyElement& t){
    return t.getDocumentFrequency()<l.getDocumentFrequency();
  }
};

#endif


