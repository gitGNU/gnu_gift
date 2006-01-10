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
// -*- mode: c++ -*-
#ifndef _CTCPSOCKET
#define _CTCPSOCKET
#include "CSocket.h"
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
//fchmod
#include <sys/types.h>
#include <sys/stat.h>
//to be more specific: internet sockets
#include <arpa/inet.h>
#include <netinet/tcp.h>

#include <string>
#include <cassert>
using namespace std;
/** 
    A structure that opens/closes a TCP socket 
    for a given path and sets the file permissions 
    to 0600.

    This class will be used with select to be the 
    entry to the local search engine.

    @see: CMultiServer
 */
class CTCPSocket:public CSocket{
protected:
  /** has this been initialized ?*/
  bool mIsInitialized;
  /** the path to the socket (visible in the file system)*/
  string mHost;
  /** the path to the socket (visible in the file system)*/
  int mPort;
  /** the socket structure of the local socket */
  struct sockaddr_in mSocketAddress;
public:
  /** constructor */
  CTCPSocket();
  /** constructor: copy the path, otherwise do nothing */
  CTCPSocket(const string& inHost,
	     int inPort);
  /** destroy this: close the socket */
  ~CTCPSocket();
  /** set the path of the TCP socket 
      and listen to it
  */
  bool listenAtHostPort(const string& inHost,int inSocket);
  /** get the hostname it's listening to of the socket */
  string getHost()const;
  /** get the hostname it's listening to of the socket */
  int getPort()const;
  /** */
  bool acceptAndServe();
  /** */
  operator bool()const;
};
#endif
