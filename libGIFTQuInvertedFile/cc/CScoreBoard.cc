// -*- mode: c++ -*- 
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
/***************************************
*
* CScoreBoard - administrates the Scoreboard of the images
*
****************************************
*
* modification history:
*
* WM 000125 changed things such, that this class
*           is able to live without 
*           knowing the CAccessor
*           results in heavy changes of all functions
*           probable errors in sorting for scoreboard pruning
* HM 090399 created the documentation
* HM 990121 corrected the limit function 
* HM 990120 added new functions 
* HM 990119 changed the output of the scoreboard 
*
****************************************
*
* compiler defines used:
*
*
****************************************/

#include "CScoreBoard.h"
#include "CWeightingFunction.h"
#include "CAcInvertedFile.h"
#include "CRelevanceLevelList.h"
#include "CIDRelevanceLevelPairList.h"
#include <map>

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
CScoreBoard::CScoreBoard(){
    mIgnoreNewValues=false;
}



/***************************************
*
* ()
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CScoreBoard::operator()(const CWeightingFunction& inWF,
			     const CDocumentFrequencyElement& inDFE){
  double lAdjustValue=inWF.apply(inDFE);
  
  //value > 0?
  //if(lAdjustValue!=0)
    {
    // has this document already scored?
    if(find(inDFE.getID())!=end()){
      //yes -> adjust the score
      (*this)[inDFE.getID()] += lAdjustValue;
    }else{
      //no -> create a new scoreboard entry if not blocked
      if (mIgnoreNewValues==false)
	{
	  insert(make_pair
		 (inDFE.getID(),
		  lAdjustValue));
	}
    }
  }
}




/***************************************
*
* output - prints out the results of the scoreboard sorted by relevance level
*
****************************************
*
* modification history
*
* 
*
****************************************/
void CScoreBoard::output(ostream& outStream,const CAccessor& inAccessor)const{
  CRelevanceLevelList lRelevanceLevelList;
  string helpString;

  for(const_iterator i=begin();
      i!=end();
      i++)
    {
      lRelevanceLevelList.push_back(CRelevanceLevel(inAccessor.IDToURL(i->first),i->second));
    }
  lRelevanceLevelList.sort();
  lRelevanceLevelList.reverse();
  
  int lCount=0;
  for(CRelevanceLevelList::const_iterator i=lRelevanceLevelList.begin();
      i!=lRelevanceLevelList.end() && lCount<100;
      i++,
	lCount++){

    helpString=(*i).getURL().substr(43,4)+";";

    outStream << helpString;
    outStream << (*i).getRelevanceLevel();
    outStream << ";";
  }
}




/***************************************
*
* setIgnore - makes new entries into the scoreboard impossible
*
****************************************
*
* modification history
*
* 
*
****************************************/
/* sets the variable to avoid adding new images to the Scoreboard */
void CScoreBoard::setIgnore()
{
  mIgnoreNewValues=true;
}



/***************************************
*
* releaseIgnore - makes new entries into the scoreboard possible again
*
****************************************
*
* modification history
*
* 
*
****************************************/
/* allows the adding of new members to the scoreboard again */
void CScoreBoard::releaseIgnore()
{
  mIgnoreNewValues=false;
}





/***************************************
*
* CSortByRelevanceLevel - needed for the sorting of the scoreboard by relevance level
*
****************************************
*
* modification history
*
* 
*
****************************************/
typedef pair<TID,CRelevanceLevel> CTIDToRelevanceLevelPair;

class CSortByRelevanceLevel:
  public binary_function<CTIDToRelevanceLevelPair,
  CTIDToRelevanceLevelPair,
  bool>{
public:
  inline bool operator()(CTIDToRelevanceLevelPair& l,
			 CTIDToRelevanceLevelPair& t){
    return 
      l.second.getRelevanceLevel()<t.second.getRelevanceLevel();
  }
};



/***************************************
*
* limitNumberTo - limits the scoreboard to the first n entries with the highest score
*
****************************************
*
* modification history
*
* 
*
****************************************/
/* limits the scoreboard to the best n results */
void CScoreBoard::limitNumberTo(int maximumNumber)
{
  CIDRelevanceLevelPairList  lRelevanceLevelList;
  
  /* transforming the map to a list */
  for(const_iterator i=begin();
      i!=end();
      i++){
    
    lRelevanceLevelList.push_back(CIDRelevanceLevelPair(i->first,
							i->second));
  }
  /* sorting the list */
  lRelevanceLevelList.sort();//by relevance level (implied);
  lRelevanceLevelList.reverse();

  clear();
  int lCount=0;
  for(CIDRelevanceLevelPairList::const_iterator 
	i=lRelevanceLevelList.begin();
      i!=lRelevanceLevelList.end() && lCount<maximumNumber;
      i++,lCount++){
        insert(make_pair(i->getID(),
			 i->getRelevanceLevel()));
  }

}
