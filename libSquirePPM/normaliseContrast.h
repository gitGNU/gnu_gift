/* -*- mode: c -*- 
*/
/* 

    GIFT, a flexible content based image retrieval system.
    Copyright (C) 1998, 1999, 2000, 2001, 2002, CUI University of Geneva

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
/***************************************************
* Copyright: JJK O'Ruanaidh 1998 All Rights Reserved
*
* This source code is for Laboratory use only and may
* not distributed outside CUI, University of Geneva for
* any purpose whatsoever without my express prior
* consent. This code is provided 'as is' and no
* liability is accepted for damage or loss caused
* by using this code.
*
****************************************************/

/* takes an image as a 1-dimensional array of pixels, which are
   coded as floating point numbers. 

   After applying this functionto inoutImage
   it contains the same image but with values
   between MAXBYTE and MINBYTE
*/
extern int normaliseContrast(float *inoutImage, 
			     int NumPixels);
