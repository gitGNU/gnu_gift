#include "libMRML/include/CReaperDistributor.h"
#include "libMRML/include/CReaper.h"

CReaperDistributor& getReaperDistributor();

void reaper(int){
  getReaperDistributor().reap();
  signal(SIGCHLD, reaper);
}


bool CReaperDistributor::addReaper(CReaper* inReaper){
  if(inReaper){
    mReapers[inReaper->getPID()]=inReaper;
    return 1;
  }else{
    return 0;
  }
}

CReaperDistributor::CReaperDistributor(){
  signal(SIGCHLD, reaper);
}

CReaperDistributor::~CReaperDistributor(){
  for(CReaperMap::iterator i=mReapers.begin();
      i!=mReapers.end();
      i++){
    delete i->second;
  }
  signal(SIGCHLD, SIG_DFL); 
}

void CReaperDistributor::reap(){
  int lChildTask;
  int lStatus;
  while ((lChildTask = waitpid(-1,&lStatus,WNOHANG))>0) {
    cout << "R" << endl;

    CReaperMap::iterator lFound=mReapers.find(lChildTask);
    
    if(lFound!=mReapers.end())
      cout << "reap" << flush;
      lFound->second->reap();
  }
}     


CReaperDistributor* gReaperDistributor(0);

CReaperDistributor& getReaperDistributor(){
  if(gReaperDistributor){
    return *gReaperDistributor;
  }else{
    return *(gReaperDistributor=new CReaperDistributor());
  }
}


