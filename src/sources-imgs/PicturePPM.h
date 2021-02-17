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

// $Log: PicturePPM.h,v $

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef _PicturePPM
#define _PicturePPM

#include "BaseImg.h"

class PicturePPM : public BaseImg
{
	int getint(FILE *fp);

public:

	// constructors
	PicturePPM	(char 	*FileName);
	PicturePPM ( int w0, int h0) : BaseImg( w0, h0, 1) { }
	PicturePPM	(unsigned char *Im0, int w0, int h0)  : BaseImg( Im0, w0, h0 ) { bytePerPixel = 1; }	
	PicturePPM() : BaseImg() { bytePerPixel = 1; }

	// read, write
	void write_ppm	(char 	*FileName, int step=1);
	void write_ppm_grab( int absc, int ordo, int widthG, int heightG, char *FileName);
	void write_ppm_grab_abs( int absc, int ordo, int absf, int ordf, char *FileName)
	{	write_ppm_grab( absc, ordo, absf-absc, ordf-ordo, FileName); }
};

typedef PicturePPM *ptrPicturePPM;

#endif
