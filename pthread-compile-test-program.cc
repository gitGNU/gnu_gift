#define   _GNU_SOURCE

#include <cstring>
#include <cassert>
#include <iostream>
#include <unistd.h>
#include <list>

#include <pthread.h>
pthread_t gThread1;
pthread_t gThread2;
pthread_mutex_t gMutex=PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
pthread_mutexattr_t gMutexAttribute;
list<int> gTestList;

void test2(void){
  pthread_mutex_lock(&gMutex);
  cout << "i" << flush;
  pthread_mutex_unlock(&gMutex);
}

void* test(void* inValue){
  cout << "Began thread " << *((int*)inValue) << endl;
  for(int i=0; i<10; i++){
    pthread_mutex_lock(&gMutex);
    cout << "t" 
	 << *((int*) inValue)
	 << ","
	 << i
	 << flush; 
    test2();
    gTestList.push_back(i);
    pthread_mutex_unlock(&gMutex);
  }
  cout << "Thread terminated " << inValue << endl;
}

int g1(1);
int g2(2);

void main(void){   
  void* lThreadReturn;
  
  pthread_mutexattr_init(&gMutexAttribute);
  assert(!(pthread_mutexattr_settype(&gMutexAttribute,
				     PTHREAD_MUTEX_RECURSIVE_NP)));
  pthread_mutex_init(&gMutex,
		     &gMutexAttribute);
  
  
  assert(!pthread_create(&gThread1, NULL, &test, (void*)&g1));
  cout << "Created thread 1" << endl;
  assert(!pthread_create(&gThread2, NULL, &test, (void*)&g2));
  cout << "Created thread 2" << endl;
  int lErrno=pthread_join(gThread1,&lThreadReturn);
  if(lErrno){
    cout << strerror(lErrno) << endl;
  }
  cout << "Joined thread 1" << endl;
  lErrno=pthread_join(gThread2,&lThreadReturn);
  if(lErrno){
    cout << strerror(lErrno) << endl;
  }
  cout << "Joined thread 2" << endl;
  lErrno=pthread_mutex_destroy(&gMutex);
  if(lErrno){
    cout << strerror(lErrno) << endl;
  }
  cout << "End of test program" << endl;
}

