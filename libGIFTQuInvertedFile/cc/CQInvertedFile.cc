// -*- mode: c++ -*- 
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
#include "libMRML/include/mrml_const.h" // constants for parsing
#include "libMRML/include/my_throw.h"
#include "libMRML/include/my_diagnose.h"
/***************************************
*
* CInvertedFileQuery - this is the class that handles a query
*
****************************************
*
* modification history:
* WM 270999 Unifying code between diverging versions
*           main difference: Accessors are now pointers
*           as well as the weighting functions.
*           so lots of "." become "->"
*           CInvertedFileQuery is inherited from CInvertedFileQuery
*           which leads quite often to type casting
*           The constructors have changed.
* WM 08  99 Weighting functions are now pointers
*           QueryFeatureWeighters, CAccessor structure as well
*           configuration using CAlgorithm
*           use RTTI to find algorithms which do not fit the Query
*           ALL THIS TO MAKE FLEXIBLE RUNTIME CONFIGURATION POSSIBLE
*           filled in some documentation gaps
* HM 140999 put in the compiler define NOSCREENOUTPUT
* HM 140999 removed the second output at the Timestamp
* HM 130999 put in the seperate normalization of positive and negative input images
* HM 300899 searched the error which suppressed uneven relevance feedback
* HM 250899 removed some couts
* HM 160899 Work on the bug with the URL
* HM 100699 ScoreBoard pruning now as well with the percentage of features for the reduction
* HM 200599 print out a double timestamp with a better precision
* HM 050599 changes to find pruning error with histograms
* HM 030599 use the pruning variables
* HM 030599 put in a second function keepScore_pruning to implement 
*           the different pruning activities
* HM 030599 put in a second function keepScore_pruning to implement the different pruning activities
* HM 120499 print out the list with all the term frequencies
* HM 090499 tests with the pruning
* HM 090399 created the documentation
* WM 10  98 created file and most of the content
*
****************************************
*
* compiler defines used:
*
* _NO_PRINT_QFW if undefined, print the query feature weighters
* _NO_FIDPRINT if undefined, feature ids are printed to the screen while evaluating
* _RTTI_WORKS If this is defined, it is assumed by the code that RTTI works
*                in the resulting code
*          or else
*          Warning: there is a relevant constant in this file
*          "<10" in setAlgorithm.
*          it has something to do with the check, of an algorithm is considered
*          allowed for Inverted file queries.
*          is necessary, because of lack of RTTI 
* NOSCREENOUTPUT - if defined, nothing is printed to the screen
*
****************************************/

//#define _RTTI_WORKS

#include "FeatureExtraction/gift_features.h"
#include <algorithm>
#include <functional>
#include <string>

#include <math.h>
#include <assert.h>

#include "libGIFTAcInvertedFile/include/map_to_list.h"

#include "libGIFTAcInvertedFile/include/CDocumentFrequencyHash.h"
#include "libGIFTQuInvertedFile/include/CWeightingFunctionPointerHash.h"
#include "libGIFTQuInvertedFile/include/CQueryNormalizer.h"
#include "libGIFTQuInvertedFile/include/CWeightingFunction.h"

#include "libGIFTAcInvertedFile/include/CDocumentFrequencyList.h"
#include <vector>
#include "libGIFTAcInvertedFile/include/CInitializedDouble.h"
#include "libGIFTQuInvertedFile/include/CScoreBoard.h"

#include "libGIFTAcInvertedFile/include/CAcInvertedFile.h"
#include "libGIFTQuInvertedFile/include/CQInvertedFile.h"
#include "libMRML/include/GIFTExceptions.h"

#include "libMRML/include/CXMLElement.h"

#include <time.h>

#define _NO_FIDPRINT    
#define _NO_PRINT_QFW
#include <sys/time.h> /* to use the gettimeofday function */

//#define NOSCREENOUTPUT  

/** A factory for weighting functions with associated normalizers.
    The weighting functions are intended to be members of
    CQInvertedFile where they will be used.
*/
CWeighter* CWeighterFactory::newWeighter(const string& inID)const{
  assert(find(inID)!=end());

  return(find(inID)->second->clone());
};


/** constructor: initializes everything
    fills the map etc.
    The only accessors are the three get.*() Functions
*/
CWeighterFactory::CWeighterFactory(){
  
  /* something FISHY here FIXME with Salton Buckley paper */

  (*this)["BestFullyWeighted"]=
    new CWeighter(new CWFBestFullyWeighted(0,0,0),
		  new CQNEuclideanLengthSquare(0),
		  new CQNMaxDocumentFrequency(0));
  (*this)["ClassicalIDF"]=
    new CWeighter(new CWFClassicalIDF(0,0,0),
		  new CQNNoNormalization(0),
		  new CQNNoNormalization(0));
  (*this)["BestProbabilistic"]=
    new CWeighter(new CWFBestProbabilistic(0,0,0),
		  new CQNNoNormalization(0),
		  new CQNMaxDocumentFrequency(0));
  (*this)["BinaryTerm"]=
    new CWeighter(new CWFBinaryTerm(0,0,0),
		  new CQNNoNormalization(0),
		  new CQNNoNormalization(0));
  (*this)["Probability"]=
    new CWeighter(new CWFProbability(0,0,0),
		  new CQNNoNormalization(0),
		  new CQNNoNormalization(0));
  (*this)["StandardTF"]=
    new CWeighter(new CWFStandardTF(0,0,0),
		  new CQNNoNormalization(0),
		  new CQNEuclideanLengthSquare(0));
  (*this)["Coordination"]=
    new CWeighter(new CWFCoordinationLevel(0,0,0),
		  new CQNNoNormalization(0),
		  new CQNNoNormalization(0));
  
};

CWeighterFactory::~CWeighterFactory(){
  for(iterator i=begin();
      i!=end();
      i++){
    delete i->second;
  }
}

/***************************************
*
* stampTime - prints the time out on the screen
*
****************************************
*
* modification history
*
* WM 12  98  creation
* 
*
****************************************/

/* to print out the time for performance measurements */
static void stampTime(){
  cout << "[Timestamp"
       << 1.0*clock()/CLOCKS_PER_SEC
       << "]"
       << endl;

  /*  void    *garbage; *//* only used for the call of the function */
  /*timeval *result; */ /* result structure for the time function */
    
  /*  result=new(timeval);
      gettimeofday(result, garbage);
      cout<<"[Timestamp"
      <<result->tv_sec
      <<";"
      <<result->tv_usec
      <<"]"
      <<endl;*/
}



/***************************************
*
* timeReached - returns true if a special timelimit is reached, 
*               used to make queries with an upper time level
*
****************************************
*
* modification history
*
* @Author Henning Mueller 200299 
*
****************************************/
bool timeReached(double timeLimit)
{
  if(1.0*clock()/CLOCKS_PER_SEC >= timeLimit)
    return true;
  else
    return false;
}



/***************************************
*
* Constructor
*
****************************************
*
* modification history
*
* 
*
****************************************/
CQInvertedFile::CQInvertedFile(CAccessorAdminCollection& inAccessorAdminCollection,
			       CAlgorithm& inAlgorithm
			       ):
  CQuery(inAccessorAdminCollection,
	 inAlgorithm){
  assert(&inAccessorAdminCollection);
  assert(&inAlgorithm);
  assert(&inAlgorithm==mAlgorithm);
  mDeb=mAlgorithm;
  mAlgorithm->activate();
  my_diagnose(&inAlgorithm);

  // mproxy has been filled in a reasonable way 
  // by CQuery::CQuery
  
  pair<bool,string> lSubType=inAlgorithm.stringReadAttribute("cui-sub-type");

  if(lSubType.first){
    if(lSubType.second=="mysql"){
      mAccessor=mAccessorAdmin->openAccessor("if_mysql");
    }else{
      mAccessor=mAccessorAdmin->openAccessor("inverted_file");
    }
  }else{
    mAccessor=mAccessorAdmin->openAccessor("inverted_file");
  }
  cout << "1st Checking accessor" << endl;
  //  mAccessor->checkNPrint();

  //there is something wrong between the generated 
  //accessor and what we want
  if(!mAccessor){
    cout << "throwing: "
	 << (VEWrongAccessor("InvertedFileQuery"))
	 << endl
	 << flush;
    try{
      my_throw(VEWrongAccessor("InvertedFileQuery"));
    }
    catch(...){
      cout << "locally caught and rethrown"
	   << flush;
      my_throw(VEWrongAccessor("InvertedFileQuery"));
    }
  }

  //here do additional things with the algorithm, if wanted and needed
  //which probably is the case
  init();
  cout << "++Accessor: " << mAccessor << flush << endl;
  cout << "Checking Accessor II"
       << endl;
  //mAccessor->checkNPrint();
  
  cout << "CQInvertedFile THIS:" << this << endl;
};
CQInvertedFile::~CQInvertedFile(){

  cout << "begin KILLING INVERTED FILE QUERY" << endl;
  assert(mDeb==mAlgorithm);
  if(mAlgorithm){
    mAlgorithm->check();
    pair<bool,string> lSubType=mAlgorithm->stringReadAttribute("cui-sub-type");
    if(lSubType.first){
      if(lSubType.second=="mysql"){
	mAccessorAdmin->closeAccessor("if_mysql");
      }else{
	mAccessorAdmin->closeAccessor("inverted_file");
      }
    }else{
      mAccessorAdmin->closeAccessor("inverted_file");
    }
  }
  mAlgorithm->deActivate();
  cout << "end KILLING INVERTED FILE QUERY" << endl;
}

/***************************************
 *
 * set the Algorithm.
 * same scheme as in setCollection
 *
 ***************************************/
bool CQInvertedFile::setAlgorithm(CAlgorithm& inAlgorithm){
  cout << "SETALGORITHM" << endl;

  if(mAlgorithm->getCollectionID()==inAlgorithm.getCollectionID()){
    
    return true;
    
  }else{
    cout << "OPENACCESSOR" << endl;
    //close the old collection
    mAccessorAdmin->closeAccessor("inverted_file");

    //
    mAccessorAdmin=&mAccessorAdminCollection->getProxy(inAlgorithm.getCollectionID());
    mAccessor=mAccessorAdmin->openAccessor("inverted_file");

    //
    return (mAccessor && CQuery::setAlgorithm(inAlgorithm));
  }
};


/***************************************
*
* Constructor
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CQInvertedFile::init(){
  int i;

  if(mAlgorithm){
    string lString("");
    mAlgorithm->toXML(lString,5);
    
    cout << "çççççççççççççççççççççççççççççççççççççççç" << endl
	 << lString
	 << "çççççççççççççççççççççççççççççççççççççççç" << endl
	 << flush;
  }
    
  if(mAlgorithm){
    mWeighter=mWeighterFactory
      .newWeighter(mAlgorithm
		   ->stringReadAttribute(mrml_const::cui_weighting_function).second);
  }

  mQueryFeatureWeighters= 
    new CWeightingFunctionPointerHash(*mWeighter->getWeightingFunction());


  mWeighter->setAccessor((*(CAcInvertedFile*)mAccessor));


  /* parameter for the seperate normalization of positive and 
     negative feedback */
  /*  mSeperateNormalizationofPositiveandNegativeImages=false;*/

  /* turns off the blocking of features */
  mBlockingOn=false; 

  /* clears the file with the selective blocking, all features
     need to be blocked actively, if wanted */
  for(i=0;i<MAXIMUMNUMBEROFEATUREGROUPS;i++)
    {
      mBlockingArray[i]=false;
    }
  
  /* parameters for pruning (Henning)*/
  mPruningUsed=false;
  mScoreBoardPruningUsed=false;
  mNumberofUsedScoreBoardPrunings=0;
  mFeaturePruningUsed=false;
  mTimePruningUsed=false;
  mStoppingTime=0;
  mPercentageofFeatures=0;
  for(i=0;i<MAX_SCOREBOARD_PRUNINGS;i++)
    {
      mScoreBoardPruningArray[i].stopAfterFeature=0;
      mScoreBoardPruningArray[i].reduceTo=0;
   }
  mEvaluateAfterPruning=false;


  /* now set the pruning parameters 
     this is made by me, WOLFGANG, so there can be some
     BUGS.
  */
  if(mAlgorithm){

    string lString("");
    mAlgorithm->toXML(lString,5);
    my_diagnose(lString);


    releaseBlockingFeatures();
    {
      pair<bool,bool> lBlock=
	mAlgorithm->boolReadAttribute(string(mrml_const::cui_block_color_histogram));

      cout << "%%%%%%%%%%%%%%%%%%%%"
	   << mAlgorithm->stringReadAttribute(string(mrml_const::cui_block_color_histogram)).second
	   << endl;
      if(lBlock.first && lBlock.second){
	blockFeatureGroup(COL_HST);
      }
    }
    {
      pair<bool,bool> lBlock=
	mAlgorithm->boolReadAttribute(string(mrml_const::cui_block_texture_histogram));
      if(lBlock.first && lBlock.second){
	blockFeatureGroup(GABOR_HST);
      }
    }
    {
      pair<bool,bool> lBlock=
	mAlgorithm->boolReadAttribute(string(mrml_const::cui_block_color_blocks));
      if(lBlock.first && (lBlock.second)){
	blockFeatureGroup(COL_POS);
      }
    }
    {
      pair<bool,bool> lBlock=
	mAlgorithm->boolReadAttribute(string(mrml_const::cui_block_texture_blocks));
      if(lBlock.first && (lBlock.second)){
	blockFeatureGroup(GABOR_POS);
      }
    }
    {
      pair<bool,double> lTimeCutoffPoint=
	mAlgorithm->doubleReadAttribute(string(mrml_const::cui_pr_time_cutoff_point));
      if(lTimeCutoffPoint.first){
	useTimePruning(lTimeCutoffPoint.second);
      }
    }
    {
      pair<bool,double> lPercentageOfFeatures=
	mAlgorithm->doubleReadAttribute(string(mrml_const::cui_pr_percentage_of_features));
      if(lPercentageOfFeatures.first){
	useFeaturePruning(lPercentageOfFeatures.second);
      }
    }
    {
      pair<bool,double> lScoreBoardReducedAt=
	mAlgorithm->doubleReadAttribute(string(mrml_const::cui_pr_score_board_reduced_at));

      pair<bool,int> lScoreBoardReducedTo=
	mAlgorithm->longReadAttribute(string(mrml_const::cui_pr_score_board_reduced_to));

      if(lScoreBoardReducedAt.first
	 &&
	 lScoreBoardReducedTo.first){
	useScoreBoardPruning(lScoreBoardReducedAt.second,
			     lScoreBoardReducedTo.second);
      }
    }
    {
      pair<bool,int> lModulo=
	mAlgorithm->longReadAttribute(string(mrml_const::cui_pr_modulo));

      pair<bool,int> lModuloClass=
	mAlgorithm->longReadAttribute(string(mrml_const::cui_pr_modulo_class));
      
      if(lModulo.first){
	mModulo=lModulo.second;
	if(lModuloClass.first){
	  mModuloClass=lModuloClass.second;
	}else{
	  mModuloClass=0;
	}
      }else{
	mModulo=0;
	mModuloClass=0;
      }
    }
  }
  cout << "Init finished " 
       << flush
       << endl;
};


/***************************************
*
* buildQueryHash - Creates a list with all the features which are in all the different images
*                  in the query, contains one "artificial" query image
*
****************************************
*
* modification history
* initally done by Henning, this has been shortened quite a bit by
* Wolfgang (The old code made a list which was then copied. Now
* we copy directly the values we need, thus eliminating the need
* for alloc/dealloc, and cutting some possibilites for bugs)
****************************************/
void CQInvertedFile::buildQueryHash(CRelevanceLevelList& inQuery,
				    CWeightingFunctionPointerHash& 
				    outQueryFeatureWeighters)const{
  

  /* checks all the relevant images which were selected */
  for(CRelevanceLevelList::const_iterator i=inQuery.begin();
      i!=inQuery.end();
      i++){
    //Get the feature list for the URL of *i (that means: for each input image)
#ifndef NOSCREENOUTPUT
    cout<< "input image to URL: _"
	<< (*i).getURL() 
	<< "_"
	<< endl
	<< flush;
#endif
    
    CDocumentFrequencyList* lReadFeatures=
      ((CAcInvertedFile*)mAccessor)->URLToFeatureList((*i).getURL());

    /* 
       Build a new feature list without blocked features.
       There are several cases
    */
    if(featuresBlocked() && !mModulo){// adding features which are not blocked
      for(CDocumentFrequencyList::iterator 
	    j=lReadFeatures->begin();
	  j!=lReadFeatures->end();
	  j++
	  ){
	if(isBlocked(((CAcInvertedFile*)mAccessor)->getFeatureDescription(j->getID()))==false){
	  outQueryFeatureWeighters.addFeature((*i).getRelevanceLevel(),
					      *j);
	}
      } 
    }// END: adding features which are not blocked
    if(featuresBlocked() && mModulo){// adding features which are not blocked,
      // and for which the feature number has the right modulo
      // use of this: dispatching multiple query processors
      for(CDocumentFrequencyList::iterator 
	    j=lReadFeatures->begin();
	  j!=lReadFeatures->end();
	  j++
	  ){
	if((isBlocked(((CAcInvertedFile*)mAccessor)->getFeatureDescription(j->getID()))==false)
	   && ((j->getID()) % mModulo == mModuloClass)){
	  outQueryFeatureWeighters.addFeature((*i).getRelevanceLevel(),
					      *j);
	} /* end of the loop for all the features of one image */
      } /* end of removing the blocked features from the list */
    }
    if(!featuresBlocked() && mModulo){
      for(CDocumentFrequencyList::iterator 
	    j=lReadFeatures->begin();
	  j!=lReadFeatures->end();
	  j++
	  ){
	if((j->getID()) % mModulo == mModuloClass){// adding features with the right modulo
	  /* adds one of the features for a special image to the list */
	  outQueryFeatureWeighters.addFeature((*i).getRelevanceLevel(),
					      *j);
	} /* end of the loop for all the features of one image */
      } /* end of removing the blocked features from the list */
    }
    if(!featuresBlocked() && !mModulo){// adding all features
      for(CDocumentFrequencyList::iterator 
	    j=lReadFeatures->begin();
	  j!=lReadFeatures->end();
	  j++
	  ){
	/* adds one of the features for a special image to the list */
	outQueryFeatureWeighters.addFeature((*i).getRelevanceLevel(),
					    *j);
      }
    }
    
#ifndef NOSCREENOUTPUT
    cout << "gotten HERE"
	 << flush
	 << endl;
#endif  

   
#ifdef PRINT
    cout << endl
	 << (*i).getURL()
	 << " "
	 << lRelevanceLevel
	 << endl;
#endif
    delete(lReadFeatures);    
  }/* end of the loop for every image in the query */
}



/***************************************
*
* buildNormalizedQueryHash
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CQInvertedFile::
buildNormalizedQueryHash(double inPositiveRelevanceSum,
			 double inNegativeRelevanceSum,
			 CWeightingFunctionPointerHash& inoutQFW) const{
  //Just to make sure...
  mWeighter->getQueryNormalizer()->reset();
  mWeighter->getDocumentNormalizer()->reset();
  
  /* for every weighting function, meaning for every element in the list of important features */


  {//for limiting the scope of the following variable definitions
    CQueryNormalizer* lDocumentNormalizer=mWeighter->getDocumentNormalizer();
    CQueryNormalizer* lQueryNormalizer=mWeighter->getQueryNormalizer();
    for(CWeightingFunctionPointerHash::const_iterator j=inoutQFW.begin();
	j!=inoutQFW.end();
	j++)
      {
	/* calculates the relevant values for the normalizing of the function */
	lQueryNormalizer->considerQueryFeature(*(*j).second);
	lDocumentNormalizer->considerQueryFeature(*(*j).second);
	
	(*j).second->setRelevanceSum(inPositiveRelevanceSum,
				     inNegativeRelevanceSum);
	(*j).second->preCalculate();
    }
  }
}

/***************************************
*
* buildNormalizedQueryList
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CQInvertedFile::
buildNormalizedQueryList(double inPositiveRelevanceSum,
			 double inNegativeRelevanceSum,
			 CWeightingFunctionPointerHash& inoutQFW,
			 CWeightingFunctionPointerList& outQFW) const 
{
  //Just to make sure...
  mWeighter->getQueryNormalizer()->reset();
  mWeighter->getDocumentNormalizer()->reset();


  buildNormalizedQueryHash(inPositiveRelevanceSum,
			   inNegativeRelevanceSum,
			   inoutQFW);

#ifndef NOSCREENOUTPUT
  cout<<"normalized List build\n";
  stampTime();
#endif

  for(CWeightingFunctionPointerHash::const_iterator i=inoutQFW.begin();
      i!=inoutQFW.end();
      i++){
    outQFW.push_back((*i).second);
  }

  
  outQFW.sort(CSortPointers_WF <CSortByAbsQueryFactor_WF>());
  outQFW.reverse();


  /* test to print out the list with the sorted features */
#ifndef _NO_PRINT_QFW
  cout << "--------------------"
       << "The query feature weighters: "
       << endl;

  for(CWeightingFunctionPointerList::const_iterator j=outQFW.begin();
	    j!=outQFW.end();
	    j++)
	    {
	    cout<<(*j)->getID();
	    cout<<";";
	    cout<<((CAcInvertedFile*)mAccessor)->FeatureToCollectionFrequency((*j)->getID());
	    
	    cout<<";";
	    cout<<(*j)->getTermFrequency();
	    cout<<";";
	    cout<<((CAcInvertedFile*)mAccessor)->getFeatureDescription((*j)->getID());
	    cout<<"\n";
    }
#endif
}







/***************************************
*
* query - here the query takes place with a list of input images
*
****************************************
*
* modification history
*
* 
*
****************************************/
CIDRelevanceLevelPairList* CQInvertedFile::fastQuery(const CXMLElement& inQuery,
						     int inNumberOfInterestingImages,
						     double inThreshold){

  cout << "--Accessor: " 
       << flush 
       << mAccessor 
       << flush 
       << endl;

  mQueryFeatureWeighters->clearFeatures();


  double lPositiveSum(0);
  double lNegativeSum(0);


  CRelevanceLevelList lQuery;

  // In the query tree I got, i simply look for 
  // the element containing a user-relevance-list
  
  bool lIsEmpty(true);// is there no image in the query?
  for(list<CXMLElement*>::const_iterator i=inQuery.child_list_begin();
      i!=inQuery.child_list_end();
      i++){

//     cout << "I:The name of this tree element: "
// 	 << endl
// 	 << (*i)->getName()
// 	 << endl;
    if((*i)->getName()=="cui-inverted-file-query-by-feature"){
      for(list<CXMLElement*>::const_iterator j=(*i)->child_list_begin();
	  j!=(*i)->child_list_end();
	  j++){
	if((*j)->getName()=="cui-term-frequency-element"){
	  pair<bool,int> lFeatureID((*j)->longReadAttribute("feature-id"));
	  pair<bool,double> lTermFrequency((*j)->doubleReadAttribute("term-frequency"));
	  
	  if(lFeatureID.first){
	    if(lTermFrequency.first){
	        mQueryFeatureWeighters->addFeature(lTermFrequency.second,CDocumentFrequencyElement(lFeatureID.second,
												   1));
	    }else{
	      mQueryFeatureWeighters->addFeature(1,CDocumentFrequencyElement(lFeatureID.second,
									     1));
	    }
	  }
	}
      }
    }
    if((*i)->getName()==mrml_const::user_relevance_element_list){
      for(list<CXMLElement*>::const_iterator j=(*i)->child_list_begin();
	  j!=(*i)->child_list_end();
	  j++){
// 	cout << "J:The name of this tree element: "
// 	     << endl
// 	     << (*j)->getName()
// 	     << endl;
	if((*j)->getName()==mrml_const::user_relevance_element){
	  if(((*j)->stringReadAttribute(mrml_const::image_location).first)
	     &&
	     ((*j)->stringReadAttribute(mrml_const::user_relevance).first)){
	    
	    if(fabs((*j)->doubleReadAttribute(mrml_const::user_relevance).second)>0.001){
	      lQuery.push_back(CRelevanceLevel((*j)->stringReadAttribute(mrml_const::image_location).second,
					       (*j)->doubleReadAttribute(mrml_const::user_relevance).second));
	      if(lQuery.back().getRelevanceLevel()>0){
		lIsEmpty=false;
		lPositiveSum+=lQuery.back().getRelevanceLevel();
	      }else{
		lIsEmpty=false;
		lNegativeSum+=lQuery.back().getRelevanceLevel();
	      }
	    }
	  }
	}
      }
    }
  }
  
  if(lIsEmpty){
    cout << "THE QUERY IS EMPTY" << endl;
    //which means, random images are requested
    return getRandomIDs(inNumberOfInterestingImages);
    cout << "RETURNING" << endl;
  }



  //... but more successfully
  cout << "Query number of documents: "
       << lQuery.size()
       << "The document: ";
  if(lQuery.size()){
    cout << lQuery.front().getRelevanceLevel()
	 << ","
	 << lQuery.front().getURL()
	 << "\n"
	 << endl;
  }else{
    cout << "NO DOCUMENT"
	 << endl;
  }
#ifndef NOSCREENOUTPUT
#endif

  //in case of an empty query
  if(!lQuery.size()){
    return new CIDRelevanceLevelPairList();
  }

  double lRelevanceSum=0; /* */
  
  //resets the normalizer for the query
  mWeighter->getQueryNormalizer()->reset();
  mWeighter->getDocumentNormalizer()->reset();

#ifndef NOSCREENOUTPUT
  cout << "building query" 
       << endl 
       << flush;
  stampTime();
#endif  

  /* creates a list of a all the features in all the input images */
  buildQueryHash(lQuery,
		 *mQueryFeatureWeighters);
  
#ifndef NOSCREENOUTPUT
  stampTime();
  cout << "..finished:left:" << mQueryFeatureWeighters->size() << endl;
#endif
  
  /* this is a list with all the weightings for every feature */
  CWeightingFunctionPointerList lParameters;
  
  /* and the list for every feature and its weighting is created here */
  buildNormalizedQueryList(lPositiveSum,
			   lNegativeSum,
			   *mQueryFeatureWeighters,
			   lParameters);


  return fastQueryByFeature(lParameters,
			    inNumberOfInterestingImages,
			    inThreshold);
}

/**
 *
 * Assuming that a correct CWeightingFunctionPointerHash has been built by fastQuery 
 * (or another function), this function will do the rest
 *
 */
CIDRelevanceLevelPairList* CQInvertedFile::fastQueryByFeature(const CWeightingFunctionPointerList& inQuery,
							      int inNumberOfInterestingImages,
							      double inThreshold){
  

#ifndef NOSCREENOUTPUT
  cout << "query." << flush << endl;
  stampTime();
#endif

  //A score for each document, 
  //each score is initialized with zero
  CScoreBoard lScoreBoard;
  
  double lQueryScore;
  
  /* calculates the value for the answer images */
  if (mPruningUsed)
    {
      cout << "Pruning used!"
	   << endl;


      lQueryScore=(keepScorePruning(lScoreBoard,
				    inQuery,
				    true,
				    inNumberOfInterestingImages
				    ));
    }
  else
    {
      cout << "Pruning NOT used!"
	   << endl;

      lQueryScore=(keepScore(lScoreBoard,
			     inQuery,
			     true));
    }

#ifndef NOSCREENOUTPUT
  stampTime();
  cout << ".query" << flush << endl;
#endif

  //Normalizing scores by query score and building a list from it
  CIDRelevanceLevelPairList* lReturnValue=new CIDRelevanceLevelPairList(); // list for the contents of the ScoreBoard
  for(CScoreBoard::iterator i=lScoreBoard.begin();
      i!=lScoreBoard.end();
      i++){
    (*i).second/=fabs(lQueryScore);
    lReturnValue->push_back(CIDRelevanceLevelPair(i->first,
						  i->second));
  }
  
#ifndef NOSCREENOUTPUT
  cout << "presort" << flush << endl;
  stampTime();
#endif
  
  lReturnValue->sort();
  lReturnValue->reverse();
  
  //Iterate over the wanted elements
  CIDRelevanceLevelPairList::iterator iWantedElements=lReturnValue->begin(); 

  //skip inNumberOfInterestingImages and delete the rest of the
  //output list
  for(int lNumberOfSkippedImages=0;
      (lNumberOfSkippedImages
       <
       inNumberOfInterestingImages)
	&& 
	(iWantedElements!=lReturnValue->end());
      iWantedElements++,
	lNumberOfSkippedImages++
      ){
  }
  
  if(iWantedElements!=lReturnValue->end()){
    lReturnValue->erase(iWantedElements,lReturnValue->end());
  }
  
  
  //Delete all results below threshold if threshold>0.0
  if(inThreshold>0.0001){
    for(CIDRelevanceLevelPairList::iterator i=lReturnValue->begin();
	i!=lReturnValue->end();){
      if((*i).getRelevanceLevel()<inThreshold){
	cout << "eliminating:" 
	     << inNumberOfInterestingImages
	     << ":"
	     << inThreshold
	     << ">"
	     << (*i).getRelevanceLevel()
	     << endl;
	
	lReturnValue->erase(i++);
      }else{
	i++;
      }
    }
  }

#ifndef NOSCREENOUTPUT
  stampTime();
  cout << "postsort" 
       << flush 
       << endl;
#endif
  return lReturnValue;
}




/***************************************
*
* keepScore - this is the main part to calculate the score by iterating the features
*
****************************************
*
* modification history
*
* HM 030599 removed all pruning activities from here
* HM   0199 added pruning activities
* WM   1098 created function
*
****************************************/
double CQInvertedFile::keepScore(CScoreBoard& inoutScoreBoard,
				     const CWeightingFunctionPointerList& inFeatures,
				     bool inPositive)const{
  double lQueryScore=0;
  int lMaximumNumberofEvaluatedFeatures=int (inFeatures.size()*0.9);

  //For all query features
  int lCount=0;
  for(CWeightingFunctionPointerList::const_iterator i=inFeatures.begin();
      (i!=inFeatures.end()); 
      i++)
    {
    
      lCount++;



#ifndef _NO_FIDPRINT    
      cout << "[FID"
	   << dec
	   << (*i)->getID()
	   << "]"
	   << flush;
#endif

      /* adjusts the query score for one more feature, this does it for the
         query image itself to have a number to normalize with */
      lQueryScore+=
	(*i)->applyOnThis();
    
      //load list of documents which contain features
      CDocumentFrequencyList* lOneFeatureResult=
	((CAcInvertedFile*)mAccessor)->FeatureToList((*i)->getID());

      //then adjust the score for the documents
      if(lOneFeatureResult)
	{
	  for(CDocumentFrequencyList::iterator j=lOneFeatureResult->begin();
	      j!=lOneFeatureResult->end();
	      j++)
	    {
	      inoutScoreBoard(*(*i),*j);//one could say inoutScoreBoard.adjust(.,.)
	    };
	}
      else{
	cerr << "FAILED:OneFeatureResult" 
	     << endl 
	     << flush;
      }
      delete lOneFeatureResult;

    }

  return lQueryScore;
};




/***************************************
*
* keepScore_pruning - this is the main part to calculate the score by iterating the features
*
****************************************
*
* modification history
*
* 
*
****************************************/
double CQInvertedFile::keepScorePruning(CScoreBoard& inoutScoreBoard,
					    const CWeightingFunctionPointerList& inFeatures,
					    bool inPositive,
					    int inElementsToRetrieve
					    )const{
  double lQueryScore=0;
  int lNumberofFeatures=inFeatures.size();
  int lMaximumNumberofEvaluatedFeatures=int (lNumberofFeatures*mPercentageofFeatures/100);

  cout << "Pruning: I will evaluate "
       << lMaximumNumberofEvaluatedFeatures
       << " Features."
       << flush
       << endl;

  int lNextScoreBoardNumber=0;

  //For all query features
  int lCount=0;
  for(CWeightingFunctionPointerList::const_iterator i=inFeatures.begin();
      (i!=inFeatures.end())//FIXME for loop comparison
	&& (!(mTimePruningUsed 
	      && (timeReached(mStoppingTime)) 
	      && (lCount>100)))
	&& (!(mFeaturePruningUsed 
	      && (lCount==lMaximumNumberofEvaluatedFeatures))); 
      i++)
    {
 
      lCount++;
      
      if(mScoreBoardPruningUsed)
	{
	  if(mNumberofUsedScoreBoardPrunings>lNextScoreBoardNumber)
	    {
	      if(lCount== 
		 (int (mScoreBoardPruningArray[lNextScoreBoardNumber]
		       .stopAfterFeature*lNumberofFeatures)))
		{
#ifndef NOSCREENOUTPUT
		  cout << mScoreBoardPruningArray[lNextScoreBoardNumber].reduceTo;
		  cout<<"\n";
#endif
		  inoutScoreBoard
		    .limitNumberTo(mScoreBoardPruningArray
				   [lNextScoreBoardNumber]
				   .reduceTo
				   *inElementsToRetrieve);
		  inoutScoreBoard.setIgnore();
		  lNextScoreBoardNumber++;
		}
	    }
	}

#ifndef _NO_FIDPRINT    
      cout << "[FID"
	   << dec
	   << (*i)->getID()
	   << "]"
	   << flush;
#endif

      /* adjusts the query score for one more feature, this does it for the
         query image itself to have a number to normalize with */
      lQueryScore+=
	(*i)->applyOnThis();
    
      //load list of documents which contain features
      CDocumentFrequencyList* lOneFeatureResult=
	((CAcInvertedFile*)mAccessor)->FeatureToList((*i)->getID());

      //then adjust the score for the documents
      if(lOneFeatureResult)
	{
	  for(CDocumentFrequencyList::iterator j=lOneFeatureResult->begin();
	      j!=lOneFeatureResult->end();
	      j++)
	    {
	      inoutScoreBoard(*(*i),
			      *j);//one could say inoutScoreBoard.adjust(.,.)
	    };
	}
      else
	cerr << "FAILED:OneFeatureResult" 
	     << endl 
	     << flush;
      
      delete lOneFeatureResult;

    }
  /* this is only to reduce the list to a special number at the end */
  /*      inoutScoreBoard.limitNumberTo(20);*/


  return lQueryScore;
};





/***************************************
*
* URLToScore - returns the score for one valid URL
*
****************************************
*
* modification history
*
* 
*
****************************************/
double CQInvertedFile::URLToScore(const string& inURL,
				      const CWeightingFunctionPointerHash&
				      inFeatures
				      )const{
  CDocumentFrequencyList* lDocumentFeatures=
    ((CAcInvertedFile*)mAccessor)->URLToFeatureList(inURL);
  TID lID=((CAcInvertedFile*)mAccessor)->URLToID(inURL).second;

  double lResult=FeatureListToScore(lID,
				    *lDocumentFeatures,
				    inFeatures);
  delete lDocumentFeatures;
  return lResult;
}



/***************************************
*
* DIDToScore - returns the score for one valid document ID
*
****************************************
*
* modification history
*
* 
*
****************************************/
double CQInvertedFile::DIDToScore(TID inDID,
				      const CWeightingFunctionPointerHash&
				      inFeatures
				      )const{

  CDocumentFrequencyList* lDocumentFeatures=
    ((CAcInvertedFile*)mAccessor)->DIDToFeatureList(inDID);

  double lResult=FeatureListToScore(inDID,
				    *lDocumentFeatures,
				    inFeatures);
  delete lDocumentFeatures;
  return lResult;
}



/***************************************
*
* FeatureListToScore
*
****************************************
*
* modification history
*
* 
*
****************************************/
double CQInvertedFile::FeatureListToScore(TID inDID,
					      const CDocumentFrequencyList& 
					      inDocumentFeatures,
					      const CWeightingFunctionPointerHash&
					      inFeatures)const{
  //Embarassing, I calculate here each time the value for the application
  //of the query on itself...
  //...after all, this only causes a factor of 2 in the speed :-I
  // will be fixed when we have a caching method
  double lQueryScore=0;
  /*  for(CWeightingFunctionPointerHash::const_iterator i=inFeatures.begin();
      i!=inFeatures.end();
      i++){
    lQueryScore+=(*i).second->applyOnThis();
  }FIXME*/
  

  double lLocalScore=0;
  //
  for(CDocumentFrequencyList::const_iterator i=inDocumentFeatures.begin();
      i!=inDocumentFeatures.end();
      i++){
    //Input was a list of FeatureID/Document-Frequency-Pairs
    //We have to translate this into DocumentID-DocumentFrequency-Pairs
    CWeightingFunctionPointerHash::const_iterator 
      lWeightingFunction=inFeatures.find((*i).getID());

    if(lWeightingFunction!=inFeatures.end()){
      lLocalScore=(*lWeightingFunction).second->
	apply(CDocumentFrequencyElement(inDID,
					(*i).getDocumentFrequency()));
    }
  }
  return lLocalScore/fabs(lQueryScore);
};



/***************************************
*
* buildNormalizedQueryHash
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CQInvertedFile::buildNormalizedQueryHash(const CRelevanceLevel& inQuery,
					      CWeightingFunctionPointerHash& 
					      outQueryFeatureWeighters)const{
  CRelevanceLevelList lQuery;
  lQuery.push_back(inQuery);
  buildQueryHash(lQuery,
		 outQueryFeatureWeighters);
  buildNormalizedQueryHash(1,
			   1,
			   outQueryFeatureWeighters);
}



/***************************************
*
* turnOnSeperateNormalization - turns on the seperate normalization
*
***************************************
*
* modification history
*
* 
*
****************************************/
void turnOnSeperateNormalization()
{
  /*  mSeperateNormalizationofPositiveandNegativeImages=true;*/
}


/***************************************
*
* activateBlockingFeatures - sets the variable to block groups of features
*
***************************************
*
* modification history
*
* 
*
****************************************/
void CQInvertedFile::activateBlockingFeatures()
{
  mBlockingOn=true;
}



/***************************************
*
* releaseBlockingFeatures - this turns off the blocking of features
*
***************************************
*
* modification history
*
* @Author Henning Mueller  
* HM 240399 created
*
****************************************/
void CQInvertedFile::releaseBlockingFeatures()
{
  mBlockingOn=false;
}

/***************************************
*
* featuresBlocked - returns true if the features are blocked
*
***************************************
*
* modification history
*
* @Author Henning Mueller  
* HM 240399 created
*
****************************************/
bool CQInvertedFile::featuresBlocked()const
{
  if(mBlockingOn==true)
    return true;
  else
    return false;
}



/***************************************
*
* blockFeatureGroup - this blocks one special group of features
*
***************************************
*
* modification history
*
* @Author Henning Mueller  
* HM 240399 created
*
****************************************/
void CQInvertedFile::blockFeatureGroup(const int inFeatureNumber)
{
  cout << endl
       << "§§§§§§§§§§§§§§§§§§§§blocking feature group: "
       << inFeatureNumber
       << endl;
 mBlockingArray[inFeatureNumber]=true;
 activateBlockingFeatures();
}



/***************************************
*
* unblockFeatureGroup - this releases the blocking of one feature group
*
***************************************
*
* modification history
*
* @Author Henning Mueller  
* HM 240399 created
*
****************************************/
void CQInvertedFile::unblockFeatureGroup(const int inFeatureNumber)
{
  mBlockingArray[inFeatureNumber]=false;
}

/***************************************
*
* isBlocked - returns true if the feature is blocked and false if not
*
***************************************
*
* modification history
*
* @Author Henning Mueller  
* HM 240399 created
*
****************************************/
bool CQInvertedFile::isBlocked(const int inFeatureNumber)const
{
  if(mBlockingArray[inFeatureNumber]==true)
    return true;
  else
    return false;
}



/***************************************
*
* sets back all the variables for the pruning
*
***************************************
*
* modification history
*
* @Author Henning Mueller  
* HM 030599 created
*
****************************************/
void CQInvertedFile::releaseAllPrunings()
{
  releaseFeaturePruning();
  releaseTimePruning();
  releaseScoreBoardPruning();
}


/***************************************
*
* activates the feature pruning with evaluating a certain percentage of the features
*
***************************************
*
* modification history
*
* @Author Henning Mueller  
* HM 030599 created
*
****************************************/
void CQInvertedFile::useFeaturePruning(double percentage)
{
  cout << endl
       << "§§§§§§§§§§§§§§§§§§§§USE FEATURE PRUNING: "
       << percentage
       << endl;

  mPruningUsed=true;
  mFeaturePruningUsed=true;
  mPercentageofFeatures=percentage;
}


/***************************************
*
* set the variables back to its normal status
*
***************************************
*
* modification history
*
* @Author Henning Mueller  
* HM 030599 created
*
****************************************/
void CQInvertedFile::releaseFeaturePruning()
{
  mFeaturePruningUsed=false;
  mPercentageofFeatures=0;
  if((!mScoreBoardPruningUsed) && (!mTimePruningUsed))
    mPruningUsed=false;
}


/***************************************
*
* activates the time pruning with the give cuOffPoint
*
***************************************
*
* modification history
*
* @Author Henning Mueller  
* HM 030599 created
*
****************************************/
void CQInvertedFile::useTimePruning(double inTimeCutoffPoint){
  mPruningUsed=true;
  mTimePruningUsed=true;
  mStoppingTime=inTimeCutoffPoint;
}


/***************************************
*
* releases the timePruning
*
***************************************
*
* modification history
*
* @Author Henning Mueller  
* HM 030599 created
*
****************************************/
void CQInvertedFile::releaseTimePruning(){
  mTimePruningUsed=false;
  mStoppingTime=0;
  if ((!mScoreBoardPruningUsed) && (!mFeaturePruningUsed))
    mPruningUsed=false;
}


/***************************************
*
* creates one entry in the scoreboardPruning
*
***************************************
*
* modification history
*
* @Author Henning Mueller  
* HM 030599 created
*
****************************************/
void CQInvertedFile::useScoreBoardPruning(double inCutoffPoint,
					      double reduceTo){
  mScoreBoardPruningUsed=true;
  mPruningUsed=true;

  /* if there is still room in the list */
  if(mNumberofUsedScoreBoardPrunings<MAX_SCOREBOARD_PRUNINGS)
    {
      mScoreBoardPruningArray[mNumberofUsedScoreBoardPrunings].stopAfterFeature=inCutoffPoint;
      mScoreBoardPruningArray[mNumberofUsedScoreBoardPrunings].reduceTo=reduceTo;
      mNumberofUsedScoreBoardPrunings++;
   }
}


/***************************************
*
* releases all the settings for the ScorboardPruning
*
***************************************
*
* modification history
*
* @Author Henning Mueller  
* HM 030599 created
*
****************************************/
void CQInvertedFile::releaseScoreBoardPruning()
{
  mScoreBoardPruningUsed=false;
  mEvaluateAfterPruning=false;
  int i;
  for(i=0;i<MAX_SCOREBOARD_PRUNINGS;i++)
    {
      mScoreBoardPruningArray[i].stopAfterFeature=0;
      mScoreBoardPruningArray[i].reduceTo=0;
   }
  mNumberofUsedScoreBoardPrunings=0;

  if((!mTimePruningUsed) && (!mFeaturePruningUsed))
    mPruningUsed=false;
}


/***************************************
*
* activates the evaluation after the scoreboardPruning
*
***************************************
*
* modification history
*
* @Author Henning Mueller  
* HM 030599 created
*
****************************************/
void CQInvertedFile::useEvaluateAfterPruning()
{
  mEvaluateAfterPruning=true;
}


/***************************************
*
* releases the evaluation after the scoreboard pruning
*
***************************************
*
* modification history
*
* @Author Henning Mueller 030599 
*
****************************************/
void CQInvertedFile::releaseEvaluateAfterPruning()
{
  mEvaluateAfterPruning=false;
}

/***************************************
*
* finishing the initialisation phase
*
***************************************
*
* modification history
*
*
****************************************/
void CQInvertedFile::finishInit(){
  CAcInvertedFile* lAccessor=
    dynamic_cast<CAcInvertedFile*>(mAccessor);
  
  if(!lAccessor){
    my_throw(VEWrongAlgorithm("finishInit"));
  }

  mWeighter->setAccessor(*lAccessor);

  
}

CWeighter& CQInvertedFile::getWeighter(){
  return *mWeighter;
};
