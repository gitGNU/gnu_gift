// -*- mode: c++ -*- 
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, University of Geneva

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
// WITHOUT THIS DEFINE THE INITIALISATION WILL COMPLAIN
#define   _GNU_SOURCE
#include "CMutex.h"
#include <assert.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
CMutex::CMutex()
{
#ifdef __GIFT_USES_THREADS__
#warning using threads
  pthread_mutexattr_init(&mMutexAttribute);
  assert(!(pthread_mutexattr_settype(&mMutexAttribute,
				     PTHREAD_MUTEX_RECURSIVE_NP)));
  assert(!pthread_mutex_init(&mMutex,
			     &mMutexAttribute));
#else
#warning NOT USING THREADS
#endif
}
void CMutex::lock()const{
#ifdef __GIFT_USES_THREADS__
#warning using threads
  pthread_mutex_lock(&mMutex);
  //cout << "&{" << getpid() << "}"<< flush;
#else
#warning NOT USING THREADS
#endif
};
void CMutex::unlock()const{
#ifdef __GIFT_USES_THREADS__
#warning using threads
  pthread_mutex_unlock(&mMutex);
  //  cout << "u{" << getpid() << "}"<< flush;
#else
#warning NOT USING THREADS
#endif
}
