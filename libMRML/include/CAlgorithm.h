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

#ifndef _CALGORITHM
#define _CALGORITHM
#include "libMRML/include/uses-declarations.h"
#include "libMRML/include/CXMLElement.h"
#include "libMRML/include/CSelfDestroyPointer.h"
#include <map>

class CQuery;
class CAlgorithmCollection;
/**
   An algorithm is a CXMLElement
   which is linked to a CSessionManager
   together they are able to build CQuery trees.

   @Author: Wolfgang M&uuml;ller
*/
class CAlgorithm:public CXMLElement{
protected:
public:
  /**
     Same as in CXMLElement, but making a CAlgorithm instead
  */
  virtual void addChild(const string&,
			const char* const* const inAttributeList=0);
  /**
     Same as in CXMLElement, but making a CAlgorithm instead
  */
  virtual void addChild(CXMLElement* inChild);

  //   /** reading an attribute for strings 
  
  //       here we extend the inheritance mechanism. We do not only look at
  //       parents, but also at the default values provided by the configuration.
  
  //       this is virtual in order to plug in other methods of inheritance
  
  //       @see: CXMLElement.h#stringReadAttribute
  //    */
  //   virtual pair<bool,string> stringReadAttribute(const string& inAttribute)const;
  /** 
      In the long run, we would like to get of build query
      tree in this class and move it elsewhere.

      The first step of this separation is the configure function, 
      which does nothing else but putting the inherited values and 
      the default values into the attribute list of this. After that
      no inheritance is necessary, and we can get at all the attributes
      using CXMLElement::createNamedValueList;
      
  */
  bool configure(CAlgorithmCollection& inBaseConfiguration);
  /** merging the attributes of two algorithms.
      This is useful:
      1. for the initial configuration
      2. for changing the configuration.


      At present this merges attribute lists givig priority 
      to the values in *this.
      

      FIXME: slow way of merging
  */
  void mergeAttributes(const CAlgorithm& inAlgorithm);
  /** merging the child lists of two algorithms.
      This is useful:
      1. for the initial configuration
      2. for changing the configuration.


      At present this merges attribute lists givig priority 
      to the values in *this.
      

      FIXME: slow way of merging
  */
  void mergeChildLists(const CAlgorithm& inAlgorithm);
  /** get the name of the collection to be treated by this algorithm
   */
  string getCollectionID()const;
  /** get the name of the algorithm
   */
  double getWeight()const;
  /** get the name of the algorithm
   */
  string getAlgorithmName()const;
  /** get the type of the algorithm
   */
  pair<bool,string> getType()const;
  /** 
      The father inside the configuration
   */
  pair<bool,string> getInheritsFrom()const;
  /** get the basetype of the algorithm
   */
  string getBaseType()const;
  /** get the id of the algorithm
   */
  string getID()const;
  /** puts out a flat algorithm tag with just ID and name */
  string toOldMRML()const;
  /** set the default we do not take any 
      destroy responsability
   */
  void setDefault(CAlgorithm& inDefault);
  /** clone this */
  CAlgorithm* clone()const;
  /** hand things through to the CXMLElement constructor */
  CAlgorithm(const char* ,
	     const char* const* const inAttributeList=0);
  /** hand things through to the CXMLElement constructor */
  CAlgorithm(const string& ,
	     const list< pair<string,string> >& inList);
  /** construct CAlgorithm from XML Element */
  CAlgorithm(const CXMLElement& inAlgorithm);
  /** copy constructor */
  CAlgorithm(const CAlgorithm& inAlgorithm);
};


//----------------------------------------
//probably obsolete
//----------------------------------------
class CSortByID_CA:public binary_function<const CAlgorithm&,const CAlgorithm&,bool>{
public:
  bool operator()(const CAlgorithm& l,
		  const CAlgorithm& t)const;
};
//----------------------------------------
//probably obsolete
//----------------------------------------
class CSortByBase_CA:public binary_function<const CAlgorithm&,const CAlgorithm&,bool>{
public:
  bool operator()(const CAlgorithm& l,
		  const CAlgorithm& t)const;
};

#endif
