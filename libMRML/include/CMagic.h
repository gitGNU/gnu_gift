// -*- mode: c++ -*-
#ifndef _CMAGIC
#define _CMAGIC
#include "libMRML/include/uses-declarations.h"
#include <assert.h>
/** This is a simple class for debugging.
    It checks if it has been properly initialised
    or if it has been destroyed twice. The method is simple:
    On construction of a CMagicObject, its member mMagic is
    set to int(42). On destruction mMember is set to int(36). 
*/
class CMagic{
  /** This should contain the magic number if everything is right */
  int mMagic;
public:
  /** This function checks the magic number of 
      this and asserts to zero, if something is wrong,
      if things are ok, this function keeps quiet.
  */
  void check()const;
  /** 
      as check(), but it also prints a small message 
      to cout, saying that the check worked.
  */
  void checkNPrint()const;
  /** sets the magic number to 42 */
  CMagic();
  /** sets the magic number to another value (36) */
  ~CMagic();
};


#endif
