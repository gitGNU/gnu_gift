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
/***************************************
*
* ... a whole collection of includes for convenience
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
#include "libGIFTQuInvertedFile/include/CQNBestFullyWeighted.h"
#include "libGIFTQuInvertedFile/include/CWFBestFullyWeighted.h"
#include "libGIFTQuInvertedFile/include/CQNNoNormalization.h"
#include "libGIFTQuInvertedFile/include/CQNEuclideanLengthSquare.h"
#include "libGIFTQuInvertedFile/include/CQNSquareDFLogICFSum.h"
#include "libGIFTQuInvertedFile/include/CQNMaxDocumentFrequency.h"
#include "libGIFTQuInvertedFile/include/CWFBestProbabilistic.h"
#include "libGIFTQuInvertedFile/include/CWFClassicalIDF.h"
#include "libGIFTQuInvertedFile/include/CWFBinaryTerm.h"
#include "libGIFTQuInvertedFile/include/CWFProbability.h"
#include "libGIFTQuInvertedFile/include/CWFStandardTF.h"
#include "libGIFTQuInvertedFile/include/CWFCoordinationLevel.h"



