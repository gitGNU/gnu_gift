#ifdef empty
/***********************************************************************
 Program name : TestSimulator.cc compiles to TestSimulator
 									
  Function : Simulating a target test with a faultless user.
							
  Creation Date : June 1999	
 					 				
  Autor : Wolfgang Müller                                                  
 									
  Last modification : 
 ***********************************************************************/

#define WOLFGANG_DEVELOPER

#include "../config.h"

#include <functional>
 
#include <strstream.h>
#include <iostream.h>

#include "CRelevanceLevelList.h"
#include "CDocumentFrequencyList.h"
#include "CInvertedFileAccessor.h"
#include "CInvertedFileQuery.h"
#include "CIFQuickAndDirtyHunter.h"
#include "TID.h"

#include <algorithm>
#include <stdlib.h>
#include <math.h>


#include <strstream>

#include "WeightingFunctionsAndNormalizers.h"

CInvertedFileAccessor gAccessor(INVERTED_FILE_LOCATION,
				OFFSET_FILE_LOCATION,
				FEATURE_FILE_LOCATION,
				FEATURE_DESCRIPTION_LOCATION
				);


#include "the_class.cc"


main(int argc,
     char** argv){
  int lMaximumNumber=5;
  const int cArgumentOffset=3;

  cout << "making it till here "<< endl;

  if(argc<=cArgumentOffset){
    cout << "Usage: "
	 << argv[0]
	 << " output_file algorithm_number {image_id}*"
	 << endl;
    exit(1);
  }else{
    CIFQueryAndWeightingFunctions lQuery(gAccessor);
    ofstream lOutputFile(argv[1]);
    int lAlgorithmNumber=atoi(argv[2]);

    const int lNumberOfImages=argc-cArgumentOffset;
    cout << "Number of Images: "
	 << lNumberOfImages
	 << endl 
	 << flush;
    

    int lIDs[lNumberOfImages];

    //turn arguments into ID list
    for(int i=0;
	i<lNumberOfImages;
	i++){
      lIDs[i]=atoi(argv[i+cArgumentOffset]);
      cout << i
	   << "th Image ID: "
	   << lIDs[i]
	   << endl;
    }

    // here use some procedure to permute, 
    // let us take an c++ algorithm if this works
    cout << "initializing query..." 
	 << lAlgorithmNumber << endl << flush;
    lQuery.initializeQuery(lAlgorithmNumber);
    int lCounter=0;

    // this list contains the output images of each step
    CRelevanceLevelList* lOutList=new CRelevanceLevelList();

    // loop till all images
    
    for(int i=0;i<lNumberOfImages;i++){
      

      bool lFound=false;
      int lCount=0;
      ///suggest things to the user, until you find
      ///a target.

      while(!lFound){//while !lFound
	// this list contains the pseudo user feedback
	
	cout << "after "
	     << lCount
	     << "th retrieval step."
	     << lIDs[i]
	     << endl
	     << flush;

	CRelevanceLevelList lUserList;

	if(lCount){
	  lFound=lQuery.getFeedbackList(lIDs[i],
					lUserList,
					*lOutList);
	  if(lFound)
	    cout << endl << "ggggg" << endl;
	}else{
	  lFound=false;
	}
	
	if(lFound)
	  cout << endl << "fffff" << endl;


	delete lOutList;
	
	cout << "BEFORE "
	     << lCount+1
	     << "th retrieval step."
	     << endl;
	
	lOutList
	  =lQuery.startQuery(lUserList,
			     5,
			     0);
	lCount++;
	cout << "1.AFTER "
	     << lCount
	     << "th retrieval step."
	     << endl;

      }

      if(lFound){// should be true anyway
	lOutputFile << lIDs[i] 
		    << " " 
		    << lCount
		    << endl;
      }
    }
  }
  cout << "finished " 
       << flush
       << endl;
}
#endif
