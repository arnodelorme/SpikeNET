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

// $Log: SpikeOrder.h,v $

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef _SPIKEORDER
#define _SPIKEORDER

class SpikeOrder
{

typedef struct
{
	carte_base 	*map;
	int			order;
#if LARGE_MAPS == OUI
    unsigned short x;
    unsigned short y;
#else
    unsigned char x;
    unsigned char y;
#endif
}	coord_map;

typedef struct
{
    int   	  size;
    coord_map *array;
} coord_map_array;


public:

	coord_map_array allSpikes;
	int				maxSpikes;
	int				tmpOrder;
	double			orderTab[ MAX_SPIKE ];
	int				orderNb [ MAX_SPIKE ];

	SpikeOrder()
	{
		allSpikes.array	= new coord_map[ MAX_SPIKE ];
		for (int  i=0; i<MAX_SPIKE; i++) orderTab[ i ] = 0;
		maxSpikes = 0;
		reinit();
	}
	
	void reinit()
	{ 
		allSpikes.size = 0;
		for (int  i=0; i<MAX_SPIKE; i++) orderNb[ i ] = 0;
	}

	void addSpike( int x, int y, carte_base *map, int inc)
	{
		allSpikes.array[ allSpikes.size   ].map   = map;
		allSpikes.array[ allSpikes.size   ].order = tmpOrder;
		allSpikes.array[ allSpikes.size   ].x = x;
		allSpikes.array[ allSpikes.size++ ].y = y;
		tmpOrder += inc;		
	}

	void addSpikeList( coord_array *c, carte_base *map, int inc)
	{
		for (int i=0; i<c->size; i++)
			addSpike( c->array[i].x, c->array[i].y, map, 0);
		tmpOrder += inc;
	}
	
	void LUTmake()
	{
		if (allSpikes.size> maxSpikes) maxSpikes = allSpikes.size;
		for (int i=0; i<allSpikes.size; i++)
		{
			orderTab[ allSpikes.array[ i ].order ] +=
				allSpikes.array[ i ].map->getActivite( allSpikes.array[ i ].x, allSpikes.array[ i ].y);
			orderNb [ allSpikes.array[ i ].order ]++;
		}
	}
};
#endif
