// -*- mode: c++ -*-
#ifndef _MYNEW
#define _MYNEW
/* -*- mode: c++ -*- 
*/
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
#include <iostream>
#include <cstdlib>
#include "libMRML/include/CDebuggingMemoryManager.h"

//#define __memdebug

void* operator new(size_t s){
  if(gMemManager.isValid()){

#ifdef __memdebug
    cout << "MyAlloc" << s << endl;
#endif
    return gMemManager.getMem(s);
  }
#ifdef __memdebug
  cout << "NormalAlloc" << flush;
#endif
  return malloc(s);
}
/* void* operator new[](size_t s){ */
/*   return new(s); */
/* } */

void operator delete(void * inToBeDeleted){
  if(inToBeDeleted){
    if(gMemManager.freeMem(inToBeDeleted)){
#ifdef __memdebug
      cout << "°my[" 
	   << inToBeDeleted 
	   << "]" 
	   << flush;// a character for which one can grep
#endif
      return;
    }else{
#ifdef __memdebug
      cout << "§pre[" 
	   << inToBeDeleted 
	   << "]" 
	   << flush;// a character for which one can grep
#endif
      free(inToBeDeleted);
    }
  }else{
    cout << "!null" << endl;
  }
}

void operator delete[](void* inToBeDeleted){ 
  delete(inToBeDeleted); 
} 

#endif
