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
#ifndef _GIFTEXCEPTIONS
#define _GIFTEXCEPTIONS
using namespace std;

#include <iostream>
#include <fstream>

/** A base class for exceptions thrown by GIFT 

 WM 211099 created docs
 WM   0899 created

 @author Wolfgang Mueller
*/
class GIFTException{
  /** This virtual function has to exsist,
      in order to enable Runtime Type 
      identification. See Stroustrup about RTTI
      for this. Exceptions rely on RTTI if I am not 
      mistaken.
  */
  virtual void dummy();
 protected:
  /** The exception message*/
  const char* mMessage;
 public:
  /** get the message contained in this */
  const char* getMessage()const;

  /** Constructor */
  GIFTException(const char* inMessage);
  /** Output into a stream */
  friend ostream& operator<<(ostream& outStream,
			     const GIFTException&);
};

/** Output operator sending a proper message */
ostream& operator<<(ostream& outStream,
		    const GIFTException&);

/** A file or a hash element was not found*/
class VENotFound:public GIFTException{
 public:
  
  /** Constructor */
  VENotFound(const char* inMessage);
  /** Output into a stream */
  friend ostream& operator<<(ostream& outStream,
			     const VENotFound& inException);
  
};

ostream& operator<<(ostream& outStream,
		    const VENotFound& inException);

/** a clear programming mistake in this code has been found*/
class VEProgrammingMistake:public GIFTException{
 public:
  
  /** Constructor */
  VEProgrammingMistake(const char* inMessage);
  /** Output into a stream */
  friend ostream& operator<<(ostream& outStream,
			     const VEProgrammingMistake& inException);
  
};

ostream& operator<<(ostream& outStream,
		    const VENotFound& inException);


/** An element which is considered bein only once 
    in a set was inserted another time. Example: 
    trying to create a session with an ID which 
    exsists already*/
class VEDoubleInsert:public GIFTException{
 public:
  /** Constructor */
  VEDoubleInsert(const char* inMessage);
  
  /** Output into a stream */
  friend ostream& operator<<(ostream& outStream,
			     const VEDoubleInsert& inException);
  
};

ostream& operator<<(ostream& outStream,
		    const VEDoubleInsert& inException);


/** i dunno (embarrassing), but it is quite late*/
class VEBaseUnknown:public GIFTException{
 public:
  /** */
  VEBaseUnknown(const char* inMessage);

  friend ostream& operator<<(ostream& outStream,
			     const VEBaseUnknown& inException);

};

ostream& operator<<(ostream& outStream,
		    const VEBaseUnknown& inException);


/** The accessor does not fit the chosen algorithm */
class VEWrongAccessor:public GIFTException{
 public:
  /** Constructor */
  VEWrongAccessor(const char* inMessage);

  /** Output into a stream */
  friend ostream& operator<<(ostream& outStream,
			     const VEWrongAccessor& inException);

};

ostream& operator<<(ostream& outStream,
		    const VEWrongAccessor& inException);


/** The the algorithm cannot be used with the collection chosen */
class VEWrongAlgorithm:public GIFTException{
 public:
  /** Constructor */
  VEWrongAlgorithm(const char* inMessage);

  /** Output into a stream*/
  friend ostream& operator<<(ostream& outStream,
			     const VEWrongAlgorithm& inException);
};

ostream& operator<<(ostream& outStream,
		    const VEWrongAlgorithm& inException);

/** The the algorithm cannot be used with the collection chosen */
class VEWrongBaseType:public GIFTException{
 public:
  /** Constructor */
  VEWrongBaseType(const char* inMessage);

  /** Output into a stream*/
  friend ostream& operator<<(ostream& outStream,
			     const VEWrongBaseType& inException);
};

ostream& operator<<(ostream& outStream,
		    const VEWrongBaseType& inException);


/** The session ID is unknown*/
class VEUnknownSession:public GIFTException{
 public:
  /** Constructor*/
  VEUnknownSession(const char* inMessage);
  /** Output into a stream */
  friend ostream& operator<<(ostream& outStream,
			     const VEUnknownSession& inException);

};
/** error in configuration, e.g. URL2FTS file */
class VEConfigurationError:public GIFTException{
 public:
  /** Constructor*/
  VEConfigurationError(const char* inMessage);
  /** Output into a stream */
  friend ostream& operator<<(ostream& outStream,
			     const VEConfigurationError& inException);

};


ostream& operator<<(ostream& outStream,
		    const VEConfigurationError& inException);




#endif
