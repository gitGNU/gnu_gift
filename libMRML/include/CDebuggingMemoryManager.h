// -*- mode: c++ -*-
#ifndef _CDEBUGMEMORYMANAGER
#define _CDEBUGMEMORYMANAGER
// -*- mode: c++ -*- 
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

typedef long CDebuggingMemoryManagerSize;

// we will allocate a chunk of 10 megs
#define MEMSIZE 10000000

/** A structure, which is useful to maintain a twice connected
  list:
  A list of Chunks and a list of free/occupied chunks.
  This should normally be a local type, but there was an internal
  compiler error in GNU C++.
  */
struct lTChunk{
public:
  ///Previous Item in List
  lTChunk* mPrev;
  
  ///Following Item in List
  lTChunk* mNext;
  
  ///Previous Item in MEMORY
  lTChunk* mPreceding;
  
  ///Following Item in MEMORY
  lTChunk* mFollowing;
  
  /**Size of this Chunk.
    The Size could be deduced from mFollowing, but I consider
    this way as more general and cleaner.
    */
  CDebuggingMemoryManagerSize mSize;

  /** We will add to this some stuff to check
      if the end of this has been overwritten
   */
  
  /** Magic number, to see, if this Pointer was allocated using
    this->GeCDebuggingMemoryManager() */
  long mMagic;
}; 

/** Class for memory management:
  This class gives you the full control about 1MByte of Memory.
  What is above will be allocated using the normal techniques.
  This is nice, if you are doubtful about the Libraries you use.
  */
class CDebuggingMemoryManager{
protected:

  /**Marking a Chunk as free and deleting him from the list
    whose member it presently is.*/
  void FreeChunk(lTChunk* inChunk);

  ///List of free memory chunks.
  lTChunk* mFreeList;
  ///List of used memory chunks.
  lTChunk* mUsedList;

  ///THE memory used by this memory administrator.
  lTChunk* mBuffer;

  ///The magic number for valid lTChunk nodes
  const long cMagic;
  ///The magic number to invalidate lTChunk nodes
  const long cUnMagic;

  ///
  long cVM;
public:

  /**Constructor. 
    The Parameter is the size of the Buffer administered by the structure
    */
  CDebuggingMemoryManager(const CDebuggingMemoryManagerSize inSize);

  ///Getting Mem.
  void* GeCDebuggingMemoryManager(const CDebuggingMemoryManagerSize inSize);

  ///Deleting Mem.
  bool FreeMem(void*);

  ///
  bool Valid();

  ///
  friend ostream& operator <<(ostream&, const CDebuggingMemoryManager& inMem);
};

///Output for diagnosis.
ostream& operator<<(ostream& o, const CDebuggingMemoryManager& inMem);

///One instance for the whole Program.
extern CDebuggingMemoryManager MemManager;

#endif
