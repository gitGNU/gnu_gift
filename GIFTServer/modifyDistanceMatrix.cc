/** here there is something to do:
    i would like to have a small program, which takes ALL
    the scores and calculates their average as well as
    their standard deviation. 

    Using these two numbers, scores are then transformed into
    pseudo distances.
*/

#include <iterator>
#include <fstream>
#include <cmath>
#include <string>
#include <map>
#include <functional>
int gCount=0;
double gSum=0;
double gSumOfSquares=0;

void calculateSums(float inFloat){
 gCount       ++;
 gSum         += inFloat;
 gSumOfSquares+= inFloat * inFloat;
}

typedef map<double,int> CMap;

void vectorToMap(float* inBegin,
		 float* inEnd,
		 CMap& outMap){
  int lCount=0;
  for(float* i=inBegin;
      i!=inEnd;
      i++){
    outMap.insert(make_pair(*i,lCount++));
    //    cout << "-" << flush;
  }
}

double rankifyMatrix(float* inMatrix,
		     int inSize){
  /// Take the matrix
  /// make pairs of score and ID 
  for(int lLine=0;
      lLine<inSize;
      lLine++){
    //this map is sorted by rank inascending order
    CMap lMap;
    vectorToMap(inMatrix + lLine*inSize,
		inMatrix + (1+lLine)*inSize,
		lMap);
    //cout << endl;

    int lCount=inSize;
    for(CMap::const_iterator i=lMap.begin();
	i!=lMap.end();
	i++){
      //in the matrix now are the query ranks
      float l= float(--lCount)/inSize;
      if(l>1)
	l=1;
      if(l<0)
	l=0;
      inMatrix[(lLine*inSize)+i->second]=1-l;
    }
  }
}


double rescale(float inFloat){
  double lAverage=(gSum/gCount);
  double lAverageSquare=
    gSumOfSquares/gCount;
  double lSDev=
    sqrt(lAverageSquare
	 -
	 lAverage*lAverage);
  //...so from score to pseudo distance
  double lReturnValue=0.5-((inFloat-lAverage)/lSDev/3);

  if(lReturnValue>.9999)
    lReturnValue=1;
  if(lReturnValue<0.0001)
    lReturnValue=0;

  //  cout << "[" << lReturnValue << "]" <<flush;
  assert((lReturnValue<=1) && (lReturnValue>=0));

  return 
    lReturnValue;
}

//changes sign of matrix

void changeSign(float* inMatrix,
		int inSize){
  inSize=inSize*inSize;

  for(float* p=inMatrix;p!=inMatrix+inSize;p++){
    *p = 1 - *p;
  }
}

//symmetrifies the matrix given by an array
void symmetrify(float* inMatrix,
		int inSize){
  for(int i=0;
      i<inSize;
      i++){
    for(int j=0;
	j<inSize;
	j++){
      float l=min(inMatrix[i*inSize+j],
		  inMatrix[j*inSize+i]);
      inMatrix[i*inSize+j]=l;
      inMatrix[j*inSize+i]=l;
    }

  }
};


main(int argc,char **argv){

  if(argc!=4){
    cout  << "usage: "
	  << argv[0]
	  << " <INFILE> <OUTFILE> <SIZE>"
	  << endl;
    exit(1);
  }
  
  string lInName=argv[1];
  string lOutName=argv[2];
  int lSize=atoi(argv[3]);

  float* lMatrix=new float[lSize*lSize];
  assert(lMatrix);
  {
    ifstream lIn(lInName.c_str());
    
    while(lIn){
      float lBuffer;
      lIn.read((char*)&lBuffer,sizeof(lBuffer));
      calculateSums(lBuffer);
    }
  }
  cout << "until here "
       << flush 
       << endl;
  {
    ifstream lIn(lInName.c_str());
    ofstream lOut(lOutName.c_str());
    
    assert(lOut);

    int lCount=0;
    while(lIn){
      float lBuffer;
      lIn.read((char*)&lBuffer,
	       sizeof(lBuffer));
      lMatrix[lCount++]=lBuffer;
    }

#ifdef VISTEX
     rankifyMatrix(lMatrix,
		   lSize);
#endif

     //symmetrify the matrix
     symmetrify(lMatrix,
		lSize);
     
     //get from score to distance
     changeSign(lMatrix,
		lSize);


    lOut.write((char*)lMatrix,
	       sizeof(float)
	       *(lSize*lSize));
      
  }
}
