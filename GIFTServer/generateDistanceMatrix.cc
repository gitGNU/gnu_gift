/***************************************
*
* program 
*
****************************************
*
* modification history:
*
* HM 090399 created the documentation
*
****************************************
*
* compiler defines used:
*
* QUERY
*
****************************************/

#include "../config.h"


#include "libMRML/include/TID.h"
#include <iostream>
#include "libMRML/include/CRelevanceLevelList.h"
#include "CInvertedFileAccessor.h"
#include "CInvertedFileQuery.h"
#include "libGIFTQuInvertedFile/include/CQNBestFullyWeighted.h"
#include "libGIFTQuInvertedFile/include/CWFBestFullyWeighted.h"
#include "libGIFTQuInvertedFile/include/CQNNoNormalization.h"
#include "libGIFTQuInvertedFile/include/CQNEuclideanLengthSquare.h"
#include "libGIFTQuInvertedFile/include/CQNSquareDFLogICFSum.h"
#include "libGIFTQuInvertedFile/include/CQNMaxDocumentFrequency.h"
#include "libGIFTQuInvertedFile/include/CWFBestProbabilistic.h"
#include "libGIFTQuInvertedFile/include/CWFClassicalIDF.h"
#include "libGIFTQuInvertedFile/include/CWFBinaryTerm.h"
#include "libGIFTQuInvertedFile/include/CWFStandardTF.h"
#include "libGIFTQuInvertedFile/include/CWFCoordinationLevel.h"
#include "CWFNormal.h"
#include "CWFColorHistogram.h"
#include "CWFTextureHistogram.h"

#include "CWFBlockOnly.h"
#include "CWFColorBlockOnly.h"
#include "CWFTextureBlockOnly.h"

#include <algorithm>
#include <cstdio>
#include <stdlib.h>
#include <string>


TID gID;

#define QUERY
//#define W_PRUNING


/****************************************
	compare by first element				
 ****************************************/
bool sortByFirst(const pair<TID,float>& l,
		 const pair<TID,float>& t){
  return 
    l.first
    >
    t.first;
}


/***************************************
*
* printListAsHTML - prints out the results as an html file to view it with a browser
*
****************************************
*
* modification history
*
* 
*
****************************************/

void printListAsHTML(CRelevanceLevelList* inQueryResultList,
		     string inQueryResultFileName){      
  if(inQueryResultList)
    {
      //an input-output-file
      ofstream lQueryResultFile(inQueryResultFileName.c_str(),
			       fstream::in+fstream::out);
      if(lQueryResultFile)
	{
	  lQueryResultFile << "<HTML>"
			   << endl 
			   << "<BODY>"
			   << endl
			   << "<TABLE>"
			   << endl
			   << "<TR>";
	  
	  int lCount=0;
	  for(CRelevanceLevelList::const_iterator i=inQueryResultList->begin();
	      i!=inQueryResultList->end() 
		&& lCount<NUMBER_OF_IMAGES;
	      i++,lCount++){
	    
            /* HACK. Squire. 981030 */
            if (lCount == 20)
              lQueryResultFile << "</TR>" << endl << "</TABLE>" << endl
                               << "<!--" // Comment out those with rank > 50
                               << endl;

	    if(!(lCount%5))
	      lQueryResultFile << "</TR>"
			       << endl
			       << "<TR>";
	    
	    lQueryResultFile << "<TD>" 
			     << endl;
	    (*i).outputHTML(lQueryResultFile);  
	    lQueryResultFile << "</TD>" 
			     << endl;
	    
	  }
	  
	  lQueryResultFile << "</TR>"
			   << endl
			   << "</TABLE>"
			   << endl 
                           << "-->" // Close the above comment
                           << endl
			   << "</BODY>"
			   << endl
			   << "</HTML>";
	}
    }else
      cout << "failed" 
	   << endl;
 
}



/***************************************
*
* main - main program, that reads the parameters and 
*        executes the creation of the inverted file or the start of a query
*
****************************************
*
* modification history
*
* 
*
****************************************/

main(int argc,
     char** argv){

  if(argc!=2){
    cout << "Usage: "
	 << argv[0]
	 << " <number_of_entries_to_skip>"
	 << endl;
  }
  //
  const int lSkipNumber=atoi(argv[1]);

  

  cout << "Constructing Accessor..." 
       << flush;

  CInvertedFileAccessor lAccessor(INVERTED_FILE_LOCATION,
				  OFFSET_FILE_LOCATION,
				  FEATURE_FILE_LOCATION,
				  FEATURE_DESCRIPTION_LOCATION);
  cout << "...after construction"
       << endl;
  
  
  cout << "Classic" << flush;
  CQueryNormalizer*   
    lDocumentNormalizer(new CQNNoNormalization(&lAccessor));
  CQueryNormalizer*   
    lQueryNormalizer(new CQNNoNormalization(&lAccessor));
  CWeightingFunction* lWeightingFunction=
    new CWFClassicalIDF(&lAccessor,
			lQueryNormalizer,
			lDocumentNormalizer);
  
  CRelevanceLevelList* lOutList;  
  
  cout << "Constructing Query-handler..."
	 << flush;
  CInvertedFileQuery lQuery(lAccessor,
			    *lWeightingFunction,
			    *lQueryNormalizer,
			    *lDocumentNormalizer);
#ifdef W_PRUNING
  lQuery.activateBlockingFeatures();
  //  lQuery.blockFeatureGroup(COL_POS);
  //lQuery.blockFeatureGroup(GABOR_HST); // in an old version this was wrong...
  lQuery.blockFeatureGroup(GABOR_POS);
#endif

  cout << "...after construction"
       << endl;
  
  
  list<TID> lAllIDs;
  
  lAccessor.getAllIDs(lAllIDs);
  
  lAllIDs.sort();
  
  {
    ofstream lTranslatorFile(DISTANCE_MATRIX_HOME
			     //no comma<-> one string
			     ".trans");
	int lCounter(0);
	for(list<TID>::const_iterator i=lAllIDs.begin();
	    i!=lAllIDs.end();
	    i++,
	      lCounter++){
	  lTranslatorFile << *i
			  << " "
			  << lCounter
			  << endl;
	}
  }
  
  fstream lDistanceFile(DISTANCE_MATRIX_HOME,
			 fstream::in+fstream::out);

  lDistanceFile.seekp(lAllIDs.size() * lSkipNumber * sizeof(float));
  //the main loop
  int lSkipCount=0;
  list<TID>::const_iterator i=lAllIDs.begin();
  for(;
      (i!=lAllIDs.end()) && (lSkipCount<lSkipNumber);
      i++,lSkipCount++){
    //do nothing
  };
  
  for(;
      i!=lAllIDs.end();
      i++){

    CRelevanceLevelList lRLL;
    lRLL.push_back(CRelevanceLevel(lAccessor.IDToURL(*i),1));
    
    cout << "The Query:"
	 << lRLL.back().getURL()
	 << endl;
    
    //here happens most of the calculation
    lOutList=lQuery.startQuery(lRLL,
			       lAllIDs.size());
    {
      if(lOutList){
	// copy the distances into a list
	list<pair<TID,float> > lLineVector;
	
	for(CRelevanceLevelList::iterator j=lOutList->begin();
	    j!=lOutList->end();
	    j++){
	  lLineVector.push_back(make_pair(lAccessor.URLToID(j->getURL()),
					  j->getRelevanceLevel()));
	}
	lLineVector.sort();//sortByFirst)
	
		
	//
	int lWriteCount=0;
	//copy this list into a file
	int lOld=0;
	for(list<pair<TID,float> >::const_iterator j=lLineVector.begin();
	    j!=lLineVector.end();
	    j++){
	  for(;
	      lOld < j->first;
	      lOld++){
	    float lBuffer(0.0);
	    lDistanceFile.write(&lBuffer,sizeof(lBuffer));
	    lWriteCount++;
	  }
	  lOld++;

	  cout << "["
	       << j->first
	       << ","
	       << j->second
	       << "]";
	  
	  float lDistance(j->second);
	  
	  lDistanceFile.write(&lDistance,sizeof(lDistance));
	  lWriteCount++;
	}
	for(;
	    lOld < lAllIDs.size();
	    lOld++){
	  float lBuffer(0.0);
	  lDistanceFile.write(&lBuffer,sizeof(lBuffer));
	  lWriteCount++;
	}
	lDistanceFile << flush;
	
	cout << endl
	     << "Written: "
	     << lWriteCount
 	     << "----------------------------------------Size: "
	     << lLineVector.size()
	     << endl;
      }
    }
    
    delete lOutList;
  }
}




