/* -*- mode: c++ -*- 
*/
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000 CUI, University of Geneva

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
// -*- mode: c++ -*-
#ifndef _CMUTEX
#define _CMUTEX
#ifdef HAVE_LIBPTHREAD
#include <pthread.h>
#endif 
/**

   This class offers an abstraction from the locking method used.

   The methods are defined as const, although, of course, the mMutex
   will change state during each method call

 */
class CMutex{

#ifdef HAVE_LIBPTHREAD
  /** the mutex if needed */
  mutable pthread_mutex_t mMutex;
  /** attributes of the mutex */
  mutable pthread_mutexattr_t mMutexAttribute;
#endif

 public:
  /** constructor */
  CMutex();
  /** lock the mutex */
  void lock()const;
  /** 
      unlock the mutex 
   */
  void unlock()const;
};
#endif
