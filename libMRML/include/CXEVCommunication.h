/* -*- mode: c++ -*- 
*/
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
// -*- mode: c++ -*-
#ifndef _CXEVCOMMUNICATION
#define _CXEVCOMMUNICATION

#include <string> //mSessionID

#include "CXMLElementVisitor.h" //base class

class CCommunicationHandler;

/**
   
   A visitor visiting document trees built from
   the communication.

*/
class CXEVCommunication:public CXMLElementVisitor{

 protected:
    /** 
	the session in which this message is sent
     */
    string mSessionID;
    /** CCommunicationHandler which will build the response from 
	our calls
     */
    CCommunicationHandler* mHandler;
 public:
    /** constructor. 
	When traversing the tree we need to 
	call member functions of 
	the communication handler with
	subtrees of the tree we traverse.
     */
    CXEVCommunication(CCommunicationHandler* inHandler);
    /** actually carrying out the visit */
    virtual bool startVisit(const CXMLElement& inVisited);
    /** actually carrying out the visit */
    virtual void endVisit(const CXMLElement& inVisited);
};

#endif
