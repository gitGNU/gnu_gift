#include "CDomainSocket.h"
CDomainSocket::CDomainSocket():
  mIsInitialized(false){
};

/** constructor: copy the path, otherwise do nothing */
CDomainSocket::CDomainSocket(const string& inPath):
  mIsInitialized(false){
  this->listenToPath(inPath);
};

  /** destroy this: close the socket */
CDomainSocket::~CDomainSocket(){
  if(mIsInitialized){
    close(mSocketDescriptor);
  }
};
  /** set the path of the domain socket */
bool CDomainSocket::listenToPath(const string& inPath){
  mPath=inPath;
  
  if (( mSocketDescriptor= socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    return false;//no exit
  }
  
  mSocketStructure.sun_family = AF_UNIX;
  
  cout << "Making socket for path " << mPath << endl;
  strcpy(mSocketStructure.sun_path, mPath.c_str());
  unlink(mSocketStructure.sun_path);
  
  int len = strlen(mSocketStructure.sun_path) + sizeof(mSocketStructure.sun_family);
  if (bind(mSocketDescriptor, (struct sockaddr *)&mSocketStructure, len) == -1) {
    perror("bind");
    return 0;
  }
  if (listen(mSocketDescriptor, 5) == -1) {
    perror("listen");
    return 0;
  }
  
  if(chmod(mPath.c_str(), 
	   S_IRUSR|S_IWUSR)){
    perror("chmod");
    return 0;
  }
  mIsInitialized=1;
  return 1;
};
  /** get the path of the socket */
CDomainSocket::operator bool()const{
  return mIsInitialized;
}
  /** get the path of the socket */
string CDomainSocket::getPath()const{
  return mPath;
}
/** Serve from the socket that accepted */
void CDomainSocket::serveStream(int inSocket){
  int done = 0;
  char str[2];
  string lMessage;
  int lRead(0);
  do {
    lRead = recv(inSocket, 
		 str, 1, 0);
    if (lRead <= 0) {
      cerr << "here?" << endl;
      if (lRead < 0) perror("recv");
      done = 1;
    }else{
      str[lRead]=char(0);
      lMessage+=str;
    }
    
  } while (!done);

  cout << " after while" << endl;
  
  if (send(inSocket, lMessage.c_str(), lMessage.size(), 0) < 0) {
    perror("send");
  }
}

bool CDomainSocket::acceptAndServe(){
  struct sockaddr_un lAcceptedSocket;
  /** 
      if we are here, this means there is data for one socket 
  */
  unsigned int t = sizeof(lAcceptedSocket);
  int s2(0);
  if ((s2 = accept(this->getSocketDescriptor(), (struct sockaddr *)&lAcceptedSocket, &t)) == -1) {
    perror("accept");
    return false;
  }
  {
    struct ucred lCredentials;
    socklen_t lSize = sizeof(lCredentials);
    if (getsockopt(s2, 
		   SOL_SOCKET, 
		   SO_PEERCRED, 
		   &lCredentials, 
		   &lSize) 
	== 0){
      // cred.uid now contains UID of the connecting process.	
      cout << "Socket credentials:" << endl
	   << "Process ID: " << lCredentials.pid << endl
	   << "User ID:    " << lCredentials.uid << endl
	   << "Group ID:   " << lCredentials.gid << endl;
    }
  }
  cout << "connected: " << this->getPath() << endl;

  this->serveStream(s2);

  close(s2);
  return true;
}
