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

// $Log: BaseImg.h,v $

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef _BASEIMG
#define _BASEIMG

class BaseImg
{
protected:
	
	int					width;
	int					height;
	int					bytePerPixel;

public:
	unsigned char 		*Im;

	// construtors
	BaseImg() { bytePerPixel = 1; }
	BaseImg( unsigned char 	*Im0, int w0, int h0 ){ Im = Im0;	width  = w0; height = h0; bytePerPixel = 1; }
	BaseImg(int w0, int h0 ){ Im = NULL;	width  = w0; height = h0; bytePerPixel = 1; }
	BaseImg( int w0, int h0, int depth0){ width  = w0; height = h0; bytePerPixel = depth0; Im = new unsigned char[ w0*h0*depth0 ]; clear(0); }

	// access methods
	int getHeight() 		{ return height; }
	int getWidth() 			{ return width; }
	int getBytePerPixel() 	{ return bytePerPixel; }
	unsigned int getRVBPix( int posx, int posy);
	
	// virtuals methods
	virtual int update() { return 0; }
	virtual void clear( int color = 0 );
	
	// no memory allocation methods
	void put_pixel( int pos_x, int pos_y, int color);
	void put_RVB_pixel( int pos_x, int pos_y, int color);
	void horzMiror();
	void vertMiror();
	double meanLum();
	void random( double randFact, int factScale = 0);
	void shiftLum( int offset);
	void recalculateContrast( double factor);
	void drawRect( int xinit, int yinit, int xend, int yend, int color);
	void grab( int absc, int ordo, int widthG, int heightG);
	void invert();

	// memory reallocation methods
	int setDepth( int newBytePerPixel);
	void smooth(int nb = 1);
	void rotate90();
	void replaceInNewImage( int widthN, int heightN, int absN, int ordN);
	void scale( double factor);
	void resize( int w0, int h0);
	void div2();
	void recopi_ecran( unsigned char *base_copy, int width_copy);

private:
	// subfunction for resize
	double addLine( int posAbs, double multiplier, double posFlt, double factor );

};

#endif
