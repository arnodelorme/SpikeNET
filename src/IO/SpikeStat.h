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

// $Log: SpikeStat.h,v $

#ifndef _SPIKESTAT
#define _SPIKESTAT

#include "allDefs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// this class deals with computing statistic of synaptic weights ORDER : value and time
// ------------------------------------------------------------------------------------

class SpikeStat
{	
	double		*tabSpike;		// average of synaptic weight values for a definite order
	double	 	*tabVar;		// variance of synaptic weight values for a definite order
	double		*tabTime;		// average of spike latency for a definite order
	int			*tabSize;		// number of spikes	for each order
	int			tabSpikeSize;	// size of all arrays depend on the number of synaptic weight

public:
	SpikeStat( int size, char *fileName = NULL)
	{
		tabSpikeSize = size;
		tabSpike     = new double[size];
		tabVar       = new double[size];
		tabTime      = new double[size];
		tabSize		 = new int[size];
		for (int i=0; i<tabSpikeSize; i++) { tabSpike[ i ] = 0; tabVar[ i ] = 0; tabSize[ i ] = 0; tabTime[ i ] = 0; }
		if (fileName != NULL) read(fileName);
	}
	
	void writeStats( char *fileName)
	{
		FILE *f = fopen(fileName, "w");
		for (int i=0; i<tabSpikeSize; i++)
		{
			double ecartype = sqrt(tabVar[ i ] - tabSpike[ i ]*tabSpike[ i ]);
    		fprintf( f, "%f\t%f\t%f\t%d\n", tabSpike[ i ], ecartype, tabTime[ i ] / tabSize[i], tabSize [ i ]);
    	}
    	fclose(f);
	}

	void read( char *fileName)
	{
		FILESTREAM	f(fileName);
		char tmp[50];
		for (int i=0; i<tabSpikeSize; i++)
		{
			f >> tmp; tabSpike[ i ] = (FORMAT_POIDS) atof( tmp);
			f >> tmp; f >> tmp; tabSize[ i ] = (FORMAT_POIDS) atof( tmp);
		}
	}
	
	FORMAT_POIDS getValue( int index )
	{ 	return (index < tabSpikeSize) ? tabSpike[ index ] : 0;	}
	
	void addValue( int order, FORMAT_POIDS value, int time)
	{
		if (order >= 0)
		{
			tabSpike[ order ] = (tabSpike[order]*tabSize[order] + value)/(tabSize[order]+1);
			tabVar  [ order ] = (value * value + tabVar  [order]*tabSize[order])/(tabSize[order]+1) ;
			tabTime [ order ] += time;
			tabSize[order] ++;
		}
		else exit(-1);
	}
};

#endif
