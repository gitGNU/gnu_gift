/*---------------------------------------------------------------------------

  Program name : Server.cc                   			        
							
  Function : An example MRML server

  Creation Date : July 1999						

  Author : Wolfgang Müller

  (some very small parts (< 200 lines)
   of this code still stem from 
   Jilali Raki's MREP based server)
 									
  Last modification: December 2001 (don't count on this date)


  defines: _NON_BLOCKING: if !=0 then the socket will be used as a non-blocking one
           __GIFT_NEW_OPERATOR_NEW 
	                  use my own memory management

  ---------------------------------------------------------------------------*/

//use stuff by Wolfgang which has not yet been "released" to the group
#define WOLFGANG_DEVELOPER
#include <exception> // standard exceptions
using namespace std;

#ifdef __GIFT_NEW_OPERATOR_NEW
//  this is a self-made memory manager
#include "libMRML/include/CDebuggingMemoryManager.h"
//  this is for replacing the new() operator
#include "libMRML/include/myNew.h"
CDebuggingMemoryManager gMemManager(MEMSIZE);
#endif

#define _GNU_SOURCE
#include <getopt.h>

#include <stdlib.h>
#include <cassert>
#include <iostream>
#include <fstream>

// mutual excludes for
// multithreading
#include "libMRML/include/CMutex.h"
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
  if(gMutex){
    gMutex->lock();
  }
  free(inToBeDeleted);
  if(gMutex){
    gMutex->unlock();
  }
}
void operator delete[](void* inToBeDeleted){ 
  delete(inToBeDeleted); 
} 
#endif

// the gift exceptions
#include "../gift-config.h"
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
#include <expat.h>
//a class for using xpat attributes in a nice way
#include "libMRML/include/CAttributeList.h"

//for making distance matrices
#include "libGIFTAcURL2FTS/include/CAcURL2FTS.h"

//This is mrml+gift specific
#include "libMRML/include/mrml_const.h" //important constants for parsing
#include "libMRML/include/CAccessorAdminCollection.h"
#include "libMRML/include/CSessionManager.h"
#include "libMRML/include/CAccessor.h" // distance matrix
//for debugging
#define _DEBUG
#define _NON_BLOCKING 0

#include "libMRML/include/CCommunicationHandler.h"

#ifdef __GIFT_SERVER_TREADS__
#define  __GIFT_USES_THREADS__
#endif

#ifdef __NO_GIFT_SERVER_THREADS__
#undef  __GIFT_USES_THREADS__
#endif

#include "processMessage.h"
#include "CProcessMessageParameters.h"

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
  cout << "Starting generateDistanceMatrix " << inAlgorithm << inCollection << inSkip << inTo << endl;
  map<string,int> lURLToPosition;

  string lOutputName=inOutputName
    +string(".Algorithm")
    +inAlgorithm
    +string(".Collection")
    +inCollection
    +".bin";
    

  CSessionManager lSessionManager(inBaseDir+"/gift-sessions.mrml",
				  inBaseDir+"/gift-config.mrml",
				  inBaseDir+"/gift-i18n.xml"
				  );
  cout << "hier 0" << endl;
  // open a session
  string lSessionID(lSessionManager.newSession("DistanceMatrixMaker",""));
  cout << "hier 1" << endl;
  // configure the session
  CAlgorithm* lConfig(new CAlgorithm("adefault",
				     0));
  assert(lConfig);
  lConfig->addAttribute(mrml_const::algorithm_id,
			inAlgorithm);
  lConfig->addAttribute(mrml_const::algorithm_type,
			inAlgorithm);
  lConfig->addAttribute(mrml_const::collection_id,
			inCollection);
  cout << "hier 1" << endl;
  lSessionManager.setAlgorithm(lSessionID,
			       lConfig);
  cout << "hier 2" << endl;

  // fstream::in+fstream::out would cast to int IMPLICITLY
  fstream lMatrix(lOutputName.c_str(),
		  fstream::in|fstream::out);

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
      
      lMatrix.write((char*)lOutVector,sizeof(float)*lSize);
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


 
#include "CMultiServer.h"
#include "CTCPSocket.h"

class CSFGift:public CSocket::CServeFunction{
  /** */
  ofstream& mLogFile;
  /** */
  CSessionManager& mSessionManager;
public:
  /** */
  CSFGift(CSessionManager& inSessionManager,
	  ofstream& inLogFile):
    mSessionManager(inSessionManager),
    mLogFile(inLogFile){
  };
  /** */  
  bool operator()(int inSocketDescriptor){
    cout << "Accepted Connection!" << endl << flush;

    
    string lAddress;
    {
      struct sockaddr  lName;
      socklen_t lNameLen(sizeof(lName));
      if(!getpeername(inSocketDescriptor, 
		      &lName, 
		      &lNameLen)){
	lAddress="Peer INET Address: " + string(inet_ntoa(((sockaddr_in*)(&lName))->sin_addr));
	cout << "Accepted from adress [" << lAddress << "]" << endl;
      } 
    }

    CProcessMessageParameters* 
      lProcessMessageParameters(new 
				CProcessMessageParameters(mSessionManager,
							  lAddress,
							  mLogFile,
							  inSocketDescriptor));
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
};


static void displayHelp()
{
  cout << PACKAGE << "-" << VERSION << endl
       << "Usage (server):              gift [--port <Port>] [--datadir <Configuration-Directory>] [--no-random-seed]" << endl;
#ifdef WITH_GENERATE_DISTANCE_MATRIX
  cout << "making distance matrices:    gift --generate-matrix [--datadir <Configuration-Directory>] <Algorithm> <Collection> <from> <to>" << endl;
#endif
  cout << endl << endl;
}

/***********************************************************************
  main								
  
  Function : waits for and accepts a connection from a Java Applet. 
  Receives the request from the client and according the 
  request, sends the answer to the client.	

  Socket opening code snipped from J.Raki

  ***********************************************************************/	       
int main(int argc, char **argv){
  gMutex=0;
  gMutex=new CMutex();

  gGIFTHome=string(getenv("GIFT_HOME")?getenv("GIFT_HOME"):getenv("HOME")?getenv("HOME"):".");

  static struct option long_options[] = 
  {
    {"port", 	       required_argument, 0, 'p'},
    {"datadir",        required_argument, 0, 'd'},
    {"help",           no_argument,       0, 'h'},
    {"no-random-seed", no_argument,       0, 'r'},
#ifdef WITH_GENERATE_DISTANCE_MATRIX
    {"generate-matrix",no_argument,       0, 'g'},
#endif
    { 0,               0,                 0,  0 }
  };

  bool doRandom = true;
  bool generateMatrix = false;

  while ( 1 ) {
    int c = getopt_long( argc, argv, "p:d:rgh", long_options, NULL );
    if ( c == -1 )
      break;

    switch ( c ) {
     case 'd':
        gGIFTHome = string(optarg) + string("/");
        break;

      case 'p':
        PORT=atoi(optarg);
        break;

      case 'r':
        doRandom = false;
        break;

       case 'g':
        generateMatrix = true;
        break;
 
     case 'h':
        displayHelp();
        ::exit(0);

      case '?':
      default:
        break;
  }
  }

  if(!generateMatrix){
    cout << "GIFT:Starting Server" << endl;
  }else{
    cout << "GIFT:Generating distance matrix" << endl;
  }

  int idx = optind;
  if ( !generateMatrix ) {
    if ( idx < argc ) // more arguments -> backwards compat
      PORT = atoi(argv[idx++]);
    if ( idx < argc )
      gGIFTHome = string(argv[idx++]) + string("/");
  }


  if ( doRandom ) {
    cerr << "Random number generator has been seeded with " 
	 << getpid() << endl;
    srand(getpid());
  } else {
    cerr << "Warning: the random generator stays unseeded" << endl;
  }

  ofstream lPortFile;
  {
    string lPortFileName=gGIFTHome+"/gift-port.txt";
    lPortFile.open(lPortFileName.c_str());
    assert(lPortFile);
  }

  // the communication handler for this application
  // class definition is just above in this file
  CSessionManager gSessionManager(gGIFTHome+"/gift-sessions.mrml",
				  gGIFTHome+"/gift-config.mrml",
				  gGIFTHome+"/gift-i18n.xml"
				  );
  ofstream gLogFile(string(gGIFTHome+"/gift-log.mrml").c_str(),
		    ios::app);

  {
    time_t lNow(time(0));
    gLogFile << endl
	     << "<!-- This instance of the GIFT was started on -->" << endl
	     << "<!-- " <<  string(ctime(&lNow)) << " -->" << endl
	     << "<!-- PID " << long(getpid()) << " -->" << endl
	     << endl;
  }
  
#ifdef WITH_GENERATE_DISTANCE_MATRIX
  if( generateMatrix ){
    if ( idx + 4 != argc ) {
      cerr << endl << "Wrong number of parameters for generating the distance matrix." << endl;
      displayHelp();
      ::exit(1);
    }

    //exit(0);
    generateDistanceMatrix(gGIFTHome,
			   string("DistanceMatrix"),
			   string(argv[idx]),
			   string(argv[idx + 1]),
			   atoi(argv[idx + 2]),
			   atoi(argv[idx + 3]));
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


  try{
    CMultiServer lServer;
    CTCPSocket lSocket1("",PORT);
    CTCPSocket lSocket2("",PORT+1);

    // write out the ports
    lPortFile << PORT << endl;
    lPortFile << PORT+1 << endl;
    lPortFile.close();
    // close that file

    CSFGift    lServeFunction(gSessionManager,gLogFile);
    lSocket1.setServeFunction((&lServeFunction));
    lSocket2.setServeFunction((&lServeFunction));
    lServer.addSocket(&lSocket1);
    lServer.addSocket(&lSocket2);
    lServer.serve();
  }
  catch(GIFTException& inCaught){
    cout << "Caught inServer Main:"
	 << inCaught
	 << endl
	 << flush;
  }
  catch(GIFTException* inCaught){
    cout << "Caught inServer Main:"
	 << *inCaught
	 << endl
	 << flush;
  }
  catch(exception& inCaught){
    cout << "Caught inServer Main:"
	 << inCaught.what()
	 << endl
	 << flush;
  }
  catch(...){
    cout << "there was an unknown exception" <<endl
	 << flush;
  }
  //return 0;
  delete gMutex;
  return 0;
}


