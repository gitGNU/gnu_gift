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
#ifndef _CSESSIONMANAGER
#define _CSESSIONMANAGER

#include <iostream>
#include <string>
#include <list>
#include <map>
#include "CXMLElement.h"
#include "CAccessorAdminCollection.h"
#include "CAlgorithm.h"
#include "CPropertySheetList.h"
#include "CAccessorElement.h"
#include "CAlgorithmCollection.h"
#include "CQueryTreeBuilder.h"
#include "CMutex.h" // multithreading
class CStaticQueryFactory;
class CAlgorithm;
class CSessionManager;
class CQueryTreeNode;
class CStaticQueryFactory;
/**  A class containing a session.
 *   At present a session is just 
 *   a quite rudimentary container
 *   for an algorithm. 
 *
 *   Later we hope to make sessions
 *   persistent (using blobs in
 *   msql, for example).
 *
 *
 *   ATTENTION MULTITHREADING:
 *    WHAT TO LOCK AND WHAT TO UNLOCK IS NOT ENTIRELY TRIVIAL
 *    THIS MIGHT BE A SOURCE OF MISTAKES
 *
 *   @author Wolfgang Müller
 */
class CSession{
protected:
  /** the mutex for this session */
  CMutex mMutexSession;
  /** */
  bool mIsOpen;
  /**  */
  string mUser;
  /**  */
  string mID;
  /** 
   * the name of this session
   */
  string mSessionName;
  /** The algorithm used for the next query
   *  At present this fully describes a session
   */
  CAlgorithm* mActiveAlgorithm;
  /** The information about the current query tree */
  pair<CQueryContainer*,CQueryTreeNode*> mQueryTree;
  /** */
  static CQueryTreeBuilder mQueryTreeBuilder;
public:
  //--------------------------------------------------
  /** Constructors */
  CSession();
  /** 
   */
  CSession(string inUser,
	   string inID,
	   string inSessionName,
	   CAlgorithm* inActiveAlgorithm
	   );
  /**
     destroys all members
   */
  ~CSession();
  //--------------------------------------------------
  /** Read the state */
  ostream& write(ostream& outStream)const;
  /** Write the state */
  istream& read(istream& inStream);
  //--------------------------------------------------
  /** get user of this session */
  string getUser()const;
  /** set user of this session */
  void setUser(const string& inUser);
  /** set ID of this session */
  string getID()const;
  /** get ID of this session */
  void setID(const string& inID);
  /** get the displayname of this session */
  string getSessionName()const;
  /** set the displayname of this session */
  void setSessionName(const string& inSessionName);
  /** get the algorithm of this session */
  CAlgorithm* getActiveAlgorithm();
  /** set the algorithm of this session.
      in fact this amounts to building a complete
      query tree. 

      We also would like to do a bit of
      tree matching to see, how much of the query
      we can rescue after reconfiguration.
  */
  bool setActiveAlgorithm(CAccessorAdminCollection& inCaller,
			  CAlgorithmCollection& inAlgorithmCollection,
			  CAlgorithm* inActiveAlgorithm,
			  CStaticQueryFactory& inBaseTypeFactory);
  /** set the collection of this session */
  //string  getActiveCollection()const;
  /** get the collection of this session */
  //  void setActiveCollection(const string& inCollection);
  //--------------------------------------------------
  /**  
   * performing a query 
   *
   * Gets from the current algorithm the right inSubAlgorithm,
   * from that the right CQuery and hands parameters through to it.
   */
  CXMLElement* query(CSessionManager&     inCaller,
		     const CXMLElement& inQuery);
  //--------------------------------------------------
  /**  retrieving random images as seeds */
  CXMLElement* getRandomImages(CSessionManager& inCaller,
				       const string& inAlgorithm,
				       int inResultSize);
  
  //--------------------------------------------------
  /** opening this session. This implements a lock, such that one
      user can have several open session which do not interfere. 

      The return value tells the use if the open was successful (true)
  */
  bool open();
  //--------------------------------------------------
  /** Closing this session
   */
  bool close();
  //--------------------------------------------------
  /** Renaming this session
      (calls setSessionName, at present)
   */
  bool rename(const string& inName);
  //--------------------------------------------------
  /**  generating XML output for configuration/shandshake */
  string toXML(bool isPrivate)const;
};

/** This structure handles the sessions; 
    by this, it encapsulates 
    the current state of the server.
    Basically, it hands all requests through to the 
    right sessions. It also handles the generation of 
    new sessions etc.. In short, it is the configuration
    manager.

    This class is now approaching what I would like it to be,
    however, cleaning is always wellcome

    ATTENTION MULTITHREADING:
     WHAT TO LOCK AND WHAT TO UNLOCK IS NOT ENTIRELY TRIVIAL
     THIS MIGHT BE A SOURCE OF MISTAKES

*/
class CSessionManager:public CAlgorithmCollection
		      //,public CAlgorithmIDListList
{
  /** the mutex for this sessionmanager */
  CMutex mMutexSessionManager;
protected:
  /**
     The accessor collection is needed for constructing queries
  */
  CAccessorAdminCollection mAccessorAdminCollection;
  /**  
       point the user to all the sessions
  */
  typedef map<string,list<CSession> >   CUserToSessions;
  /**  */
  typedef map<string,CSession*>         CIDToSession;
  /**  */
  mutable CUserToSessions mUserToSessions;
  /**  */
  mutable CIDToSession    mIDToSession;
  /** These are the building blocks of 
      the property sheets to be generated 
      by *this.
  */
  CPropertySheetList* mPropertySheetList;
  /** The subtree from which the mPropertySheets
      will be built */
  CXMLElement* mPropertySheetSubtree;
  /** The base type factory is a factory for query 
      base types */
  CStaticQueryFactory* mBaseTypeFactory;
public:
  /**  
       The session manager slurps in two XML files
       and generates out of this its initial state.
  */
  CSessionManager(string inSessions,
		  string inConfiguration);
  //----------------------------------------
  /**
   *this CSessionManager has all the information
   to make a query structure.
  */
  CQuery* makeQuery(const string& inBaseType,
		    CAlgorithm& inAlgorithm);
  //----------------------------------------
  /** Initialization: read from a stream
   */
  istream& read(istream&);
  /**  Write into a stream*/
  ostream& write(ostream&);
  //----------------------------------------
  /** to be used by the Interface. 
      Not yet implemented.
  */
  CXMLElement* openSession(string inUserName,
			   string inSessionID,
			   string inSessionName);
  //----------------------------------------
  /** to be used by the Interface. 
      Not yet implemented.
  */
  CXMLElement* renameSession(string inSessionID,
			     string inSessionName);
  /**  to be used by the Interface. 
       Not yet implemented.*/
  CXMLElement* deleteSession(string inSessionID);
  /**  Closes the session, but keeps it in memory.
       FUTURE: put the state of the session into a 
       blob
   */
  CXMLElement* closeSession(string inSessionID);
  /**  VeryImportantFunction:

   This function looks for a CSession with CSession::mSessionID==inSessionID
   and hands through the rest of the parameter list to CSession::query.

   */
  CXMLElement* query(const string& inSessionID,
		     const CXMLElement& inRelevanceLevelList);
  //--------------------------------------------------
  /**  retrieving random images (as seeds for subsequent searches)
   *@param inSessionID  The ID of the session in which the
   *                    request was issued.
   *@param inAlgorithm  One would think this is not needed when 
   *                    retrieving random images. However, it is
   *                    useful to know the collection involved,
   *                    and if the random images we are retrieving
   *                    will fit the algorithm used later.
   *@param inCollection The collection from which random images will
   *                    be retrieved.
   */
  CXMLElement* getRandomImages(const string& inSessionID,
			       const string& inAlgorithm,
			       int inResultSize);
  //--------------------------------------------------
  /**  retrieving the URLs of all images (for generating distance matrices) */
  list<CAccessorElement>* getAllAccessorElements(const string& inSessionID,
						 const string& inAlgorithm
						 );
  //--------------------------------------------------
  /**  retrieving the IDs of all images (for generating distance matrices) */
  list<TID>* getAllIDs(const string& inSessionID,
		       const string& inAlgorithm
		       );
  //----------------------------------------
  /** set the algorithm used in this session
      it will also lead to a new construction of
      the query. 

      We have to perform some comparisons here
      present version is probably a HACK
  */
  bool setAlgorithm(const string& inSessionID,
		    CAlgorithm* inAlgorithm);
  /** making a new session and returning its ID */
  string newSession(const string& inUser,
		    const string& inSessionName);
  /** making a new session and for a given ID, returnig the ID */
  string newSession(const string& inID,
		    const string& inUser,
		    const string& inSessionName);
  /**  Set the name of the current session. 
       Presently not implemented or tested*/
  void setSessionName(const string& inOldSessionName,
		      const string& inNewSessionName);
  //----------------------------------------
  /**  turn this into a part of the welcome message 
       in fact, this generates standard property sheets out of the 
       collections and algorithms available

       It means: 

       1. Make out of each algorithm in the collection a property sheet.
       2. Make out of the algorithm-id-list-list a property sheet with
          a parent node which does not send messages.
       3. Make out of the collection list a choice of collections,
          each having as a child a property sheet out of 2.

       If we do things this way we are losing a bit of the flexibility 
       given in the property sheet specification. However, for the moment
       I (WM) consider this loss nost as big compared to what we earn in 
       terms of configuration of the system. If necessary recode.

  string convertPropertyToXML(bool isPrivate=false)const;

  THIS IS DEPRECATED STUFF, SO IT'S DELETED NOW!

  */

  //----------------------------------------
  /**  turn this into a part of the welcome message 
       
       THIS CODE IS TO LEAVE SOON
   */
  string toXML(bool isPrivate=false)const;
  /** The sessions for one user only.
   *
   *@param inUser    the name of the user for which we want the session names.
   *@param isPrivate send internal configuration information along?
   *                 (distinction between messages for internet and config 
   *                  files)
   */
  string toXMLSessions(const string& inUser,
		       bool isPrivate=false)const;
  /** The sessions for everybody known to the system */
  string toXMLSessionConfiguration()const;
  /**  this is almost the complete shandshake message  */
  pair<string,string> toXMLHandshake(const string& inUser);
  /** Get the property sheet of the algorithm with ID inAlgorithmID.
      We look for the attribute cui-property-sheet-id in the algorithm. 
      If this exsists, we look for the right property sheet id in the 
      CPropertySheetList.
   */
  CXMLElement* getPropertySheet(string inSessionID,
				string inAlgorithmID)const;
  //----------------------------------------
  /** Building a property sheet list
      I consider building a list on a call by call basis as 
      tedious. So what I do is:
      I first build a tree, and then I break down this tree
      into our list.
      
      This function makes a new tree 
  */
  void startPropertySheetSubtree();
  /** This function gets us the current property sheet subtree.
  */
  CXMLElement* getPropertySheetSubtree();
  /** This function does the actual breaking down into a 
      CPropertySheetList.
  */
  void endPropertySheetSubtree();
  /** This function probably will quite soon be removed from this class,
   and it will become part of a class containing the CSessionManager*/
  CXMLElement* getCollections()const;
  /** This function probably will quite soon be removed from this class,
      and it will become part of a class containing the CSessionManager*/
  CXMLElement* getAlgorithms()const;
};

#endif
