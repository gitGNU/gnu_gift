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
* CADDITIONALDOCUMENTINFORMATION
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
#ifndef _CADDITIONALDOCUMENTINFORMATION
#define _CADDITIONALDOCUMENTINFORMATION
#include "libGIFTAcInvertedFile/include/uses-declarations.h"
#include "libMRML/include/TID.h"
#include <string>
#include <functional>
#include <iostream>
#include <fstream>
/** 
  Additional document information (ADI) is information
  is needed for weighting schemes which
  take into account properties of whole documents.
  Things like e.g. the euclidean length of a vector have to be
  calculated beforehand.
  */
class CAdditionalDocumentInformation{
protected:
  ///Filename of the document (from which the ADI file will be built)
  string mFileNameBase;
  ///Maximum Document frequency of a feature for the whole document
  double mMaximumDF;
  ///Sum of squared document frequencies off all features of the document.
  double mDFSquareSum;
  ///Sum of (DF*DF*log(ICF)) for all features of the document
  double mSquareDFLogICFSum;

public:
/***************************************
*
* 
*
***************************************/
  ///Constructor sets filename
  CAdditionalDocumentInformation(const string& inName="N.N.");
  
/***************************************
*
* 
*
***************************************/
  ///...If neccesary set filename later
  void setFileNameBase(const string& inName);

/***************************************
*
* 
*
***************************************/
  ///Reset mMaximumDF and mDFSquareSum
  void resetDF();

/***************************************
*
* 
*
***************************************/
  ///Add the right things to mMaximumDF and mDFSquareSum
  void adjustDF(double inDF);

/***************************************
*
* 
*
***************************************/
  ///reset mSquareDFLogICFSum
  void resetSquareDFLogICF();

/***************************************
*
* 
*
***************************************/
  ///add the right things to mSquareDFLogICFSum
  void adjustSquareDFLogICF(double);

/***************************************
*
* 
*
***************************************/
  ///@name Accessors
  //@{
  double getMaximumDF()const;

/***************************************
*
* 
*
***************************************/
  ///
  double getDFSquareSum()const;

/***************************************
*
* 
*
***************************************/
  ///
  double getSquareDFLogICFSum()const;

/***************************************
*
* 
*
***************************************/
  //@}
  //@name input/output to streams
  //@{
  ///
  bool output(ostream& outStream)const;

/***************************************
*
* 
*
***************************************/
  ///
  bool output()const;

/***************************************
*
* 
*
***************************************/
  ///
  bool input(istream& inStream);

/***************************************
*
* 
*
***************************************/
  ///
  bool input();

  //@}
};


#endif
