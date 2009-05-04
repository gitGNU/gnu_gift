#include <memory>
#include <iostream> // for printouts
#include <cstring>
#include "libMRML/include/CXMLElementBuilder.h"

void newStartXMLElement(void *inUserData, 
			 const char *inElementName, 
			 const char **inAttributes){

#ifdef XEB_PRINTOUT
  cout << "STARTING:" << inElementName << endl;
#endif

  CXMLElement** lDocumentTree=(CXMLElement**)inUserData;
  if(!(*lDocumentTree)){
    *lDocumentTree=new CXMLElement(inElementName,
				   inAttributes);
  }else{
    (*lDocumentTree)->addChild(inElementName,
			       inAttributes);
  }
}
void newXMLTextElement(void *inUserData, 
			const XML_Char *inText,
			const int inSize){

  CSelfDestroyPointer<char> lBuffer((char*)operator new(inSize+1));
  
  strncpy(lBuffer,inText,inSize);
  lBuffer[inSize]=(char)0;

  string lText(lBuffer);

#ifdef XEB_PRINTOUT
  cout << inSize << ":--------------------TEXT_" << lText << "_" << endl;
#endif
  

//   bool lWhitespaceOnly(true);
//   for(char* i(lBuffer);
//       i!=((char*)lBuffer)+inSize;
//       i++){
//     if((*i!=' ')
//        && (*i!='\n')
//        && (*i!='\t')){
//       lWhitespaceOnly=false;
//     }
//   }


  //  if(!lWhitespaceOnly){
  CXMLElement** lDocumentTree=(CXMLElement**)inUserData;
  if(*lDocumentTree){
    (*lDocumentTree)->addChild(new CXMLElement(CXMLElement::cTextNode,
					       (char*)lBuffer));
    (*lDocumentTree)->moveUp();
  }
  //}else{
  //cout << "rejected: WHITESPACE ONLY"
  //     << endl;
  //}
}
void newEndXMLElement(void *inUserData, 
		       const char *inElementName){
#ifdef XEB_PRINTOUT
  cout << "ENDING:" << inElementName << endl;
#endif
  CXMLElement** lDocumentTree=(CXMLElement**)inUserData;

  (*lDocumentTree)->moveUp();
}

CXMLElement* CXMLElementBuilder::stringToElement(const string& inString){
  XML_Parser lParser = XML_ParserCreate(NULL);//default encoding
  CXMLElement** lDocumentRoot=new CXMLElement*;
  *lDocumentRoot=0;
  XML_SetUserData(lParser,
		  lDocumentRoot);//ex this
  XML_SetElementHandler(lParser, 
			newStartXMLElement,//ex startMRMLElement
			newEndXMLElement);//ex  endMRMLElement
  XML_SetCharacterDataHandler(lParser,
 			      newXMLTextElement);

  bool lDone=false;

  /* I just got a 
     sligthly misterious bug in a realloc in expat.
     I try to solve this by copying c_str() to char*
     using a strdup() */

  char* lCopy(strdup(inString.c_str()));

  if (!XML_Parse(lParser, 
		 lCopy, 
		 inString.size(), 
		 lDone)) {
    cerr << "CCommunicationHandler.cc: __LINE__ XML ERROR: "
	 << XML_ErrorString(XML_GetErrorCode(lParser))
	 << " at line "
	 << XML_GetCurrentLineNumber(lParser)
	 << endl;
    
    delete *lDocumentRoot;
    delete lDocumentRoot;
    return 0;// instead of exit
  }
  free(lCopy);
  XML_ParserFree(lParser);
  CXMLElement* lReturnValue(*lDocumentRoot);
  delete lDocumentRoot;
  return lReturnValue;
}


CXMLElementBuilder::CXMLElementBuilder(){};
