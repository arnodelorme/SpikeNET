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

// $Log: BrimeExec2.cpp,v $

// ******************************
// *					    	*
// *		SpikeNet            *
// *						    *
// ******************************

#include "SpikeNETlib.h"
#include "allDefs.h"
#include "ImageListRich.h"

    char 	EnvVar::MAIN_DIRECTORY[100] = "networkSupervTestMultiMap";
    int	    EnvVar::RESCUE   			= 0;
    
    
#define  BRIME_MODE	1		// 1=LEARN 0=RUN

int main(int argc, char *argv[])
{
	ImageListRich		*img_list;
	img_list = new ImageListRich( 1 );

	img_list->reserv();
	img_list->init( 1 );
	GlobalImg **currentImg;

	// coordinate declaration
	char *allMapLearn[10];
	CoordStruct pos[10];
	CoordStruct size[10];
	
	//PicturePPM *outputImg = new PicturePPM( 240, 30);
	//outputImg->clear( 255 ) ;

#if BRIME_MODE == 1
	while((currentImg = img_list->revert()) != NULL) {
	
		int 		coordx, coordy;
		char 		*mapLearn = NULL;
		int			nbLoop = 0;
	
		// transfering coordinates
		while(img_list->getMapLearn( &mapLearn, &coordx , &coordy) != NULL)
		{
			pos[ nbLoop ].x = coordx;
			pos[ nbLoop ].y = coordy;
			allMapLearn[ nbLoop ] = mapLearn;
			nbLoop++;
		}
		for (int i=0; i<nbLoop; i++)
			printf("treating image %s Learn %s %d %d\n", img_list->getCurrentImg(), allMapLearn[ i ], pos[ i ].x, pos[ i ].y);
//		exit(-1);
		learn( &imgStr, &pos, &size, "car", "networkBrime");
	}
#endif		
}
