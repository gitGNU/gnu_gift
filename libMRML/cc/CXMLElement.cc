// -*- mode: c++ -*- 
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
#include "libMRML/include/my_throw.h"
#include "libMRML/include/CXMLElement.h"
#include "libMRML/include/CXMLElementVisitor.h"
#include <cstdio>


/** change the father of this*/
void CXMLElement::setFather(CXMLElement* inFather){
    mFather=inFather;
};

/** change the father of this*/
CXMLElement* CXMLElement::getFather()const{
    return mFather;
};

/** get the name of this*/
int CXMLElement::getTypeOfNode()const{
    return mTypeOfNode;
};

/** get the text contained in this*/
string CXMLElement::getText()const{
    return mText;
};
/** get the name contained in this*/
string CXMLElement::getName()const{
    return mName;
};

/** adding an attribute for integers */
void CXMLElement::addAttribute(const string& inAttribute,long inValue){
    mAttributes.add(inAttribute,
		    inValue);
};
/** adding an attribute for doubles */
void CXMLElement::addAttribute(const string& inAttribute,double inValue){
    mAttributes.add(inAttribute,
		    inValue);
};
/** adding an attribute for strings */
void CXMLElement::addAttribute(const string& inAttribute,const string& inValue){
//     cout << mName
// 	 << " adding "
// 	 << inAttribute
// 	 << ","
// 	 << inValue
// 	 << endl;

    mAttributes.add(inAttribute,
		    inValue);
};

/** reading an attribute for integers */
pair<bool,bool> CXMLElement::boolReadAttribute(const string& inAttribute)const{
    return mAttributes.boolReadAttribute(inAttribute);
};
/** reading an attribute for integers */
pair<bool,long> CXMLElement::longReadAttribute(const string& inAttribute)const{
    return mAttributes.longReadAttribute(inAttribute);
};
/** reading an attribute for doubles */
pair<bool,double> CXMLElement::doubleReadAttribute(const string& inAttribute)const{
    return mAttributes.doubleReadAttribute(inAttribute);
};
/** reading an attribute for strings */
pair<bool,string> CXMLElement::stringReadAttribute(const string& inAttribute)const{
    return mAttributes.stringReadAttribute(inAttribute);
};

/** clone this element with all its content */
CXMLElement* CXMLElement::clone(bool inDeep)const{
    CXMLElement* lReturnValue=new CXMLElement(mName,mAttributes);

    if(!lReturnValue){
	my_throw("shit");
    }


//     copy(mAttributes.begin(),
// 	 mAttributes.end(),
// 	 inserter(lReturnValue->mAttributes,
// 		  lReturnValue->mAttributes.begin()));

    

    if(inDeep){
	for(lCChildren::const_iterator i=mChildren.begin();
	    i!=mChildren.end();
	    i++){
	    lReturnValue->mChildren.push_back((*i)->clone(inDeep));
	}
    }else{
	for(lCChildren::const_iterator i=mChildren.begin();
	    i!=mChildren.end();
	    i++){
	    lReturnValue->mChildren.push_back(*i);
	}
    }
    return lReturnValue;
};
  
/** 
    Convert this into an XML string
*/
void CXMLElement::toXML(string& outString,
			const int inNiveau=0)const{
#ifdef _MOVEDEBUG
    cout << "THIS" << this << endl;
#endif

    if(mTypeOfNode==cElementNode){
      for(int i=0;
	  i<inNiveau;
	  i++){
	outString+="    ";
      }
      outString+=string("<"
			+mName
			+" "
			);
      mAttributes.toXML(outString);
      //if there are children, visit them
      if(mChildren.size()){
	outString+=">\n";
	for(lCChildren::const_iterator i=mChildren.begin();
	    i!=mChildren.end();
	    i++){
	  (*i)->toXML(outString,inNiveau+1);
	}
	//...and then close the tag.
	for(int i=0;
	    i<inNiveau;
	    i++){
	    outString+="    ";
	}
	outString+=string("</")+mName+string(">\n");
      }else{
	//if there are none, this will be an empty tag
	outString+="/>\n";
      }
    }else{
      if(mTypeOfNode==cTextNode){
	outString+="<!--here was a text node-->";
	outString+=mText;
      }
    }
}
/** visit this using a visitor */
void CXMLElement::traverse(CXMLElementVisitor& inoutVisitor)const{
    //if there are children, visit them
    if(inoutVisitor.startVisit(*this)){
	if(mChildren.size()){
	    for(lCChildren::const_iterator i=mChildren.begin();
		i!=mChildren.end();
		i++){
		(*i)->traverse(inoutVisitor);
	    }
	}
	inoutVisitor.endVisit(*this);
    }
};

/** get the first of the list of children */
list<CXMLElement*>::const_iterator CXMLElement::child_list_begin()const{
    return mChildren.begin();
};  

/** get the first of the list of children */
list<CXMLElement*>::const_iterator CXMLElement::child_list_end()const{
    return mChildren.end();
};  
/** get the first of the list of children */
list<CXMLElement*>::iterator CXMLElement::child_list_begin(){
    return mChildren.begin();
};  

/** get the first of the list of children */
list<CXMLElement*>::iterator CXMLElement::child_list_end(){
    return mChildren.end();
};  

/**
   Add a child with the given attributes
   to the current node, and MAKE 
   THIS CHILD THE NEW CURRENT NODE.
*/
void CXMLElement::addChild(const string& inName,
			   const char* const* const inAttributeList=0){
    //initialize new child
    addChild(new CXMLElement(inName,
			     inAttributeList));
}
/** */
void CXMLElement::addChild(CXMLElement* inXMLElement){
    /* if the assertion is zero,
       the this element is finished*/
    assert(mCurrentChild);

    if(!mCurrentChild->mChildren.size()
       || (inXMLElement->mTypeOfNode==cElementNode)
       || ((mCurrentChild->mChildren.size())
	   && (mCurrentChild->mChildren.back()->mTypeOfNode==cElementNode))){
      
      // either first node, or neighbour of a non-text node
      
      //eliminate whitespace-only nodes
      if(mCurrentChild->mChildren.size()){
	if(mCurrentChild->mChildren.back()->mTypeOfNode==cTextNode){
	  bool lWhitespaceOnly(true);
	  for(int i=0;
	      i<mCurrentChild->mChildren.back()->mText.size();
	      i++){

	    
	    if((mCurrentChild->mChildren.back()->mText[i]!=' ')
	       && (mCurrentChild->mChildren.back()->mText[i]!='\n')
	       && (mCurrentChild->mChildren.back()->mText[i]!='\t')){
	      lWhitespaceOnly=false;
	    }
	  }
	  if(lWhitespaceOnly){
	    delete mCurrentChild->mChildren.back();
	    mCurrentChild->mChildren.pop_back();
	  }
	}
      }



	mCurrentChild->mChildren.push_back(inXMLElement);
	inXMLElement->setFather(mCurrentChild);
    }else{
	//text node, unite with previous text nodes, if possible
	if((mCurrentChild->mChildren.size())
	   && (mCurrentChild->mChildren.back()->mTypeOfNode==cTextNode)){
	    mCurrentChild->mChildren.back()->mText+=inXMLElement->mText;
	    delete inXMLElement;
	}
    }
    
    // move down
#ifdef _MOVEDEBUG
    cout << "movingDown " 
	 << mCurrentChild 
	 << "->" 
	 << inXMLElement 
	 << endl;
#endif
    mCurrentChild=mCurrentChild->mChildren.back();
};

/**
   make the father of the current node the new current node.
  */
void CXMLElement::moveUp(){
#ifdef _MOVEDEBUG
    cout << "movingUp   " 
	 << mCurrentChild 
	 << "->" 
	 << mCurrentChild->getFather() 
	 <<endl;
#endif
    mCurrentChild=mCurrentChild->getFather();
};

/** 
    construct this from what we get from expat 
    
    Special:
    mCurrentChild=this for unifying the 
    building up of this element. So this element 
    can function as simply an element, but also 
    as a DOM-builder.
*/
CXMLElement::CXMLElement(const string& inName,
			 const char* const * const 
			 inAttributes):
    mAttributes(inAttributes),
    mText(""),
    mName(inName),
    mTypeOfNode(CXMLElement::cElementNode)
{
    mFather=0;
    mCurrentChild=this;
}
/** destroy this and all its children */
CXMLElement::~CXMLElement(){
  for(lCChildren::iterator i=mChildren.begin();
      i!=mChildren.end();
      i++){
    delete *i;
  };
}

/** Constructing this from more c++-like structures */
CXMLElement::CXMLElement(const string& inName,
			 const list< pair<string,string> >& inAttributes):
    mFather(0),
    mText(""),
    mName(inName),
    mAttributes(inAttributes),
    mTypeOfNode(CXMLElement::cElementNode){
  
};
/** Constructing this from more c++-like structures */
CXMLElement::CXMLElement(const string& inName,
			 const CAttributeList& inAttributes):
    mFather(0),
    mText(""),
    mName(inName),
    mAttributes(inAttributes),
    mTypeOfNode(CXMLElement::cElementNode){
  
};
/** constructing */
CXMLElement::CXMLElement(int inType,
			 const string& inText):
  mFather(0),
  mName(""),
  mText(inText),
  mAttributes(0),
  mTypeOfNode(inType){
  
};


/** 
    a deep copy of this
*/
CXMLElement::CXMLElement(const CXMLElement& in):
    mAttributes(in.mAttributes),
    mName(in.mName),
    mText(in.mText),
    mTypeOfNode(in.mTypeOfNode){
    mFather=0;
    mCurrentChild=this;

    cout << "checking before making children " << endl;
    check();
    cout << "making children " << endl;

    for(lCChildren::const_iterator i=in.mChildren.begin();
	i!=in.mChildren.end();
	i++){
	mChildren.push_back(new CXMLElement(**i));
	mChildren.back()->setFather(this);
    }
}

bool CXMLElement::isSubtreeFinished()const{
    return !mCurrentChild;
    //    || (mCurrentChild==this);

    // we have moved up beyond the root of this tree, so
    // this element is finished
}

/** For doing flat configuration, 
    we need to find out all attributes which are visible
    in this algorithm
*/
list<pair<string,string> >* CXMLElement::createNamedValueList()const{

    list<pair<string,string> >* lReturnValue=new list<pair<string,string> >();
    mAttributes.check();

    if(lReturnValue)
	for(CAttributeList::const_iterator i=mAttributes.begin();
	    i!=mAttributes.end();
	    i++){
#ifdef SINGLE
	    lReturnValue->push_back(*i);
#else
	    lReturnValue->push_back(make_pair(i->first,string(i->second.first)));
#endif
	}
  
    return lReturnValue;
};
/** get the number of attributes in this */
int CXMLElement::getNumberOfAttributes()const{
    return mAttributes.size();
};
/**
   This constant is destined to be a value of mTypeOfNode
*/
int const CXMLElement::cTextNode=1; 
/**
   This constant is destined to be a value of mTypeOfNode
*/
int const CXMLElement::cElementNode=2; 
void CXMLElement::check()const{
  mAttributes.check();
  for(lCChildren::const_iterator i=mChildren.begin();
      i!=mChildren.end();
      i++){
    cout << "+" << flush;
    (*i)
->check();
    cout << "/" << flush;
  }
}
