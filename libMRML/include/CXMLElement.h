/* -*- mode: c++ -*- 
*/
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000 CUI, University of Geneva

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
/* -*- Mode: c++ -*- */
/* the tag above is for making xemacs find the right mode for this file
   (avoids typing M-x c++-mode M-x font-lock-mode M-x font-lock-mode)
 */
#ifndef _CXMLElement
#define _CXMLElement

#include <map>
#include <list>
#include <string>
#include "CMagic.h"
#include "CAttributeList.h"
class CXMLElementVisitor;
#include "CMutex.h"
/**
   This class captures the structure of an XML element. 
   An Element has children and attributes.

   The way a Document tree is built:

   AddChildToCurrent: Adds a child, AND MAKES THE CHILD THE NEW CURRENT NODE.
   moveUp:            Makes the father the new current node.

   @author Wolfgang Müller
 */
class CXMLElement:public CMagic{
  /** a typedef for simple convenience */
  typedef list<CXMLElement*> lCChildren;
  /** the father of this*/
  CXMLElement* mFather;
  /** The child-grandchild-etc. which has just been inserted */
  CXMLElement* mCurrentChild;
protected:
  /** a list containing the children of this element*/
  lCChildren mChildren;
  /** the content: attributes */
  CAttributeList mAttributes;
  /** the name of this element */
  string mName;
  /** change the father of this */
  void setFather(CXMLElement*);

  /** 
      The text/CData contained in this element 
      FIXME: look at zero characters
   */
  string mText;
  /**
     The kind of this node:
     Stroustrup would kill me for that,
     but I think doing some inheritance here
     is not really what I want, so I do some if-ing
  */
  const int mTypeOfNode;
public:
  /**
     This constant is destined to be a value of mTypeOfNode
   */
  static int const cTextNode; 
  /**
     This constant is destined to be a value of mTypeOfNode
   */
  static int const cElementNode; 
  
public:
  /** read the father of this */
  CXMLElement* getFather()const;

    /** adding an attribute for integers using the function for strings */
  void addAttribute(const string& inAttribute,long inValue);
  /** adding an attribute for doubles using using the function for strings */
  void addAttribute(const string& inAttribute,double inValue);
  /** adding an attribute */
  void addAttribute(const string& inAttribute,const string& inValue);

  /** reading an attribute for booleans using stringReadAttribute
	it supports several pairs:
	yes/no
	true/false
	y/n
    */
  pair<bool,bool> boolReadAttribute(const string& inAttribute)const;
  /** reading an attribute for integers using stringReadAttribute */
  pair<bool,long> longReadAttribute(const string& inAttribute)const;
  /** reading an attribute for doubles using stringReadAttribute */
  pair<bool,double> doubleReadAttribute(const string& inAttribute)const;
  /** reading an attribute for strings 

	All the attribute functions implement a primitive version
	of inheritance: We look also at the parents of this, if we
	do not find a given attribute in this.

	this is virtual in order to plug in other methods of inheritance

    */
  virtual pair<bool,string> stringReadAttribute(const string& inAttribute)const;

  /** clone this element with all its content 

	@parameter inDeep if true, this function 
	clones deeply, i.e. children will be cloned,
	instead of just cloning the pointers to the children
	and the attributes.
    */
  CXMLElement* clone(bool inDeep=true)const;

  /** 
	Convert this into an XML string
    */
  void toXML(string& outString,const int=0)const;
  /** 
	Visit the subtree using a visitor
    */
  void traverse(CXMLElementVisitor& inoutVisitor)const;
    
  /** For interoperating with the STL */
  list<CXMLElement*>::const_iterator child_list_begin()const;
  /** For interoperating with the STL
     */
  list<CXMLElement*>::const_iterator child_list_end()const;
  /** For interoperating with the STL */
  list<CXMLElement*>::iterator child_list_begin();
  /** For interoperating with the STL
     */
  list<CXMLElement*>::iterator child_list_end();


  /**
       Like above. Hovewer, here we would like to add anything inherited
       from CXMLElement.
    */
  virtual void addChild(CXMLElement* inChild);
  /**
       Add a child with the given attributes
       to the current node, and MAKE 
       THIS CHILD THE NEW CURRENT NODE.
    */
  virtual void addChild(const string&,
			const char* const* const inAttributeList=0);

  /**
       make the father of the current node the new current node.
    */
  void moveUp();
  /**
       A subtree is finished if the current child is identical 
       with itself
    */
  bool isSubtreeFinished()const;

  /**
       Constructing this from a list which is built
       from what we get from expat
    */
  CXMLElement(const string&,
	      const char* const* const inAttributeList=0);
  /** delete this by first deleting the children */
  virtual ~CXMLElement();
  /**
     Constructing a text node from what expat gives us
    */
  CXMLElement(const int inType,
	      const string&);
  /** Constructing this from more c++-like structures */
  CXMLElement(const string& inString,
	      const list< pair<string,string> >& inList);
  /** Constructing this from more c++-like structures */
  CXMLElement(const string& inString,
	      const CAttributeList& inList);
  /**
       Make this a deep copy of the input
    */
  CXMLElement(const CXMLElement& in);
  /** 
	For doing flat configuration, 
	we need to find out all attributes which are visible
	in this algorithm
    */
  list<pair<string,string> >* createNamedValueList()const;
  /** get the text of this */
  string getText()const;
  /** get the name of this */
  string getName()const;
  /** get the name of this */
  int getTypeOfNode()const;
  /** get the number of attributes in this */
  int getNumberOfAttributes()const;
  /** checks consistency of attributes */
  void check()const;
  
};

#endif
