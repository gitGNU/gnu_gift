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
/** my way of throwing exceptions */
#include "libMRML/include/mrml_const.h"
#include "libMRML/include/my_throw.h"
#include "libMRML/include/my_assert.h"
#include "libMRML/include/my_diagnose.h"

#include <stdio.h>
#include "expat/xmlparse/xmlparse.h"
#include "libGIFTQuInvertedFile/include/CQInvertedFile.h"
#include "libMRML/include/CSessionManager.h"
#include "libMRML/include/CAttributeList.h"
#include "libMRML/include/CXMLHelper.h"
#include "libMRML/include/GIFTExceptions.h"

#include "libMRML/include/CQueryTreeNode.h"

#include <stdlib.h>
#include "libMRML/include/CAlgorithm.h" //getPropertySheet

#include "libMRML/include/directory.h" // directorys as determined by ./configure
#include "libMRML/include/createErrorMessage.h"

//the "ID administration subsystem"
TID gID;


void startConfigElement(void *userData, 
			const char *inElementName, 
			const char **inAttributes)
{
  bool lSuccessful=true;
  //the calling CSessionManager
  CSessionManager* lCaller((CSessionManager*)userData);

#ifdef CONFIG_PRINTOUT
  cout << "<"
       << inElementName
       << " WhileReading=\"true\" "
       << flush;
#endif

  CAttributeList lAttributes(inAttributes);

  if(string(inElementName)==
     string(mrml_const::cui_property_list)){
    lCaller->startPropertySheetSubtree();
  }

  //if we have started a property sheet list
  //everything goes there
  if(lCaller->getPropertySheetSubtree()){
    if((string(inElementName)==
	string(mrml_const::cui_property_item))
	||
       (string(inElementName)==
	string(mrml_const::property_sheet))){
	lCaller->getPropertySheetSubtree()
	    ->addChild(inElementName,
		       inAttributes);
    }
  }
  // / removing for removing cui-algorithm-id-list-list
//   //
//   // a list of algorithms
//   //
//   if(string(inElementName)
//      ==
//      string(mrml_const::cui_algorithm_id_list)){
//     string lListID=lAttributes[mrml_const::cui_algorithm_id_list_id];
//     if(lListID.size()){
//       lCaller->addAlgorithmIDList(lListID);
//     }
//   }
//   if(string(inElementName)
//      ==
//      string(mrml_const::cui_algorithm_id)){
//       cout << "HIER++" << endl;
//     string lID=lAttributes[mrml_const::cui_algorithm_id];
//     if(lID.size()){
//       lCaller->addToCurrent(lID);
//     }
//   }
//   if(string(inElementName)
//      ==
//      string(mrml_const::cui_algorithm_id_list_list)){
//       cout << "HIER" << endl;
//   }
  if(string(inElementName)
     ==
     string(mrml_const::algorithm_list)){
    //nothing
  }
//   if(string(inElementName)
//      ==
//      string(mrml_const::algorithm)
//      || (lCaller->getAlgorithmInConstruction())
//      ){
//       {
// 	  if(!lCaller->getAlgorithmInConstruction()){
// 	      lCaller->startAlgorithmConstruction(new CAlgorithm(inElementName,
// 								 inAttributes));
// 	  }else{
// 	      lCaller->getAlgorithmInConstruction()->addChild(inElementName,
// 							      inAttributes);
// 	  }      
//       }
//   }

#ifdef CONFIG_PRINTOUT
  cout << " >"
       << endl
       << flush;
#endif
}

void endConfigElement(void *userData, const char *inElementName)
{
  CSessionManager* lCaller((CSessionManager*)userData);

//   if((string(inElementName)
//      ==
//      string(mrml_const::algorithm))
//      ||lCaller->getAlgorithmInConstruction()){
//     {
//       my_assert(lCaller->getAlgorithmInConstruction(),
// 		"Algorithm in construction not found");

//       lCaller->getAlgorithmInConstruction()->moveUp();
//       if(lCaller->getAlgorithmInConstruction()->isSubtreeFinished()){
// 	lCaller->addAlgorithm(lCaller->getAlgorithmInConstruction());
// 	lCaller->startAlgorithmConstruction((CAlgorithm*)0);
//       }
//     }
//   }
  //if we have started a property sheet list
  //everything goes there
  if(lCaller->getPropertySheetSubtree()){
    if((string(inElementName)==
	string(mrml_const::cui_property_item))
	||
       (string(inElementName)==
	string(mrml_const::property_sheet))){
      lCaller->getPropertySheetSubtree()
	->moveUp();
    }
  }
  if(string(inElementName)==
     string(mrml_const::cui_property_list)){
    lCaller->endPropertySheetSubtree();
  }
#ifdef CONFIG_PRINTOUT
  cout << "</"
       << inElementName
       << " >"
       << endl;
#endif
}

#include <time.h>
#include "libMRML/include/CSessionManager.h"
#include "libMRML/include/TID.h"
#include <iostream>
extern TID gID;

string getNewID(){
    char lID[20];
    sprintf(lID,"%ld",++gID);
    return lID;
}

/** */
CQueryTreeBuilder CSession::mQueryTreeBuilder;


ostream& CSession::write(ostream& outStream)const{
  mMutexSession.lock();
   outStream << mUser 
	     << " " 
	     << mID 
	     << " " 
	     << mSessionName 
	     << " " 
     //<< mActiveAlgorithm 
	     << endl;
  mMutexSession.unlock();
   return outStream;
};
istream& CSession::read(istream& inStream){
  mMutexSession.lock();
  inStream >> mUser 
	   >> mID 
	   >> mSessionName;
  mMutexSession.unlock();
  return inStream;
      //>> mActiveAlgorithm 
		    ;
};


bool CSession::open(){
  mMutexSession.lock();
  if(mIsOpen){
    return false;
  }
  mIsOpen=true;
  mMutexSession.unlock();
  return mIsOpen;
};

bool CSession::close(){
  mMutexSession.lock();
  mIsOpen=false;
  mMutexSession.unlock();
  return true;
};

bool CSession::rename(const string& inName){
  mMutexSession.lock();
  setSessionName(inName);
  mMutexSession.unlock();
  return true;
};


string CSession::toXML(bool isPrivate)const{
  mMutexSession.lock();
  CXMLHelper lHelper;
  lHelper.setName(mrml_const::session);
  lHelper.setEmpty();
  lHelper.addAttribute(mrml_const::session_id,mID);
  lHelper.addAttribute(mrml_const::session_name,mSessionName);
  if(isPrivate){
    lHelper.addAttribute("activealgorithm",mActiveAlgorithm->getID());
  }
  mMutexSession.unlock();
  return lHelper.toString();
}

//
CSession::CSession():
  mActiveAlgorithm(0),
  mSessionName(""),
  mID(""),
  mIsOpen(0),
  mUser("")
{
  mMutexSession.lock();
  // nothing: use it with read
  /** The information about the current query tree */
  mQueryTree.first=0;
  mQueryTree.second=0;
  mMutexSession.unlock();
};
//
CSession::CSession(string inUser,
		   string inID,
		   string inSessionName,
		   CAlgorithm* inActiveAlgorithm)
{
  // mutex not needed. if two call this 
  // at the same time, there is some
  // s... happening any way.
  mUser=inUser;
  mID=inID;
  mSessionName=inSessionName;
  mActiveAlgorithm=inActiveAlgorithm;
  mQueryTree.first=0;
  mQueryTree.second=0;
  mIsOpen=false;
};
/** delete all members */
CSession::~CSession(){
  // mutex not needed. if two call this 
  // at the same time, there is some
  // s... happening any way.
  delete mQueryTree.first;
  delete mQueryTree.second;
  delete mActiveAlgorithm;
}
//
string CSession::getUser()const{
  return mUser;
};
void CSession::setUser(const string& inUser){
  mMutexSession.lock();
  mUser=inUser;
  mMutexSession.unlock();
};

string CSession::getID()const{
  // mutex not needed. if two call this 
  // at the same time, there is some
  // s... happening any way.
  return mID;
};
void CSession::setID(const string& inID){
  // mutex not needed. if two call this 
  // at the same time, there is some
  // s... happening any way.
  mID=inID;
};

string CSession::getSessionName()const{
  return mSessionName;
};
void CSession::setSessionName(const string& inSessionName){
  mMutexSession.lock();
  mSessionName=inSessionName;
  mMutexSession.unlock();
};

CAlgorithm* CSession::getActiveAlgorithm(){
  return mActiveAlgorithm;
};
bool CSession::setActiveAlgorithm(CAccessorAdminCollection& inCaller,
				  CAlgorithmCollection& inAlgorithmCollection,
				  CAlgorithm* inActiveAlgorithm,
				  CStaticQueryFactory& inBaseTypeFactory){
  mMutexSession.lock();
  assert(inActiveAlgorithm);
  //  inActiveAlgorithm->check();
  
  {
    if(mQueryTree.first){
      CQueryContainer::iterator lFound=mQueryTree.first->find(inActiveAlgorithm->getID());
      if(lFound!=mQueryTree.first->end()){
	mQueryTree.first->erase(lFound);
      }
    }
    delete mQueryTree.second;
    mQueryTree.second=0;
  }
  cout << "[set" << endl;
  mActiveAlgorithm=inActiveAlgorithm;
  
  if(inActiveAlgorithm->stringReadAttribute(mrml_const::algorithm_type).first){


    cout << inActiveAlgorithm->stringReadAttribute(mrml_const::algorithm_type).second
	 << endl;
    
    cout << "set]" << endl;
    cout << "[build" 
	 << endl;
    mActiveAlgorithm->configure(inAlgorithmCollection);
    string lString;
    mActiveAlgorithm->toXML(lString);
    cout << "-Algorithm CONFIGURED-: configuration"
	 << endl 
	 << lString;
    cout << "-Algorithm CONFIGURED-" 
       << endl;
    mQueryTree=mQueryTreeBuilder.buildQueryTree(*inActiveAlgorithm,
						inCaller,
						inBaseTypeFactory,
						0);
  }
  cout << "build]" << endl;
  mMutexSession.unlock();
  return true;
};


//--------------------------------------------------
CXMLElement* CSession::query(CSessionManager&     inCaller,
			     const CXMLElement& inQuery){
  try{
    mMutexSession.lock();
    // i.e. if a user opens a session twice, he it is impossible to
    // do two queries in parallel on this server
    pair<bool,string> lAlgorithmID=inQuery.stringReadAttribute(mrml_const::algorithm_id);
    
    if(lAlgorithmID.first){
      cout << "THE ID "
	   << lAlgorithmID.first
	   << ";"
	   << lAlgorithmID.second
	   << endl;
      
      mQueryTree.first->checkNPrint();
      
      CQueryTreeNode* lQuery(mQueryTree.first->getQueryByID(lAlgorithmID.second));

      CXMLElement* lReturnValue(lQuery->query(inQuery));
      mMutexSession.unlock();
      return lReturnValue;
    }else{
      CQueryTreeNode* lQuery(mQueryTree.first->getQueryByID("adefault"));
      CXMLElement* lReturnValue(lQuery->query(inQuery));
      mMutexSession.unlock();
      return lReturnValue;
    }
  }
  catch(GIFTException* inException){
    return createErrorMessage(inException->getMessage());
  }
  catch(GIFTException& inException){
    return createErrorMessage(inException.getMessage());
  }
  catch(...){
    cout << "something caught" << endl;
    cerr << "something caught" << endl;
    
    exit(10);
  }
};

//--------------------------------------------------
// retrieving random images as seeds
CXMLElement* CSession::getRandomImages(CSessionManager& inCaller,
				       const string& inAlgorithmID,
				       int inNumberOfImages){
  mMutexSession.lock();
  CXMLElement lQuery(mrml_const::query_step,
		     0);

  lQuery.addAttribute(mrml_const::result_size,
		      long(inNumberOfImages));

  CXMLElement* lReturnValue(mQueryTree.first->getQueryByID(inAlgorithmID)->query(lQuery));
  mMutexSession.unlock();
  return lReturnValue;

};
ostream& operator<<(ostream& outStream,
		    const CSession& inSession){
  inSession.write(outStream);
  return outStream;
}

///
CSessionManager::CSessionManager(string inSessionFileName,
				 string inConfigFileName):
  mPropertySheetList(0),
  mPropertySheetSubtree(0),
  mAccessorAdminCollection(inConfigFileName),
  CAlgorithmCollection(inConfigFileName),
  mBaseTypeFactory(new CDynamicQueryFactory(__LIBDIR__)){

  ifstream lSessions(inSessionFileName.c_str());
  ifstream lConfigFile(inConfigFileName.c_str());
  
  lConfigFile.seekg(0,ios::end);
  int lSize=lConfigFile.tellg();
  char lConfigFileContent[lSize+1];

  lConfigFile.seekg(0,ios::beg);
  lConfigFile.read(lConfigFileContent,
		    lSize);

  lConfigFileContent[lSize]=0;

#ifdef PRINTOUT_CONFIG		      
  cout << "I did it: I read the config file:"
       << lConfigFileContent
       << endl
       << flush;
#endif

  XML_Parser lParser = XML_ParserCreate(NULL);//default encoding
  XML_SetUserData(lParser,
		  this);
  XML_SetElementHandler(lParser, 
			startConfigElement, 
			endConfigElement);
  int lDone=true;
  do {
    if (!XML_Parse(lParser, 
		   lConfigFileContent, 
		   lSize, 
		   lDone)) {
      cerr << "CSessionManager: XML ERROR: "
	   << XML_ErrorString(XML_GetErrorCode(lParser))
	   << " at line "
	   << XML_GetCurrentLineNumber(lParser)
	   << endl;
      exit(1);
    }
  } while (!lDone);


  XML_ParserFree(lParser);  
  //
#ifdef CONFIG_PRINTOUT
  cout << endl
       << endl 
       << "Before generating XML"
       << endl
       << endl;
  cout << toXML()
       << endl;
#endif

}

///
string CSessionManager::toXMLSessions(const string& inUser,
				      bool isPrivate)const{
  mMutexSessionManager.lock();
  string lReturnValue;

  CXMLHelper lHelper;
  lHelper.setName("ssessionlist");

  if(isPrivate){
    lHelper.addAttribute("user",inUser);
  }

  lReturnValue+=lHelper.toString();


  const list<CSession>& lList=mUserToSessions.find(inUser)->second;

  for(list<CSession>::const_iterator i=lList.begin();
      i!=lList.end();
      i++){
    lReturnValue+=i->toXML(isPrivate);
  }

  lHelper.setEnd();
  lReturnValue+=lHelper.toString();
  mMutexSessionManager.unlock();
  return lReturnValue;
}



string CSessionManager::toXML(bool isPrivate)const{
  mMutexSessionManager.lock();
  string lReturnValue(CAlgorithmCollection::toXML(isPrivate));
  //lReturnValue += convertPropertyToXML(isPrivate);
  //lReturnValue += CAlgorithmIDListList::toXML();
  lReturnValue += mAccessorAdminCollection.toXML(isPrivate);
  mMutexSessionManager.unlock();
  return lReturnValue;
}

/** creates a full property sheet out of the data in this.
string CSessionManager::convertPropertyToXML(bool isPrivate)const{

  string lReturnValue("\n\n<!-- generated by convertPropertyToXML -->\n");

  //0. The root of the property sheet which we want to generate
  CXMLElement lRootProperty(mrml_const::property_sheet,0);
  lRootProperty.addAttribute("caption","Algorithms");
  lRootProperty.addAttribute(mrml_const::property_sheet_type,"subset");
  lRootProperty.addAttribute(mrml_const::property_sheet_id,"all-algorithms");
  lRootProperty.addAttribute(mrml_const::send_type,"element");
  lRootProperty.addAttribute(mrml_const::send_value,"algorithm");

  
  // 2. make out of each algorithm in the collection a property sheet.
  for(CAlgorithmCollection::CContent::const_iterator i=
	CAlgorithmCollection::begin();
      i!=CAlgorithmCollection::end();
      i++){

    //
    CAlgorithm* lAlgorithm(i->second);
    string lType(lAlgorithm->getType().second);

    my_diagnose("in for loop: Algorithm type is " << lType);

    my_assert(lAlgorithm && lType.size(),
	      "In the config file");


		      
    // 2.a. make a new XMLElement, initialise it with the algorithm
    //      base settings
    CXMLElement* lAlgorithmSheet=new CXMLElement("property");
    assert(lAlgorithmSheet);
    //set the caption
    lAlgorithmSheet->addAttribute("caption",
				  lAlgorithm
				  ->stringReadAttribute("name").second);
    lAlgorithmSheet->addAttribute("type","setelement");
    //send the type of the algorithm
    lAlgorithmSheet->addAttribute("sendtype","attribute");
    lAlgorithmSheet->addAttribute("sendname","type");
    lAlgorithmSheet->addAttribute("sendvalue",lAlgorithm->getType().second);
    //send also an automatically generated id.
    lAlgorithmSheet->addAttribute("autoid","yes");
    lAlgorithmSheet->addAttribute("autoidname","id");
    // 2.a. which collections can be used with this algorithm?
    //      look this up, make this the first child.

    // ---- this is the part I dread:
    // ---- this piece of code is amazingly ugly.
    // ---- but please remember: this happens only once in a session
    // ---- and this is what happens, if you change EVERYTHING and
    // ---- you would like interfaces to stay about the same....
    // ---- frustration starts to get visible here...

    CXMLElement* lCollectionSheet=new CXMLElement("property");
    //set the caption
    lCollectionSheet->addAttribute("caption",
				   "Collections");
    
    lCollectionSheet->addAttribute("type","subset");
    lCollectionSheet->addAttribute("id","coll"+lAlgorithm->getType().second);
    lCollectionSheet->addAttribute("sendtype","element");
    lCollectionSheet->addAttribute("sendname","algorithm");

    //go over all collections
    for(CAccessorAdminCollection::const_iterator 
	  j=mAccessorAdminCollection.begin();
	j!=mAccessorAdminCollection.end();
	j++){
      
      my_diagnose("checking for collection " 
		  << 
		  j->second->getAlgorithmIDListID());

      //look if this algorithm is allowed for this collection
      //if yes, than add this to the collectionNode

      // j->first is the first element of the list
      bool lIsMember=CAlgorithmIDListList::isIDListMember(j->second->getAlgorithmIDListID(),
							  lType);

      if(lIsMember){
	//here we insert something containing the right caption
	CXMLElement* lCollectionSheetElement=new CXMLElement(mrml_const::property_sheet);
	//set the caption
	lCollectionSheetElement->addAttribute(mrml_const::caption,
					      j->second->getName());
	lCollectionSheetElement->addAttribute(mrml_const::property_sheet_type,"set-element");
	//send the type of the collection
	lCollectionSheetElement->addAttribute(mrml_const::send_type,"attribute");
	lCollectionSheetElement->addAttribute(mrml_const::send_name,mrml_const::collection_id);
	lCollectionSheetElement->addAttribute(mrml_const::send_value,j->second->getID());

 	lCollectionSheet->addChild(lCollectionSheetElement);
 	lCollectionSheet->moveUp();
	
      }
      
    }
    // 2.b. make the newly made lCollectionSheet a new child of the algorithm
    //      sheet
    lAlgorithmSheet->addChild(lCollectionSheet);
    lAlgorithmSheet->moveUp();



    
    // ---- now things are getting a bit cleaner again

    // 2.c. the property sheet chosen in the config
    //      look this up, if it exsists make it the second child.
    {
      pair<bool,string> lUserConfigID=
	lAlgorithm->stringReadAttribute("vi-more-config");
      if(lUserConfigID.first){

	CXMLElement* lUserConfiguredProperties=
	  mPropertySheetList->newPropertySheet(lUserConfigID.second);

	
	if(lUserConfiguredProperties){
	  lAlgorithmSheet->addChild(lUserConfiguredProperties);
	  lAlgorithmSheet->moveUp();
	}
      }
    }
    
    // 2.d. make the newly made lAlgorithmSheet a new child of the root.
    lRootProperty.addChild(lAlgorithmSheet);
    lRootProperty.moveUp();
  }


  lRootProperty.toXML(lReturnValue);

  lReturnValue+="\n\n<!-- END: generated by convertPropertyToXML -->\n";
  return lReturnValue;
}*/

///
pair<string,string> CSessionManager::toXMLHandshake(const string& inUser){
  assert(!"I think I should delete this");
    mMutexSessionManager.unlock();
  //
  pair<string,string> lReturnValue;

  //need to make a new default session:
  lReturnValue.first  =newSession(inUser,
				  "");

  lReturnValue.second+="<"+mrml_const::cui_shandshake+">";
  lReturnValue.second+=toXMLSessions(inUser,false);//this is going to be public
  lReturnValue.second+=toXML(false);//this is going to be public
  lReturnValue.second+="</"+mrml_const::cui_shandshake+">";
  mMutexSessionManager.unlock();
  return lReturnValue;
}
///
string CSessionManager::toXMLSessionConfiguration()const{
  string lReturnValue;
  CXMLHelper lHelper;
  

  assert(!"here is no code!");

  return lReturnValue;
}
///
istream& CSessionManager::read(istream& inStream){
  mMutexSessionManager.lock();

    while(!inStream.eof() && inStream){
	CSession lSession;
	lSession.read(inStream);
	if(lSession.getUser().size()){
	    mUserToSessions[lSession.getUser()].push_back(lSession);
	}
    }
  mMutexSessionManager.unlock();
};
///
ostream& CSessionManager::write(ostream& outStream){
  mMutexSessionManager.lock();

    //for each list
    for(map<string,list<CSession> >::const_iterator i=mUserToSessions.begin();
	i!=mUserToSessions.end();
	i++){
	//write the list
	copy(i->second.begin(),
	     i->second.end(),
	     ostream_iterator<CSession>(outStream));
    }
  mMutexSessionManager.unlock();
};
string CSessionManager::newSession(const string& inUser,
				   const string& inSessionName){
  mMutexSessionManager.lock();

  char now[20];
  
  sprintf(now,"%ld",time(0));
  
  string lID=getNewID();

  lID=this->newSession(lID,
		       inUser,
		       inSessionName);
  
  mMutexSessionManager.unlock();
  return lID;
};
string CSessionManager::newSession(const string& inID,
				   const string& inUser,
				   const string& inSessionName){
  mMutexSessionManager.lock();

  char now[20];
  
  sprintf(now,"%ld",time(0));
  
  string lID=inID;
  
  string lSessionName("Session"+string(now));
  
  if(inSessionName.size()){
    lSessionName=inSessionName;
  }
  
  CSession lDefault(inUser,
		    lID,
		    lSessionName,
		    0);
  
  mUserToSessions[inUser].push_back(lDefault);
  //a pointer to the same session
  mIDToSession[lID]=&(mUserToSessions[inUser].back());
  mIDToSession[lID]->open();
  
  mIDToSession[lID]->setActiveAlgorithm(mAccessorAdminCollection,
       					*this,
					makeDefaultAlgorithm(),
       					*mBaseTypeFactory);
  
  mMutexSessionManager.unlock();
  return inID;
};
///
CXMLElement* CSessionManager::openSession(string inUserName,
					  string inSessionID,
					  string inSessionName){

  mMutexSessionManager.lock();
    cout << "ENTERING OPENSESSION"
	 << endl;

  CXMLElement* lReturnValue(0);
  string lReturnID("");
  if(inSessionID.size()){
    CIDToSession::const_iterator lFound(mIDToSession.find(inSessionID));
    if(lFound!=mIDToSession.end()){
      if(lFound->second->open())
	  cout << "USING AN OLD SESSION" << endl;
	lReturnID=inSessionID;
    }else{
      cout << "making a new session I" << endl;
      lReturnID= newSession(inUserName,
			    inSessionName);
    }
  }else{
      cout << "making a new session II" << endl;
      lReturnID= newSession(inUserName,
			    inSessionName);
  }
  if(lReturnID.size()){
    lReturnValue=new CXMLElement(mrml_const::acknowledge_session_op,0);
    if(lReturnValue){
      lReturnValue->addAttribute(mrml_const::session_id,
				 lReturnID);
    }
  }else{
    lReturnValue=new CXMLElement(mrml_const::error,0);
    assert(lReturnValue);
    lReturnValue->addAttribute(mrml_const::message,
			       "Open session: The session with name:"
			       +inSessionName
			       +" and ID:"
			       +inSessionID
			       +"could not be opened");
  }

  // this is added for making parsing log files easier,
  // and to resolve possible ambiguities in log files.
  if(lReturnValue){
    lReturnValue->addAttribute(mrml_const::session_name,
			       inSessionName);
    lReturnValue->addAttribute(mrml_const::user_name,
			       inUserName);
  }

  mMutexSessionManager.unlock();
  return lReturnValue;
};
///
CXMLElement* CSessionManager::renameSession(string inSessionID,
					    string inSessionName){
  mMutexSessionManager.lock();
  CXMLElement* lReturnValue(0);
  string lReturnID("");
  if(inSessionID.size()){
    CIDToSession::const_iterator lFound(mIDToSession.find(inSessionID));
    if(lFound!=mIDToSession.end()){
      if(lFound->second->rename(inSessionName))
	lReturnID=inSessionID;
    }
  }
  if(lReturnID.size()){
    lReturnValue=new CXMLElement(mrml_const::acknowledge_session_op,0);
    assert(lReturnValue);
    lReturnValue->addAttribute(mrml_const::session_id,
			       lReturnID);
  }else{
    lReturnValue=new CXMLElement(mrml_const::error,0);
    assert(lReturnValue);
    lReturnValue->addAttribute(mrml_const::message,
			       "Rename session: The session with name: `"
			       +inSessionName
			       +"' and ID: `"
			       +inSessionID
			       +"' could not be renamed");
  }
  mMutexSessionManager.unlock();
  return lReturnValue;
};
///
CXMLElement* CSessionManager::closeSession(string inSessionID){
  mMutexSessionManager.lock();
  string lReturnID;
  if(inSessionID.size()){
    CIDToSession::iterator lFound(mIDToSession.find(inSessionID));
    if(lFound->second->close()){
      lReturnID=inSessionID;
    }
  }
  CXMLElement* lReturnValue=new CXMLElement(mrml_const::acknowledge_session_op,0);
  lReturnValue->addAttribute(mrml_const::session_id,
			     lReturnID);
  mMutexSessionManager.unlock();
  return (CXMLElement*)0;
}

CXMLElement* CSessionManager::deleteSession(string inSessionID){
  mMutexSessionManager.lock();
  CXMLElement* lReturnValue(0);

  bool lSuccess=false;

  cout << endl << "BEFORE BEFORE Deleting session " << inSessionID << endl;
  if(inSessionID.size()){
    CIDToSession::iterator lFound(mIDToSession.find(inSessionID));
    cout << endl << "BEFORE Deleting session " << inSessionID << endl;
    if(lFound!=mIDToSession.end()){
      lSuccess=true;
      string lUser(lFound->second->getUser());
      mIDToSession.erase(lFound);

      cout << endl << "Deleting session " << inSessionID << " for user " << lUser << endl;

      CUserToSessions::iterator lUserFound=mUserToSessions.find(lUser);
      assert(lUserFound!=mUserToSessions.end());
      
      for(list<CSession>::iterator i=lUserFound->second.begin();
	  i!=lUserFound->second.end();
	  i++){
	if(i->getID()==inSessionID){
	  lUserFound->second.erase(i);
	  break;
	}
      }
      cout << "after break" << endl;
    }
  }

  if(lSuccess){
    lReturnValue=new CXMLElement(mrml_const::acknowledge_session_op,0);
    assert(lReturnValue);
    lReturnValue->addAttribute(mrml_const::session_id,
			       inSessionID);
  }else{
    lReturnValue=new CXMLElement(mrml_const::error,0);
    assert(lReturnValue);
    lReturnValue->addAttribute(mrml_const::message,
			       "Close session: The session with ID: `"
			       +inSessionID
			       +"' could not be deleted");
  }
  mMutexSessionManager.unlock();
  return lReturnValue;
};
bool CSessionManager::setAlgorithm(const string& inSessionID,
 				   CAlgorithm* inAlgorithm){
  mMutexSessionManager.lock();
  //this should also kick of the 
  //creation of a suitable query tree

  cout << "still here inSessionID: _"
       << inSessionID
       << "_"
       << flush
       << endl;

  CSession* lSession=mIDToSession[inSessionID];

  
  //  assert(lSession);
  if(!lSession){
    //create a session with the required sessionID
    this->newSession(inSessionID,
		     "I-dunno",
		     "I-know-even-less");
		     
  }

  bool lResult=lSession->setActiveAlgorithm(mAccessorAdminCollection,
 					    *this,
 					    inAlgorithm,
 					    *mBaseTypeFactory);
  mMutexSessionManager.unlock();
  return lResult;
};

CXMLElement* CSessionManager::query(const string& inSessionID,
				    const CXMLElement& inRelevanceLevelList){
  // this is function not completely locked, because the functions called do not neccessarily 
  // need a lock. LOCKING THIS FUNCTION COMPLETELY WOULD MEAN LOSING CONCURRENCY.
  //mMutexSessionManager.lock();//debugging
  CIDToSession::const_iterator lFound=mIDToSession.find(inSessionID);
  //mMutexSessionManager.unlock();
  //
  if(lFound==mIDToSession.end()){
#ifdef GIFT_THROW_UNKNOWN_SESSION
    cout << "throwing: "
 	 <<(VEUnknownSession(inSessionID.c_str()))
 	 << endl
 	 << flush;
#else
    //     my_throw(VEUnknownSession(inSessionID.c_str()));
    //     cerr << "this line should not be reached" 
    // 	 << endl;
    //     assert(0);
    CXMLElement* lError(new CXMLElement(mrml_const::error,0));
    lError->addAttribute(mrml_const::message,"Could not process query: unknown session (ID:"+inSessionID+").");
    return lError;
#endif
  }
  try{
    CXMLElement* lReturnValue(lFound->second->query(*this,
						    inRelevanceLevelList));
    //mMutexSessionManager.unlock();//debugging
    return lReturnValue;
  }  
  catch(GIFTException& inCaught){
    return createErrorMessage(inCaught); 
  }
  catch(GIFTException* inCaught){
    return createErrorMessage(inCaught); 
  }
};
//--------------------------------------------------
// retrieving random images as seeds
CXMLElement* CSessionManager::getRandomImages(const string& inSessionID,
						      const string& inAlgorithm,
						      int inNumberOfImages){

    //
  mMutexSessionManager.lock();
  CIDToSession::const_iterator lFound=mIDToSession.find(inSessionID);
  mMutexSessionManager.unlock();
  //
  if(lFound==mIDToSession.end()){
#ifdef GIFT_THROW_UNKNOWN_SESSION
    cout << "throwing: "
	 <<(VEUnknownSession(inSessionID.c_str()))
	 << endl
	   << flush;
#else
    my_throw(VEUnknownSession(inSessionID.c_str()));
    CXMLElement* lError(new CXMLElement("error",0));
    lError->addAttribute("message","Could not process query: unknown session (ID:"+inSessionID+").");
#endif
  }
  //
  return lFound->second->getRandomImages(*this,
					 inAlgorithm,
					 inNumberOfImages);
};
CQuery* CSessionManager::makeQuery(const string& inBaseType,
				   CAlgorithm& inAlgorithm){
  mMutexSessionManager.lock();
  CQuery* lReturnValue=mBaseTypeFactory->makeQuery(inBaseType,
						   mAccessorAdminCollection,
						   inAlgorithm);
  mMutexSessionManager.unlock();
  return lReturnValue;
}
//----------------------------------------
/** Building a property sheet list
    I consider building a list on a call by call basis as 
    tedious. So what I do is:
    I first build a tree, and then I break down this tree
    into our list.
    
    This function makes a new tree 
*/
void CSessionManager::startPropertySheetSubtree(){
  //don't FIXME this should work without any if in real c++
  delete mPropertySheetSubtree;
  mPropertySheetSubtree=new CXMLElement(mrml_const::cui_property_list,
 					0);
};
/** This function gets us the current property sheet subtree.
 */
CXMLElement* CSessionManager::getPropertySheetSubtree(){
  return mPropertySheetSubtree;
};
/** This function does the actual breaking down into a 
    CPropertySheetList.
*/
void CSessionManager::endPropertySheetSubtree(){
   mPropertySheetList=new CPropertySheetList(mPropertySheetSubtree);
   mPropertySheetSubtree=0;
};
/** gets a property sheet for the algorithm inAlgorithmID */
CXMLElement* CSessionManager::getPropertySheet(string /*ignored inSessionID*/,
					       string inAlgorithmID)const{
  mMutexSessionManager.lock();
  CAlgorithm& lAlgorithm(getAlgorithmByType(inAlgorithmID));
  CXMLElement* lReturnValue(0);
  if(lAlgorithm.stringReadAttribute(mrml_const::cui_property_sheet_id).first){
    lReturnValue=mPropertySheetList->newPropertySheet(lAlgorithm
						      .stringReadAttribute(mrml_const::cui_property_sheet_id).second);
  }
  mMutexSessionManager.unlock();
  return lReturnValue;
};


CXMLElement* CSessionManager::getCollections()const{
  mMutexSessionManager.lock();
  cout << "getcollections" 
       << endl;
  CXMLElement* lReturnValue(mAccessorAdminCollection.toXMLElement());
  mMutexSessionManager.unlock();
  return lReturnValue;
};
CXMLElement* CSessionManager::getAlgorithms()const{
  mMutexSessionManager.lock();
  CXMLElement* lReturnValue(((CAlgorithmCollection*)this)->toXMLElement());
  mMutexSessionManager.unlock();
  return lReturnValue;
};
