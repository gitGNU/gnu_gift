#include <iostream>
using namespace std;
main(){

  cerr << "This programm reads from stdin binary floats"
       << endl
       << "subtracts them from 1"
       << "and writes them to stdout";
  
  while(cin){
    float lBuffer;
    cin.read((char*)&lBuffer,
	     sizeof(lBuffer));
    lBuffer= 1-lBuffer;
    if(cin){
      cout.write((char*)&lBuffer,
		 sizeof(lBuffer));
    }
  }
}
