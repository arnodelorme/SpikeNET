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

// $Log: WatchAct.h,v $

#ifndef _WATCH_ACT
#define _WATCH_ACT

#define MAX_WATCH	200
#include "allDefs.h"

// this class intend to watch the activity of maps for a definite image (and make stats)
// -------------------------------------------------------------------------------------
// name of the variable might be wrong - to cheeck in more details

class WatchAct
{
	typedef char	*ptrChar;

	ptrChar			*tabImg;		// names of the images

	ptrChar			*tabWatch;		// names of the maps
	FORMAT_COMPUTE	*tabAct;		// activity of the maps
	int				maxWatch;		// maximal number of maps to be watched
	int				currentWatch;	// map currently been watched
	int				currentImg;		// image currently been treated

public:	
	WatchAct()
	{
		maxWatch = MAX_WATCH;
		tabWatch = new ptrChar[ MAX_WATCH ];
		tabImg 	 = new ptrChar[ MAX_IMAGE ];
		tabAct	 = new FORMAT_COMPUTE[ MAX_IMAGE * MAX_WATCH];
		currentWatch = 0;
		currentImg   = -1;
	}
	
	void addWatch( char *name0)
	{	
		if (currentWatch < MAX_WATCH)
			tabWatch[ currentWatch++ ] = name0;
		else
			ERROR( 215, ("array overflow"))
	}
	void addImg( char *imageName)
	{
		currentImg++;
		tabImg[ currentImg ] = imageName;
	}
	
	void addAct( int pos, FORMAT_COMPUTE activity)
	{	tabAct[ pos + currentImg * MAX_WATCH] = activity;	}

	void addAct( char *name, FORMAT_COMPUTE activity)
	{
		int i=0;	
		do
		{	if ( name == tabWatch[ i ] ) break;
			i++;
		} while( i < currentWatch );
		if (i==currentWatch) ERROR( 215, ("array overflow"))
		tabAct[ currentImg * MAX_WATCH + i] = activity;
	}	
				
	void writeAll( char *fileName)
	{
		if (currentWatch)
		{
			FILE *f = fopen( fileName, "w");
			if (f == NULL) ERROR(13, ("unable to open file %s for writing", fileName))
		
			//write	Names and sort data
			for (int i=0; i<currentWatch; i++)
				fprintf( f, "%s\t", tabWatch[i]);
			fprintf( f, "\n");
		
			//write Datas
			for (int k=0; k< currentImg; k++)
			{
				fprintf(f, "%s\t", tabImg[ k ]);
				for (int j=0; j<currentWatch; j++)
					fprintf( f, "%d\t", tabAct[ j + k * MAX_WATCH ]);
				fprintf( f, "\n");
			}
			fclose(f);
		}
	}
};

#endif