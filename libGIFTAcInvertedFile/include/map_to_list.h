/**
*
* MAPTOLIST
*
*
*
* modification history:
*
* HM 090399 created the documentation
*
*
*
* compiler defines used:
*
*
*/
#ifndef _MAPTOLIST
#define _MAPTOLIST

#include <map>
#include <list>


/**
 *
 *  generating a list out of the elements of a map 
 *
 */
template<class key,class element>
list<element>* map_to_list(const typename map<key,element>::const_iterator inBegin,
			   const typename map<key,element>::const_iterator inEnd){
  list<element> lNewList=new list<element>;
  
  while(inBegin!=inEnd){
    lNewList->push_back((inBegin++).second);
  }

  return lNewList;
}

#endif
