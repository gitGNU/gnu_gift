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



//configuration files for sessions and algorithms/collections
#ifndef _CCOMMUNICATIONHANDLER
#define _CCOMMUNICATIONHANDLER

//the expat xml parser by J.Clark
#include "expat/xmlparse/xmlparse.h"
//a class for using xpat attributes in a nice way
#include "libMRML/include/CAttributeList.h"

//This is mrml+gift specific
#include "libMRML/include/CSessionManager.h"
#include "libMRML/include/CXMLElement.h"
#include <functional>
#include <iostream>
#include <fstream>
#include "libMRML/include/CMutex.h"
//----------------------------------------
/** prototypes for use with expat */
extern void startMRMLElement(void *userData, 
			     const char *inElementName, 
			     const char **inAttributes);
extern void endMRMLElement(void *userData, const char *name);

/** a helper class for use with expat, which also handles the socket stuff.
 *@author: Wolfgang Müller
 */
class CCommunicationHandler{

  /** 
      the root of the Document tree which we are making.
      It is destroyed automatically when overwritten.
  */
  CSelfDestroyPointer<CXMLElement> mDocumentRoot;
  
  /** the xml parser which is used for all this
   */
  XML_Parser mParser;  
  
  /**  <SEQUENCE MATTERS HERE> */
  /** session manager */
  CSessionManager& mSessionManager;
  /**  </SEQUENCE MATTERS HERE> 
       
       An xml subtree, containing
       all configured algorithms 
  */
  CAlgorithm* mAlgorithmTree;
  /**
     A DOM-alike tree built from this text
     
  */
  CSelfDestroyPointer<CXMLElement> mCurrentTree;

  /** 
      The string containing and describing 
      the peer socket. This can be an IP socket,
      or unix socket peer credentials
   */
  string mPeerAddressString;
  
public:
  /**
     A DOM-alike tree which will be built from all the answers 
     given to the requests

     at the moment this is public, because I need it to 
     slowly hack the negotiation code.
  */
  CSelfDestroyPointer<CXMLElement> mMultiResponse;

protected:
  /** We have said, that the user can tell, if he wants a query
      in the normal sense or rather a collection of random images,
      in which case the inner parts of the query are ignored. 

      The flag used to perform this is this variable.
  */
  int mQueryAtRandomCount;
protected:
  /** The socket, which is used for this communication */
  int mSocket;
  /** The logfile which logs all communication. *this
      will APPEND NEW DATA to the logfile, as you would expect
  */
  ofstream& mLog;
public:
  //----------------------------------------
  /** communications: */
  //----------------------------------------
  /**  setting the communication socket for this session */
  void setSocket(int inSocket);

  //----------------------------------------
  /**  helpers for creating mrml messages */
  //----------------------------------------
  /** the preamble for a session */
  string preamble();



  /** Frame: this is for all messages the same */
  string frame(const string& inSession,
	       const string& inString);

    
  //----------------------------------------
  /** making attributes out of name/value pairs */
  string toAttribute(string inName,
		     string inString);

  
  string toAttribute(string inName,
		     int inInt);

  
  string toAttribute(string inName,
		     double inFloat);
    
  //     //----------------------------------------
  //     /** turning a relevance level element into a string */
  //     /** (obsolete) */
  //     string stringOfRelevanceLevelElement(const CRelevanceLevel& inRE,
  // 					 double inUserRelevance=0.5);
    
  //     string stringOfRelevanceLevelList(const CRelevanceLevelList& inRLL);
    
    
  //----------------------------------------
  /** Error message */
  int sendError(const string& inSession,
		const string& inMessage);

  //----------------------------------------
  /**  Opening a session */
  void openSession(const string& inUserName,
		   const string& inSessionName);
  /**  renaming the current session */
  void renameSession(const string& inSessionID,
		     const string& inNewName);

  /**  renaming the current session */
  void deleteSession(const string& inName);


  /**  Gets the property sheet contained in the algorithm 
       with inAlgorithmID*/
  void getPropertySheet(const string& inSessionID,
			const string& inAlgorithmID);
  //----------------------------------------
  /** the handshake message (old MRML)*/
  int sendHandshake(const string& inUser);
    
  /** getting the sessions for one user (new MRML)*/
  void getSessions(const string& inUser);
  /** getting collections available on a system*/
  void getCollections();
  /** getting algorithms available on a system*/
  void getAlgorithms();
  //----------------------------------------
  /** the result of a query */
  int sendResult(const string& inSession,
		 const CXMLElement& inRLL);

  
  //----------------------------------------
  /** random images */
  int sendRandomImages(const string& inSession,
		       const string& inAlgorithm,
		       const string& inCollection,
		       const string& inNumberOfImages) ;
  ;
  
  
  //----------------------------------------
  /** processing a query */
  //----------------------------------------
  CXMLElement* gQueryImages;
  string mSessionID;
  int    mResultSize;
  double mCutoff;
  string mCollection;
  string mAlgorithm;

  //----------------------------------------
  /** setting properties of the query */
  void setResultSize(int inResultSize);


  void setResultCutoff(const string& inCutoff);

  void setResultCutoff(double inCutoff);


  void setCollectionID(const string& inID);


  void setAlgorithmID(const string& inID);

  //----------------------------------------
  /** Start building a tree by successive adding
      of XML elements */
  void startTreeBuilding(const char* inElementName,
			 const char*const*const inAttributes);
  /** Start building a tree by successive adding
      of XML elements */
  void addToCurrentTree(const char* inElementName,
			const char*const*const inAttributes);
  /** 
      move up in the tree
  */
  void moveUpCurrentTree();
  /** 
      is this building a tree at present?
  */
  bool isBuildingTree()const;

  //----------------------------------------
  /** parse XML using expat */
  //----------------------------------------
  void parseString(const string& inMessage);


  //----------------------------------------
  /** 
      Clear the algorithm tree element.
      We do not assume destruction responsability here

      This responsability is transferred to the 
      containing session.

      FIXME do something cleaner for the case of failure
  */
  void clearAlgorithmElement();
  //----------------------------------------
  /** start of 
      an element in the tree of configured
      algorithms.

      If mAlgorithm=0 then generate a new
      XML element with the content given
      by inName and inAttributes

      @param inName The name like given by expat
      @param inAttributes an attributes list like given by
      expat
  */
  //----------------------------------------
  void startAlgorithmElement(const char* inName,
			     const char* const* const inAttributes);
  //----------------------------------------
  /** end of 
      an element in the tree of configured
      algorithms                          */
  //----------------------------------------
  void endAlgorithmElement();
  //----------------------------------------
  /** 
      clear the pointer to the algorithm tree
  */
  //----------------------------------------
  void initAlgorithmElement();
  //----------------------------------------
  /** read the pointer to the algorithm tree
   */
  //----------------------------------------
  CAlgorithm* readAlgorithmElement();
  //----------------------------------------
  /** 
      End the configuration i.e. make out
      of the algorithm structure a query
      processing structure.
  */
  void endConfiguration();

  //----------------------------------------
  /** parsing from a stream: 
   * read each character 
   * parse it */
  bool mParsingFinished;
  void clearParsingFinished();
  void setParsingFinished();
  bool isParsingFinished()const;

  bool readAndParse();

  

  void makeParser();


  /** 

      If we process multiple queries which are part of one message,
      we have to first collect the answers from the requests, and then
      send the whole message.

      startMultiRequest and endMultiRequest

      are the functions which administer this process.

      startMultiRequest clears the message which is going to be built.
  */
  void startMultiRequest(const string& inSessionID);
  /** sends the message which has been built*/
  void endMultiRequest();
  /** 
      adds an XMLElement to the multi-response which is built
  */
  void addToMultiResponse(CXMLElement* inElement);


  
  //----------------------------------------
  /** Using the session manager which is a member */
  /** of this for other purposes */
  //----------------------------------------
  CSessionManager& getSessionManager();
  
  /** incrementing the QueryAtRandomCount flag */
  void incrementQueryAtRandomCount();
  /** reading this flag */
  int getQueryAtRandomCount()const;

  /** 
      set the name of the peer,
      this is just an informative string,
      destined for the log.

      The string can contain either the IP of the
      connecting computer, or else the peer credentials
      of the connecting tasks.
  */
  void setPeerAddressString(string);
  /** get the Peer adress string */
  const string& getPeerAddressString()const;
  
  //----------------------------------------
  /** constructor/destructor */
  //----------------------------------------
  CCommunicationHandler(CSessionManager& inSessionManager,
			ofstream& inLogFile);

  ~CCommunicationHandler();
};


#endif
