// -*- mode: c++ -*- 
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
#include "GIFTExceptions.h"

GIFTException::GIFTException(const char* inMessage):
    mMessage(inMessage){}

ostream& operator<<(ostream& outStream,const GIFTException& inException){
    outStream << "AnGIFTException occured: "
	      << flush
	      << inException.mMessage
	      << flush
	      << endl;
}

void GIFTException::dummy(){//needed for RTTI/exception catching
}

VEProgrammingMistake::VEProgrammingMistake(const char* inMessage):
    GIFTException(inMessage){
};
ostream& operator<<(ostream& outStream,const VEProgrammingMistake& inException){
    outStream << "You are experiencing a CLEAR programming mistake in"
	      << endl
	      << "the c++ part of the server. Please write a bugreport."
	      << endl
	      << "These were the last words before the error:"
	      << endl
	      << flush
	      << inException.mMessage
	      << flush
	      << endl;
}

VENotFound::VENotFound(const char* inMessage):
    GIFTException(inMessage){
};
ostream& operator<<(ostream& outStream,const VENotFound& inException){
    outStream << "AnVENotFound occured: "
	      << flush
	      << inException.mMessage
	      << flush
	      << endl;
}

VEBaseUnknown::VEBaseUnknown(const char* inMessage):
    GIFTException(inMessage){
};
ostream& operator<<(ostream& outStream,const VEBaseUnknown& inException){
    outStream << "AnVEBaseUnknown occured: "
	      << flush
	      << inException.mMessage
	      << flush
	      << endl;
}


VEDoubleInsert::VEDoubleInsert(const char* inMessage):
    GIFTException(inMessage){
};
ostream& operator<<(ostream& outStream,const VEDoubleInsert& inException){
    outStream << "AnVEDoubleInsert occured: "
	      << flush
	      << inException.mMessage
	      << flush
	      << endl;
}


VEWrongAccessor::VEWrongAccessor(const char* inMessage):
    GIFTException(inMessage){
};
ostream& operator<<(ostream& outStream,const VEWrongAccessor& inException){
    outStream << "AnVEWrongAccessor occured: "
	      << flush
	      << inException.mMessage
	      << flush
	      << endl;
}


VEWrongAlgorithm::VEWrongAlgorithm(const char* inMessage):
    GIFTException(inMessage){
};
ostream& operator<<(ostream& outStream,const VEWrongAlgorithm& inException){
    outStream << "AnVEWrongAlgorithm occured: "
	      << flush
	      << inException.mMessage
	      << flush
	      << endl;
}
VEWrongBaseType::VEWrongBaseType(const char* inMessage):
    GIFTException(inMessage){
};
ostream& operator<<(ostream& outStream,const VEWrongBaseType& inException){
    outStream << "AnVEWrongBaseType occured: "
	      << flush
	      << inException.mMessage
	      << flush
	      << endl;
}


VEUnknownSession::VEUnknownSession(const char* inMessage):
    GIFTException(inMessage){
};
ostream& operator<<(ostream& outStream,const VEUnknownSession& inException){
    outStream << "AnVEUnknownSession occured: "
	      << flush
	      << inException.mMessage
	      << flush
	      << endl;
}

