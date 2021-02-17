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

// $Log: SpikeLists.h,v $

#include "allDefs.h"
#include "allStructs.h"

#ifndef _SPIKELISTS
#define _SPIKELISTS

// this class deals handling spike lists for a map
// -----------------------------------------------

class SpikeLists
{
protected:
	int 		nb;				// number of spike lists
	int 		current_ptr;	// current spike list being treated
	coord_array	*next_list;		// next spike list being treated
	coord_array *allSpikeLists; // array of pointer to spike lists
	
public:

	// constructors
	SpikeLists( )				{ allSpikeLists = NULL; }
	SpikeLists( int nbReserv )	{ allSpikeLists = NULL; reInit( nbReserv ); }
	
	// reintialize
	void reInit(int nbReserv, coord_array *megaList );		// from a definite spike list (if megaList=NULL assume that it is already calculated)
	void reInit(int nbReserv );								// from a number of spike lists to store
	void reInit();											// to the beginning of the spike list array

	// access
	coord_array *getSpikeList(int offset)	{ return  allSpikeLists + ((current_ptr + offset) % nb); }
	coord_array *getNext()					{ return next_list; }
	void nextSpikeList( int reinit );						// go to the next spike list (if reinit, put next next spike list size to 0)


	// other methods
	void affiche(int offset=-1);							// print to screen the current (-1) spike list, otherwise spike list at a definite offset
	void write( int nb2write, char *fileName);				// write (text) the spike list (special format for Neuron)
	void writeCount( int nb2write, char *fileName );		// append (text) to a spike list file and compute stat
};

#endif
