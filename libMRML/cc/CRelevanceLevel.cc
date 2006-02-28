// -*- mode: c++ -*- 
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
/***************************************
*
* CRelevanceLevel
*
****************************************
*
* modification history:
*
* HM 090399 created the documentation
*
****************************************
*
* compiler defines used:
*
*
****************************************/
#include <cstdio>
#include "libMRML/include/CRelevanceLevel.h"



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
/* Constructor with no inputs */
CRelevanceLevel::CRelevanceLevel():
  mURL("No URL!"),
  mRelevanceLevel(0)
{};



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
/* constructor with input */
CRelevanceLevel::CRelevanceLevel(const string& inURL,
				 double inRelevanceLevel):
  mURL(inURL),
  mRelevanceLevel(inRelevanceLevel){
};



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
/* constructur with input */
CRelevanceLevel::CRelevanceLevel(const CRelevanceLevel& inElement):
  mURL(inElement.mURL),
  mRelevanceLevel(inElement.mRelevanceLevel)
{
};



/***************************************
*
* input
*
****************************************
*
* modification history
*
* 
*
****************************************/
/* reads URL and relevance level from a file */
bool CRelevanceLevel::input(istream& inStream){
  inStream >> mURL
	   >> mRelevanceLevel;
};



/***************************************
*
* output
*
****************************************
*
* modification history
*
* 
*
****************************************/
/* writes URL and relevance level to a file */
bool CRelevanceLevel::output(ostream& outStream)const{
  outStream << mURL << " " << mRelevanceLevel << endl;
};



/***************************************
*
* outputHTML
*
****************************************
*
* modification history
*
* 
*
****************************************/
/* writes the output data as an html file */
bool CRelevanceLevel::outputHTML(ostream& outStream)const{
  outStream << "<IMG SRC=\"" 
	    << mURL 
	    << "\">" 
	    << endl
	    << "<CENTER>"
	    << endl
	    << mRelevanceLevel 
	    << endl
	    << "</CENTER>"
	    << endl;
};



/***************************************
*
* getURL
*
****************************************
*
* modification history
*
* 
*
****************************************/
/* returns the URL */
string CRelevanceLevel::getURL()const{
  return mURL;
};



/***************************************
*
* getRelevanceLevel
*
****************************************
*
* modification history
*
* 
*
****************************************/
/* returns only the relevance level */
double CRelevanceLevel::getRelevanceLevel()const{
  return mRelevanceLevel;
};



/***************************************
*
* setRelevanceLevel
*
****************************************
*
* modification history
*
* 
*
****************************************/
/* sets the relevance level directly */
void CRelevanceLevel::setRelevanceLevel(const double inRelevanceLevel){
  mRelevanceLevel=
    inRelevanceLevel;
};



/***************************************
*
* adjustRelevanceLevel - adds another relevance level to the current
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CRelevanceLevel::adjustRelevanceLevel(const double inRelevanceLevel){
  mRelevanceLevel+=
    inRelevanceLevel;
};



/***************************************
*
* divideRelevanceLevelBy
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CRelevanceLevel::divideRelevanceLevelBy(const double inDivisor)
{
  mRelevanceLevel/=
    inDivisor;
};

string CRelevanceLevel::toXML()const{

  char lBuffer[20];
  sprintf(lBuffer,"%lf",mRelevanceLevel);

  return string(string("<sresultelement userrelevance=\"0.5\" "
		       "calculatedsimilarity=\"")+string(lBuffer)+"\" "
		+string("imagelocation=\"")+mURL+string("\"/>\n"));
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
bool operator<(const CRelevanceLevel& l,const CRelevanceLevel& t)
{
  return l.getRelevanceLevel()<t.getRelevanceLevel();
};


bool CSortByURL_RL::operator()(const CRelevanceLevel& l,
			       const CRelevanceLevel& t)const{
  return 
    l.getURL()
    <
    t.getURL();
};
