#include "CSocket.h"
#include "CMultiServer.h"
/** a Server that serves multiple sockets at 
    the same time */
bool CMultiServer::addSocket(CSocket* inSocket){
  if(inSocket && bool(*inSocket)){
    mSockets.push_back(inSocket);
    mDescriptorToSocket[mSockets.back()->getSocketDescriptor()]=
      mSockets.back();
    return 1;
  }else{
    return 0;
  }
};
/** start serving these sockets */
void CMultiServer::serve(){
  for(;;) {
      
    fd_set lReadFileDescriptors;
    FD_ZERO(&lReadFileDescriptors);
    int lMax(0);
    for(list<CSocket*>::iterator i=mSockets.begin();
	i!=mSockets.end();
	i++){
      
      FD_SET(((*i)->getSocketDescriptor()),&lReadFileDescriptors);
      
      if((*i)->getSocketDescriptor()>lMax){
	lMax=(*i)->getSocketDescriptor();
      }
    }

    int done, n;
    cout << "Waiting for connection..." << endl;

    int lReturnValue = select(lMax+1, &lReadFileDescriptors, NULL, NULL, NULL);

    cout << "...select" << endl;

    for(list<CSocket*>::iterator i=mSockets.begin();
	i!=mSockets.end();
	i++){
      if(FD_ISSET(((*i)->getSocketDescriptor()),
		  &lReadFileDescriptors)){
	try{
	  cerr << "trying to accept " << (*i)->getSocketDescriptor() << " " << bool(**i) << endl;
	  if(**i){
	    (*i)->acceptAndServe();
	  }
	}catch(const char*){
	  cerr << "Accept failed, exception caught" << endl;
	}
      }
    }
    cout << "endfor " << endl;
  }
}

