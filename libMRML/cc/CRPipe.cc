#include "libMRML/include/CReaper.h"
#include "libMRML/include/CRPipe.h"
#include <iostream>
using namespace std;

CRPipe::CRPipe(int inPID, 
	       int inPipeToClose)
  :CReaper(inPID),
   mPipeToClose(inPipeToClose){
}

void CRPipe::reap(){
  //cout << "ing..." << flush;
  close(mPipeToClose);
  //cout << "...finished" << endl;
}
