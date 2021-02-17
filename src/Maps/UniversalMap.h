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

// $Log: UniversalMap.h,v $

#include <stdlib.h>
#include <math.h>
#include "allDefs.h"
#include "allStructs.h"
#include "Weights.h"
#include "SpikeInputs.h"
#include "Sender.h"
#include "SpikeListsExtended.h"
#include "OutputDisplay.h"
#include "Rebuild.h"
#include "GlobalImgTreat.h"

#ifndef _UNIVMAP
#define _UNIVMAP

// carte de base desquelles toute les autres découlent
// ---------------------------------------------------

class UniversalMap {

public:
	int					temps;					// pointeur local sur le temp

    virtual void 			init_neuron(GlobalImgTreat **listImg, int supervParam) = 0;
    virtual void 			affiche() = 0;    
    virtual void 			detect_convolution(carte_base *une_carte_from, char *name_convolution, int convLock, double randFact, double multFact, double decFact, char *modFact, char *nameGroup, int duplicator, char *latency, int noZoom, double renorm) =0;
   	virtual void 			add_convolutions() 			= 0;

    virtual void 			resetComputeSync() 			= 0;
    virtual void 			resetComputeSyncLearn() 	= 0;
	virtual int 			compute_all() 				= 0;
	virtual int 			learn_all()   				= 0;
   	virtual FORMAT_COMPUTE	seuil_mod() 				= 0;
   	virtual double 			seuil_mod( double d)		= 0;
};

#endif