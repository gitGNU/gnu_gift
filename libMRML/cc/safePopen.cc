#include "libMRML/include/CReaperDistributor.h"
#include "libMRML/include/CRPipe.h"
#include "libMRML/include/safePopen.h"

extern CReaperDistributor &getReaperDistributor();

int safePopen(char* const* argv){
  int lMode[2];
  if(pipe(lMode)){
    throw "Pipe error";
  }else{
    int lExecveTask;
    if(!(lExecveTask=fork())){
      char lWrite[12];
      sprintf(lWrite,"%d",lMode[1]);
	
      close(1);
      dup(lMode[1]);
	
      const char* lInstruction=argv[0];
	
      if(execve(lInstruction,
		argv,
		0)){
	cerr << "Could not execute " << lInstruction << ":" << strerror(errno) << endl;
      };
    }else{
      CReaper* lReaper=new CRPipe(lExecveTask,lMode[1]);
      getReaperDistributor().addReaper(lReaper);
      return lMode[0];
    }
  }
  return 0;
}

