/* -*- mode: c++ -*- 
*/
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

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
#ifndef _CSOCKET
#define _CSOCKET

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <functional>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
//fchmod
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <assert.h>
using namespace std;
/** 
    This class takes a unary function

    @see: CMultiServer
 */
class CSocket{
public:
  /** this is the function that's called when serving */
  class CServeFunction:public unary_function<bool,int>{
  public:
    virtual bool operator()(int inStream)=0;
  };
protected:
  /** has this been initialized ?*/
  bool mIsInitialized;
  /** int */
  int mSocketDescriptor;
  /** */
  CServeFunction* mServeFunction;
public:
  /** constructor */
  CSocket();
  /** destroy this: close the socket */
  ~CSocket();
  /** get the descriptor of the socket */
  int getSocketDescriptor()const;
  /** Serve from the socket that accepted */
  void setServeFunction(CServeFunction* inServeFunction);
  /** accept this socket and serve */
  virtual bool acceptAndServe()=0;
  /** serve to this socket */
  virtual void serveStream(int);
  /** */
  virtual operator bool()const;
};
#endif
