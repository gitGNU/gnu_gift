#include "libGIFTAcInvertedFile/include/CIFBuilderTriplet.h"

/** default constructor */
CIFBuilderTriplet::CIFBuilderTriplet():
  mFeatureID(0),
  mDocumentID(0),
  mDocumentFrequency(0){
};
/** convenience constructor */
CIFBuilderTriplet::CIFBuilderTriplet(TID inFeatureID,
				     TID inDocumentID,
				     float inDocumentFrequency):
  mFeatureID(inFeatureID),
  mDocumentID(inDocumentID),
  mDocumentFrequency(inDocumentFrequency){
};

  /** testing if all fields are equal */
bool CIFBuilderTriplet::isIdentical(const CIFBuilderTriplet& inTriplet)const{
  inTriplet.mFeatureID==mFeatureID;
  inTriplet.mDocumentID==mDocumentID;
  inTriplet.mDocumentFrequency==mDocumentFrequency;
};


bool operator<(const CIFBuilderTriplet& l,// less
	       const CIFBuilderTriplet& t // than
	       ){
    return 
	(l.mFeatureID<t.mFeatureID) 
	|| 
	(
	 (l.mFeatureID<t.mFeatureID) 
	 &&
	 (t.mDocumentFrequency < l.mDocumentFrequency) // amounts to comparison of inverse
	 );
}
