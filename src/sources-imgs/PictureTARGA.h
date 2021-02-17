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

// $Log: PictureTARGA.h,v $

#ifndef _PictureTARGA
#define _PictureTARGA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "BaseImg.h"

class PictureTARGA : public BaseImg
{
public:

	// constructors
	PictureTARGA (char  *fileName);
	PictureTARGA (unsigned char *Im0, int width0, int height0) : BaseImg( Im0, width0, height0) { bytePerPixel = 3; }
	PictureTARGA ( int w0, int h0) : BaseImg( w0, h0, 3) {  }
	
	// read, write
	void writeTARGA(char *fileName);
    void writeTARGA_grab(int absc, int ordo, int widthG, int heightG, char *fileName);		
    void writeTARGA_grab_abs(int absc, int ordo, int abse, int orde, char *fileName)
    { 	writeTARGA_grab(absc, ordo, abse-absc, orde-ordo, fileName); }
};

#endif

