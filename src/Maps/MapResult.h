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

// $Log: MapResult.h,v $

#include <stdlib.h>
#include <string.h>

#include "allDefs.h"
#include "GlobalImg.h"

#ifndef _MAPRESULT
#define _MAPRESULT

typedef char * ptr_char;

// handling results on supervised maps (drawing rectangels on objects)
// for one big image only, give all the maps that need to discharge on this map

class MapResult
{
protected:
	class MapDescRes
	{
		public:
			char map[30];		// name of the map
			int x;				// x position to discharge
			int y;				// y position to discharge
			int offsetx;		// half width to accept discharge in that position 
			int offsety;		// half height to accept discharge in that position 
			int desactivated;	// state of that position
			
		MapDescRes( char *mapName, int coordx, int coordy, int offsetx0, int offsety0)
		{	strcpy(map, mapName); x = coordx; y = coordy; offsetx = offsetx0; offsety = offsety0; desactivated =0;}
		
		int testPos( int posx, int posy, char *mapName) 
		{
			int res = ((abs(posx-x)<=offsetx) && (abs(posy-y)<=offsety));
			if (res)
			{
				if (desactivated) return -1;
				desactivated = 1;
				if (!strcmp( mapName, map)) return 2;
				else						return 1;
			}
			else return 0;
			// 2  = at this position
			// 1  = at this position but wrong map
			// -1 = at this position but position desactivated
			// 0  = not at this position
		}
	};
	
	MapDescRes 		*tabRes[MAX_IMAGE];		// number of positions 
	int		   		nbItems;				// number of positions fo this image
	PicturePPM		*pict;					// image to put rectangle
	
public:

	MapResult( char *fileName, char *ImageFileName)
	{
		pict = new PicturePPM( ImageFileName );
	
		FILESTREAM ff( fileName);
		char mapName[30], coordx[30], coordy[30], offsetx[30], offsety[30];
		nbItems = 0;
		int eof1 = EOF+1;
		do
		{
			ff >> mapName;
			ff >> coordx;
			ff >> coordy;
			ff >> offsetx;
			ff >> offsety;
			eof1 = ff.get();
			if ((eof1 != EOF) || (mapName[0] != 0))
				tabRes[nbItems++] = new MapDescRes( mapName, atoi(coordx), atoi(coordy), atoi(offsetx), atoi(offsety));
		} while (eof1 != EOF);
	}

	MapResult( char *fileName, char *ImageFileName, int offsetX = 0, int offsetY = 0)
	{
		pict = new PicturePPM( ImageFileName );
	
		FILESTREAM ff( fileName);
		char mapName[30], coordx[30], coordy[30], offsetx[30], offsety[30];
		nbItems = 0;
		int eof1 = EOF+1;
		do
		{
			ff >> mapName;
			ff >> coordx;
			ff >> coordy;
			ff >> offsetx;
			ff >> offsety;
			eof1 = ff.get();
			if ((eof1 != EOF) || (mapName[0] != 0))
				tabRes[nbItems++] = new MapDescRes( mapName, atoi(coordx)-offsetX, atoi(coordy)-offsetY, atoi(offsetx), atoi(offsety));
		} while (eof1 != EOF);
	}
	
	int testForRes( int posx, int posy, char *mapName)
	{
		int currentPos = 0;
		int res;
		while( (!(res = tabRes[ currentPos ]->testPos( posx, posy, mapName))) && (currentPos < nbItems)) currentPos++;
		
		if (currentPos != nbItems)
		{
			if (res == 2) pict->drawRect( tabRes[ currentPos ]->x-10, tabRes[ currentPos ]->y-13, tabRes[ currentPos ]->x+10, tabRes[ currentPos ]->y+13, 255);
			if (res == 1) pict->drawRect( tabRes[ currentPos ]->x-10, tabRes[ currentPos ]->y-13, tabRes[ currentPos ]->x+10, tabRes[ currentPos ]->y+13, 128);
			if (res == 2) printf("%s: ACCURATE detection at location %d %d\n", mapName, posx, posy);
			else	if (res == 1) printf("%s: FALSE detection at location %d %d\n", mapName, posx, posy);
					//else	if (res == -1) printf("ALREADY detected for %s at poisition %d %d\n", mapName, posx, posy);
		}
		else { res = 0; } //printf("no detection for map %s at position %d-%d\n", mapName, posx, posy); }
		return res;
	}
	void save( char *newFileName)
	{ pict->write_ppm( newFileName);}
};
#endif
