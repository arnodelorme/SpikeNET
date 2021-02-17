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

// $Log: SpikeListsExtended.h,v $

#include "allDefs.h"
#include "allStructs.h"
#include "SpikeLists.h"
#include "GlobalImg.h"

#ifndef _SPIKELISTSEXTENDED
#define _SPIKELISTSEXTENDED

// this class deals handling spike lists for a map
// -----------------------------------------------

class SpikeListsExtended : public SpikeLists
{
	GlobalImg *latencyImg;
public:

	// constructors
	SpikeListsExtended() : SpikeLists( )						{ latencyImg = NULL; }
	SpikeListsExtended( int nbReserv) : SpikeLists(  nbReserv )	{ latencyImg = NULL; }
	
	void makeSpikeStat( GlobalImg *ImTreat, int max_norm);
	int computeSpikeList( int max_norm );
 	void nomalizeSize( int count, int norm, double spikePC);
 	int makeSpikeList( GlobalImg *ImTreat, int max_norm, int norm, double spikePC, int border = 0);


	void computeLatenciesRenorm( GlobalImg *ImTreat, int max_norm, int border);
	void computeLatenciesAbsolute( GlobalImg *ImTreat, int max_norm, int border);
	double computeIFthreshold();
	double realIF( double I);
};

#endif
