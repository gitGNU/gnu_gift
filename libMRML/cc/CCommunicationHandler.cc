//configuration files for sessions and algorithms/collections

//Sockets
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
//
#include "CXEVCommunication.h" // the visitor analyzing the document trees built
#include "CCommunicationHandler.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "mrml_const.h" //mrml string constants newStart/EndMRMLElement
#include "../include/CTimeStampGenerator.h" // for time stamps
string gGIFTHome;//dirty but necessary
CMutex* gMutex;   //we need a global one

/***********************************************************************
 itoa								
									
 Function : Converts integer to String using sprintf
									
 Input    : Integer 

 Return   : String 

 ***********************************************************************/
string itoa (int val,int length) {
  char *car = new char[length+1];

  for(int i=0;
      i<length+1;
      i++){
    car[i]=0;
  };

  
  sprintf(car,"%0*2$d",val,length);

  return (string)car; 
}

string dtoa (double val) {

  char *car = new char[30];

  for(int i=0;
      i<30;
      i++){
    car[i]=0;
  };

  
  sprintf(car,"%f",val);

  return (string)car; 

}//..maybe this line is also by J. Raki ;-)



//prototype
bool readChar(int inSocket,char* outChar);
//waiting that everything has arrived
//
void waitWriteStream(int inWritingSocket){
  

  fd_set lStreamsToWaitFor;
  FD_ZERO(&lStreamsToWaitFor);
  FD_SET(inWritingSocket,&lStreamsToWaitFor);
  
  timeval lWaitingTime;
  lWaitingTime.tv_sec=5;
  lWaitingTime.tv_usec=0;

  int lCount=0;
  cout << "waiting" << flush;
  
  while(!select(inWritingSocket+1,
		0,
		&lStreamsToWaitFor,
		0,
		&lWaitingTime))lCount++;

  cout << "endwaiting" 
       << lCount 
       << flush 
       << endl;
}
void waitReadStream(int inReadingSocket){
  
  fd_set lStreamsToWaitFor;
  FD_ZERO(&lStreamsToWaitFor);
  FD_SET(inReadingSocket,&lStreamsToWaitFor);
  
  timeval lWaitingTime;
  lWaitingTime.tv_sec=5;
  lWaitingTime.tv_usec=0;

  //cout << endl << "READwaiting" <<endl<< flush;
  
  int lResult=select(inReadingSocket+1,
		    &lStreamsToWaitFor,
		    0,
		    0,
		    0);
  if(lResult<1){
    cerr << "Error in waitReadStream: " 
	 << strerror(errno) 
	 << endl;
  }
  cout << "reading eof" << endl;
  char lBuffer;
  readChar(inReadingSocket,&lBuffer);
  cout << "FINISHED reading eof" << endl;
  //&lWaitingTime));
  //cout << "endwaiting" << flush << endl;
}

void waitExceptionStream(int inExceptioningSocket){
  
  fd_set lStreamsToWaitFor;
  FD_ZERO(&lStreamsToWaitFor);
  FD_SET(inExceptioningSocket,&lStreamsToWaitFor);
  
  timeval lWaitingTime;
  lWaitingTime.tv_sec=5;
  lWaitingTime.tv_usec=0;

  //cout << endl << "READwaiting" <<endl<< flush;
  
  while(!select(inExceptioningSocket+1,
		&lStreamsToWaitFor,
		0,
		0,
		&lWaitingTime));
  cout << "Exc arrived" << flush << endl;
}


/***********************************************************************
  sendMessage								
  
  Function : Send a message through the stream s 	
  
  Input    : Socket stream s, number of objects to send

  Return   : 0 if no error else -1
  ***********************************************************************/

#undef _DEBUG

bool sendMessage(int inSocket,
		 string inString,
		 ostream& outLogFile) {
#ifdef _DEBUG
  cout << "Message to send: " 
       << inString 
       << "End of message to send"
       << endl 
       << flush;
#endif 

  {
    ofstream lOutLastMessageFile(string(gGIFTHome
					+string("/gift-last-out-message.mrml")).c_str());
    lOutLastMessageFile << inString
			<< flush;
  }
  
  gMutex->lock();

  outLogFile << inString 
	     << flush;

  gMutex->unlock();
  
  //i get problems when writing more than 64K to a socket.
#define _SOCKET_WORKAROUND
#ifdef _SOCKET_WORKAROUND
  int i=0;
  int lWriteSize=0x1000;

  // //this is really embarassing
  //sleep(1);
  
  waitWriteStream(inSocket);
  for(;
      i<inString.size()/lWriteSize;
      i++){
    int lOffset=i*lWriteSize;

    int lError=0;
    while((lError=write(inSocket,
			inString.c_str()+lOffset,
			lWriteSize) // The flags are none for the moment.
	   <=0));
    
    if(lError>0){
      return false;
    }

    waitWriteStream(inSocket);
  }
  if(write(inSocket,
	   inString.c_str()
	   +i*lWriteSize,
	   inString.size()%lWriteSize) // The flags are none for the moment.
     <=0 )
    return false;

#else
  if(write(inSocket,
	   inString.c_str(),
	   inString.size()) // The flags are none for the moment.
     <=0 )//this closing bracket has come still untouched from J Raki ;-)
    return false;
#endif
  
#ifdef _DEBUG
  cout << "END:Sending a message... " << endl << flush; 
#endif 


  waitWriteStream(inSocket);

  cout << "Message successfully sent!"
       << endl;

  return true;
}

/***********************************************************************
  ReadMessage								
  
  Function : read the message coming from the Java Applet using the 
  communication protocol	
  
  Input    : Socket stream s  
			     			
  Return   : 0 if no error else -1
  ***********************************************************************/
bool asyncReadChar(int inSocket,char* outChar){
  waitReadStream(inSocket);
  if(read(inSocket, outChar, 1)>=0)
    return true;
  else
    return false;
}
bool readChar(int inSocket,char* outChar){
  if(read(inSocket, outChar, 1)>=0)
    return true;
  else{
    return false;
  }
}


int readMessage(int inSocket,
		string& outMessage) { //J. Raki (obsolete)

  cout << "readMessage:" << flush;  

  char lChar[2];
  lChar[1]=0;

  while(readChar(inSocket,lChar)){
#ifdef _DEBUG
      cout << ValInt[0] 
	   << flush;
#endif 
      
      outMessage += lChar;
  }

  return 0;
}


extern string gGIFTHome;// this variable contains the directory which 
                        //contains the configuration of the GIFT
extern bool asyncReadChar(int,
			  char*);
extern bool sendMessage(int,
			string,
			ostream&);

void newStartMRMLElement(void *inUserData, 
			 const char *inElementName, 
			 const char **inAttributes){

  cout << "STARTING:" << inElementName << endl;

  CXMLElement* lDocumentTree=(CXMLElement*)inUserData;
  lDocumentTree->addChild(inElementName,
			  inAttributes);
}
void newMRMLTextElement(void *inUserData, 
			const XML_Char *inText,
			const int inSize){

  CSelfDestroyPointer<char> lBuffer((char*)operator new(inSize+1));
  
  strncpy(lBuffer,inText,inSize);
  lBuffer[inSize]=(char)0;

  string lText(lBuffer);

  cout << inSize << ":--------------------TEXT_" << lText << "_" << endl;
  

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
  CXMLElement* lDocumentTree=(CXMLElement*)inUserData;
  lDocumentTree->addChild(new CXMLElement(CXMLElement::cTextNode,
					  (char*)lBuffer));
  lDocumentTree->moveUp();
  //}else{
  //cout << "rejected: WHITESPACE ONLY"
  //     << endl;
  //}
}
void newEndMRMLElement(void *inUserData, 
		       const char *inElementName){
  cout << "ENDING:" << inElementName << endl;
  CXMLElement* lDocumentTree=(CXMLElement*)inUserData;

  lDocumentTree->moveUp();
  if(string(inElementName)==mrml_const::mrml){
    lDocumentTree->moveUp();
  }
}

//----------------------------------------
//communications:
//----------------------------------------
/// setting the communication socket for this session
void CCommunicationHandler::setSocket(int inSocket){
  mSocket=inSocket;
}
//----------------------------------------
// helpers for creating mrml messages
//----------------------------------------
//the preamble for a session
string CCommunicationHandler::preamble(){
  //ZORAN: I had to change this to something that is available all the time.
  //       BTW, this also preoduced a Socket Error, namely a connection refused,
  //       but was not linked to our problems.
  return string("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n "
		//"<!DOCTYPE mrml SYSTEM \"http://isrpc85.epfl.ch/Circus/code/mrml.dtd\">\n"
		//"<!DOCTYPE mrml SYSTEM \"http://isrpc85.epfl.ch/Circus/code/mrml.dtd\">\n"
		);
}


//Frame: this is for all messages the same
string CCommunicationHandler::frame(const string& inSession,
				    const string& inString){
  return string(preamble()+"<mrml session-id=\""+inSession+"\">\n"+inString+"\n</mrml>\n");
}
    
//----------------------------------------
//making attributes out of name/value pairs
string CCommunicationHandler::toAttribute(string inName,
					  string inString){
  return 
    inName
    +string("=\"")
    +inString
    +string("\"");
}
  
string CCommunicationHandler::toAttribute(string inName,
					  int inInt){
    
  char lBuffer[20];
  sprintf(lBuffer,"%d",inInt);
    
  string lString(lBuffer);
  return 
    inName
    +string("\"")
    +lBuffer
    +string("\" ");
}
  
string CCommunicationHandler::toAttribute(string inName,
					  double inFloat){
    
  char lBuffer[20];
  sprintf(lBuffer,"%lf",inFloat);
    
  string lString(lBuffer);
  return 
    inName
    +string("=\"")
    +lBuffer
    +string("\"");
}
// //----------------------------------------
// //turning a relevance level element into a string
// //(obsolete)
// string CCommunicationHandler::stringOfRelevanceLevelElement(const CRelevanceLevel& inRE,
// 							    double inUserRelevance=0.5){
//   return string("<sresultelement "
// 		+string(" ")
// 		+ toAttribute("calculatedsimilarity",inRE.getRelevanceLevel())
// 		+string(" ")
// 		+ toAttribute("imagelocation",inRE.getURL())
// 		+string(" ")
// 		+ toAttribute("userrelevance",inUserRelevance)
// 		+ " />\n");
// }
// string CCommunicationHandler::stringOfRelevanceLevelList(const CRelevanceLevelList& inRLL){
//   string lReturnValue("<sresultelementlist>\n");
    
//   for(CRelevanceLevelList::const_iterator i=inRLL.begin();
//       i!=inRLL.end();
//       i++){
//     lReturnValue+=stringOfRelevanceLevelElement(*i);
//   }
    
//   return lReturnValue+"</sresultelementlist>\n";
// }
  
//----------------------------------------
//Error message
int CCommunicationHandler::sendError(const string& inSession,
				     const string& inMessage){
  

  CXMLElement* lErrorElement=new CXMLElement("error",0);
  
  lErrorElement->addAttribute("message",inMessage);
}
//----------------------------------------
// making a new session
void CCommunicationHandler::openSession(const string& inUserName,
					const string& inSessionName){

  CXMLElement* lOpenedSession(mSessionManager.openSession(inUserName,
							  "",
							  inSessionName));
  //HACK to change the session of the multi response
  if(lOpenedSession){
    string lNewID(lOpenedSession->stringReadAttribute(mrml_const::session_id).second);

    cout << mMultiResponse->stringReadAttribute(mrml_const::session_id).second
	 << "-> new Session ID: " 
	 << lNewID
	 << endl;

    mMultiResponse->addAttribute(string(mrml_const::session_id),
				 lNewID);
    cout << "current state in multi reponse :"
	 << mMultiResponse->stringReadAttribute(mrml_const::session_id).second 
	 << endl;
  }
  addToMultiResponse(lOpenedSession);
}
// renaming the current session
void CCommunicationHandler::renameSession(const string& inSessionID,
					  const string& inName){
  addToMultiResponse(mSessionManager.renameSession(inSessionID,
						   inName));
  //FIXME : here we need to do SOMETHING
}
// renaming the current session
void CCommunicationHandler::deleteSession(const string& inSessionID){
  addToMultiResponse(mSessionManager.deleteSession(inSessionID));
}
// getting the property sheet for a given algorithm
void CCommunicationHandler::getPropertySheet(const string& inSessionID,
					     const string& inAlgorithmID){
  CXMLElement* lPropertySheet(mSessionManager.getPropertySheet(inSessionID,
							       inAlgorithmID));
  if(lPropertySheet){
    addToMultiResponse(lPropertySheet);
  }
}

//----------------------------------------
//the handshake message
int CCommunicationHandler::sendHandshake(const string& inUser){
    
  //at present this is only a dummy

  pair<string,string> 
    lSessionIDHandshakePair=mSessionManager.toXMLHandshake(inUser);

  string& lNewestSession(lSessionIDHandshakePair.first);
  string& lHandshake(lSessionIDHandshakePair.second);

  return sendMessage(mSocket,
		     frame(lNewestSession,
			   lHandshake).c_str(),
		     mLog);
}

//----------------------------------------
void CCommunicationHandler::getSessions(const string& inUser){
    
}
  
//----------------------------------------
void CCommunicationHandler::getCollections(){
  CXMLElement* lCollectionList(mSessionManager.getCollections());
  addToMultiResponse(lCollectionList);
}
//----------------------------------------
void CCommunicationHandler::getAlgorithms(){
  CXMLElement* lAlgorithms(mSessionManager.getAlgorithms());
  addToMultiResponse(lAlgorithms);
}
  
//----------------------------------------
//the result of a query
int CCommunicationHandler::sendResult(const string& inSession,
				      const CXMLElement& inRLL){
  assert("something wrong");
}
  
//----------------------------------------
//random images
int CCommunicationHandler::sendRandomImages(const string& inSession,
					    const string& inAlgorithm,
					    const string& inCollection,
					    const string& inNumberOfImages) {
  
  int lNumberOfImages=atoi(inNumberOfImages.c_str());
  
  CXMLElement* 
    lRLL=mSessionManager.getRandomImages(inSession,
					 inAlgorithm,
					 lNumberOfImages);
  
  cout << "SENDRANDOM" 
       << endl 
       << flush;
  
  addToMultiResponse(lRLL);
  return true;
};
  
  
//----------------------------------------
//setting properties of the query
void CCommunicationHandler::setResultSize(int inResultSize){
  mResultSize=inResultSize;
}

void CCommunicationHandler::setResultCutoff(const string& inCutoff){
  setResultCutoff(atof(inCutoff.c_str()));
}
void CCommunicationHandler::setResultCutoff(double inCutoff){
  mCutoff=inCutoff;
}

void CCommunicationHandler::setCollectionID(const string& inID){
  mCollection=inID;
}

void CCommunicationHandler::setAlgorithmID(const string& inID){
  mAlgorithm=inID;
}

//----------------------------------------
//parse XML using expat
//----------------------------------------
void CCommunicationHandler::parseString(const string& inMessage){
  gMutex->lock();
  bool lDone=false;
  do {
    if (!XML_Parse(mParser, 
		   inMessage.c_str(), 
		   inMessage.size(), 
		   lDone)) {
      cerr << "CCommunicationHandler.cc: __LINE__ XML ERROR: "
	   << XML_ErrorString(XML_GetErrorCode(mParser))
	   << " at line "
	   << XML_GetCurrentLineNumber(mParser)
	   << endl;

      mDocumentRoot=0;

      return;// instead of exit
    }
  } while (!lDone);
  gMutex->unlock();
}

//----------------------------------------
//parsing from a stream:
//read each character
//parse it
bool mParsingFinished;
void CCommunicationHandler::clearParsingFinished(){
  mParsingFinished=false;
};
void CCommunicationHandler::setParsingFinished(){
  mParsingFinished=true;
};
bool CCommunicationHandler::isParsingFinished()const{
  return mParsingFinished;
};


bool CCommunicationHandler::readAndParse(){
  gMutex->lock();
  clearParsingFinished();
  makeParser();
  bool lSuccess=false;
  char lBuffer[20]; 

  string lLogString;

  cerr << "repa" << endl;

  do {
      

    cout //<< "-" 
      << flush;
#ifdef _DEBUG
#endif 
      
    //was asyncReadChar
      
    if(readChar(mSocket, lBuffer)) {
      cout 
	//<< "<"
	<< lBuffer[0] 
	//<< ">"
	<< flush;
#ifdef _DEBUG
#endif 
      lBuffer[1]=0;
      lLogString+=lBuffer;

      if (!XML_Parse(mParser, 
		     lBuffer, 
		     1, 
		     false)) {
	cerr << "CCommunicationHandler.cc:"
	     << __LINE__ 
	     << ": XML ERROR "
	     << XML_ErrorString(XML_GetErrorCode(mParser))
	     << " at xml line "
	     << XML_GetCurrentLineNumber(mParser)
	     << endl;
	lSuccess=false;
      }else
	lSuccess=true;// i read at least one character
    }else{
      cerr << "Stream broke down!"
	   << XML_ErrorString(XML_GetErrorCode(mParser))
	   << " at line "
	   << XML_GetCurrentLineNumber(mParser)
	   << endl;
      lSuccess=false;
    }	
  } while (lSuccess && 
	   //!isParsingFinished() ex
	   !(mDocumentRoot->isSubtreeFinished())
	   );
  XML_Parse(mParser, 
	    lBuffer, 
	    0, 
	    true);

  {
    ofstream lInLogFile((gGIFTHome
			 +
			 string("/gift-last-in-message.mrml")).c_str());
    lInLogFile << lLogString 
	       << flush
	       << endl;
  }


  if(lSuccess){
    mLog << lLogString 
	 << flush
	 << endl;
  }

  CXEVCommunication lVisitor(this);

  gMutex->unlock();

  mDocumentRoot->traverse(lVisitor);

  return lSuccess;
}
  

void CCommunicationHandler::makeParser(){
  gMutex->lock();
  if(mParser)
    XML_ParserFree(mParser);
  mParser = XML_ParserCreate(NULL);//default encoding
  mDocumentRoot=new CXMLElement("__ROOT__",0);
  XML_SetUserData(mParser,
		  mDocumentRoot);//ex this
  XML_SetElementHandler(mParser, 
			newStartMRMLElement,//ex startMRMLElement
			newEndMRMLElement);//ex  endMRMLElement
  XML_SetCharacterDataHandler(mParser,
 			      newMRMLTextElement);
  gMutex->unlock();
}



//----------------------------------------
/** 
    Clears the algorithmTree element
*/
//----------------------------------------
void CCommunicationHandler::clearAlgorithmElement(){
  
  mAlgorithmTree=0;
};
//----------------------------------------
/** start of 
    an element in the tree of configured
    algorithms.
*/
//----------------------------------------
void CCommunicationHandler::startAlgorithmElement(const char* inName,
						  const char* const* const inAttributes){
  if(!mAlgorithmTree){
    mAlgorithmTree=new CAlgorithm(inName,inAttributes);
  }else{
    mAlgorithmTree->addChild(inName,inAttributes);
  }
};
//----------------------------------------
/** end of 
    an element in the tree of configured
    algorithms                          */
//----------------------------------------
void CCommunicationHandler::endAlgorithmElement(){
  if(mAlgorithmTree){
    mAlgorithmTree->moveUp();
  }
};
//----------------------------------------
/** 
    clear the pointer to the algorithm tree
*/
//----------------------------------------
void CCommunicationHandler::initAlgorithmElement(){
  mAlgorithmTree=0;
};
//----------------------------------------
/** 
    read the pointer to the algorithm tree
*/
//----------------------------------------
CAlgorithm* CCommunicationHandler::readAlgorithmElement(){
  return mAlgorithmTree;
};

int CCommunicationHandler::getQueryAtRandomCount()const{
  return mQueryAtRandomCount;
}
void CCommunicationHandler::incrementQueryAtRandomCount(){
  mQueryAtRandomCount++;
}

//----------------------------------------
//Using the session manager which is a member
//of this for other purposes
//----------------------------------------
CSessionManager& CCommunicationHandler::getSessionManager(){
  return mSessionManager;
}
//----------------------------------------
/** Start building a tree by successive adding
    of XML elements */
void CCommunicationHandler::startTreeBuilding(const char* inElementName,
					      const char*const*const inAttributes){
  mCurrentTree=new CXMLElement(inElementName,
			       inAttributes);  
};
/** Start building a tree by successive adding
    of XML elements */
void CCommunicationHandler::addToCurrentTree(const char* inElementName,
					     const char*const* inAttributes){
  mCurrentTree->addChild(new CXMLElement(inElementName,
					 inAttributes));  
};
/** 
    move up in the tree
*/
void CCommunicationHandler::moveUpCurrentTree(){
  mCurrentTree->moveUp();
};
/** 
    is this building a tree at present?
*/
bool CCommunicationHandler::isBuildingTree()const{
  return (mCurrentTree && (!mCurrentTree->isSubtreeFinished()));
};
//----------------------------------------
//constructor/destructor
//----------------------------------------
CCommunicationHandler::CCommunicationHandler(CSessionManager& inSessionManager,
					     ofstream& inLogFile):
  mSessionManager(inSessionManager),
  mLog(inLogFile),
  mQueryAtRandomCount(0),
  mCurrentTree(0){
  mResultSize=0;
  mCutoff=0.0;
  ///Constructing an expat parser
  mParser=0;
  makeParser();
}
CCommunicationHandler::~CCommunicationHandler(){
  //deleting the expat parser
  gMutex->lock();
  XML_ParserFree(mParser);
  gMutex->unlock();

}

/** 
    
    If we process multiple queries which are part of one message,
    we have to first collect the answers from the requests, and then
    send the whole message.
    
    startMultiRequest and endMultiRequest
    
    are the functions which administer this process.
    
    startMultiRequest clears the message which is going to be built.
*/
void CCommunicationHandler::startMultiRequest(const string& inSessionID){
  mMultiResponse=new CXMLElement("mrml",0);
  mMultiResponse->addAttribute("session-id",
			       inSessionID);
  
  CTimeStampGenerator lGenerator;
  addToMultiResponse(lGenerator.generateTimeStamp());
};
/** sends the message which has been built*/
void CCommunicationHandler::endMultiRequest(){
  if(mMultiResponse){
    string lMessage;

    mMultiResponse->addAttribute("just-for-test",
				 "and-of-course-for-fun");
    CTimeStampGenerator lGenerator;
    addToMultiResponse(lGenerator.generateTimeStamp());

    mMultiResponse->toXML(lMessage);

    cout << "endMultiRequest: WRITING: "
	 << lMessage
	 << endl;

    sendMessage(mSocket,
		preamble()+"\n"+lMessage+"\n",
		mLog);
  }
};
/** 
    adds an XMLElement to the multi-response which is built
*/
void CCommunicationHandler::addToMultiResponse(CXMLElement* inElement){

  if(0){
    string lOutString;
    inElement->toXML(lOutString);
    cout << "CCommunicationHandler::addToMultiResponse: adding "
	 << lOutString
	 << endl;
  }

  assert(mMultiResponse);
  mMultiResponse->addChild(inElement);
  mMultiResponse->moveUp();
};



