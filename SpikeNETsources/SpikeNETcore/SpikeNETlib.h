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

// $Log: SpikeNETlib.h,v $


#ifndef _SPIKENET_LIB
#define _SPIKENET_LIB

typedef struct
{
	int	width;
	int	height;
	unsigned char *data;
} ImageStruct;

typedef struct
{
	int x;
	int	y;
} CoordStruct;

/* fonction a utiliser pour l'apprentissage */
void learn( ImageStruct *Im, CoordStruct *coordvalues, CoordStruct *sizeValues, char *directoryLearn, char *network);

/* fonction a utiliser pour l'execution */
ImageStruct *run( ImageStruct *Im, char *directoryLearn, char *network);

/* modification du seuil pendant l'execution */
void modifyThreshold( double val);

#endif