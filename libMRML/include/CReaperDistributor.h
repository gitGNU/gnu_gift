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
#ifndef _CREAPERDISTRIBUTOR
#define _CREAPERDISTRIBUTOR
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <cerrno>
#include <csignal>

#include <map>
#include <vector>
class CReaper;
using namespace std;
/**
   This is a structure for administering
   reapers that come into action on reception 
   of a sigchild.
   
   @author: Wolfgang Mueller
 */
class CReaperDistributor{
  /**
     A type for a  map containing reapers
   */
  typedef map<int,CReaper*> CReaperMap;
  /** A map containing the reapers we want to use */
  CReaperMap mReapers;
public:
  /** add Reaper. The inReaper->getPID() will be used
      for putting the reaper into mReapers.
   */
  bool addReaper(CReaper* inReaper);
  /** Call the reaper of all processes that happen 
      to terminate during this call of reap */
  void reap();
  /** set sigchild handler on function reaper 
   */
  CReaperDistributor();
  /** destroy this and all reapers 
      set sigchild handler to default
   */
  ~CReaperDistributor();
};
#endif
