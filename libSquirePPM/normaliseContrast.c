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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "typedefs.h"

int normaliseContrast(float *inoutImage, 
		      int NumPixels){
  float           lMinimalPixel, lMaximalPixel, lNormalisingFactor;
  int             i;

  lMaximalPixel = inoutImage[0];
  lMinimalPixel = inoutImage[0];
  for (i = 1; i < NumPixels; i++) {
    if (inoutImage[i] > lMaximalPixel)
      lMaximalPixel = inoutImage[i];
    else if (inoutImage[i] < lMinimalPixel)
      lMinimalPixel = inoutImage[i];
  }

  if (fabs(lMinimalPixel - lMaximalPixel) < 1e-10) {
    fprintf(stderr, "Can't normalise this Image\n");
    return 1;
  }

  lNormalisingFactor = 
    (float) (MAX_BYTE - MIN_BYTE) 
    /(lMaximalPixel - lMinimalPixel);

  for (i = 0; i < NumPixels; i++) {
    inoutImage[i] -= lMinimalPixel * lNormalisingFactor;
    inoutImage[i] += MIN_BYTE;
  }

  return 0;

}
