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

/**
   Author of this file:
   Wolfgang Müller
   
*/

#ifndef _CREAPER
#define _CREAPER
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include <map>
#include <vector>
/**
   This file is what Wall et al. call REAPER
   in the perlipc manpage. It class knows which 
   PID it's waiting for. When the time has come
   the reaper distributor will call reap() on this.
   
   @author: Wolfgang Mueller
   @see: CReaperDistributor, man perlipc
*/
class CReaper{
protected:
  /** the ID of the process to which the reaper belongs */
  int mPID;
public:
  /** the PID for which this reaper
      is responsible */
  CReaper(int inPID);
  /** react to a sigchild signal */
  virtual void reap()=0; 
  /** 
      get the PID of this reaper 
  */
  int getPID()const;
};
#endif
