#include "CSocket.h"

using namespace std;


/** constructor */
CSocket::CSocket():
  mSocketDescriptor(0),
  mIsInitialized(0){
};
/** destroy this: close the socket */
CSocket::~CSocket(){
  if(mIsInitialized
       && mSocketDescriptor){
    close(mSocketDescriptor);
  }
};
/** get the descriptor of the socket */
int CSocket::getSocketDescriptor()const{
  return mSocketDescriptor;
};
/** Serve from the socket that accepted */
void CSocket::setServeFunction(CSocket::CServeFunction* inServeFunction){
  mServeFunction=inServeFunction;
};
/** Serve from the socket that accepted */
void CSocket::serveStream(int inAcceptedSocket){
  mServeFunction->operator()(inAcceptedSocket);
};
/** */
CSocket::operator bool()const{
  return mIsInitialized;
};
