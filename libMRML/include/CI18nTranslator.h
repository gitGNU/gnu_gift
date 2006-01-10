// -*- mode: c++ -*-
#ifndef _CI18NTRANSLATOR
#define _CI18NTRANSLATOR
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
#include <map>
#include <string>
#include "libMRML/include/CXMLElement.h"
#include <expat.h>
//#include "expat/xmlparse/xmlparse.h"
/**
   This class offers runtime translation of strings. In contrast
   to GNU gettext we can chose during runtime the language without 
   additional overhead.
*/
class CI18nTranslator{
  /** a map that translates strings */
  typedef map<string,string> CTranslatorMap;
  /** a map that contains the decision if a given 
      string is to be translated */
  typedef map<string,bool> CShouldBeTranslatedMap;
  /** a map that contains translators for a number of languages */
  typedef map<string,CTranslatorMap> CLanguageMap;

  /**
     This map contains translators for each language given
     in the gift-i18n file.
   */
  CLanguageMap mLanguageMap;
  /**
     This map contains information about which 
   */
  CShouldBeTranslatedMap mShouldBeTranslatedAttribute;
  /**
     the current language while parsing the config file 
  */
  string mCurrentLanguage;
  

protected:
public:
  /** should a given attribute string be translated? */
  bool shouldBeTranslatedAttribute(string inAttribute)const;
  /** should a given attribute string be translated? */
  bool shouldBeTranslatedText(string inContainingTag)const;
  /** set the current language during construction */
  void setCurrentLanguage(const string& inLanguage);
  /** set the current language during construction */
  string getCurrentLanguage()const;
  /** Should the attribute be translated */
  void setShouldBeTranslatedAttribute(string inString);
  /** Add one translation pair */
  void addTranslationPair(string inString,string inTranslatedString);

  /** The constructor */
  CI18nTranslator(string inTranslationFileName);
  /** Get an MRML tag containing all offered languages*/
  const CXMLElement* getLanguagesXML()const;
  /** Get a string list containing all languages */
  bool hasLanguage(string inLanguageCode)const;
  /** Translate an attribute given by its name and its value 
      @returns pair: first, if the string should be translated,
      second, its value
  */
  const pair<bool,string> translateAttribute(string inLanguageCode,
					     const string& inAttribute,
					     const string& inValue)const;
  /** Translate an tag containing pdata given by its name and its value 
      @returns pair: first, if the string should be translated,
      second, its value
  */
  const pair<bool,string> translateText(string inLanguageCode,
					const string& inContainingTag,
					const string& inText);
  /** Translate a complete XML Tree*/
  void translateXMLTree(string inLanguageCode,
			CXMLElement& inoutToBeTranslated)const;
  /** for parsing the config file */
  static void CI18nTranslator::startXMLElement(void *inUserData, 
					       const char *inElementName, 
					       const char **inAttributes);
  /** for parsing the config file */
  static void CI18nTranslator::endXMLElement(void *inUserData, 
					     const char *inElementName);
};
#endif
