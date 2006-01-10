// -*- mode: c++ -*-
#ifndef _CXEVI18NTRANSLATOR
#define _CXEVI18NTRANSLATOR
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

#include "libMRML/include/uses-declarations.h"
#include <string> 
#include "libMRML/include/CXMLElementVisitor.h"
#include "libMRML/include/CXEVI18nTranslator.h"  
#include "libMRML/include/CI18nTranslator.h"  
/**
   
A visitor that visits XML elements in order to translate them

*/
class CXEVI18nTranslator:public CXMLElementVisitor{

protected:
  /** the language code used for translating */
  string mLanguageCode;
  /** the translator used for translating */
  const CI18nTranslator& mTranslator;
public:
  /** 
      constructor. 
  */
  CXEVI18nTranslator(const CI18nTranslator& inTranslator,
		     string inLanguageCode);
  /** actually carrying out the visit */
  virtual bool startVisit(CXMLElement& inVisited);
  /** actually carrying out the visit */
  virtual void endVisit(CXMLElement& inVisited);
  /** This methods' use is forbidden. Just there to satisfy interface. */
  virtual bool startVisit(const CXMLElement& inVisited);
  /** This methods' use is forbidden. Just there to satisfy interface. */
  virtual void endVisit(const CXMLElement& inVisited);
};
#endif
