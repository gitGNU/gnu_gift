// -*- mode: c++ -*- 
#ifdef __GIFT_WITH_MYSQL__
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
#include "libGIFTAcInvertedFile/include/CAcIFMySQL.h"
#include <sqlplus.hh>
#include <iostream>
#include <map> 
#include <list>
#include <fstream>
#include <sys/stat.h>
#include "libMRML/include/mrml_const.h"
#include "libMRML/include/CXMLElement.h"
#include "libGIFTAcInvertedFile/include/CDocumentFrequencyElement.h"

sql_create_3 (documentFeatureConnection,
              1, 3,             
              int, documentID,   
              int, featureID,
              double, docFreq);

sql_create_8 (documentTable,
	      1, 8,             
	      int, documentID,   
	      int, nbrFeatures,
	      string, url,
	      string, urlThumbnail,
	      string, urlFeatureFile,
	      string, FeaturesAndDocFreq,
	      string, XMLdocumentDesc,
	      string, keywords);





/** Constructor */

CAcIFMySQL::CAcIFMySQL(const CXMLElement& inCE):
  cnx(0),
  mURL2FTS(new CAcURL2FTS(inCE)),
  // analyse the Connection Element inCE
  GdbName(inCE.stringReadAttribute(mrml_const::cui_sql_database_name).second),
  GdbHost(inCE.stringReadAttribute(mrml_const::cui_sql_host).second),
  GdbUser(inCE.stringReadAttribute(mrml_const::cui_sql_username).second),
  GdbPassword(inCE.stringReadAttribute(mrml_const::cui_sql_password).second)
  
{

  
  cout << endl << "MYSQL getting connected" << endl;

  bool createNewDB=false;
  // create connection object
  try { 
    string GdbName2Select = GdbName; // keep copy for selecting db later
    cnx = new Connection(use_exceptions); 
       
    // try to make the connection without choosing the database yet
    try { 
      GdbName = "";
      cnx->connect(GdbName.c_str(),GdbHost.c_str(),GdbUser.c_str(),GdbPassword.c_str());
      cout << "Connection to '" << GdbHost << "' for user '" ;
      cout << GdbUser << "' ok\n";
    }
    catch (BadQuery er) {
      cerr << "Connection to '" << GdbHost << "' for user '" ;
      cerr << GdbUser << "' failed!\n" << er.error << endl;
      exit(0);
    }
	
    // select the database
    bool dbExists = selectDB(GdbName2Select);
    if (!dbExists) 
      if (createNewDB)
	createDB(GdbName);
      else
	exit(0);
  }

  catch (BadQuery er) {
    cerr << "Creating Connection object failed. Program aborted!\n";
    cerr << er.error << endl;
    exit(0);
  }
}

/** Destructor */
CAcIFMySQL::~CAcIFMySQL() {
  delete cnx;
}

/** select a database :
    returns true if selecting successful or chosen database already selected, 
    returns false if failure */
bool CAcIFMySQL::selectDB(string ldbName){
  bool outcome;

  if (ldbName == GdbName)
    {
      cout << "Selecting database '" << ldbName << "': db already in use.\n";
      return true;
    }
  else
    GdbName = ldbName;

  outcome = cnx->select_db(ldbName.c_str());
  if (outcome) 
    {
      cout << "Selecting database '" << ldbName << "' ok\n";
      return true;
    }
else
{
  cout << "Selecting database '" << ldbName << "' failed!\n";
  return false;
}
}


/** create an empty database :
    returns true if success, returns false if attempt to create
    currently selected database or failure with creation */
bool CAcIFMySQL::createDB(string ldbName) {
  bool outcome;

  if (ldbName == GdbName)
    {
      cout << "Creating database '" << ldbName << "': db currently in use.\n";
      return false;
    }

  outcome = cnx->create_db(ldbName.c_str());
  if (outcome) 
    {
      cout << "Creating database '" << ldbName << "' ok\n";
      return true;
    }
else
{
  cout << "Creating database '" << ldbName << "' failed!\n";
  return false;
}
}


/** drop a database :
    returns true if success, returns false if attempt to drop
    currently selected database or failure with dropping */
bool CAcIFMySQL::dropDB(string ldbName) {
  bool outcome;

  if (ldbName == GdbName)
    {
      cout << "Dropping database '" << ldbName << "' impossible:\n";
      cout << "This db is currently in use.\n";
      return false;
    }

  outcome = cnx->drop_db(ldbName.c_str());
  if (outcome) 
    {
      cout << "Dropping database '" << ldbName << "' ok\n";
      return true;
    }
  else
{
  cout << "Dropping database '" << ldbName << "' failed!\n";
  return false;
}
}

/** initialise tables for GIFT database */
void CAcIFMySQL::initialiseGIFTTables ( bool replaceExistingTables, 
					 bool getGIFTData, 
					 string giftDataPath ){
  //  tablelist will be filled with the GIFT table declarations
  list<string> tableList;
    
    // define description table
  tableList.push_back ( "CREATE TABLE databaseDescription (
name         varchar(30) NOT NULL,
description  varchar(255),
nbrIndexedImages int UNSIGNED DEFAULT '0' NOT NULL,
nbrImagesInDB    int UNSIGNED DEFAULT '0' NOT NULL, 
nbrFeatures  int UNSIGNED DEFAULT '0' NOT NULL,
PRIMARY KEY (name),
INDEX nameIndex (name) )" );

  // define documentTable
  tableList.push_back ( "CREATE TABLE documentTable (
documentID      int UNSIGNED NOT NULL,
nbrFeatures     int UNSIGNED,
url             varchar(100) NOT NULL,
urlThumbnail    varchar(100),
urlFeatureFile  varchar(100),
FeaturesAndDocFreq  blob,
XMLdocumentDesc    blob,
keywords        varchar(255),
PRIMARY KEY    (documentID),
INDEX documentIDindex (documentID),
INDEX urlIndex (url) )" );
  
  // define featureTable
  tableList.push_back ( "CREATE TABLE featureTable (
featureID        int UNSIGNED NOT NULL,
featureDescr     varchar(100),
collectionFreq   real,
minimumDocFreq   real,
maximumDocFreq   real,
documentID_DocFreq  blob,
nbrDocumentsWithFeature  int,
XMLfeatureDesc   blob,
PRIMARY KEY     (featureID),
INDEX featureIDindex (featureID) )" );

  // define documentFeatureConnection table
  tableList.push_back ( "CREATE TABLE documentFeatureConnection (
documentID   int UNSIGNED NOT NULL,
featureID    int UNSIGNED NOT NULL,
docFreq      real NOT NULL,
PRIMARY KEY (documentID, featureID),
INDEX documentIDindex (documentID),
INDEX featureIDindex (featureID) )" );

  // define annotationTable_1
  tableList.push_back ( "CREATE TABLE annotationTable_1 (
documentID   int UNSIGNED NOT NULL,
text         varchar(127) NOT NULL,
docFreq      real,
PRIMARY KEY (documentID),
INDEX documentIDindex (documentID) )" );

  // define annotationTable_2
  tableList.push_back ( "CREATE TABLE annotationTable_2 (
featureID    int UNSIGNED NOT NULL,
text         varchar(127) NOT NULL,
PRIMARY KEY (featureID),
INDEX featureIDindex (featureID) )" );
   

  // execute creation of the tableList and load corresponding inFile if existing
  list<string>::iterator i;
  string currentDef, tableName, fileToLoad;

  Query query = cnx->query();

  for (i = tableList.begin(); i != tableList.end(); i++)
   {
      currentDef = *i;
      tableName = currentDef.substr(13,currentDef.find(" (") - 13);

      // if table already exists, drop it if required
      if (recordCount(tableName) >= 0)
	if (replaceExistingTables)
	  {
	    try {
	      query << "drop table " << tableName;
	      query.execute(RESET_QUERY);
	      cout << "Dropped existing '" << tableName << "'\n";
	    }
	    catch (BadQuery er) {
	      cerr << er.error << " (skipped this table)\n";
	      continue;
	    }
	  }
	
      if (recordCount(tableName) < 0){  // table doesn't exist
	try {
	  // create new table 
	  query << currentDef << endl;
	  
	  query.execute(RESET_QUERY);
	  cout << "Created new '" << tableName << "'\n";

	  // construct name of file to load
	  fileToLoad = giftDataPath + "/" + tableName + ".in";
	  
	  // check if fileToLoad exists
	  ifstream inFile(fileToLoad.c_str());
	  if (inFile)   // file exists => load it into table
	    {
	      query << "LOAD DATA INFILE '" << fileToLoad << "' INTO TABLE " << tableName << " FIELDS TERMINATED BY ' '";
	      try {
		query.execute(RESET_QUERY);
		cout << "-> corresponding file found and loaded ok\n";
		}
	      catch (BadQuery er) {
		cerr << "Error loading file into table: " << er.error << endl;
		continue;
	      }
	    }
	}
	catch (BadQuery er) {
	  cerr << "Error creating '" << tableName << " : "  << er.error << endl;
	  continue;
	}
      }
      else
	cout << "Kept existing '" << tableName << "'\n";
   } // end for every table loop
}


/** returns number of rows or -1 if table not existing or not accessible */
int CAcIFMySQL::recordCount(string ltableName)const{
  try {
    Query query = cnx->query();
    query << "select count(*) as nbrRows from " << ltableName;
    
    Result res = query.store(RESET_QUERY);
    Row row = res.fetch_row();
    return (row["nbrRows"]);
  }
  catch (BadQuery er) {
    return -1;
  }
}


/** Translate a DocumentID to a URL (for output) */
string CAcIFMySQL::IDToURL(TID inID)const {
  if (recordCount("documentTable") > 0)
    {
      try {
	Query query = cnx->query();
	query << "SELECT url FROM documentTable WHERE documentID = '" << inID << "'";
	Result res = query.store(RESET_QUERY);
	Row row = res.fetch_row();
	return string(row["url"]);
      }
      catch (BadQuery er) {
	cerr << "Error: " << er.error << endl;
	return "";
      }
    }
  else
    {
      cerr << "Impossible to convert documentID " << inID << " to URL ";
      cerr << "(documentTable empty or not correctly accessible) !\n";
      return "";
    }
}
 
/** Translate an URL to its document ID  - returns 0 if error */
pair<bool,TID> CAcIFMySQL::URLToID(const string& inURL)const { 
  if (recordCount("documentTable") > 0)
    {
      try {
	Query query = cnx->query();
	query << "SELECT documentID FROM documentTable WHERE url = '" << inURL << "'";
	Result res = query.store(RESET_QUERY);
	Row row = res.fetch_row();
	return make_pair(bool(1),TID(row["documentID"]));
      }
      catch (BadQuery er) {
	cerr << "Error: " << er.error << endl;
	return make_pair(bool(0),0);
      }
    }
  else
    {
      cerr << "Impossible to convert URL " << inURL << " to documentID ";
      cerr << "(documentTable empty or not correctly accessible) !\n";
      return make_pair(bool(0),0);
    }	
}


/** Get list of documents containing feature inFID */
CDocumentFrequencyList* CAcIFMySQL::FeatureToList(TFeatureID inFID)const {

  CDocumentFrequencyList* resultList(0);

  vector <documentFeatureConnection> queryReturn;


  if (recordCount("documentFeatureConnection") > 0)
    {
      try {
	Query query = cnx->query();
	query << "SELECT * from documentFeatureConnection where featureID = " << inFID;
	query.storein(queryReturn);
	// pass through queryReturn and store documentID and docFreq in resultList
	resultList=new CDocumentFrequencyList(queryReturn.end()-queryReturn.begin());
#ifndef _CDocumentFrequencyListIsList
	CDocumentFrequencyElement* resultListBack=resultList->begin();
#endif
	for (vector <documentFeatureConnection>::iterator It = queryReturn.begin(); It != queryReturn.end(); It++)
	  {
	    // 	      resultElement.mID = It->documentID;
	    // 	      resultElement.mFrequency = It->docFreq;
#ifdef _CDocumentFrequencyListIsList
	    resultList->push_back(CDocumentFrequencyElement(It->documentID,
							   It->docFreq));
#else
	    *resultListBack=CDocumentFrequencyElement(It->documentID,
						      It->docFreq);
	    resultListBack++;
#endif
	  }
#ifdef _CDocumentFrequencyListIsArray
	resultList->setEnd(resultListBack);
#endif

      }catch (BadQuery er) {
	cerr << "Error: " << er.error << endl;
      }
    }
    else
      {
	cerr << "Impossible to convert featureID " << inFID << " to docList ";
	cerr << "(documentFeatureConnection empty or not correctly accessible) !\n";
      }
    return resultList;
  }


/** Get list of features contained by a document with URL inURL */
CDocumentFrequencyList* CAcIFMySQL::URLToFeatureList(string inURL)const{
    TID tid;
    
    pair<bool,TID> lID=URLToID(inURL);
    if (lID.first)
      return DIDToFeatureList(tid);
    return 0;
}


/** Get list of features contained by a document with ID inDID */
CDocumentFrequencyList* CAcIFMySQL::DIDToFeatureList(TID inDID)const {
  CDocumentFrequencyList* resultList;
  vector <documentFeatureConnection> queryReturn;
   
  if (recordCount("documentFeatureConnection") > 0)
    {
      try {
	Query query = cnx->query();
	query << "SELECT * from documentFeatureConnection where documentID = " << inDID;
	query.storein(queryReturn);
	  // pass through queryReturn and store featureID and docFreq in resultList
	resultList=new CDocumentFrequencyList(queryReturn.end()-queryReturn.begin());
#ifndef _CDocumentFrequencyListIsList
	CDocumentFrequencyElement* resultListBack=resultList->begin();
#endif
	for (vector <documentFeatureConnection>::iterator It = queryReturn.begin(); It != queryReturn.end(); It++)
	  {
#ifdef _CDocumentFrequencyListIsList
	    resultList->push_back(CDocumentFrequencyElement(It->documentID,
							   It->docFreq));
#else
	    *resultListBack=CDocumentFrequencyElement(It->documentID,
						      It->docFreq);
	    resultListBack++;
#endif
	  }
#ifdef _CDocumentFrequencyListIsArray
	resultList->setEnd(resultListBack);
#endif

      }
      catch (BadQuery er) {
	cerr << "Error: " << er.error << endl;
      }
    }
  else
    {
      cerr << "Impossible to convert documentID " << inDID << " to featureList ";
      cerr << "(documentFeatureConnection empty or not correctly accessible) !\n";
    }
  return resultList;
}
  

/** Get list of documents with features contained in a
    CDocumentFrequencyList inFeatureList*/
CDocumentFrequencyList* CAcIFMySQL::featureList2docList
                                 (CDocumentFrequencyList inFeatureList)
  {
    CDocumentFrequencyList* resultList;    // CDocumentFrequencyList that will be returned
    vector <documentFeatureConnection> queryReturn;   // CDocumentFrequencyList we get back from query
    
    int totalFeatures;     // total number of features we get back for a doc
    
    // unsortedDocCollection will contain pairs <docID, numberOfOccurences>
    map <int, int> unsortedDocCollection; 
    map <int, int>::iterator uIt;
    
    // sortedDocCollection will contain pairs <nbrOfOccurrances, docID>
    multimap <int, int, greater<int> > sortedDocCollection;
    multimap <int, int, greater<int> >::iterator sIt;
    
    totalFeatures = 0; // initialisation
    
    if (recordCount("documentFeatureConnection") > 0)
      {
	try {
	  CDocumentFrequencyList::iterator fIt;
	  // go through list of features
	  for (fIt=inFeatureList.begin(); fIt!=inFeatureList.end(); fIt++)
	    {
	      totalFeatures++;
	      // for every feature, get list of corresponding documentIDs
	      Query query = cnx->query();
	      query << "SELECT * from documentFeatureConnection where featureID = " << fIt->getID();
	      query.storein(queryReturn);
	      // go through list of documentsID
	      for (It=queryReturn.begin(); It!=queryReturn.end(); It++)
		{
		  // make histogram of documents found
		  uIt = unsortedDocCollection.find(It->documentID);
		  if (uIt != unsortedDocCollection.end()) 
		    // docID already existing -> increase frequency
		    unsortedDocCollection[It->documentID]++;
		  else
		    // new docID
		    unsortedDocCollection[It->documentID] = 1;
		}
	      queryReturn.clear(); 
	    }

	  // sort unsortedDocCollection -> copy into multimap container with 
	  // nbrOfOccurances as the sort criterium
	  int nbrOfOccurrances,docID;
	  for (uIt = unsortedDocCollection.begin(); uIt != unsortedDocCollection.end(); uIt++)
	    {
	      docID = (*uIt).first;
	      nbrOfOccurrances = (*uIt).second;
	      sortedDocCollection.insert(pair<int,int>(nbrOfOccurrances,docID));
	    }

#ifndef _CDocumentFrequencyListIsList
	CDocumentFrequencyElement* resultListBack=resultList->begin();
#endif

	resultList=new CDocumentFrequencyList(sortedDocCollection.size());
	  // copy sorted document collection into document frequency list and
	  // divide nbrOfOccurrances by total number of Features
	  for (sIt=sortedDocCollection.begin(); sIt!=sortedDocCollection.end(); sIt++)
	    { 
#ifdef _CDocumentFrequencyListIsList
	    resultList->push_back(CDocumentFrequencyElement(sIt->second,
							      sIt->first));
#else
	    *resultListBack=CDocumentFrequencyElement(sIt->second,
						      sIt->first);
	    resultListBack++;
#endif
	    }
#ifdef _CDocumentFrequencyListIsArray
	resultList->setEnd(resultListBack);
#endif

	}
	catch (BadQuery er) {
	  cerr << "Error: " << er.error << endl << endl;
	}
      }
    else
      {
	cerr << "Impossible to convert featuresList to docList ";
	cerr << "(documentFeatureConnection empty or not correctly accessible) !\n";
      }
    return resultList;      
  }
	  

/* show contents of a DocumentFrequencyList */	    
void CAcIFMySQL::showDocFreqList(CDocumentFrequencyList docFreqList)
  {
    CDocumentFrequencyList::iterator It;
    if (docFreqList.begin() == docFreqList.end())   // empty list
      cout << "(list is empty)\n";
    else
      {
	for (It = docFreqList.begin(); It != docFreqList.end(); It++) 
	  {
	    cout << It->getID() << "\t" << It->getDocumentFrequency() << "\n";
	  }
	cout << endl;
      }
  }
 

/* convert a blob file to a DocumentFrequencyList */
CDocumentFrequencyList* CAcIFMySQL::blob2docFreqList(string inBlobFile)const{
  CDocumentFrequencyList* resultList=new CDocumentFrequencyList();
  
  ifstream inFileBuf (inBlobFile.c_str(),ios::in | ios::binary); // blob file
  struct stat fileStatus;   // will contain status of blob file	
  
  if ((inFileBuf.rdbuf())->is_open()) // If non-zero, the stream is open
    {
	if (stat (inBlobFile.c_str(),&fileStatus) == -1) return resultList; // error
	
	unsigned int blobLength = fileStatus.st_size;  
	if (!blobLength) return resultList; // error
	
	char  *read_buffer = new char[blobLength];	
	inFileBuf.read(read_buffer,blobLength); 
	
	// construct a string with contents of blob 
	string blobContent(read_buffer,blobLength);
	
	// insert contents of blob into resultsList
	cout << blobContent << endl;  // evtl .str()
	
	delete[] read_buffer;
    }
  
  else 
    cerr << "Blob file " << inBlobFile << "could not be opened" << endl;
  
  return resultList;
}

/** List of the IDs of all documents present in the inverted file */
void CAcIFMySQL::getAllIDs(list<TID>& outIDList)const{

  mURL2FTS->getAllIDs(outIDList);
};
/** List of triplets (ID,imageURL,thumbnailURL) of all
    the documents present in the inverted file */
void CAcIFMySQL::getAllAccessorElements(list<CAccessorElement>& outAccessorElementList)const{
  mURL2FTS->getAllAccessorElements(outAccessorElementList);
};
/** get a given number of random AccessorElement's 
    @param inoutResultList the list which will contain the result
    @param inSize          the desired size of the inoutResultList
*/
void CAcIFMySQL::getRandomIDs(list<TID>& outRandomIDList,
				   list<TID>::size_type inSize)const{
  return mURL2FTS->getRandomIDs(outRandomIDList,
				inSize);
};
/** For drawing random sets. Why is this part of an CAccessorImplementation?
    The way the accessor is organised might influence the way
    random sets can be drawn. At present everything happens in
    RAM, but we do not want to be fixed on that.
    
      @param inoutResultList the list which will contain the result
      @param inSize          the desired size of the inoutResultList
*/
void CAcIFMySQL::getRandomAccessorElements(list<CAccessorElement>& outResult,
						list<CAccessorElement>::size_type inSize)const{
  mURL2FTS->getRandomAccessorElements(outResult,
				      inSize);
};
/** The number of images in this accessor */
int CAcIFMySQL::size()const{
  return mURL2FTS->size();
};
/** The number of images in this accessor */
CAcIFMySQL::operator bool()const{
  return bool(mURL2FTS);
};
/** The number of images in this accessor */
bool CAcIFMySQL::operator ()()const{
  return bool(mURL2FTS);
};
pair<bool,CAccessorElement> CAcIFMySQL::IDToAccessorElement(TID inID)const{
  return mURL2FTS->IDToAccessorElement(inID);
};
list<TID>* CAcIFMySQL::getAllFeatureIDs()const{
  assert(0);
};
double CAcIFMySQL::FeatureToCollectionFrequency(TFeatureID inFeatureID)const{
  return 1;
}
double CAcIFMySQL::DIDToMaxDocumentFrequency(TID inID)const{
  return 1;
}

double CAcIFMySQL::DIDToDFSquareSum(TID inID)const{
  return 1;
}

double CAcIFMySQL::DIDToSquareDFLogICFSum(TID inID)const{
  return 1;
}

unsigned int CAcIFMySQL::getFeatureDescription(TID inID)const{

}
#endif
