#include "libMRML/include/mrml_const.h"

#include "libMRML/include/CCommunicationHandler.h"
#include "libMRML/include/CXEVCommunication.h"

CXEVCommunication::CXEVCommunication(CCommunicationHandler* inHandler):
    mHandler(inHandler),
    mSessionID(""){

};

bool CXEVCommunication::startVisit(const CXMLElement& inElement){
    CXMLElement* lFather(inElement.getFather());

    cout << "CHECKING: " << inElement.getName() << endl;
    if(lFather){
      lFather->check();
    }else{
      inElement.check();
    }

    inElement.checkNPrint();

    cout << "VISITING: " << inElement.getName() << endl;

    CSessionManager& lSessionManager(mHandler->getSessionManager());

    //----------------------------------------
    //the (deprecated) ihandshake message
    if(inElement.getName()==string(mrml_const::cui_ihandshake)){
	mHandler->sendHandshake(inElement.stringReadAttribute(mrml_const::user_name).second);
	//delete the multiresult. DIRTY
	mHandler->mMultiResponse=0;
    }
    //----------------------------------------
    //thek start of the message: extract the session id
    if(inElement.getName()==mrml_const::mrml){
	mSessionID=inElement.stringReadAttribute(mrml_const::session_id).second;
	mHandler->startMultiRequest(mSessionID);
    }
    //----------------------------------------
    //the configure-session signal
    if(lFather
       &&
       lFather->stringReadAttribute(mrml_const::session_id).first
       &&
       (lFather->getName()==mrml_const::configure_session)
       &&
       (inElement.getName()==mrml_const::algorithm)){

      CXMLElement* lAcknowledgement=new CXMLElement(mrml_const::acknowledge_session_op,0);
      lAcknowledgement->addAttribute(mrml_const::session_id,lFather->stringReadAttribute(mrml_const::session_id).second);

	cout << "CONFIGURESESSIONçççççççççççççççççççç"
	     <<	    lFather->stringReadAttribute(mrml_const::session_id).second
	     << endl;

	lSessionManager.setAlgorithm(lFather->stringReadAttribute(mrml_const::session_id).second,
				     ((CAlgorithm&)inElement).clone());	

	mHandler->addToMultiResponse(lAcknowledgement);


	//do not traverse deeper
	return false;
    }
    //----------------------------------------
    //the get-sessions
    if(inElement.getName()==mrml_const::get_sessions){
	mHandler->getSessions(inElement.stringReadAttribute(mrml_const::user_name).second);	
	//do not traverse deeper
	return false;
    }
    //----------------------------------------
    //the get-collections
    if(inElement.getName()==mrml_const::get_collections){
	mHandler->getCollections();	
	//do not traverse deeper
	return false;
    }
    //----------------------------------------
    //the get-algorithms
    if(inElement.getName()==mrml_const::get_algorithms){
	mHandler->getAlgorithms();	
	//do not traverse deeper
	return false;
    }
    //------------------------------
    // query-step
    if(inElement.getName()==mrml_const::query_step){

	mHandler->addToMultiResponse(lSessionManager.query(mSessionID,
							   inElement));
	return false;
    }
    //------------------------------
    // open-session
    if(inElement.getName()==mrml_const::open_session){
	cout << "HIER OPEN SESSION" << endl;
	mHandler->openSession(inElement.stringReadAttribute(mrml_const::user_name).second,
			      inElement.stringReadAttribute(mrml_const::session_name).second);
	return false;
    }
    //------------------------------
    // rename-session
    if(inElement.getName()==mrml_const::rename_session){
      mHandler->renameSession(inElement.stringReadAttribute(mrml_const::session_id).second,
			      inElement.stringReadAttribute(mrml_const::session_name).second);
      return false;
    }
    //------------------------------
    // close-session
    if(inElement.getName()==mrml_const::delete_session){
      cout << "deleteSESSION" << endl;
      mHandler->deleteSession(inElement.stringReadAttribute(mrml_const::session_id).second);
      return false;
    }
    //------------------------------
    // get-property-sheet
    if(inElement.getName()==mrml_const::get_property_sheet){
	mHandler->renameSession(inElement.stringReadAttribute(mrml_const::session_id).second,
				inElement.stringReadAttribute(mrml_const::session_name).second);
	return false;
    }
    //  
    if(inElement.getName()==string(mrml_const::cui_random_images)){
	mHandler->sendRandomImages(mSessionID,
				   inElement.stringReadAttribute(mrml_const::algorithm_id).second,
				   inElement.stringReadAttribute(mrml_const::collection_id).second,
				   inElement.stringReadAttribute(mrml_const::result_size).second);
    }

    //  
    if(inElement.getName()==string(mrml_const::get_property_sheet)){
	mHandler->getPropertySheet(mSessionID,
				   inElement.stringReadAttribute(mrml_const::algorithm_id).second);
    }

    if((inElement.getName()==mrml_const::mrml)
       ||
       (inElement.getName()==mrml_const::configure_session)
       ||
       (inElement.getName()=="__ROOT__")){
	return true;
    }

    return false;
};

void CXEVCommunication::endVisit(const CXMLElement& inElement){
  if(inElement.getName()==mrml_const::mrml){
    mHandler->endMultiRequest();
  }
}
