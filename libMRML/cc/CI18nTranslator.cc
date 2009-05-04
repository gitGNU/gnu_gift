// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

     Copyright (C) 2003, 2004 Bayreuth University
    Copyright (C) 2005,2006 Bamberg University
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
#include "libMRML/include/CI18nTranslator.h"
#include "libMRML/include/CXEVI18nTranslator.h"
#include <iostream>
#include <fstream>
#include <cstring>
/**
   This class offers runtime translation of strings. In contrast
   to GNU gettext we can chose during runtime the language without 
   additional overhead.
*/
/** The constructor */
CI18nTranslator::CI18nTranslator(string inTranslationFileName){
  ifstream inFile(inTranslationFileName.c_str());

  inFile.seekg(0,ios::end);
  int lFileSize(inFile.tellg());
  inFile.seekg(0,ios::beg);
  
  cout << "FileSize:" 
       << lFileSize 
       << " characters" 
       << endl;
  if(inFile){
    char* lBuffer=new char[lFileSize+1];
    inFile.read(lBuffer,lFileSize);
    lBuffer[lFileSize]=char(0);
    
    cout << "Read " << inTranslationFileName << endl;
    
    
  XML_Parser lParser = XML_ParserCreate(NULL);//default encoding
  
  XML_SetUserData(lParser,
		  this);
  XML_SetElementHandler(lParser, 
			CI18nTranslator::startXMLElement,
			CI18nTranslator::endXMLElement);
  bool lDone=false;
  if (!XML_Parse(lParser, 
		 lBuffer, 
		 lFileSize, 
		 lDone)) {
    cerr << __FILE__ <<":" << __LINE__ << ": XML ERROR: "
	 << XML_ErrorString(XML_GetErrorCode(lParser))
	 << " at line "
	 << XML_GetCurrentLineNumber(lParser)
	 << endl
	 << "Internationalization will be disabled in this run of the GIFT"
	 << endl;
  }
  XML_ParserFree(lParser);  
  }else{
    cerr << __FILE__ <<":" << __LINE__ << ": File unreadable."
	 << endl << "Internationalization will be disabled in this run of the GIFT"
	 << endl;

  }
};

/** Get an MRML tag containing all offered languages*/
const CXMLElement* CI18nTranslator::getLanguagesXML()const{
  return 0;
};
/** Get an MRML tag containing all offered languages*/
bool CI18nTranslator::hasLanguage(string inLanguage)const{
  return mLanguageMap.find(inLanguage)!=mLanguageMap.end();
};
/** Translate an attribute given by its name and its value 
    @returns pair: first, if the string should be translated,
    second, its value
*/
const pair<bool,string> CI18nTranslator::translateAttribute(string inLanguageCode,
							    const string& inAttribute,
							    const string& inValue)const{
  return make_pair(true, inValue);
};
/** Translate an tag containing pdata given by its name and its value 
    @returns pair: first, if the string should be translated,
    second, its value
*/
const pair<bool,string> CI18nTranslator::translateText(string inLanguageCode,
						       const string& inContainingTag,
						       const string& inText){
  return make_pair(true, inText);
};
/** Translate a complete XML Tree*/
void CI18nTranslator::translateXMLTree(string inLanguageCode,
				       CXMLElement& inoutToBeTranslated)const{
  CXEVI18nTranslator lVisitor(*this,inLanguageCode);
  inoutToBeTranslated.traverse(lVisitor);
};

void CI18nTranslator::setCurrentLanguage(const string& inLanguage){
  mCurrentLanguage=inLanguage;
}
string CI18nTranslator::getCurrentLanguage()const{
  return mCurrentLanguage;
}
/** Should the attribute be translated */
void CI18nTranslator::setShouldBeTranslatedAttribute(string inString){
  mShouldBeTranslatedAttribute[inString]=true;
};
/** Should the attribute be translated */
bool CI18nTranslator::shouldBeTranslatedAttribute(string inString)const{
  return  
    mShouldBeTranslatedAttribute.find(inString)!=mShouldBeTranslatedAttribute.end() 
    && mShouldBeTranslatedAttribute.find(inString)->second;
};
/** Add one translation pair */
void CI18nTranslator::addTranslationPair(string inString,string inTranslatedString){
  mLanguageMap[mCurrentLanguage][inString]=inTranslatedString;
};

void CI18nTranslator::startXMLElement(void *inUserData, 
					 const char *inElementName, 
					 const char **inAttributes){

  CI18nTranslator* self=(CI18nTranslator*)inUserData;

  CAttributeList lAttributes(inAttributes);

  if(strcmp("attribute",inElementName) && lAttributes.stringReadAttribute("name").first){
    self->setShouldBeTranslatedAttribute(lAttributes.stringReadAttribute("name").second);
  }
  if(strcmp("translation-list",inElementName) && lAttributes.stringReadAttribute("language-code").first){
    self->setCurrentLanguage(lAttributes.stringReadAttribute("language-code").second);
  }
  if(strcmp("translate",inElementName) && lAttributes.stringReadAttribute("key").first && lAttributes.stringReadAttribute("to").first){
    self->addTranslationPair(lAttributes.stringReadAttribute("key").second,
			     lAttributes.stringReadAttribute("to").second);
  }
}
void CI18nTranslator::endXMLElement(void *inUserData, 
		       const char *inElementName){
  CI18nTranslator* self=(CI18nTranslator*)inUserData;
}

