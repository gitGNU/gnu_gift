#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>

using namespace std;

main(){

  ostream_iterator<char> lOut(cout);

  cerr << "This takes characters from stdin,"
       << endl
       << "reshuffles them in order to"
       << endl
       << "change 'endianicity' and gives them to"
       << endl
       << "stdout"
       << endl;

  while(cin){
    char gBuffer[4];
    cin.read(gBuffer,4);
    if(cin){
      reverse(gBuffer,gBuffer+4);
      copy(gBuffer,
	   gBuffer+4,
	   lOut);
    }
  }
}
