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
#ifndef _CDOMAINSOCKET
#define _CDOMAINSOCKET

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
#include "CSocket.h"
using namespace std;

/** 
    A structure that opens/closes a domain socket 
    for a given path and sets the file permissions 
    to 0600.

    This class will be used with select to be the 
    entry to the local search engine.

    @see: CMultiServer
 */
class CDomainSocket:public CSocket{
protected:
  /** has this been initialized ?*/
  bool mIsInitialized;
  /** the path to the socket (visible in the file system)*/
  string mPath;
  /** the socket structure of the local socket */
  struct sockaddr_un mSocketStructure;
public:
  /** constructor */
  CDomainSocket();
  /** constructor: copy the path, otherwise do nothing */
  CDomainSocket(const string& inPath);
  /** destroy this: close the socket */
  ~CDomainSocket();
  /** set the path of the domain socket 
      and listen to it
  */
  bool listenToPath(const string& inPath);
  /** get the path of the socket */
  string getPath()const;
  /** Serve from the socket that accepted */
  virtual void serveStream(int);
  /** */
  virtual void acceptAndServe();

  /** */
  operator bool()const;
};
#endif
