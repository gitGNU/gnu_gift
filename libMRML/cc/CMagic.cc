#include "libMRML/include/CMagic.h"

#include <iostream>

/** This function checks the magic number of 
    this and asserts to zero, if something is wrong
*/
void CMagic::check()const{
  if(mMagic==36){
    cout << "Was constructed, has been destroyed in the meanwhile "
	 << endl;
    assert(!"Was constructed, but destroyed to early");
  }else{
    if(mMagic!=42){
      cout << "The magic number is: " 
	   << mMagic 
	   << endl;
      assert(!"No construction");
    }
  }
};
void CMagic::checkNPrint()const{
  check();
  cout << "CHECKING SUCCEEDED" << endl;
};
/** sets the magic number */
CMagic::CMagic(){
  mMagic=42;
};
  /** sets the magic to another value */
CMagic::~CMagic(){
  mMagic=36;
};
