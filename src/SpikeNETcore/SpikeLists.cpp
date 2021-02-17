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

// $Log: SpikeLists.cpp,v $

#include "SpikeLists.h"
	
	void SpikeLists::reInit(int nbReserv, coord_array *megaList )
	{
		if (megaList != NULL) allSpikeLists = megaList;
		nb = nbReserv;
		current_ptr = -1;
	}

	void SpikeLists::reInit(int nbReserv )
	{
		if (allSpikeLists == NULL)
		{
			nb = nbReserv;
			allSpikeLists = new coord_array[ nbReserv ];
		
			if (nb > MAX_SPIKE_LIST)
				ERROR(202 ,("max latency overflow"));
			for (int i=0; i< nb; i++)
				allSpikeLists[ i ].array = new coord[MAX_SPIKE];
		}
		reInit();
	}	

	void SpikeLists::reInit()
	{
		current_ptr  = 0;
		next_list    = allSpikeLists + 1 ;
		for (int i=0; i< nb; i++)
			allSpikeLists[ i ].size = 0;
	}	

	void SpikeLists::write( int nb2write, char *fileName)
	{
#define NEURON
		FILE *f = fopen(fileName, "w");
		if (!f) ERROR( 13, ("unable to open file %s for writing", fileName))
#ifdef NEURON
		int wordCount = nb2write;
		for (int ii=0; ii< nb2write; ii++)
			 wordCount += allSpikeLists[ ii ].size*2;
		fprintf( f, "%d\n", wordCount);
#endif
		for (int i=0; i< nb2write; i++)
		{
			wordCount++;
			fprintf(f, "%d\t", allSpikeLists[ i ].size);
			for (int j=0; j< allSpikeLists[ i ].size; j++)
				{ fprintf(f, "%d\t%d\t", allSpikeLists[ i ].array[j].x, allSpikeLists[ i ].array[j].y); wordCount+=2; }
			fprintf(f, "\n");
		}
		fclose( f );
	}
	
	void SpikeLists::writeCount( int nb2write, char *fileName )
	{
		//read
		FILESTREAM *f = new FILESTREAM(fileName);
		short *tmpSum = new short[EnvVar::MAX_NORM+10];
		int *tmpVar= new int[EnvVar::MAX_NORM+10];
		short *tmpCount= new short[EnvVar::MAX_NORM+10];
		char tmpRead[100];
		
		for (int i=0; i< nb2write; i++)
		{
			*f >> tmpRead;
			tmpSum[ i ] = atoi(tmpRead) + allSpikeLists[ i ].size;
			*f >> tmpRead;
			tmpCount[ i ] = atoi(tmpRead)+1;
			*f >> tmpRead;
			tmpVar[ i ] = atoi(tmpRead)+allSpikeLists[ i ].size*allSpikeLists[ i ].size;
		}
		delete f;

		//write
		FILE *fo = fopen(fileName, "w");
		if (!fo) ERROR( 13, ("unable to open file %s for writing", fileName));
		for (int ii=0; ii< nb2write; ii++)
			fprintf(fo, "%d\t%d\t%d\n", tmpSum[ii], tmpCount[ii], tmpVar[ii] );
		fclose( fo );
		delete tmpSum, tmpVar, tmpCount; 			
	}
		
	void SpikeLists::affiche(int offset) {
		coord_array *spike_list = getSpikeList( 0);
		printf("%d\n", spike_list->size);
		for (int i=0; i< spike_list->size; i++)
			printf("%d-%d\n", spike_list->array[ i ].x,  spike_list->array[ i ].y);

	}
	
	void SpikeLists::nextSpikeList( int reinit )
	{
		current_ptr  = (current_ptr + 1) % nb;
		next_list    =  allSpikeLists + ((current_ptr + 1) % nb);
		if (reinit == YES) next_list->size = 0;
	}