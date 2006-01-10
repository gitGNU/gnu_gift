// -*- mode: c++ -*- 
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
#include "CCommunicationHandler.h"
#include "CProcessMessageParameters.h"
#include <sys/socket.h>
#include <cerrno>
#include <unistd.h>
#include <cstdio>
#include <cstring>
//waiting that everything has arrived
extern void waitWriteStream(int inWritingSocket);
extern void waitReadStream(int inReadingSocket);

extern void waitExceptionStream(int inExceptioningSocket);

/**

   This function takes a socket and uses it for one message

 */
void* processMessage(void* inProcessMessageParameters){
  //gMutex->lock();//for debugging only
  CProcessMessageParameters* lParameters((CProcessMessageParameters*) inProcessMessageParameters);
  
  assert(lParameters);
  
  CCommunicationHandler* lCommunicationHandler(new CCommunicationHandler(lParameters->mSessionManager,
									 lParameters->mLogFile));
  lCommunicationHandler->setPeerAddressString(lParameters->mPeerAddress);
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
