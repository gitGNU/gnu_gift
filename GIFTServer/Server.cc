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
           __GIFT_NEW_OPERATOR_NEW 
	                  use my own memory management

  ---------------------------------------------------------------------------*/

//use stuff by Wolfgang which has not yet been "released" to the group
#define WOLFGANG_DEVELOPER

#ifdef __GIFT_NEW_OPERATOR_NEW
//  this is a self-made memory manager
#include "CDebuggingMemoryManager.h"
//  this is for replacing the new() operator
#include "myNew.h"
CDebuggingMemoryManager gMemManager(MEMSIZE);
#endif

#include <stdlib.h>
#include <assert.h>
#include <iostream>

// mutual excludes for
// multithreading
#include "CMutex.h"
extern CMutex* gMutex;

#ifdef __GIFT_NEW_IS_MALLOC
void* operator new(size_t s){
//   assert(s<10000000);
//   if(s>10000000){
//     cout << ">" << flush;
//   }else{
//     if(s>100000){
//       cout << "!" << flush;
//     }else{
//       if(s>1000){
// 	cout << "£" << flush;
//       }else{
// 	cout << "." << flush;
//       }
//     }
//   }
  if(gMutex){
    gMutex->lock();
  }
  void *lResult(malloc(s));
  if(gMutex){
    gMutex->unlock();
  }
  return lResult;
}
void* operator new[](size_t s){
  return operator new(s);
}
void operator delete(void * inToBeDeleted){
  free(inToBeDeleted);
}
void operator delete[](void* inToBeDeleted){ 
  delete(inToBeDeleted); 
} 
#endif

// the gift exceptions
#include "../gift-config.h"
#include "GIFTExceptions.h"

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
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
//to be more specific: internet sockets
#include <arpa/inet.h>
#include <netinet/tcp.h>
//will we need the line below?
//#include <linux/socket.h>

//the expat xml parser by J.Clark
#include "xmlparse.h"
//a class for using xpat attributes in a nice way
#include "CAttributeList.h"

//for making distance matrices
#include "CAcURL2FTS.h"

//This is mrml+gift specific
#include "mrml_const.h" //important constants for parsing
#include "CAccessorAdminCollection.h"
#include "CSessionManager.h"
#include "CAccessor.h" // distance matrix
//for debugging
#define _DEBUG
#define _NON_BLOCKING 0

#include "CCommunicationHandler.h"

#ifdef __GIFT_SERVER_TREADS__
#define  __GIFT_USES_THREADS__
#endif

#ifdef __NO_GIFT_SERVER_THREADS__
#undef  __GIFT_USES_THREADS__
#endif

//#undef  __GIFT_USES_THREADS__ // for debugging
long PORT = 12789; /* port number: this line, for example, is by J. Raki ;-)*/



/***********************************************************************
 itoa								
									
 Function : Converts integer to String using sprintf
									
 Input    : Integer 

 Return   : String 

 ***********************************************************************/
extern string itoa (int val,int length) ;

extern string dtoa (double val) ;//..maybe this line is also by J. Raki ;-)



extern string gGIFTHome;

//waiting that everything has arrived
extern void waitWriteStream(int inWritingSocket);
extern void waitReadStream(int inReadingSocket);

extern void waitExceptionStream(int inExceptioningSocket);


/***********************************************************************
  sendMessage								
  
  Function : Send a message through the stream s 	
  
  Input    : Socket stream s, number of objects to send

  Return   : 0 if no error else -1
  ***********************************************************************/

#undef _DEBUG

bool sendMessage(int inSocket,
		 string inString,
		 ostream& outLogFile);

/***********************************************************************
  ReadMessage								
  
  Function : read the message coming from the Java Applet using the 
  communication protocol	
  
  Input    : Socket stream s  
			     			
  Return   : 0 if no error else -1
  ***********************************************************************/
bool asyncReadChar(int inSocket,char* outChar);


int readMessage(int inSocket,
		string& outMessage);


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

/** 
    the function started by a new thread 
    does not take any parameters except 
    a pointer to void. We want to make that point
    to something richer.
*/
class CProcessMessageParameters{
public:
  /** the file descriptor of the socket */
  int mSocket;
  /** The log file for the communication */
  ofstream& mLogFile;
  /** The sessionManager of the communication */
  CSessionManager& mSessionManager;
  /** makes this structure (needed because of the
      use of references). */
  CProcessMessageParameters(CSessionManager& inSessionManager,
			    ofstream& inLogFile,
			    int inSocket):
    mLogFile(inLogFile),
    mSessionManager(inSessionManager),
    mSocket(inSocket){
  };
};

/**

   This function takes a socket and uses it for one message

 */
void* processMessage(void* inProcessMessageParameters){
  //gMutex->lock();//for debugging only
  CProcessMessageParameters* lParameters((CProcessMessageParameters*) inProcessMessageParameters);
  
  assert(lParameters);
  
  CCommunicationHandler* lCommunicationHandler(new CCommunicationHandler(lParameters->mSessionManager,
									 lParameters->mLogFile));
					      
  lCommunicationHandler->setSocket(lParameters->mSocket);
  // this is the line which processes
  if (!lCommunicationHandler->readAndParse()) {
    printf("can't read from socket %s\n",strerror(errno)) ;
  }

  // and now comes the long and painful process of closing the socket
  // here we need something else!
  cout << "flushing everything!"
       << endl;
  fflush(0);
  
  cout << "shutting down the socket!"
       << endl;
  //this is really embarassing
  if(shutdown(lParameters->mSocket,1)){
    cerr << "Shutdown 1 failed: " << strerror(errno) << endl;
  }
  //new for better tear down??
  cout << "waitreadstream (I)" << endl;
  waitReadStream(lParameters->mSocket);
  {
    char lBuf;
    while(recv(lParameters->mSocket,&lBuf,1,MSG_WAITALL)>0){
      cout << "." << flush;
    }
  }
  cout << "waitreadstream (II)" << endl;
  //sleep(1);
  close(lParameters->mSocket);
  cout << "Deleting thread parameters"
       << endl;
  delete lParameters;
  delete lCommunicationHandler;
  cout << "DONE"
       << endl;
  //gMutex->unlock();//for debugging only

  return((void*)0);
}


/***********************************************************************
  main								
  
  Function : waits for and accepts a connection from a Java Applet. 
  Receives the request from the client and according the 
  request, sends the answer to the client.	

  Socket opening code snipped from J.Raki

  ***********************************************************************/	       
void main(int argc, char **argv){

  gMutex=new CMutex();

  gGIFTHome=string(getenv("GIFT_HOME")?getenv("GIFT_HOME"):getenv("HOME")?getenv("HOME"):".");

  cout << PACKAGE << "-" << VERSION << endl
       << "Usage (server):              gift [<Port> [<Configuration-Directory>]]" << endl
       << "making distance matrices:    gift <Port(ignored)> <Configuration-Directory> <Algorithm> <Colection> <from> <to>" << endl
       << endl
       << endl;

  if(argc>1){
    PORT=atoi(argv[1]);
  }
  if(argc>2 && argv[2]){
    gGIFTHome=string(argv[2])+string("/");
  }

  // the communication handler for this application
  // class definition is just above in this file
  CSessionManager gSessionManager(gGIFTHome+"/gift-sessions.mrml",
				  gGIFTHome+"/gift-config.mrml");
  ofstream gLogFile(string(gGIFTHome+"/gift-log.mrml").c_str(),
		    ios::app);
  
#ifdef WITH_GENERATE_DISTANCE_MATRIX
  if(argc==7){
    generateDistanceMatrix(gGIFTHome,
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
       << gGIFTHome
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
  /* assigns a name to the socket */
  if(bind(lSocketDescriptor,
	  (struct sockaddr *) &lSocketAddress ,
	  sizeof(lSocketAddress) ) == -1 ) {
    printf("bind error: %s\n",strerror(errno)) ;
    exit(1) ;
  }
  
  /*listen to incoming connections */
  /* backlog = 5 */
  if(listen(lSocketDescriptor,
	    5)==-1){ 
    printf("listen error: %s\n",strerror(errno)) ;
    exit(1) ;    
  }
  
  /* loop to wait for connections */
  try{
    for(;;){
#ifdef HAVE_LIBSOCKET
       int lSocketCopySize = sizeof(lSocketCopy) ;
#else
       unsigned int lSocketCopySize = sizeof(lSocketCopy) ;
#endif
      /* accept a connection*/
      cout << "Waiting for a connection..." 
	   << endl 
	   << flush;
      if((lOutSocket=accept(lSocketDescriptor, 
			    ( struct sockaddr * ) &lSocketCopy, 
			    &lSocketCopySize)
	  ) < 0 ) {// error

	cout << "Error when accepting connection:"
	     << endl
	     << strerror(errno)
	     << endl;
	exit(1) ;
      }else{// no error
	cout << "Accepted Connection!" << endl << flush;

	CProcessMessageParameters* 
	  lProcessMessageParameters(new 
				    CProcessMessageParameters(gSessionManager,
							      gLogFile,
							      lOutSocket));
#ifdef  __GIFT_USES_THREADS__
#warning "threading used"
	pthread_t lThread;
	int lErrorNumber(0);
	if(lErrorNumber=pthread_create(&lThread, NULL, &processMessage,lProcessMessageParameters)){
	  cerr << "Error in creating new thread: "
	       << strerror(lErrorNumber) << endl
	       << "Doing normal function call instead"
	       << endl;
	  processMessage(lProcessMessageParameters);
	}else{
	  cout << "Successfully created new message processing thread"
	       << endl;
	  if(lErrorNumber=pthread_detach(lThread)){
	    cerr << "Error in detaching thread: "
		 << strerror(lErrorNumber) << endl;
	  }else{
	    cout << "Successfully detached thread"
		 << endl;
	  }
	}
#else
#warning "threading blocked"
	cout << "calling processMessage (no thread)" << endl;
	processMessage(lProcessMessageParameters);
	cout << "returned processMessage (no thread)" << endl;
#endif
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
  delete gMutex;
} 





 

