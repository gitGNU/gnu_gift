/* -*- mode: c++ -*- 
*/
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
// -*- mode: c++ -*-
#ifndef _CMULTISERVER
#define _CMULTISERVER

#include "CSocket.h"
#include <list>
#include <map>
/** a Server that serves multiple sockets at 
    the same time */
class CMultiServer{
protected:
  /** a list containing domain sockets */
  list<CSocket*> mSockets;
  /** find for a given file descriptor the socket */
  map<int,CSocket*> mDescriptorToSocket;
public:
  bool addSocket(CSocket* inSocket);
  /** start serving these sockets */
  void serve();
};
#endif
