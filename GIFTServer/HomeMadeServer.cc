/*---------------------------------------------------------------------------

  Program name : Server.cc                   			        
							
  Function : An example MRML server

  Creation Date : July 1999						

  Author : Wolfgang Müller

  (some very small parts (< 200 lines)
   of this code still stem from 
   Jilali Raki's MREP based server)
 									
  Last modification: August 1999


  defines: _NON_BLOCKING: if !=0 then the socket will be used as a non-blocking one

  ---------------------------------------------------------------------------*/

//use stuff by Wolfgang which has not yet been "released" to the group
#define WOLFGANG_DEVELOPER

// the gift exceptions
#include "libMRML/include/GIFTExceptions.h"

// c++ standard library stuff
#include <functional>
#include <iostream>
#include <algorithm>
#include <string>
//c standard functions
#include <cstdio>
#include <stdlib.h>
#include <cmath>

//Sockets
#include <ctime>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
//to be more specific: internet sockets
#include <arpa/inet.h>
#include <netinet/tcp.h>
//will we need the line below?
//#include <linux/socket.h>

//the expat xml parser by J.Clark
//#include "expat/xmlparse/xmlparse.h"
#include <xmlparse.h>
//a class for using xpat attributes in a nice way
#include "libMRML/include/CAttributeList.h"

//"classical" gift specific includes
#include "libMRML/include/CRelevanceLevelList.h"
#include "libGIFTAcInvertedFile/include/CDocumentFrequencyList.h"
#include "libGIFTAcInvertedFile/include/CAcInvertedFile.h"
#include "libGIFTQuInvertedFile/include/CQInvertedFile.h"
#include "libMRML/include/TID.h"
#include "libGIFTAcInvertedFile/include/WeightingFunctionsAndNormalizers.h"
#include "libMRML/include/CAccessorElement.h"
#include "libMRML/include/CIDRelevanceLevelPairList.h"
//This is mrml+gift specific
#include "libMRML/include/mrml_const.h" //important constants for parsing
#include "libMRML/include/CAccessorAdminCollection.h"
#include "libMRML/include/CSessionManager.h"
#include "libMRML/include/CAccessor.h" // distance matrix
//for debugging
#define _DEBUG
#define _NON_BLOCKING 0
#include "libMRML/include/CCommunicationHandler.h"

//the "ID administration subsystem"
TID gID;

long PORT = 12789; /* port number: this line, for example, is by J. Raki ;-)*/



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
  
  while(!select(inReadingSocket+1,
		&lStreamsToWaitFor,
		0,
		0,
		&lWaitingTime));
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
    string lGIFTHome(".");
    
    if(getenv("HOME")){
      lGIFTHome=string(getenv("HOME"));
    }
      if(getenv("GIFT_HOME")){
	lGIFTHome=string(getenv("GIFT_HOME"));
      }
	
	{
	  ofstream lOutLastMessageFile(string(lGIFTHome
					      +string("/gift-last-out-message.mrml")).c_str());
	  lOutLastMessageFile << inString
	  << flush;
	}
  }

  outLogFile << inString 
	     << flush;


  
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


int readMessage(int inSocket,
		string& outMessage) { //J. Raki (obsolete)

  cout << "readMessage:" << flush;  

  char lChar[2];
  lChar[1]=0;

  while(asyncReadChar(inSocket,lChar)){
#ifdef _DEBUG
      cout << ValInt[0] 
	   << flush;
#endif 
      
      outMessage += lChar;
  }

  return 0;
}


#define WITH_GENERATE_DISTANCE_MATRIX
#ifdef WITH_GENERATE_DISTANCE_MATRIX

void generateDistanceMatrix(const string& inBaseDir,
			    const string& inOutputName,
			    const string& inAlgorithm,
			    const string& inCollection,
			    int inSkip,
			    int inTo){

  map<string,int> lURLToPosition;

  string lOutputName=inOutputName
    +string(".Algorithm")
    +inAlgorithm
    +string(".Collection")
    +inCollection
    +".bin";
    

  CSessionManager lSessionManager(inBaseDir+"/gift-sessions.mrml",
				  inBaseDir+"/gift-config.mrml");
  
  // open a session
  string lSessionID(lSessionManager.newSession("DistanceMatrixMaker",""));
  // configure the session
  CAlgorithm* lConfig(new CAlgorithm("algorithm",
				     0));
  assert(lConfig);
  lConfig->addAttribute(mrml_const::algorithm_id,
			inAlgorithm);
  lConfig->addAttribute(mrml_const::algorithm_type,
			inAlgorithm);
  lConfig->addAttribute(mrml_const::collection_id,
			inCollection);
  
  lSessionManager.setAlgorithm(lSessionID,
			       lConfig);


  fstream lMatrix(lOutputName.c_str(),
		  fstream::in+fstream::out);

  cout << "----------------------------------------"
       << endl
       << "I am generating a distance matrix"
       << endl
       << "Using the algorithm:  " << inAlgorithm
       << endl
       << "Using the collection: " << inCollection << ","
       << endl
       << "I am skipping the first " << inSkip << " lines."
       << endl
       << "I am ending before the  " << inTo << "th  line"
       << endl
       << "----------------------------------------"
       << endl;

  cout << "----------------------------------------"
       << endl
       << "Getting a list of all IDs in the collection"
       << endl
       << "SessionID: " << lSessionID
       << endl
       << "AlgorithmID: " << inAlgorithm
       << endl
       << "CollectionID: " << inCollection
       << endl;


  /*
    Get an accessor. We need this for getting at a list
    of all the images. Mabe we do this later using 
    a meta data query and coming from the outside.
  */

  CAccessorAdminCollection lAccessorAdminCollection(inBaseDir+"/gift-config.mrml");
  CAccessorAdmin& lProxy(lAccessorAdminCollection.getProxy(inCollection));
  assert(&lProxy);
  CAcURL2FTS* lAccessor=(CAcURL2FTS*)lProxy.openAccessor("url2fts");
  assert(lAccessor);
  list<CAccessorElement> lAccessorElements; 
  lAccessor->getAllAccessorElements(lAccessorElements);
  
  if(lAccessorElements.size()){
    
    //for guaranteeing a sequence of rising IDs (probably obsolete)
    lAccessorElements.sort(CSortByID_CAE());
    
    int lSize=lAccessorElements.size();
    
    
    
    //write the translator if nothing to be skipped
    {
      ofstream lTranslator(string(lOutputName+".trans").c_str());
      int lLine=0;
      for(list<CAccessorElement>::const_iterator i=lAccessorElements.begin();
	  i!=lAccessorElements.end();
	  i++){
	lTranslator << i->getID()
		    << " "
		    << lLine
		    << endl;
	lURLToPosition[i->getURL()]=lLine;
	lLine++;
      }
      inTo-=inSkip;
    }
    
    
    
    
    //skip the beginning
    {
      for(int i=0;
	  i<inSkip;
	  i++){
	lAccessorElements.pop_front();
      }
    }
    
    //skip the beginning also in the matrix
    lMatrix.seekp(lSize * inSkip * sizeof(float));
    
    
    int lCount=0;
    for(list<CAccessorElement>::const_iterator i=lAccessorElements.begin();
	i!=lAccessorElements.end() && lCount<inTo;
	i++,lCount++){

      
      cout    << "----------------------------------------"
	      << endl
	      << "querying for: " 
	      << i->getID()
	      << ": "
	      << i->getURL()
	      << endl;
      
      CXMLElement lQuery(mrml_const::query_step,0);
      lQuery.addAttribute(mrml_const::algorithm_id,
			  inAlgorithm);
      lQuery.addAttribute(mrml_const::result_size,
			  long(0x7fffffff));//simply get everything
      lQuery.addAttribute(mrml_const::result_cutoff,
			  0.0);//and do not cut anything off
      
      CXMLElement* lUserRelevanceList=new CXMLElement(mrml_const::user_relevance_element_list,
						      0);
      assert(lUserRelevanceList);
      
      CXMLElement* lUserRelevanceElement=new CXMLElement(mrml_const::user_relevance_element,
							 0);
      lUserRelevanceElement->addAttribute(mrml_const::user_relevance,
					  1.0);
      lUserRelevanceElement->addAttribute(mrml_const::image_location,
					  i->getURL());

      lQuery.addChild(lUserRelevanceList);
      lQuery.addChild(lUserRelevanceElement);
      lQuery.moveUp();
      lQuery.moveUp();
      
      string lXML;

      lQuery.toXML(lXML);
      
      cout << "XML:" 
	   << lXML
	   << endl
	   << "----------------------------------------"
	   << endl;
      
      CXMLElement* lResult=lSessionManager
	.query(lSessionID,
	       lQuery);
      
      {
	  string lString;
	  lResult->toXML(lString);
	  cout << lString 
	       << endl;
      }

      float lOutVector[lSize];
      {//fill lOutVecor with zeroes
	for(float* p=lOutVector;
	    p!=lOutVector+lSize;
	    *(p++)=0){}
      }
      
      float lZero=0.0;
      int lLastPosition=0;
      
      for(CXMLElement::lCChildren::iterator k=lResult->child_list_begin();
       	  k!=lResult->child_list_end();
       	  k++){
	      for(CXMLElement::lCChildren::iterator j=(*k)->child_list_begin();
		  j!=(*k)->child_list_end();
		  j++){
		  string lURL=(*j)->stringReadAttribute(mrml_const::image_location).second;
		  double lRelevanceLevel=(*j)->doubleReadAttribute(mrml_const::calculated_similarity).second;
		  
		  lOutVector[lURLToPosition[lURL]]=lRelevanceLevel;
		  
		  cout << endl << "[" << lURLToPosition[lURL] << "__" << lRelevanceLevel << "]" << flush;
	      }
      }
      
      lMatrix.write(lOutVector,sizeof(float)*lSize);
      lMatrix << flush;
      delete lResult;
      
      cout << endl
	   << "----------------------------------------"
	   << "Writing " << sizeof(float)*lSize << " Bytes" << endl

	   << "Still to go:" << inTo-lCount-1 << " lines."
	   << endl
	   << "----------------------------------------"
	   << endl;

    }
    lMatrix.close();
  }else{

  }
}
#endif

/***********************************************************************
  main								
  
  Function : waits for and accepts a connection from a Java Applet. 
  Receives the request from the client and according the 
  request, sends the answer to the client.	

  Socket opening code snipped from J.Raki

  ***********************************************************************/	       
void main(int argc, char **argv){

  string lConfigurationDirectory(getenv("GIFT_HOME")?getenv("GIFT_HOME"):"");
  if(!lConfigurationDirectory.size()){
    lConfigurationDirectory=(getenv("HOME")?getenv("HOME"):"");
  }

  cout << "GIFT 0.1.3" << endl
       << "Usage (server):              gift [<Port> [<Configuration-Directory>]]" << endl
       << "making distance matrices:    gift <Port(ignored)> <Configuration-Directory> <Algorithm> <Colection> <from> <to>" << endl
       << endl
       << endl;

  if(argc>1){
    PORT=atoi(argv[1]);
  }
  if(argc>2){
    lConfigurationDirectory=string(argv[2]);
  }

  // the communication handler for this application
  // class definition is just above in this file
  CCommunicationHandler gHandler(lConfigurationDirectory);




#ifdef WITH_GENERATE_DISTANCE_MATRIX
  if(argc==7){
    generateDistanceMatrix(lConfigurationDirectory,
			   string("DistanceMatrix"),
			   string(argv[3]),
			   string(argv[4]),
			   atoi(argv[5]),
			   atoi(argv[6]));
    exit(0);
  }
#endif

  cout << "----------------------------------------"
       << endl
       << "The current configuration directory is: "
       << lConfigurationDirectory
       << endl;

  cout << "----------------------------------------"
       << endl
       << "Opening port " << PORT
       << endl
       << "----------------------------------------"
       << endl;

  int lSocketDescriptor;
  int lOutSocket;
  //a bound socket
  //this prototype is copied when accepting
  struct sockaddr_in lSocketAddress;
  //the copy made of the prototype when accepting
  struct sockaddr_in lSocketCopy; 
  
  lSocketAddress.sin_family = AF_INET ;
  /* get the port number */
  lSocketAddress.sin_port =  htons(PORT);
  lSocketAddress.sin_addr.s_addr = INADDR_ANY ;
  
  /* create a socket */
  if((lSocketDescriptor = socket(PF_INET,
				 SOCK_STREAM,
				 IPPROTO_TCP))<0) {
    printf("SOCKET error %s\n",strerror(errno)) ;
    
    exit(1) ;
  }

  

  int lOptionOn=1;
  if(0 > setsockopt(lSocketDescriptor,
		    SOL_SOCKET,
		    SO_REUSEADDR,
		    (char*)&lOptionOn,
		    sizeof(lOptionOn))){
      cerr << "could not set REUSEADDR: " 
	   << strerror(errno)
	   << flush 
	   << endl;
      exit(1);
  };  
  if(0 > setsockopt(lSocketDescriptor,            /* socket affected */
		    IPPROTO_TCP,     /* set option at TCP level */
		    TCP_NODELAY,     /* name of option */
		    (char *) &lOptionOn,  /* the cast is historical cruft */
					
		    sizeof(lOptionOn))){
      cerr << "could not set NODELAY" 
	   << strerror(errno)
	   << flush 
	   << endl;
      exit(1);
  }

  linger lLinger;
  lLinger.l_onoff=1;
  lLinger.l_linger=1000;//linger for ten seconds
  
  /* assigns a name to the socket */
  if(bind(lSocketDescriptor,
	  (struct sockaddr *) &lSocketAddress ,
	  sizeof(lSocketAddress) ) == -1 ) {
    printf("bind error: %s\n",strerror(errno)) ;
    exit(1) ;
  }
  
  /*listen to incoming connections */
  listen(lSocketDescriptor,
	 5) ; /* backlog = 5 */
  
  /* loop to wait for connections */
  try{
    for(;;){
#ifdef HAVE_LIBSOCKET
       int lSocketCopySize = sizeof(lSocketCopy) ;
#else
       unsigned int lSocketCopySize = sizeof(lSocketCopy) ;
#endif
      /* accept a connection*/
      cout << "Waiting for a connection..." << endl << flush;
      if(1==_NON_BLOCKING){//setting non-blocking new
	long lArgument=0;
	long lReturnValue;
	lReturnValue=fcntl(lSocketDescriptor,F_GETFL,lArgument);
	cout << "before setNonBlocking:" << endl
	     << "lReturnValue" << lReturnValue
	     << "lArgument" << lArgument 
	       << endl;
	lReturnValue |= O_NONBLOCK;
	fcntl(lSocketDescriptor,F_SETFL,lReturnValue);
	lReturnValue=fcntl(lSocketDescriptor,F_GETFL,lArgument);
	cout << "after setNonBlocking:" << endl
	     << "lReturnValue" << lReturnValue
	     << "lArgument" << lArgument 
	     << endl;
      }
      if((lOutSocket=accept(lSocketDescriptor, 
			    ( struct sockaddr * ) &lSocketCopy, 
			    &lSocketCopySize)
	  ) < 0 ) {

	cout << "Error when accepting connection"
	     << strerror(errno)
	     << endl;
	exit(1) ;
      }else{

	if(1==_NON_BLOCKING){//setting non-blocking 
	  long lArgument=0;
	  long lReturnValue;
	  lReturnValue=fcntl(lOutSocket,F_GETFL,lArgument);
	  cout << "Getting non blocking value for accepted connection:" << endl
	       << "lReturnValue" << lReturnValue
	       << "lArgument" << lArgument 
	       << endl;
	}

	gHandler.setSocket(lOutSocket);
	cout << "Accepted Connection!" << endl << flush;
	if (!gHandler.readAndParse()) {
	    printf("can't read from socket %s\n",strerror(errno)) ;
	    //	    exit(1) ;
	}

	cout << "flushing everything!"
	     << endl;
	fflush(0);

	if(1==_NON_BLOCKING){
	  cout << "wait for read!"
	       << endl;
	  waitReadStream(lOutSocket);
	}
	cout << "shutting down the socket!"
	     << endl;
	//this is really embarassing
	shutdown(lOutSocket,
		 1);
	sleep(1);
	close(lOutSocket);
      }
    }
  }
  catch(GIFTException& inCaught){
    cout << "Caught inServer Main:"
	 << inCaught
	 << endl
	 << flush;
  }
  catch(...){
    cout << "there was an unknown exception" <<endl
	 << flush;
  }
  //return 0;
} 





 

