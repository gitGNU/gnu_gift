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
#ifndef _CALGORITHMCOLLECTION
#define _CALGORITHMCOLLECTION
#include "../include/uses-declarations.h"
#include <string>
#include <map>
#include "libMRML/include/CAlgorithm.h"
#ifdef LINK_DYNAMICALLY
#include "libMRML/include/CStaticQueryFactory.h"
#else
#include "libMRML/include/CDynamicQueryFactory.h"
#endif
#include "libMRML/include/CXMLElement.h" // for toXMLElement

class CAccessorAdminCollection;


/**
  The algorithm collection is kind of a factory
  for CQuery structures:
  
  We provide a list of algorithms which correspond
  to so-called "basetypes". At present there are
  only two: bayesian and inverted_file.

  Each algorithm which will be configured in 
  the configuration file will have a basetype.
  Corresponding to this there is generated a 
  CQuery structure for each algorithm out of the
  configuration. 
  
  @Author Wolfgang Mueller
 */
class CAlgorithmCollection:public CStaticQueryFactory{
protected:
  /** as the name sais*/
  CAlgorithm* mAlgorithmInConstruction;
  /** The content of an algorithm collection is a map
     between algorihm type and a pointer to 
     the corresponding CAlgorithm */
protected:
  typedef map<string,CAlgorithm*> CContent;

  /** 
   */
  CContent mContent;
  /** for being able to read all the content */
  CContent::const_iterator begin()const;
  /** for being able to read all the content */
  CContent::const_iterator end()const;
public:
  //----------------------------------------
  /** 
      returns a pointer to the algorithm in construction
  */
  CAlgorithm* getAlgorithmInConstruction();
  //----------------------------------------
  /** 
   */
  void startAlgorithmConstruction(CAlgorithm* inAlgorithm);
  /** Constructor */
  CAlgorithmCollection(string inConfigFileName);
  /** Destructor destroys all members of CContent 
   */
  ~CAlgorithmCollection();
  /** adding a completely built algorithm XML document tree*/
  void addAlgorithm(CAlgorithm* inAlgorithm);
  /** Algorithm for TYPE => permits configuring 
      probably only the pointered version is interesting
   */
  CAlgorithm& getAlgorithmByType(const string& inType)const;
  /** Algorithm for TYPE => permits configuring 
      probably only the pointered version is interesting
   */
  bool containsAlgorithmByType(const string& inType)const;
  /** 
      Creates a new algorithm for agiven type by cloning
      what you would get with getAlgorithm.
      The values made here will be used as default
      in the subsequent CQuery building process.
  */
  CAlgorithm* makeAlgorithmByType(const string& inType)const;
  //----------------------------------------
  /** A default algorithm for the default session
      This algorithm will be given by the ONE
      ALGORITHM which has the type "adefault"
  */
  CAlgorithm* getDefaultAlgorithm()const;
  //----------------------------------------
  /** creates a copy of what you'd get with
      the above function get...*/
  CAlgorithm* makeDefaultAlgorithm()const;
  //----------------------------------------
  /** For handshaking:
      This makes you a list of all algorithms which work with a given
      query paradigm.
   */
  CXMLElement* getAlgorithmList(const CXMLElement& inGetAlgorithms)const;
  //----------------------------------------
  /** for shandshake and for state output
   */
  string toXML(bool inIsPrivate=false)const;
  /** for get-algorithms: the output is an CXMLElement, containing
      an algorithm-list element.
   */
  CXMLElement* toXMLElement()const;
  /** handed through to the CStaticQueryFactory */
  CQuery* makeQuery(const string & inBaseType, 
		    CAccessorAdminCollection & inAccessorAdminCollection,
		    CAlgorithm & inAlgorithm,
		    CStaticQueryFactory& inFactory);

};
#endif
