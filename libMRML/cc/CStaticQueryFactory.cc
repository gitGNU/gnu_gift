#include "my_throw.h"

#include "CStaticQueryFactory.h"

#include "GIFTExceptions.h"
#include "CAlgorithm.h"
#include "CQInvertedFile.h"
#include "CQParallel.h"
#include "CQMultiple.h"
#ifdef __GIFT_BAYESIAN__
#include "CIFQuickAndDirtyHunter.h"
#endif
#include "CQHierarchy.h"

#ifdef __GIFT_WITH_PERL__
#include "CQPerl.h"
#endif

//These are files which belong to yet unpublished worked
#ifdef __GIFT_UNPUBLISHED_WORK__
#include "CQIFFerDeLance.h"
#endif

#include "CAccessorAdminCollection.h"

CQuery* CStaticQueryFactory::makeQuery(const string & inBaseType, 
				    CAccessorAdminCollection & inAccessorAdminCollection,
				    CAlgorithm & inAlgorithm){
#ifndef LINK_DYNAMICALLY
  if(inBaseType=="inverted_file"){
    return(new CQInvertedFile(inAccessorAdminCollection,
			      inAlgorithm));
  }
#ifdef __GIFT_BAYESIAN__
  if(inBaseType=="bayesian"){
    return(new CIFQuickAndDirtyHunter(inAccessorAdminCollection,
				      inAlgorithm));
  }
#endif
#ifdef __GIFT_WITH_PERL__
  if(inBaseType=="perl"){
    return(new CQPerl(inAccessorAdminCollection,
		      inAlgorithm));
  }
#endif
//   if(inBaseType=="parallel"){
//     return(new CQParallel(inAccessorAdminCollection,
// 			  inAlgorithm));
//   }
  if(inBaseType=="multiple"){
    return(new CQMultiple(inAccessorAdminCollection,
			  inAlgorithm));
  }
#ifdef __GIFT_UNPUBLISHED_WORK__
  if(inBaseType=="fer_de_lance"){
    return(new CQIFFerDeLance(inAccessorAdminCollection,
			      inAlgorithm));
  }
#endif
  if(inBaseType=="hierarchy"){
    return(new CQHierarchy(inAccessorAdminCollection,
			   inAlgorithm));
  }
  my_throw(VEWrongBaseType(inBaseType.c_str()));
#endif
  return 0;
}
