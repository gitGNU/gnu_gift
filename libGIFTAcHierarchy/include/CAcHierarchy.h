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
#ifndef _CACHIERARCHY
#define _CACHIERARCHY
#include "libGIFTAcHierarchy/include/uses-declarations.h"
#include <list>
#include <map>
#include "libMRML/include/CAccessorImplementation.h"

class CXMLElement;
#include "libMRML/include/CIDRelevanceLevelPairList.h"
#include "libMRML/include/CMutex.h" // multithreading
/** 
    A very simple accessor for hierarchies of images 
    Stored in an XML file.

    This was intended as the ultra simple example system for the 
    browser benchmarking paper. It performed too bad in our benchmark to be taken
    seriously. 

    Currently I see the use of this as the base for systems, where the
    author knows how to generate a hierarchy of images, but the author
    fears the overhead needed for including his/her system into the gift.

    Please note that IDs must start at 1 (ONE!!!).

    @author Wolfgang Müller
*/
class CAcHierarchy:public CAccessorImplementation{
protected:
  /** mutual exclude for multithreading*/
  CMutex mMutexHierarchy;
  /** is this well constructed ?*/
  bool isOK;
  /** 
      In fact, because images can arise multiple times in the 
      hierarchy, it is necessary to store the full path 
      for each time the image arises in the tree. 

      We base this on the idea that in each selection, each image will
      arise at most once. As a consequence, lists of selected images
      <em>must</em> be sufficient to define the path from the
      hierarchy's root to a given node or leaf of the hierarchy.

  */
  typedef list<TID> CPath;
  /** This class is there for mnemonic reasons only.
      Evidently it has the same type as CPath, however
      it designates a list of children.
  */
  typedef list<TID> CChildren;
  /**
     A stack of the nodes visited when building the hierarchy
  */
  CPath mNodeStack;
  /**
     The graph is represented as a map from path to children.
     @see CPath for an explanation
   */
  typedef map<CPath,CChildren> CGraph;
  /**
     Edges: The edges from father to son.
     The edges from son to father are not needed, because it means
     simply popping from the path.
  */
  CGraph mEdges;
  /**  
       called immediately by the constructor, this function
       does the main construction work.
       Starts the xml parser for reading the hierarchy 
       file etc.
  */
  void init(string inFileName);
  /**
     adds an edge from father to son
     and an edge from son to father
  */
  void addEdge(const CPath& inFather, 
	       TID inSon);
  
public:
  /**  
       constructor opens a hierarchy file and reads it.

       Like every accessor, this accessor takes a <collection />
       MRML element as input (@see CXMLElement for how to access 
       the attributes of this element). Currently this accessor
       understands  the following attributes:

       cui-base-dir:                the base directory containing the following file:
       cui-hierarchy-file-location: an xml file containing the hierarchy that you
                                    can browse using this accessor.
  */
  CAcHierarchy(const CXMLElement& inConfiguration);
  /** 
      Gets a list of all children of an element. As it was said before,
      an element of the hierarchy is well described by a path
      in the tree, from the root to this element.
  */
  CIDRelevanceLevelPairList* getChildren(const CPath& inID)const;
  /** 
      Adds an element to the collection 
   */
  void addElement(TID inID, 
		  string inLocation, 
		  string inThumbnail);
  /**

     pushes the node onto the stack

   */
  void push(TID inNode);
  /**

     pops the node and registers the edge from
     new top to old top and back

   */
  void popNodeAndRegisterEdge();
  /**
     has this been successfully read?
  */
  operator bool()const;
};

#endif
