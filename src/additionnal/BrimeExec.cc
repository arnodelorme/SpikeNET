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

// $Log: BrimeExec.cc,v $

// ******************************
// *					    	*
// *		SpikeNet            *
// *						    *
// ******************************

#include "SpikeNETlib.h"
#include "PicturePPM.h"
#define  BRIME_MODE	0		// 1=LEARN 0=RUN

int main(int argc, char *argv[])
{
	
	char *tabImg[]	=
	{ ":Images:CarsCam24Voie2:FramesTiff001.PPM",	
	":Images:CarsCam24Voie2:FramesTiff002.PPM",
	":Images:CarsCam24Voie2:FramesTiff003.PPM",
	":Images:CarsCam24Voie2:FramesTiff004.PPM",
	":Images:CarsCam24Voie2:FramesTiff005.PPM",
	":Images:CarsCam24Voie2:FramesTiff006.PPM",
	":Images:CarsCam24Voie2:FramesTiff007.PPM",
	":Images:CarsCam24Voie2:FramesTiff008.PPM",
	":Images:CarsCam24Voie2:FramesTiff009.PPM",
	":Images:CarsCam24Voie2:FramesTiff010.PPM",
	":Images:CarsCam24Voie2:FramesTiff011.PPM",
	":Images:CarsCam24Voie2:FramesTiff012.PPM",
	":Images:CarsCam24Voie2:FramesTiff013.PPM" };

	int posx[] = { 100,92,70,92,59,69,101,97,103,96,81,109,75 };
	int posy[] = { 49,38,48,48,50,89,48,52,41,48,48,49,57 };

	int nbImgs = 13;
	ImageStruct imgStr;
	CoordStruct pos;
//	CoordStruct size; size.x = 33; size.y = 33;
	CoordStruct size; size.x = 15; size.y = 15;

#if BRIME_MODE == 1
    //learn 
	for (int i=0; i<nbImgs; i++)
	{
		PicturePPM *baseImg = new PicturePPM( tabImg[i] );
		imgStr.width = baseImg->getWidth();
		imgStr.height = baseImg->getHeight();
		imgStr.data = baseImg->Im;
		
		pos.x = posx[i];
		pos.y = posy[i];
		
		learn( &imgStr, &pos, &size, "car", "networkBrime");
	}
#else	
	/* execute */
	for (int i=0; i<nbImgs; i++)
	{
		PicturePPM *baseImg = new PicturePPM( tabImg[i] );
		imgStr.width = baseImg->getWidth();
		imgStr.height = baseImg->getHeight();
		imgStr.data = baseImg->Im;

		ImageStruct *Im;
		Im = run( &imgStr, "car", "networkBrime");
		PicturePPM *baseImg2 = new PicturePPM( Im->data, Im->width, Im->height);
		baseImg2->write_ppm(":saved");
	}
#endif
}


