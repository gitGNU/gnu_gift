/**************************************************
*	Class WeightingFunction
* 
*
***************************************************
*
* Compiler Defines
*
***************************************************
*
* Modification History
*
* HM 990108 Put in the documentation
*
* $Header:
* $Log:
***************************************************/
#include <math.h>
#include "libGIFTQuInvertedFile/include/CWeightingFunction.h"
#include "iostream.h"
#include "FeatureExtraction/gift_features.h"

#include <algorithm>
#include <math.h>

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
CWeightingFunction::CWeightingFunction(const CAcInvertedFile* 
				       inAccessor,
				       CQueryNormalizer*
				       inQueryNormalizer,
				       CQueryNormalizer*
				       inThisNormalizer):
  mAccessor(inAccessor),
  mQueryNormalizer(inQueryNormalizer),
  mThisNormalizer(inThisNormalizer),
  mID(0),
  mPositiveTermFrequency(0),
  mNegativeTermFrequency(0),
  mPositiveRelevanceSum(0),
  mNegativeRelevanceSum(0)
{
  //empty
};

/** This virtual destructor is here inorder to call the (inherited)
    default constructors
 */
CWeightingFunction::~CWeightingFunction(){
  //intentionally empty
};

/***************************************
*
* getQueryFactor
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
double CWeightingFunction::getQueryFactor()const{
  return mQueryFactor;
};



/***************************************
*
* preCalculate
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
void CWeightingFunction::preCalculate(){
  mQueryFactor=getTermFrequency();
};



/* sets the two normalizing functions directly */
/***************************************
*
* setNormalizers
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CWeightingFunction::setNormalizers(CQueryNormalizer* inQueryNormalizer,
					CQueryNormalizer* inThisNormalizer)
{
  mQueryNormalizer=inQueryNormalizer;
  mThisNormalizer=inThisNormalizer;
}


/* sets the sccessor directly */
/***************************************
*
* setAccessor
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CWeightingFunction::setAccessor(const CAcInvertedFile* inAccessor)
{
  mAccessor=inAccessor;
}



/***************************************
*
* setID
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CWeightingFunction::setID(TID inID)
{
  //Setting the feature ID amounts to an initialization:
  //in fact this is the first point, where everything
  //necessary gets known. (in the general case)
  mID=inID;
  if(mAccessor){
    mPositiveTermFrequency=0;
    mNegativeTermFrequency=0;
    // the following line is needed, because we treat histogram
    // features differently from block features.
    mFeatureDescription=
      mAccessor->getFeatureDescription(mID);
  }
}



/// gets the ID of the feature from the weighting function */
/***************************************
*
* getID
*
****************************************
*
* modification history
*
* 
*
****************************************/
TID CWeightingFunction::getID()const{
  return mID;
};


  
/***************************************
*
* setRelevanceSum
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CWeightingFunction::setRelevanceSum(double inPositiveRelevanceSum,
					 double inNegativeRelevanceSum){

  mPositiveRelevanceSum = inPositiveRelevanceSum;
  mNegativeRelevanceSum = inNegativeRelevanceSum;
};



/***************************************
*
* addQueryFeature
*
****************************************
*
* modification history
*
* 
*
****************************************/
///
void CWeightingFunction::addQueryFeature(double inRelevanceLevel,
					 const 
					 CDocumentFrequencyElement& 
					 inQueryFeature){
  // adjustRelevanceSum(inRelevanceLevel); DELETED WM
  if(inRelevanceLevel>0){
    mPositiveTermFrequency+=
      (inRelevanceLevel)*
      inQueryFeature.getDocumentFrequency();
  }else{
    mNegativeTermFrequency+=
      inRelevanceLevel*
      inQueryFeature.getDocumentFrequency();
  }
};



/***************************************
*
* getTermFrequency
*
****************************************
*
* modification history
*
* 
*
****************************************/
/* returns the term frequency divided by the relevance sum */
double CWeightingFunction::getTermFrequency()const{

  double lReturnValue(0);
  if(mPositiveRelevanceSum){
    lReturnValue += mPositiveTermFrequency/mPositiveRelevanceSum;
  }
  if(mNegativeRelevanceSum){
    // both of these numbers are negative, which would become a positve number,
    // and we have to subtract it
    lReturnValue -= 0.5 * fabs(mNegativeTermFrequency)/fabs(mNegativeRelevanceSum);
  }
  return lReturnValue;
}




/***************************************
*
* subApply
*
****************************************
*
* modification history
*
* 
*
****************************************/
///Giving a weight and doing histogram intersection
double CWeightingFunction::subApply(const double inDocumentFrequency,
				    const double inNormalizingFactor)const{

  

  /* normalizinmg factor is not used ??? */

  //This function is to be called by almost every
  //Weigting function.
  //It does additional weighting based on the kind of the feature.
  double lRetVal=0;

#ifdef PRINT
  cout << "CWF-sA" <<endl;
#endif

  if(getTermFrequency()!=0)
    if((mFeatureDescription==COL_HST) || (mFeatureDescription==GABOR_HST))
      {
	lRetVal=
	  copysign(min(inDocumentFrequency,
		       fabs(getTermFrequency())),
		   getTermFrequency());
      }
    else /* not a histogram feature */
      {
	lRetVal=getTermFrequency();
      }
  return lRetVal;
};




/***************************************
*
* apply
*
****************************************
*
* modification history
*
* 
*
****************************************/
///Giving a weight and doing histogram intersection
double CWeightingFunction::apply(const CDocumentFrequencyElement& 
				 inResultFeature)const
{
  return subApply(inResultFeature.getDocumentFrequency(),
		  1);
};




/***************************************
*
* applyOnThis
*
****************************************
*
* modification history
*
* 
*
****************************************/
///Giving a weight
double CWeightingFunction::applyOnThis()const
{
  return subApply(getTermFrequency(),
		  mThisNormalizer->getValue());
};




/***************************************
*
* constructNew
*
****************************************
*
* modification history
*
* 
*
****************************************/
///Giving a weight
CWeightingFunction* CWeightingFunction::constructNew(TID inID)const
{
  CWeightingFunction* lRetVal=clone();
  lRetVal->setID(inID);
  return lRetVal;
};

/***************************************
*
* clone
*
****************************************
*
* modification history
*
* 
*
****************************************/
///Giving a weight
CWeightingFunction* CWeightingFunction::clone()const
{
  CWeightingFunction* lRetVal=new CWeightingFunction(mAccessor,
						     mQueryNormalizer,
						     mThisNormalizer);
  return lRetVal;
};



/***************************************
*
* <
*
****************************************
*
* modification history
*
* 
*
****************************************/
bool operator<(const CWeightingFunction& l,
	       const CWeightingFunction& t){
  return 
    l.getID()
    <
    t.getID();
}
