// Copyright (C) 1997-2002 Arnaud Delorme, arno@salk.edu
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

// $Log: GlobalImg.h,v $

#include "defImg.h"
#include "PicturePPM.h"
#include "PictureTARGA.h"

#ifndef _GLOBALIMG
#define _GLOBALIMG

class GlobalImg : public BaseImg
{
 public:
  
  FORMAT_POIDS 	*ImInt;
  
  // min and max of the image
  FORMAT_POIDS	max;
  FORMAT_POIDS	min;
  
  // constructors
  GlobalImg() : BaseImg() { ImInt = NULL; }
  GlobalImg(char *fileName, int type);  //type 0 = image type 1 = convolution
  GlobalImg(unsigned char *Im0, int w0, int h0);
  GlobalImg(int w0, int h0);
  GlobalImg( int w0, int h0, int depth0) : BaseImg( w0, h0, depth0) { ImInt = new FORMAT_POIDS[ w0*h0*depth0 ]; }

  // access methods
  FORMAT_POIDS getMin() { return min; }
  FORMAT_POIDS getMax() { return max; }

  void revertImg(char *fileName);						// load a new image into the current bloc of memory
  void convert2char(int renorm = 1);					// update Im from ImInt (by default, renormalize values)
  void updateInt();										// update ImInt form Im
  void setSize(int w, int h);							// set a new size for the image (delete the previous one)
  void minmax();										// compute min and max pixels
  void renormalize( FORMAT_POIDS renorm );				// renormalize so that the sum is renorm
  void renormalizeMax( FORMAT_POIDS renorm );			// renormalize from 0 to renorm
  void saveText( char *fileName);						// save the file in text format
  
  // virtual methods
  virtual int update() { convert2char(); return 0; }	// recursive version of the methods for display
  virtual void clear(int val=0);						// clear the image; override the method in BaseImg
 
};

#endif
