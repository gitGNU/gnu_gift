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
#include "CQMultiple.h"
#include "mrml_const.h"
#ifdef  __GIFT_CQMULTIPLE_THREADS__ 
#define  __GIFT_USES_THREADS__ 
#endif

#ifdef  __NO_GIFT_CQMULTIPLE_THREADS__ 
#undef  __GIFT_USES_THREADS__ 
#endif


#ifdef HAVE_LIBPTHREAD
#include <pthread.h>
#endif
//§#undef HAVE_LIBPTHREAD//for debugging purposes
/** 

We read the algorithm to find out 

how we will dispatch the queries to the child nodes

in which way we will merge the results 
coming from the child nodes 

 */
void CQMultiple::init(){
  

  

};

/**
 *
 * default constructor
 * 
 */
CQMultiple::CQMultiple(){
  assert(0);
};
/**
 * constructor
 * see CQuery
 */
CQMultiple::CQMultiple(CAccessorAdminCollection& inAccessorAdminCollection,
		       CAlgorithm&          inAlgorithm):
  CQuery(inAccessorAdminCollection,
	 inAlgorithm){
  {

    pair<bool,bool> lUsesURLs(inAlgorithm.boolReadAttribute("cui-uses-result-urls"));

    mUsesResultURLs=(lUsesURLs.first && lUsesURLs.second);

    // mproxy has been filled in a reasonable way 
    // by CQuery::CQuery
    mAccessor=mAccessorAdmin->openAccessor("url2fts");
    assert(mAccessor);
  }
};
    
/**
 *
 * destructor: at present empty
 * 
 */
CQMultiple::~CQMultiple(){

  cout << "destroying this "
       << __FILE__
       << __LINE__
       << flush
       << endl;

  //i thought i will need this, but at present I do not have this impression
  //it does not hurt, so we leave it in
};

class CWeightedResult{
public:
  /** The weight of this */
  double mWeight;
  /** The weight of this */
  CIDRelevanceLevelPairList* mList;
  /** The weight of this */
  CIDRelevanceLevelPairList::iterator mI;
};
  
/** everything happening in the children */
bool CQMultiple::setAlgorithm(CAlgorithm & inAlgorithm){
  if(mAlgorithm && mAlgorithm->getCollectionID()==inAlgorithm.getCollectionID()){
    
    return true;
    
  }else{
    //close the old collection, if exsisting
    if(mAccessorAdmin)
      mAccessorAdmin->closeAccessor("url2fts");
    //
    mAccessorAdmin=&mAccessorAdminCollection->getProxy(inAlgorithm.getCollectionID());
    mAccessor=mAccessorAdmin->openAccessor("url2fts");

    assert(mAccessor);
    //
    return (CQuery::setAlgorithm(inAlgorithm) && mAccessor);
  }
};
/**
 *
 * calls fastQuery for every child, merges the results
 * and translates them back into URLs
 *

 NOTE THIS IS COMMENTED OUT. IT WAS TOO INEFFICIENT. IT WOULD BECOME A CHOICE
 IF WE WOULD OUTPUT THINGS RATHER SORTED BY THEIR URL THAN BY THEIR SCORE

CIDRelevanceLevelPairList* CQMultiple::fastQuery(const CXMLElement& inQuery,
						 int inNumberOfInterestingImages,
						 double inDifferenceToBest){

  cout << "CMultiple Number of children:"
       << mChildren.size()
       << endl;

  list<CWeightedResult> lTemporary;
  double lWeightSum(0);

  for(lCChildren::const_iterator i=mChildren.begin();
      i!=mChildren.end();
      i++){
    lWeightSum+=i->mWeight;

    lTemporary.push_back(CWeightedResult());
    
    cout << "this CMultiple:" << this 
	 << ", i->mQuery:" << i->mQuery 
	 << ", i->mWeight:" << i->mWeight 
	 << endl;


    lTemporary.back().mList=i->mQuery->fastQuery(inQuery,
						 mAccessor->size(),
						 inDifferenceToBest);
    
    if(lTemporary.back().mList){
      lTemporary.back().mList->sort(CSortByID_IRLP());
      lTemporary.back().mI=lTemporary.back().mList->begin();
            
      lTemporary.back().mWeight=i->mWeight;
    }else{
      lTemporary.pop_back();
    }
  }
  
  CIDRelevanceLevelPairList* lReturnValue=new CIDRelevanceLevelPairList();
  
  while(lTemporary.size()){
    
    list<CWeightedResult>::iterator lMinimumIterator=lTemporary.begin();
    for(list<CWeightedResult>::iterator i=++lTemporary.begin();
	i!=lTemporary.end();
	i++){
      //the ID pointed to by i is smaller lMinimumIterator
      if((i->mI)->getID()<(lMinimumIterator->mI)->getID()){
	lMinimumIterator=i;
      }
    }
    if((lReturnValue->size()) 
       && ((lMinimumIterator->mI)->getID()==lReturnValue->back().getID())){
      lReturnValue->back().setRelevanceLevel(
					     lReturnValue->back().getRelevanceLevel()
					     +
					     ((lMinimumIterator->mI)->getRelevanceLevel()
					      *
					      lMinimumIterator->mWeight)
					     );
    }else{
      lReturnValue->push_back(*lMinimumIterator->mI);
      lReturnValue->back().setRelevanceLevel(
					     ((lMinimumIterator->mI)->getRelevanceLevel()
					      *
					      lMinimumIterator->mWeight)
					     );
    }
    (lMinimumIterator->mI)++;
    //end of this list, so we delete it
    if(lMinimumIterator->mI==lMinimumIterator->mList->end()){
      lTemporary.erase(lMinimumIterator);
    }
  }
  //lReturnValue->sort(CSortByID_IRLP());// for testing, to see if there are any duplicates
  lReturnValue->sort();
  lReturnValue->reverse();
  cout << "Size of the result "
       << lReturnValue->size()
       << endl;

  {
    CIDRelevanceLevelPairList::iterator iSkip=lReturnValue->begin();
    for(int i=0;
	i<inNumberOfInterestingImages && i<lReturnValue->size();
	i++){
      iSkip->setRelevanceLevel(iSkip->getRelevanceLevel()/lWeightSum);
      iSkip++;
    }
    lReturnValue->erase(iSkip,lReturnValue->end());
  }

  return lReturnValue;
};

 */


/** all information about a query thread for a CQMultiple*/
class CQMThread{
  /** the thread administrator */
#ifdef __GIFT_USES_THREADS__
#warning treading active
  pthread_t mThread;
#endif
  /** is this called as a thread or as a function */
  bool mIsThreaded;
  /** this is the weight the result should receive */
  double mWeight;
public:
  /**  query processor used in this thread*/
  CQuery& mQueryProcessor;
  /** the query (an xml parse tree) */
  const CXMLElement* mQuery;
  /** the desired result size */
  int mResultSize;
  /** the cutoff */
  double mDifferenceToBest;
  /** the result of the query*/
  CXMLElement* mResult;
  /** the result of the fastQuery*/
  CIDRelevanceLevelPairList* mFastResult;
  /** destructor */
  ~CQMThread(){
    delete mQuery;
  }
  /** constructor */
  CQMThread(CQuery& inQueryProcessor,
	    const CXMLElement& inQuery,
	    double inWeight,
	    int inResultSize,
	    double inDifferenceToBest):
    mQueryProcessor(inQueryProcessor),
    mQuery(new CXMLElement(inQuery)),
    mWeight(inWeight),
    mResultSize(inResultSize),
    mDifferenceToBest(inDifferenceToBest),
    mFastResult(0),
    mResult(0){
  };
  /** copy constructor */
  CQMThread(const CQMThread& in):
    mQueryProcessor(in.mQueryProcessor),
    mQuery(new CXMLElement(*in.mQuery)),
    mWeight(in.mWeight),
    mResultSize(in.mResultSize),
    mDifferenceToBest(in.mDifferenceToBest),
    mResult(0),
    mFastResult(0),
    mIsThreaded(0){
  };
  /** running the thread */
  void runThread(void){
    mResult=0;
#ifdef __GIFT_USES_THREADS__
#warning treading active
    mIsThreaded=true;
    pthread_create(&mThread,0,CQMultiple::doQueryThread,this);
#else
#warning treading blocked
    CQMultiple::doQueryThread(this);
#endif
  }
  /** running the thread */
  void runFastThread(void){
    mFastResult=0;
#ifdef __GIFT_USES_THREADS__
#warning treading active
    mIsThreaded=true;
    pthread_create(&mThread,0,CQMultiple::doFastQueryThread,this);
#else
#warning treading blocked
    CQMultiple::doFastQueryThread(this);
#endif
  }
  /** joining this thread with the caller */
  void join(void){
#ifdef __GIFT_USES_THREADS__
    void** lReturnValue(NULL);
    cout << "PREJOIN" << endl;
    if(mIsThreaded){
      int lJoinResult(pthread_join(mThread,lReturnValue));

      if(lJoinResult){
	cout << "CQMultiple: ERROR IN JOINING " 
	     << lJoinResult
	     << endl;
      }
    }else{
      cout << "This was an unthreaded join!" 
	   << endl;
    }
    mIsThreaded=false;
    cout << "POSTJOIN" << endl;
#endif
  }
  /** gives back the weight the results should receive 
      when summing up */
  double getWeight()const{
    return mWeight;
  }

};

void* CQMultiple::doFastQueryThread(void* inParameter){
  class CQMThread* lParam((CQMThread*) inParameter);
  lParam->mFastResult=lParam->mQueryProcessor.fastQuery(*(lParam->mQuery),
						    lParam->mResultSize,
						    lParam->mDifferenceToBest);
  cout << "I AM FINISHED HERE " << lParam << "result" << lParam->mResult << endl;
  return 0;
}
/**
   do the query thread, but starting query
 */
void* CQMultiple::doQueryThread(void* inParameter){
  class CQMThread* lParam((CQMThread*) inParameter);
  lParam->mResult=lParam->mQueryProcessor.query(*(lParam->mQuery));
  cout << "I AM FINISHED HERE " << lParam << "result" << lParam->mResult << endl;
  return 0;
}

/**
 *
 * calls fastQuery for every child, merges the results
 * and translates them back into URLs
 *
 * NEW, MORE EFFICIENT VERSION
 */
CIDRelevanceLevelPairList* CQMultiple::fastQuery(const CXMLElement& inQuery,
						 int inNumberOfInterestingImages,
						 double inDifferenceToBest){

  cout << "CMultiple Number of children:"
       << mChildren.size()
       << endl;


  //list<CWeightedResult> lTemporary;
  double lWeightSum(0);

  map<TID,CIDRelevanceLevelPair> lResultMap;


  // no mutex protection needed as this is not called except by main thread

  //
  // rip this into two parts
  // in order to 
  // make it possible to run the querying in one thread
  // and do the merging after having waited for each thread
  //

  list<CQMThread> lListOfThreads;

  lCChildren::const_iterator lLast=mChildren.end();
  lLast--;
  for(lCChildren::const_iterator i=mChildren.begin();
      i!=mChildren.end();
      i++){
    lWeightSum+=i->mWeight;
    
    //lTemporary.push_back(CWeightedResult());
    
    cout << "çç---------------------this CMultiple:fastQuery" << this 
	 << ", i->mQuery:" << i->mQuery 
	 << ", i->mWeight:" << i->mWeight 
	 << endl;
    
    
    lListOfThreads.push_back(CQMThread(*(i->mQuery),          // The Query processor to choose
				       inQuery,            // the query to be processed
				       i->mWeight,         // the weight the result will receive
				       mAccessor->size(),  // the size of the accessor (to get all potential results)
				       inDifferenceToBest));// the difference to the best which is allowed for a result
    /* EX-LEAK
       the following was a special branch for reducing the
       number of spawned threads by one. Apparently this did 
       not work and caused a memory leak. Now it seems to work.
       
	   if(1==0 
	   && (i==lLast)){
	   lListOfThreads.back().callFunction();//something to do for the main thread
	   }else*/
      
    {
      cout << "Running thread" 
	   << endl;
      lListOfThreads.back().runFastThread();//run the thread
      cout << "loop" 
	   << endl;
    }
    cout << "endloop" 
	 << endl;
  }
  // here we would join all threads

  for(list<CQMThread>::iterator lThread=lListOfThreads.begin();
      lThread!=lListOfThreads.end();
      lThread++){

    cout << "joining..." << endl;

    lThread->join();

    cout << "before merging " << endl;

    if(!lThread->mFastResult){
      cout << "THE THE RESULT OF THIS THREAD WAS NIL " 
	   << endl;
    }
    if(lThread->mFastResult){
      for(CIDRelevanceLevelPairList::iterator i=lThread->mFastResult->begin();
	  i!=lThread->mFastResult->end();
	  i++){
	
	map<TID,CIDRelevanceLevelPair>::const_iterator lFound=lResultMap.find(i->getID());

	i->setRelevanceLevel(i->getRelevanceLevel()*lThread->getWeight());
	
	if(lFound==lResultMap.end()){

	  lResultMap.insert(make_pair(i->getID(),
				      *i));
	}else{
	  
	  lResultMap[i->getID()].setRelevanceLevel(lResultMap[i->getID()].getRelevanceLevel()
						   +i->getRelevanceLevel()
						   );
	}
      }
      delete lThread->mFastResult;
    }

    
    cout << "after merging " << endl;
  }
  
  CIDRelevanceLevelPairList* lReturnValue=new CIDRelevanceLevelPairList();

  cout << "<pushing>"
       << endl;

  for(map<TID,CIDRelevanceLevelPair>::const_iterator i=lResultMap.begin();
      i!=lResultMap.end();
      i++){
    lReturnValue->push_back(i->second);
  }

  cout << "</pushing>\n<sorting>"
       << endl;

  lReturnValue->sort();
  lReturnValue->reverse();
  cout << "Size of the result "
       << lReturnValue->size()
       << endl;
  cout << "</sorting>"
       << endl;

  cout << "<cutting>"
       << endl;
  {
    CIDRelevanceLevelPairList::iterator iSkip=lReturnValue->begin();
    for(int i=0;
	i<inNumberOfInterestingImages && i<lReturnValue->size();
	i++){
      iSkip->setRelevanceLevel(iSkip->getRelevanceLevel()/lWeightSum);
      iSkip++;
    }
    lReturnValue->erase(iSkip,lReturnValue->end());
  }
  cout << "</cutting>"
       << endl;
  return lReturnValue;
};

/**
   A list of triplets to be merged
*/
class CMergeTriplet{
  /** */
  string mImageLocation;
  /** */
  string mThumbnailLocation;
  /** */
  float mRelevanceLevel;
public:
  /** */
  CMergeTriplet(const string& inImageLocation,
		const string& inThumbnailLocation
		):
    mImageLocation(inImageLocation),
    mThumbnailLocation(inThumbnailLocation),
    mRelevanceLevel(0){};
  /** */
  CMergeTriplet(const CMergeTriplet& in):
    mImageLocation(in.mImageLocation),
    mThumbnailLocation(in.mThumbnailLocation),
    mRelevanceLevel(in.mRelevanceLevel){};
  /** */
  CMergeTriplet():
    mImageLocation(""),
    mThumbnailLocation(""),
    mRelevanceLevel(0){};
  /** 
   */
  void addToRelevance(float inToBeAdded){
    mRelevanceLevel+=inToBeAdded;
  }
  /** 
   */
  void setSimilarity(float inValue){
    mRelevanceLevel=inValue;
  }
  /**
     Return the accumulated similarity
   */
  float getCalculatedSimilarity()const{
    return mRelevanceLevel;
  }
  /**
     Return the thumbnail location
   */
  string getThumbnailLocation()const{
    return mThumbnailLocation;
  }
  /**
     Return the image location
   */
  string getImageLocation()const{
    return mImageLocation;
  }
};
/**
   Sort MergeTriplets by relevance in descending order
 */
class CSortDescendingByRelevance_MT:binary_function<bool,double,double>{
public:
  bool operator() (CMergeTriplet& l, 
		   CMergeTriplet& t){
    return l.getCalculatedSimilarity()>t.getCalculatedSimilarity();
  }
};

/**
 *
 * calls fastQuery for every child, merges the results
 * and translates them back into URLs
 *
 * NEW, MORE EFFICIENT VERSION
 */
CXMLElement* CQMultiple::query(const CXMLElement& inQuery){

  if(!mUsesResultURLs){
    // if the mUsesReusltURLs is not set,
    // just call fastquery, and assemble from that a result,
    // as CQuery does.
    return CQuery::query(inQuery);
  }
  
  pair<bool,long> lNumberOfInterestingImages=
    inQuery.longReadAttribute(mrml_const::result_size);
  
  int inNumberOfInterestingImages=
    lNumberOfInterestingImages.second;

  pair<bool,long> lCutoff=
    inQuery.longReadAttribute(mrml_const::result_cutoff);
  
  int inCutoff=
    lCutoff.second;

  // do a deep clone of the query (for const cast)
  CXMLElement* lQuery=inQuery.clone(1);

  if(lQuery->child_list_begin()!=lQuery->child_list_end()){

    //
    // set the result size to a multiple of the 
    // number of the images requested 
    // to get a higher probability that
    // the combination reflects the real score
    // you want more explanation?
    // you get it at help-gift@gnu.org
    //
    lQuery->addAttribute(mrml_const::result_size,
			 long(inNumberOfInterestingImages*5));
    
    cout << "CMultiple::query Number of children:"
	 << mChildren.size()
	 << endl;
    
    
    //list<CWeightedResult> lTemporary;
    double lWeightSum(0);
    
    map<string,CMergeTriplet> lResultMap;
    
    // no mutex protection needed as this is not called except by main thread
    
    //
    // rip this into two parts
    // in order to 
    // make it possible to run the querying in one thread
    // and do the merging after having waited for each thread
    //
    
    list<CQMThread> lListOfThreads;

    lCChildren::const_iterator lLast=mChildren.end();
    lLast--;
    for(lCChildren::const_iterator i=mChildren.begin();
	i!=mChildren.end();
	i++){
      lWeightSum+=i->mWeight;
    
      //lTemporary.push_back(CWeightedResult());
    
      cout << "**-------------------------------this CMultiple QUERY:" << this 
	   << ", i->mQuery:" << i->mQuery 
	   << ", i->mWeight:" << i->mWeight 
	   << endl;
    
    
      lListOfThreads.push_back(CQMThread(*(i->mQuery),          // The Query processor to choose
					 *lQuery,            // the query to be processed
					 i->mWeight,         // the weight the result will receive
					 mAccessor->size(),  // the size of the accessor (to get all potential results)
					 inCutoff));// the difference to the best which is allowed for a result
      /* EX-LEAK
	 the following was a special branch for reducing the
	 number of spawned threads by one. Apparently this did 
	 not work and caused a memory leak. Now it seems to work.
       
	 if(1==0 
	 && (i==lLast)){
	 lListOfThreads.back().callFunction();//something to do for the main thread
	 }else*/
      
      {
	cout << "Running thread" 
	     << endl;
	lListOfThreads.back().runThread();//run the thread
	cout << "loop" 
	     << endl;
      }
      cout << "endloop" 
	   << endl;
    }
    // here we would join all threads

    for(list<CQMThread>::iterator lThread=lListOfThreads.begin();
	lThread!=lListOfThreads.end();
	lThread++){

      cout << "joining..." << endl;

      lThread->join();

      cout << "before merging " << endl;

      if(!lThread->mResult){
	cout << "THE THE RESULT OF THIS THREAD WAS NIL " 
	     << endl;
      }
      if(lThread->mResult){
	/*
	  OK. At this point we got back a query-result XML element.
	  now we want the result-element-list
	*/
	cout << "H" << flush;
	for(CXMLElement::lCChildren::const_iterator i=lThread->mResult->child_list_begin();
	    i!=lThread->mResult->child_list_end();
	    i++){
	  cout << "I" << flush;
	  if((*i)->getName() == mrml_const::query_result_element_list){
	    for(CXMLElement::lCChildren::const_iterator j=(*i)->child_list_begin();
		j!=(*i)->child_list_end();
		j++){
	      cout << "J" << flush;
	      if((*j)->getName() == mrml_const::query_result_element){
		cout << "K" << flush;
		/* 
		   inside this, *j points now to an XML element which
		   is a query-result-element. from this we will read now
		   the calculated-relevance,
		   as well as the image and thumbnail location.
		*/
		pair<bool,double> lCalculatedRelevance=
		  (*j)->doubleReadAttribute(mrml_const::calculated_similarity);
		pair<bool,string> lImageLocation=
		  (*j)->stringReadAttribute(mrml_const::image_location);
		pair<bool,string> lThumbnailLocation=
		  (*j)->stringReadAttribute(mrml_const::thumbnail_location);

		cout << "L" << flush;

		// no relevance corresponds to relevance 0
		if(! lCalculatedRelevance.first){
		  lCalculatedRelevance=make_pair(bool(0),
						 double(0));
		}

		cout << "L" << flush;
		// if there is a thumbnail and no image,
		// we take the thumbnail location as image location
		if((lThumbnailLocation.first)
		   && (!lImageLocation.first)){
		  lImageLocation=lThumbnailLocation;
		}
		cout << "L" << flush;
		// if there is an image and no thumbnail,
		// we take the image location as thumbnail location
		if((!lThumbnailLocation.first)
		   && (lImageLocation.first)){
		  lThumbnailLocation=lImageLocation;
		}
		cout << "L" << flush;
	      
		// now we are guaranteed to have a well-initialised
		// image location
		if(lImageLocation.first){
		  map<string,CMergeTriplet>::iterator lFound(lResultMap.find(lImageLocation.second));
		  
		  if(lFound!=lResultMap.end()){
		    cout << "A" << flush;
		    lFound->second.addToRelevance(lCalculatedRelevance.second);
		    cout << "[" << lFound->second.getCalculatedSimilarity() << "]" << flush;
		  }else{
		    // this result is not yet in the result map
		    lFound=lResultMap.insert(make_pair(lImageLocation.second,CMergeTriplet(lImageLocation.second,
										    lThumbnailLocation.second))).first;
		    lFound->second.addToRelevance(lCalculatedRelevance.second);
		  }
		cout << "M" << flush;

		}
	      }
	    }
	  }
	}
	delete lThread->mFastResult;
      }

    
    
      cout << "after thread " << endl;
    }
    cout << "ALL THREADS FINISHED " << endl;
  
    {
      // now we build a list of merge triplets
      // that is sorted by score in descending order
      list<CMergeTriplet> lResultList;
      for(map<string,CMergeTriplet>::const_iterator i=lResultMap.begin();
	  i!=lResultMap.end();
	  i++){
	lResultList.push_back(i->second);
      }
      cout << "DELETING " << endl;
      lResultList.sort(CSortDescendingByRelevance_MT());
      cout << "DELETING " << endl;
    
      {
	list<CMergeTriplet>::iterator iSkip=lResultList.begin();
	for(int i=0;
	    i<inNumberOfInterestingImages && i<lResultList.size();
	    i++){
	  iSkip->setSimilarity(iSkip->getCalculatedSimilarity()/lWeightSum);
	  iSkip++;
	}
	lResultList.erase(iSkip,lResultList.end());
      }

      // now let's build a result element tree
      CXMLElement* lReturnValue(new CXMLElement(mrml_const::query_result,0));
      CXMLElement* lReturnList(new CXMLElement(mrml_const::query_result_element_list,0));
      lReturnValue->addChild(lReturnList);
    
      assert(mAccessor);
    
      for(list<CMergeTriplet>::const_iterator i=lResultList.begin();
	  i!=lResultList.end();
	  i++){
      
	CXMLElement* lReturnElement(new CXMLElement(mrml_const::query_result_element,
						    0));
	{
	  double lRelevanceLevel(i->getCalculatedSimilarity());
	  string lString(mrml_const::calculated_similarity);
	  lReturnElement->addAttribute(lString,
				       lRelevanceLevel);
	}
      
      
      
	{
	  string lURL(i->getImageLocation());
	  string lString(mrml_const::image_location);
	  lReturnElement->addAttribute(lString,
				       lURL);
	}

	{
	  string lURL(i->getThumbnailLocation());
	  string lString(mrml_const::thumbnail_location);
	  lReturnElement->addAttribute(lString,
				       lURL);
	}
      
	lReturnValue->addChild(lReturnElement);
      
	lReturnValue->moveUp();
      
      }
      //gMutex->unlock();//debugging
      return lReturnValue;
    }
  }else{
    //gMutex->unlock();//debugging
    return getRandomImages(inNumberOfInterestingImages);
  }

  //gMutex->unlock();//debugging
  // missing sort and output
  list<pair<string,string> > lAttributes;
  lAttributes.push_back(make_pair(mrml_const::message,
				  string("empty query result, i seem to have missed all ifs and elses!")));
  return new CXMLElement(mrml_const::error,lAttributes);
};

