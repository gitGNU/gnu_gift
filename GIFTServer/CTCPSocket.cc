#include <iostream>
#include "CTCPSocket.h"

CTCPSocket::CTCPSocket():
  mIsInitialized(false){
};

/** constructor: copy the path, otherwise do nothing */
CTCPSocket::CTCPSocket(const string& inPath,
		       int   inPort):
  mIsInitialized(false){
  this->listenAtHostPort(inPath,
			 inPort);
};

  /** destroy this: close the socket */
CTCPSocket::~CTCPSocket(){
  if(mIsInitialized){
    close(mSocketDescriptor);
  }
};
/** set the path of the domain socket */
bool CTCPSocket::listenAtHostPort(const string& inHost,
				  int inPort){

  if(inHost.size()){
    cerr << __FILE__ << ":" << __LINE__<< ":Warning:"
	 << "Hostname currently unused" << endl;
  }

  mSocketAddress.sin_family = AF_INET ;
  /* get the port number */
  mSocketAddress.sin_port =  htons(inPort);
  mSocketAddress.sin_addr.s_addr = INADDR_ANY ;

  if((mSocketDescriptor = socket(PF_INET,
				 SOCK_STREAM,
				 IPPROTO_TCP))<0){
    cerr << "could not create socket" << endl;
    throw("socket");
  }
  
  int lOptionOn;
  if(0 > setsockopt(mSocketDescriptor,
		    SOL_SOCKET,
		    SO_REUSEADDR,
		    (char*)&lOptionOn,
		    sizeof(lOptionOn))){
    cerr << "could not set REUSEADDR: " 
	 << strerror(errno)
	 << flush 
	 << endl;
    throw("setsockopt");
  };  
  /* assigns a name to the socket */
  if(bind(mSocketDescriptor,
	  (struct sockaddr *) &mSocketAddress ,
	  sizeof(mSocketAddress) ) == -1 ) {
    printf("bind error: %s\n",strerror(errno)) ;
    throw("bind");
  }
  /*listen to incoming connections */
  /* backlog = 5 */
  if(listen(mSocketDescriptor,
	    5)==-1){ 
    
    printf("listen error: %s\n",strerror(errno)) ;
    throw("listen");
  }

  cerr << "TCP socket successfully initialized:" << mSocketDescriptor<< endl;

  return mIsInitialized=1;
};
  /** get the path of the socket */
CTCPSocket::operator bool()const{
  return mIsInitialized;
}
  /** get the path of the socket */
string CTCPSocket::getHost()const{
  return mHost;
}
  /** get the path of the socket */
int CTCPSocket::getPort()const{
  return mPort;
}
void CTCPSocket::acceptAndServe(){
  struct sockaddr_in lAcceptedSocket;
  /** 
      if we are here, this means there is data for one socket 
  */
  cerr << "Accept and serve: " << this->getSocketDescriptor() << endl;

  unsigned int lSize = sizeof(lAcceptedSocket);
  int s2(0);
  if(-1 == (s2 = accept(this->getSocketDescriptor(), (struct sockaddr *)&lAcceptedSocket, &lSize))) {
    perror("accept");
    exit(1);
    throw("accept");
  }
  cout << "connected: " << (this)->getSocketDescriptor() << "->" << s2 << endl;

  (this)->serveStream(s2);
  
  close(s2);
}
