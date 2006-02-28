/* -*- mode: c++ -*- 
*/
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

     Copyright (C) 2003, 2004 Bayreuth University
    Copyright (C) 2005,2006 Bamberg University
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
/**  -*- mode: c++ -*- */
/**
 *
 * CWEIGHTINGFUNCTION
 *
*
*
* modification history:
*
* HM 090399 created some documentation
*
* @author Wolfgang Müller
*
* compiler defines used:
*
*
*/
#ifndef _CWEIGHTINGFUNCTION
#define _CWEIGHTINGFUNCTION
#include "libGIFTQuInvertedFile/include/uses-declarations.h"
#include <cmath>
#include "libMRML/include/TID.h"
#include "libGIFTAcInvertedFile/include/CDocumentFrequencyElement.h"
#include "libGIFTAcInvertedFile/include/CAcInvertedFile.h"

class CQueryNormalizer;

/** a universally usable class for weighting. 
    
    The rationale is, that we want to implement very flexible 
    feature weighting, so flexible that it does not necessarily 
    have anything to do with the initial concept of feature
    weighting.

    The scenario: We have query/feedback from the user. That is:
    We have a list of images with either positive or negative
    relevance for the user. Each of these images contains a list
    of features. 

    Now we want to combine the different features. What we
    usually need to know is: how many images where in
    the positive feedback, and how many images where
    in the negative feedback.

*/
class CWeightingFunction{
protected:
  /** This value depends on all the elements of the query, which
      have a positive user-assigned relevance.

      i.e. not necessarily only on the elements which contain 
      this feature, so it has to be explicitly set using
      the function setRelevanceSum from the outside.
      
      If relevance=+-1 then this is the number of positive images in the query
  */
  double mPositiveRelevanceSum;
  /** This value depends on all the elements of the query, which
      have a negative user-assigned relevance.

      i.e. not necessarily only on the elements which contain 
      this feature, so it has to be explicitly set using
      the function setRelevanceSum from the outside.
      
      If relevance=+-1 then this is the number of negative images in the query
  */
  double mNegativeRelevanceSum;

  /* A pseudo term frequency calculated from the input
     at present by calculating the mean.

     mPositiveTermFrequency/mPositiveRelevanceSum is one term in a 
     a pseudo term frequency used for calculating query weights */
  double mPositiveTermFrequency;
  /* A pseudo term frequency calculated from the input
     at present by calculating the mean.

     mNegativeTermFrequency/mNegativeRelevanceSum is the other term in a 
     a pseudo term frequency used for calculating query weights */
  double mNegativeTermFrequency;

  /**  
       At present the feature description is a simple integer which expresses 
       if the feature is a histogram feature or not and if it is a texture feature
       or not. We probably will use more elaborate descriptions in the future.
   */
  int mFeatureDescription;

  /**  */
  TID mID;

  /** The accessor for the file on which the query is performed */
  const CAcInvertedFile* mAccessor;

  /** FIXME THERE IS A GOOD DESCRIPTION MISSING FOR THE NEXT
      TWO VARIABLES */
  mutable CQueryNormalizer* mQueryNormalizer;

  /** A normalizer for apply on self 
   */ 
  mutable CQueryNormalizer* mThisNormalizer;

  /**
     The query usually will separate into a part
     which is equal for all the query and a part which is to be 
     calculated for each document. The part which stays constant
  */
  double mQueryFactor;
  /**  */
  double mDocumentFactor;

public:

  /**
   *
   *  The query factor contains all informatio which
   *  depends only on the query. (for efficiency)
   *
   */
  double getQueryFactor()const;

  /**
   *
   * Calculate the QueryFactor.
   *
   */
  virtual void preCalculate();

  /**
   *
   * Constructor: 
   * @param inAccessor: the accessor containing this \
   *                     (for getting information about single documents)
   * @param inQueryNormalizer: FIXME
   * @param inThisNormalizer: FIXME
   */
  CWeightingFunction(const CAcInvertedFile* inAccessor=0,
		     CQueryNormalizer* inQueryNormalizer=0,
		     CQueryNormalizer* inThisNormalizer=0);

  /**
   *
   * Set the accessor (should not be needed) 
   *
   */
  void setAccessor(const CAcInvertedFile*);

  /**
   *
   *  Set the query normalizer (should not be needed);  
   *
   */
  void setNormalizers(CQueryNormalizer* inQueryNormalizer,
		      CQueryNormalizer* inThisNormalizer);

  /**
   *
   * Set the ID of this feature
   *
   */
  void setID(TID);

  /**
   *
   * get the ID of this feature 
   *
   */
  TID getID()const;

  /**
   *
   * Setting the relevance sum, 
   * that is the sum of the absolute values of all 
   * relevance levels of all documents of the query
   *
   * for rocchio we need this sum to be once calculated
   * for positive, once for negative feedback images
   * 
   */
  void setRelevanceSum(double inPositiveRelevanceSum,
		       double inNegativeRelevanceSum);

  /**
   *
   * Add a feature. A feature will be a document frequency element
   * out of the feature file.
   *
   */
  virtual void addQueryFeature(double inRelevanceLevel,
			       const CDocumentFrequencyElement& 
			       inQueryFeature);

  /**
   *
   * Giving a pseudo term frequency for normalization
   *
   */
  virtual double getTermFrequency()const;

  /**
   *
   * Apply this on a document frequency
   *
   */
  virtual double subApply(const double inDocumentFrequency,
			  const double inNormalizingFactor)const;

  /**
   *
   * Preparing the right parameters for subApply: Apply this on another feature
   *
   */
  virtual double apply(const CDocumentFrequencyElement& inResultFeature)const;

  /**
   *
   * Preparing the right parameters for subApply: Apply this on itself 
   *
   */
 
  double applyOnThis()const;

  /**For the "prototype pattern": 
     Kinda virtual copy constructor: 
     Gamma/Helm/Johnson/Vlissides p. 107*/
  virtual CWeightingFunction* constructNew(TID inID)const;

  /**For the "prototype pattern": 
     Kinda virtual copy constructor: 
     Gamma/Helm/Johnson/Vlissides p. 107

     This is a new version of constructNew which
     manages to live without the setID function
  */
  virtual CWeightingFunction* clone()const;

  /** This virtual destructor is here inorder to call the (inherited)
      default constructors
  */

  virtual ~CWeightingFunction();
  
  friend class CSortByDFTimesLogICF_WF;
};

/**
 *
 * 
 *
 */
bool operator<(const CWeightingFunction&,
	       const CWeightingFunction&);

/** CWeightingFunction: Sort by FeatureID  */
class CSortByFeatureID_WF:
  public binary_function
<CWeightingFunction,CWeightingFunction,bool>{
  /**  */
  inline bool operator()(const CWeightingFunction& l,
			 const CWeightingFunction& t){
    return l.getID()<t.getID();
  }
};

/**
 *
 * CWeightingFunction: Sort by DocumentFrequency/CollectionFrequency 
 *
 */ 
 
class CSortByDFTimesLogICF_WF:
  public binary_function
<CWeightingFunction,CWeightingFunction,bool>{
  /**  */
  inline bool operator()(const CWeightingFunction& l,
			 const CWeightingFunction& t){
    return 
      l.getTermFrequency()
      *
      fabs(log(l.mAccessor->FeatureToCollectionFrequency(l.getID())))
      <
      t.getTermFrequency()
      *
      fabs(log(t.mAccessor->FeatureToCollectionFrequency(t.getID())));
  };
};

/**
*
* CWeightingFunction: Sort by DocumentFrequency/CollectionFrequency 
* 
*/
class CSortByQueryFactor_WF:
  public binary_function
<CWeightingFunction,CWeightingFunction,bool>{
public:
  /**  */
  inline bool operator()(const CWeightingFunction& l,
			 const CWeightingFunction& t){
    return 
      l.getQueryFactor()
      <
      t.getQueryFactor();
  };
};
/**
   CWeightingFunction: Sort by the absolute value
   of the query factor. This is useful for pruning
   when the feedback is negative.
*/

class CSortByAbsQueryFactor_WF:
  public binary_function
<CWeightingFunction,CWeightingFunction,bool>{
public:
  /**  */
  inline bool operator()(const CWeightingFunction& l,
			 const CWeightingFunction& t){
    return 
      fabs(l.getQueryFactor())
      <
      fabs(t.getQueryFactor());
  };
};

/**
*
* 
*
*/
template<class CSortOp>
class CSortPointers_WF:
  binary_function<CWeightingFunction*,CWeightingFunction*,bool>{
protected:
  /**  */
  CSortOp mSorter;
public:
  /**  */
  inline CSortPointers_WF(){};
  /**  */
  inline bool operator()(const CWeightingFunction* l,
			 const CWeightingFunction* t){
    return mSorter(*l,*t);
  };
};

#include "libGIFTQuInvertedFile/include/CQueryNormalizer.h"

#endif


