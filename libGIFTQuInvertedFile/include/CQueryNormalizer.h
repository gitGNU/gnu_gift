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
/***************************************
*
* CQUERYNORMALIZER
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
#ifndef _CQUERYNORMALIZER
#define _CQUERYNORMALIZER

#include "libGIFTAcInvertedFile/include/CDocumentFrequencyElement.h"
#include "libGIFTQuInvertedFile/include/CWeightingFunction.h"
#include "libGIFTAcInvertedFile/include/CAcInvertedFile.h"

/** For normalizing weights like in Salton/Buckley we need to now all the
  features in the query or all the features in a document. This base class is
  supposed to handle both.

  For getting additional information it needs to know the accessor.
 */

class CQueryNormalizer{

 protected:

    /***************************************
     *
     * 
     *
     ***************************************/
    ///
    double mValue;
    /***************************************
     *
     * 
     *
     ***************************************/
    ///
    const CAcInvertedFile* mAccessor;

 public:
    /***************************************
     *
     * 
     *
     ***************************************/
    ///
    CQueryNormalizer(const CAcInvertedFile* inAccessor=0);

    /***************************************
     *
     * 
     *
     ***************************************/
    ///
    void setAccessor(const CAcInvertedFile* inAccessor);

    /***************************************
     *
     * 
     *
     ***************************************/
    ///
    virtual void reset();

    /***************************************
     *
     * 
     *
     ***************************************/
    ///Translating the information into a normalizing value in the end
    virtual double getValue()const;

    /***************************************
     *
     * 
     *
     ***************************************/
    ///Considers a feature 
    virtual void considerQueryFeature(const CWeightingFunction& 
				      inFeature);
    /***************************************
     *
     * returning a copy of this
     *
     ***************************************/
    virtual CQueryNormalizer* clone()const;
};
#endif
