
#include "libMRML/include/CReaper.h"
#include "libMRML/include/CRPipe.h"

CRPipe::CRPipe(int inPID, 
	       int inPipeToClose)
  :CReaper(inPID),
   mPipeToClose(inPipeToClose){
}

void CRPipe::reap(){
  close(mPipeToClose);
}
