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
 * CQPerl.h 
 * 
 *
 *
 ****************************************
 *
 * modification history:
 *
 * WM 08  99 creation
 *
 ****************************************
 *
 * compiler defines used:
 *
 *
 ****************************************/
#ifdef __GIFT_WITH_PERL__

#ifdef bool
#undef bool
#define _bool char
#endif

#include "GIFTExceptions.h"
#include "my_throw.h"
#include "mrml_const.h" // for parsing
#include <algorithm>
#include <functional>
#include <string>

#include "GIFTExceptions.h"

#include <time.h>

#include "CAlgorithm.h"
#include "CAccessorAdminCollection.h"
#include "CQPerl.h"
#include "CAcPerl.h"
#include "CXEVTripletList.h"

extern "C" void xs_init(void);

PerlInterpreter* CQPerl::mPerl(0);

map<string,string> gLoadedModules;

/**
   Transforms a CXMLElement to an CXMLTripletList,
   and finally pushes this list onto the Perl stack.
*/
CXMLElement* CQPerl::callPerl(const CXMLElement* inElement,string inFunction){
  dSP ;
  int count;
  
  ENTER;
  SAVETMPS;
  PUSHMARK(SP) ;

  cout << endl
       << "inElement:" 
       << inElement 
       << endl;
  //the object reference of our perl query processor
  //i.e. $self
  XPUSHs(sv_2mortal(newSVsv(mPerlQueryProcessor)));
  if(inElement){
    cout << "I making the triplet list:"
	 << endl
	 << flush;
    
    CXEVTripletList lVisitor;
      
    inElement->traverse(lVisitor);

    cout << "I am here in pushXMLtoPerl:"
	 << endl
	 << flush;
      

    for(CXMLTripletList::const_iterator i=lVisitor.getTripletList().begin();
	i!=lVisitor.getTripletList().end();
	i++){

      char* lName= new char[i->mName .size()+1];
      char* lValue=new char[i->mValue.size()+1];


      lName= strcpy(lName ,i->mName .c_str());
      lValue=strcpy(lValue,i->mValue.c_str());

      XPUSHs(sv_2mortal(newSViv(i->mInstruction)));
      XPUSHs(sv_2mortal(newSVpv(lName,
				(i->mName).size())));
      XPUSHs(sv_2mortal(newSVpv(lValue,
				(i->mValue).size())));
    }
  }

  PUTBACK;

  cout << "I am going to call the perl function "
       << inFunction
       << endl;
  
  int lResultSize(perl_call_pv((char*)(inFunction.c_str()), G_ARRAY));
  
  SPAGAIN;

  cout << "ready"
       << endl;

  CXMLElement* lReturnValue(0);

  CXMLElement* lNewest(0);

  lResultSize -= lResultSize % 3;

  for(int i=0;
      i<lResultSize;
      i+=3){
    
    SV*   lPoppedInstruction=POPs;
    int   lInstruction=SvIV(lPoppedInstruction);

    STRLEN lLengthOfPopped;
    
    SV*   lPoppedName=POPs;
    char* lName=SvPV(lPoppedName,lLengthOfPopped);
    
    SV*   lPoppedValue=POPs;
    char* lValue=SvPV(lPoppedValue,lLengthOfPopped);
    
    
    /** do some XML triplet stuff here */
    
#ifdef __PRINTOUT
    cout << lName
	 << ","
	 << lValue
	 << endl;
#endif

    if(lInstruction==CXMLTriplet::element){
      lNewest=new CXMLElement(lName,
			      0);
      if(!lReturnValue){
	lReturnValue=lNewest;
      }else{
	lReturnValue->addChild(lNewest);
      }
    }
    //FIXME text is missing here
    if(lInstruction==CXMLTriplet::attribute){
      if(lNewest){

#ifdef __PRINTOUT
	cout << "adding attribute: "
	     << lName
	     << ","
	     << lValue
	     << endl;
#endif

	lNewest->addAttribute(lName,
			      lValue);
      }
    }

    if(lInstruction==CXMLTriplet::up){
      if(lReturnValue){
	lReturnValue->moveUp();
      }
    }
  }
  
  PUTBACK;
  FREETMPS;
  LEAVE;

#ifdef __PRINTOUT
  cout << "Before return " 
       << endl;
#endif

  return lReturnValue;
};

/**
   Transforms a CXMLElement to an CXMLTripletList,
   and finally pushes this list onto the Perl stack.
*/
void CQPerl::callConstruct(){
  char lBuffer[200];
  dSP ;
  int count;
  
  ENTER;
  SAVETMPS;
  PUSHMARK(SP) ;

  strncpy(lBuffer,mPackage.c_str(),200);
  XPUSHs(sv_2mortal(newSVpv(lBuffer,
			    strlen(lBuffer))));

  strncpy(lBuffer,qualifyFunctionName("new").c_str(),200);


  PUTBACK;

  int lResultSize(perl_call_pv(lBuffer, G_ARRAY));
  
  SPAGAIN;

  cout << "ready"
       << endl;

  cout << "Getting the newly constructed element" 
       << endl;

  mPerlQueryProcessor=(newSVsv(POPs));

  for(int i=1;
      i<lResultSize;
      i++){
    SV*   lPopped=POPs;
  }
  
  PUTBACK;
  FREETMPS;
  LEAVE;

};

/** qualify a function name by the package in which 
    the function resides */
string CQPerl::qualifyFunctionName(const char* inName){
  cout << "QUALIFIED NAME: " 
       << mPackage+string("::")+string(inName) 
       << endl;

  return mPackage+string("::")+string(inName);
}


/***************************************
*
* Constructor
*
****************************************
*
* modification history
* WM 20000509 created
* 
*
****************************************/
CQPerl::CQPerl(CAccessorAdminCollection& inAccessorAdminCollection,
	       CAlgorithm& inAlgorithm
	       ):
  CQuery(inAccessorAdminCollection,
	 inAlgorithm),
  mPerlQueryProcessor(0)
{
  if(!mPerl){
    cerr << "Construct perl for keeps" << endl;
    mPerl=perl_alloc();
    perl_construct(mPerl);
  }




  // mproxy has been filled in a reasonable way 
  // by CQuery::CQuery
  mAccessor=mAccessorAdmin->openAccessor("perl");
  
  
  pair<bool,string> lScriptFile=inAlgorithm.stringReadAttribute("cui-perl-script-file");
  pair<bool,string> lPackage=inAlgorithm.stringReadAttribute("cui-perl-package");

  assert(lPackage.first);

  mPackage=lPackage.second;

  

  if(lScriptFile.first){

    char *lARGV[] = { "", (char*)(lScriptFile.second.c_str())};

    map<string,string>::iterator lFound=gLoadedModules.find(lScriptFile.second);
    if(lFound==gLoadedModules.end()){
      cerr << "Loading new perl source code" 
	   << lScriptFile.second
	   << endl;
      perl_parse(mPerl, 
		 xs_init, 
		 1, 
		 lARGV, 
		 NULL);
      gLoadedModules[lScriptFile.second]="loaded";
    }else{
      cerr << "Found old perl script" 
	   << lScriptFile.second
	   << endl;
    }

    CSelfDestroyPointer<CXMLElement> lCollectionElement;
    if(mAccessor){
      // make this survive also with an
      // empty mAccessor

      lCollectionElement=((CAcPerl*)mAccessor)->cloneCollectionElement();
    }
    callConstruct();

    char lBuffer[200];

    strncpy(lBuffer,qualifyFunctionName("setAlgorithm").c_str(),200);
    callPerl(&inAlgorithm,
	     lBuffer);
    //FIXME here we have to construct
    //      a CPerlAccessor
    if(mAccessor){

      cout << "perl accessor exsists!"
	   << endl;

      strncpy(lBuffer,qualifyFunctionName("setCollection").c_str(),200);
      callPerl(lCollectionElement,
	       lBuffer);
    }else{
      cout << "perl accessor does NOT exsist!"
	   << endl;

      strncpy(lBuffer,qualifyFunctionName("setCollection").c_str(),200);
      callPerl(0,
	       lBuffer);
    }
    // now use this configuration data
    strncpy(lBuffer,qualifyFunctionName("configure").c_str(),200);
    callPerl(0,
	     lBuffer);

  }else{
    
    my_throw(VEProgrammingMistake("no Perl scriptfile"));
    mAccessorAdmin->closeAccessor("perl");
  }
}
/***************************************
*
* Destructor
*
****************************************
*
* modification history
* WM 20000509 created
* 
*
****************************************/
CQPerl::~CQPerl()
{
  assert(mPerlQueryProcessor);
  //this leads to immediate destruction
  SvREFCNT_dec(mPerlQueryProcessor);

  //perl_destruct(mPerl);
  //perl_free(mPerl);
  cout << "perl destroyed üüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüüü" << endl;
}
/***************************************
*
* Constructor
*
****************************************
*
* modification history
* WM created
* 
*
****************************************/
CQPerl::CQPerl():CQuery()
  //  ,mPerl(0)
{
  if(!mPerl){
    cerr << "Construct perl for keeps (2)" << endl;
    mPerl=perl_alloc();
    perl_construct(mPerl);
  }
    
}

/***************************************
*
* Getting random images as a seed
*
****************************************
*
* modification history
* 
*
****************************************/
CXMLElement* CQPerl::getRandomImages(int inNumberOfInterestingImages)const{

  // call some perl here that does as promised

  return 0;
};

/***************************************
*
* Getting random images as a seed
*
****************************************
*
* modification history
* WM 19990824 created
* 
*
****************************************/
CIDRelevanceLevelPairList* CQPerl::getRandomIDs(int inNumberOfInterestingImages)const{
  return 0;
};




/***************************************
*
* Getting all images for making distance matrices 
* or similar stuff
*
****************************************
*
* modification history
* WM 19990824 created
* 
*
****************************************/
list<TID>* CQPerl::getAllIDs()const{
  return 0;
};



/***************************************
*
* Getting all images for making distance matrices 
* or similar stuff
*
****************************************
*
* modification history
* WM 19990824 created
* 
*
****************************************/
list<CAccessorElement>* CQPerl::getAllAccessorElements()const{
  return 0;
};


/***************************************
*
* Setting the current algorithm
*
****************************************
*
* modification history
* WM 19990824 created
* 
*
****************************************/
bool CQPerl::setAlgorithm(CAlgorithm& inAlgorithm){
  mAlgorithm=&inAlgorithm;
};


/**
   Calls a specified perl function
 */
CXMLElement* CQPerl::query(const CXMLElement& inQuery){


  pair<bool,string> lQueryFunction=mAlgorithm->stringReadAttribute("cui-perl-query-function");
  pair <bool,string> lRandomQueryFunction=mAlgorithm->stringReadAttribute("cui-perl-random-function");

  if(!lQueryFunction.first){
    my_throw(VEProgrammingMistake("no Perl query function specified"));
  }

  pair<bool,long> lNumberOfInterestingImages=
    inQuery.longReadAttribute(mrml_const::result_size);
  
  int inNumberOfInterestingImages=
    lNumberOfInterestingImages.second;

  pair<bool,long> lCutoff=
    inQuery.longReadAttribute(mrml_const::result_cutoff);
  
  int inCutoff=
    lCutoff.second;
  
  if(inQuery.child_list_begin()!=inQuery.child_list_end()){
    /** 
	1. turn the query into a triplet list
	2. push the triplet list on the stack
	3. pop the result triplet list from the stack
	4. generate an XML tree from it.
    */
    CXMLElement* lReturnValue(callPerl(&inQuery,"CGIFTLink::processGIFTQueryCall"
				       //qualifyFunctionName(lQueryFunction.second.c_str())
				       )
			      );

      
    return lReturnValue;
  }else{

    CXMLElement* lReturnValue(0);

    if(&inQuery){

      string lOutElement;

      inQuery.toXML(lOutElement);

      cout << "Calling perl with " 
	   << &inQuery
	   << endl
	   << lOutElement  
	   << endl;

      lReturnValue=callPerl(&inQuery,
			    "CGIFTLink::processGIFTRandomQueryCall"
			    //qualifyFunctionName(lRandomQueryFunction.second.c_str())
			    );

    }

    cout << "finished calling perl" 
	 << endl;

    return lReturnValue;
  }

  cout << "RETURNING" << endl << flush;

  return 0;
}

CIDRelevanceLevelPairList* CQPerl::fastQuery(const CXMLElement& inQuery,
					     int inNumberOfInterestingImages,
					     double inThreshold){
  return 0;
}

void CQPerl::init(){};
#endif
