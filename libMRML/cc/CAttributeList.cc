// -*- mode: c++ -*- 
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
#include "CAttributeList.h"
#include "string.h"
#include <cstdio>
CAttributeList::CAttributeList(const char * const * const inAttributeList){
  clear();
  if(inAttributeList){
    const char*const*  lAttributes(inAttributeList);
    while(lAttributes[0] && strlen(lAttributes[0])){
#ifndef _IS_SEQUENCE_AL
      if(lAttributes[1]){
	//	cout << "[L" << strlen(lAttributes[1]) << "L]" << flush;
	if(strlen(lAttributes[1])){
	  
	  int lLen(strlen(lAttributes[1])+1);
	  char* lBuffer(new char[lLen]);
	  strcpy(lBuffer,lAttributes[1]);

	  insert(pair<string,char*>(lAttributes[0],
				    lBuffer));
	  
	  //cout << "[" << lBuffer << "]" << flush;
	  assert(strlen(operator[](lAttributes[0]))==lLen-1);
	}else{
	  char* lBuffer(new char[1]);
	  lBuffer[0]=char(0);
	  insert(make_pair(string(lAttributes[0]),
			   lBuffer));
	}
      }
#else
      push_back(make_pair(string(lAttributes[0]),
			  string((lAttributes[1] && strlen(lAttributes[1]))?lAttributes[1]:"")));
#endif
      lAttributes+=2;
    }
  }
}
CAttributeList::~CAttributeList(){
  for(iterator i=begin();
      i!=end();
      i++){
    delete[] i->second;
  }
}

CAttributeList::CAttributeList(const list<pair <string,string> >& inAttributes){
  clear();
  for(list<pair <string,string> >::const_iterator i=inAttributes.begin();
      i!=inAttributes.end();
      i++){
#ifndef _IS_SEQUENCE_AL
    int lLen(i->second.size()+1);
    char* lBuffer(new char[lLen]);
    strcpy(lBuffer,i->second.c_str());
    insert(make_pair(i->first,lBuffer));
    
    //    insert(*i); if the representation is map<string,string>
#else
    push_back(*i);
#endif
  }  
}
CAttributeList::CAttributeList(const CAttributeList& inAttributes){
  clear();
  for(CAttributeList::const_iterator i=inAttributes.begin();
      i!=inAttributes.end();
      i++){
#ifndef _IS_SEQUENCE_AL
    int lLen(strlen(i->second)+1);
    char* lBuffer(new char[lLen]);
    strcpy(lBuffer,i->second);
    insert(make_pair(i->first,lBuffer));
#else
    push_back(*i);
#endif
  }  
}
CAttributeList::const_iterator CAttributeList::find(string inString)const{
#ifdef _PRINTOUTS_AL
  cout << endl
       << "CAttributeList::find("
       << flush;
  cout << inString
       << ") size " << flush;
  cout << size()
       << " "
       << inString.size()
       << endl;
#endif  

#ifndef _CAL_FIND_WORKAROUND
  return map<string,char*>::find(inString);
#else
  for(const_iterator i=begin();
      i!=end();
      i++){
#ifdef _PRINTOUTS_AL
    cout << "~" << flush;
    cout << "[" << flush;
    cout << i->first <<flush;
    cout << "," << flush;
    cout << i->second << flush;
    cout << "]" << flush;
#endif

    if(i->first==inString){
#ifdef _PRINTOUTS_AL
      cout << "+" << flush;
#endif
      return i;
    }
#ifdef _PRINTOUTS_AL
    cout << "." << flush;
#endif
  }
  cout << "-" << flush;
  return end();
#endif
}
void CAttributeList::add(const string& inFirst,
			 const string& inSecond){


#ifdef _PRINTOUTS_AL
  checkNPrint();
  cout << "Adding to CAttributeList "
       << inFirst
       << " "
       << inSecond
       << endl;
#endif

#ifndef _IS_SEQUENCE_AL
  char* lBuffer=new char[inSecond.size()+1];
  lBuffer[inSecond.size()]=char(0);
  strncpy(lBuffer,inSecond.c_str(),inSecond.size());
  operator[](inFirst)=lBuffer;
  //  (*this).insert(make_pair(inFirst,inSecond));
#else
  (*this).push_back(make_pair(inFirst,inSecond));
#endif
}
/** adding an attribute for integers */
void CAttributeList::add(const string& inAttribute,long inValue){
  char lBuffer[20];
  for(int i=0;
      i<20;
      i++){
    lBuffer[i]=0;
  }
  sprintf(lBuffer,
	  "%ld",
	  inValue);
  add(inAttribute,
      lBuffer);
};
/** adding an attribute for doubles */
void CAttributeList::add(const string& inAttribute,double inValue){
  char lBuffer[20];

  for(int i=0;
      i<20;
      i++){
    lBuffer[i]=0;
  }
  sprintf(lBuffer,
	  "%lf",
	  inValue);
  add(inAttribute,
      lBuffer);
};

/** reading an attribute for integers */
pair<bool,bool> CAttributeList::boolReadAttribute(const string& inAttribute)const{
  pair<bool,string> lReadAttribute=stringReadAttribute(inAttribute);

  if(!lReadAttribute.first){
    return make_pair(bool(0),bool(0));
  }else{
    char* lLastCorrectCharacter=0;

    bool lReturnValue=((lReadAttribute.second=="yes")
		       ||
		       (lReadAttribute.second=="y")
		       ||
		       (lReadAttribute.second=="true")
		       ||
		       (lReadAttribute.second=="t"));
    return make_pair(lReadAttribute.first,
		     lReturnValue);
  }
};
/** reading an attribute for integers */
pair<bool,long> CAttributeList::longReadAttribute(const string& inAttribute)const{
  pair<bool,string> lReadAttribute=stringReadAttribute(inAttribute);

  if(!lReadAttribute.first){
    return make_pair(bool(0),long(0));
  }else{
    char* lLastCorrectCharacter=0;

    long lReturnValue=strtol(lReadAttribute.second.c_str(),
			     &lLastCorrectCharacter,
			     10);
    bool lIsValid=!(*lLastCorrectCharacter);
    
    return make_pair(lIsValid,
		     lReturnValue);
  }
};
/** reading an attribute for doubles */
pair<bool,double> CAttributeList::doubleReadAttribute(const string& inAttribute)const{
#ifdef __D_CATTRIBUTELIST__DOUBLEREADATTRIBUTE__
  cout << "In doubleReadAttribute "
       << flush
       << endl;
#endif

  pair<bool,string> lReadAttribute=stringReadAttribute(inAttribute);

#ifdef __D_CATTRIBUTELIST__DOUBLEREADATTRIBUTE__
  cout << "AGAIN In doubleReadAttribute "
       << flush
       << endl;
#endif

  if(!lReadAttribute.first){
    return make_pair(bool(0),
		     double(0));
  }else{
    char* lLastCorrectCharacter=0;

    double lReturnValue=strtod(lReadAttribute.second.c_str(),
			       &lLastCorrectCharacter);
    bool lIsValid=!(*lLastCorrectCharacter);
    
    return make_pair(lIsValid,
		     lReturnValue);
  }
};
/** reading an attribute for strings */
pair<bool,string> CAttributeList::stringReadAttribute(const string& inAttribute)const{
  
  //check, if the magic number is correct at this time

#ifdef _PRINTOUTS_AL
  checkNPrint();
  cout << inAttribute 
       << endl 
       << flush;
#endif

#ifdef _PRINTOUTS_AL
  find(inAttribute);
  cout << "came back from find()" << flush;
#endif

  CAttributeList::const_iterator lFoundPosition(find(inAttribute));

#ifdef _PRINTOUTS_AL
  cout << "came back 2nd time from mAttributes.find()" << flush;
#endif

  if(lFoundPosition != end()){
    //cout << "[A;" << lFoundPosition->first << ";A]" << flush;
    //cout << "[A;" << lFoundPosition->first.size() << ";A]" << flush;
    string lFoundString(lFoundPosition->second);
    //cout << "[S" << strlen(lFoundPosition->second) << "S]" << flush;
    pair<bool,string> lReturnValue=make_pair(bool(1),
					     lFoundString);
    //cout << "[s" << lReturnValue.second.size() << "s]" << flush;
    return lReturnValue;
  }
  return make_pair(bool(0),
		   string(""));
};
/** reading an attribute for strings */
void CAttributeList::toXML(string& outString)const{
  outString += " ";
  for(const_iterator i=begin();
      i!=end();
      i++){
    outString += i->first + "=\"" + i->second + "\" ";
  }
};
