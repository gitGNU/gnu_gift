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
// -*- mode: c++ -*-
#ifndef _CQPERL
#define _CQPERL

#ifdef __GIFT_WITH_PERL__

#ifdef bool
#undef bool
#define _bool char
#endif

#include <EXTERN.h>
#include <perl.h>
#undef list // perl lists

#include <list>
#include "libMRML/include/CQuery.h"


/** Linking GIFT to Perl.
    The goal is to hand through about all necessary 
    communication and configuration data to Perl.
*/
class CQPerl:public CQuery{
 protected:
  /**
     The package containing the script
   */
  string mPackage;
  /**
     The scalar value which is going to hold the 
     object reference of the perl object contained by this
  */
  SV* mPerlQueryProcessor;
  /** qualify a function name by the package in which 
      the function resides */
  string qualifyFunctionName(const char* inName);
  /** The perl interpreter used by *this */
  static PerlInterpreter* mPerl; 

  /**
     Transforms a CXMLElement to an CXMLTripletList,
     and finally pushes this list onto the Perl stack.

     cleans up behind itself
   */
  CXMLElement* callPerl(const CXMLElement* inElement,
			string inFunction);
  /**
     calls "new" on the cui-perl-package
   */
  void callConstruct();

 public:
  /**
   *
   *
   *
   */
  CQPerl(CAccessorAdminCollection& inAccessorAdminCollection,
	 CAlgorithm&          inAlgorithm);
  /**
   *
   *
   *
   */
  CQPerl();
  /**
   *
   * destroying the perl
   *
   */
  ~CQPerl();
  /**
   *
   * do a query
   *
   */
  virtual CXMLElement* query(const CXMLElement& inQuery);
  
  /**
   *
   * @short a query which returns ID/RelevanceLevel pairs instead of
   * instead of URL/RelevanceLevel pairs
   *
   */
  virtual CIDRelevanceLevelPairList* fastQuery(const CXMLElement& inQuery,
					       int inNumberOfInterestingImages,
					       double inDifferenceToBest);

  /**
   *
   * get some random images (handed through to accessor)
   *
   */
  virtual CXMLElement* getRandomImages(int inNumberOfInterestingImages)const;
  /**
   *
   * get some random images (handed through to accessor)
   *
   */
  virtual CIDRelevanceLevelPairList* getRandomIDs(int inNumberOfInterestingImages)const;
  /**
   *
   * get the IDs of all images (handed through to accessor)
   *
   */
  virtual list<TID>* getAllIDs()const;
  /**
   *
   * get the IDs of all images (handed through to accessor)
   *
   */
  virtual list<CAccessorElement>* getAllAccessorElements()const;

  /**
   *
   * set the algorithm and configure this
   *
   */
  bool setAlgorithm(CAlgorithm& inAlgorithm);


  /**
   *
   * an initialising function called by the constructors.
   * will we ever need it?
   *
   */
  void init();
};


#endif

#endif
