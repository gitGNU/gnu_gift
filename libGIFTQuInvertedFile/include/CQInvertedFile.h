/* -*- mode: c++ -*- 
*/
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, University of Geneva

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
// -*- mode: c++ -*-

/**
*
* CQInvertedFile.h - each separate Query is 
* one instance of this class, sets everything for one query
*
* @Author: Wolfgang Müller, Pruning and some of the bugfixes Henning Müller
*          
*
* modification history:
*
* WM 08  99 added things for session management
*           added compiler defines documentation
* HM 030999 created the documentation
* HM 030599 put in the parameters for the pruning
* WM 10  98 creation
*
*
*
* compiler defines used:
* _CINVERTEDFILEQUERY avoids double inclusion
*
*/
#ifndef _CINVERTEDFILEQUERY
#define _CINVERTEDFILEQUERY
#include <memory>
#include <map>
#include "CSelfDestroyPointer.h"
#include "CRelevanceLevelList.h"
#include "CIDRelevanceLevelPairList.h"
#include "CWeightingFunctionPointerList.h"
#include "CWeightingFunctionPointerHash.h"
#include "CAlgorithm.h"
#include "CQuery.h"
#include "CAcInvertedFile.h"
#include "CWeightingFunction.h"
#include "CQueryNormalizer.h"
#include "WeightingFunctionsAndNormalizers.h"
class CScoreBoard;
class CAcInvertedFile;

#include "CWeighter.h"

/** A factory for weighting functions with associated normalizers.
    The weighting functions are intended to be members of
    CQInvertedFile where they will be used.
*/
class CWeighterFactory:protected map< string,CWeighter* >{
public:
  /** clones a weighter from the factory 
      and sets the pointers in a way,
      that normalizers etc. are pointing in
      the right way to each other.

      However, you still have to set the accessor
      Why not set it at once?
      Maybe.
   */
  CWeighter* newWeighter(const string& inID)const;
  /** constructor: initializes everything
      fills the map etc.
  */
  CWeighterFactory();
  /** Destructor deletes the content it points to
  */
  ~CWeighterFactory();
};

/** The Query manager for Queries on inverted Files */
class CQInvertedFile:public CQuery{

protected:


  CAlgorithm* mDeb;

  /** You can choose to use only every lModulo-th feature */
  int mModulo;
  /** Every lModulo-th feature, starting at lModuloClass  */
  int mModuloClass;
  /** 
      This is a factory for weighters.
      With this we can find the weighters
   */
  CWeighterFactory mWeighterFactory;
  /**
     The weighter currently used.
  */
  CSelfDestroyPointer<CWeighter> mWeighter;

  /**  object of this type to have a list of the features with the weights */
  CSelfDestroyPointer<CWeightingFunctionPointerHash> 
  mQueryFeatureWeighters;

  /**
   *
   * calculates the score for all the images based on a list of features
   *
   * @author Wolfgang Müller
   */
  double keepScore(CScoreBoard& inoutScoreBoard,
		   const CWeightingFunctionPointerList& inFeatures,
		   bool lPositive)const;

  /**
   *
   * calculates the results for the images using some sort of pruning
   *
   * @author Henning Müller
   */
  double keepScorePruning(CScoreBoard& inoutScoreBoard,
			  const CWeightingFunctionPointerList& inFeatures,
			  bool inPositive,
			  int inDesiredNumberOfDocuments)const;

  /**
   *
   * 
   * @short creates a list of all the features which 
   * are in one or more of the query images
   * should be replaced by a version which operates on IDs not URLs
   *
   * @author Wolfgang Müller + Pruning: Henning Müller/reprogrammed WM 09-10-00
   */
  void buildQueryHash(CRelevanceLevelList& inQuery,
		      CWeightingFunctionPointerHash& 
		      outQueryFeatureWeighters)const;

  /**
   *
   * Building a list of normalized WF from a hash of Weighting functions 
   *
   */
  void buildNormalizedQueryList(double inPositiveRelevanceSum,
				double inNegativeRelevanceSum,
				CWeightingFunctionPointerHash& 
				inQFW,
				CWeightingFunctionPointerList& 
				outQFW)const;

  /**
   *
   * Building a list of normalized WF from a hash of Weighting functions
   *
   */
  void buildNormalizedQueryHash(double inPositiveRelevanceSum,
				double inNegativeRelevanceSum,
				CWeightingFunctionPointerHash& 
				inQFW)const;
  
protected:
  /**
   *
   * Initializer, used by both construcors
   *
   */
  void init();

public:
  /** 
      finish the initialisation phase
      make the weighting function know who its normalizers are
  */
  void finishInit();
  /**
   *
   * New constructor, taking as parameter the
   * algorithm structure, which contains all 
   * the algorithm configuration
   *
   */
  CQInvertedFile(CAccessorAdminCollection& inAccessorAdminCollection,
		 CAlgorithm& inAlgorithm);
  /**
   *
   * destructor
   *
   */
  ~CQInvertedFile();
  /**
   *
   * set the Algorithm.
   * same scheme as in setCollection
   *
   */
  virtual bool setAlgorithm(CAlgorithm& inAlgorithm);


  /**
   *
   * @short a query which returns ID/RelevanceLevel pairs instead of
   * instead of URL/RelevanceLevel pairs;
   * this is faster for merging tasks (to explain the name)
   * queries for URLs are answered by query (in herited)
   *
   */
  virtual CIDRelevanceLevelPairList* fastQuery(const CXMLElement& inQuery,
					       int inNumberOfInterestingImages,
					       double inDifferenceToBest);
  
  /**
   *
   * Assuming that a correct CWeightingFunctionPointerHash has been built by fastQuery 
   * (or another function), this function will do the rest
   *
   */
  virtual CIDRelevanceLevelPairList* fastQueryByFeature(const  CWeightingFunctionPointerList& inQuery,
							int inNumberOfInterestingImages,
							double inDifferenceToBest);
  
  /**
   *
   *  Returns the score for one image in the query 
   *
   */
  double DIDToScore(TID inDID,
		    const CWeightingFunctionPointerHash& inQuery)const;


  /**
   * 
   *  Returns the score for one image in the query 
   *
   **/
  double URLToScore(const string& inURL,
		    const CWeightingFunctionPointerHash& inQuery)const;

  /**
   *
   * Turns the featurelist of a document into a score.
   *
   */
  double FeatureListToScore(TID inDID,
			    const CDocumentFrequencyList& inFeatureList,
			    const CWeightingFunctionPointerHash& inQuery)const;

  /**
   *
   *
   */
  void buildNormalizedQueryHash(const CRelevanceLevel& inQuery,
				CWeightingFunctionPointerHash& 
				outQueryFeatureWeighters)const;


  /* variables needed for the blocking of special feature groups */

  /* the upper limit for the number of feature groups needed for the array for the blocking */
  static const int MAXIMUMNUMBEROFEATUREGROUPS=50; 
  /* you need to switch this on to activate the blocking of features */
  bool mBlockingOn;
  /* array with all the feature groups and an information if blocked or not */
  bool mBlockingArray[MAXIMUMNUMBEROFEATUREGROUPS];


  /**
   *
   * activateBlockingFeatures - sets the variable to block groups of features
   *
   */
  void activateBlockingFeatures();

  /**
   *
   * releaseBlockingFeatures - this turns off the blocking of features
   *
   */
  void releaseBlockingFeatures();

  /**
   *
   * featuresBlocked - returns true if the features are blocked
   *
   */
  bool featuresBlocked()const;

  /**
   *
   * blockFeatureGroup - this blocks one special group of features
   *
   */
  void blockFeatureGroup(const int featureNumber);

  /**
   *
   * unblockFeatureGroup - this releases the blocking of one feature group
   *
   */
  void unblockFeatureGroup(const int featureNumber);

  /**
   *
   * isBlocked - returns true if the feature is blocked and false if not
   *
   */
  bool isBlocked(const int featureNumber)const;



  /** These are the variables and functions used for the pruning process 
      
      the maximum number of sets which can be used to prune the ScoreBoard 
   */
  static const int MAX_SCOREBOARD_PRUNINGS=10; 

  /** Shows if any pruning is used 
   */
  bool mPruningUsed;

  /** true if the scoreboardis Pruned 
   */
  bool mScoreBoardPruningUsed;
  /**
     Parameters for scoreboard pruning
     @author Henning M&uuml;ller
  */       
  typedef struct{ 
    /**
       @short this marks a fraction of the number of features used
    */
    double stopAfterFeature;
    /**
       reduce the scoreboard to a factor of the number of images you 
       want to retrieve 
    */
    double reduceTo;
  } parameterPruningType;
  /** @Author Henning Müller */
  parameterPruningType 
     mScoreBoardPruningArray[MAX_SCOREBOARD_PRUNINGS];
  /** @Author Henning Müller */
  int mNumberofUsedScoreBoardPrunings;

  /* true if feature pruning is used */
  bool mFeaturePruningUsed;
  /** @Author Henning Müller */
  double mPercentageofFeatures;

  /* true if time pruning is used */
  bool mTimePruningUsed;
  /** @Author Henning Müller */
  double mStoppingTime;

  /* if this variable is used with scoreboardpruning, the resulting 
     scoreboard will be evaluated with an uninverted file 
     @Author Henning Müller */
  bool mEvaluateAfterPruning;

  /**
   *
   * sets back all the variables for the pruning
   *
   * @Author Henning Müller 
   */
  void releaseAllPrunings();


  /**
   *
   * activates the feature pruning with evaluating 
   * a certain percentage of the features
   *
   * @Author Henning Müller 
   */
  void useFeaturePruning(double percentage);

  /**
   *
   * set the variables back to its normal status
   *
   * @Author Henning Müller 
   */
  void releaseFeaturePruning();

  /**
   *
   * activates the time pruning with the give cuOffPoint
   *
   * @Author Henning Müller 
   */
  void useTimePruning(double inTimeCutoffPoint);

  /**
   *
   * releases the timePruning
   *
   * @Author Henning Müller 
   */
  void releaseTimePruning();

  /**
   *
   * creates one entry in the scoreboardPruning
   *
   * @Author Henning Müller 
   */
  void useScoreBoardPruning(double inCutAfterFraction,
			    double    inReduceToFactor);

  /**
   *
   * releases all the settings for the ScorboardPruning
   *
   * @Author Henning Müller 
   */
  void releaseScoreBoardPruning();

  /**
   *
   * activates the evaluation after the scoreboardPruning
   *
   * @Author Henning Müller 
   */
  void useEvaluateAfterPruning();

  /**
   *
   * releases the evaluation after the scoreboard pruning
   *
   * @Author Henning Müller 
   */
  void releaseEvaluateAfterPruning();
  /**
   * For FerDeLance queries
   */
  CWeighter& getWeighter();
}; /* end of class */

#endif

