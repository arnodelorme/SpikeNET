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

// $Log: SpikeInputs.h,v $


#ifndef _AFFERENCE
#define _AFFERENCE

#include <stdlib.h>
#include <math.h>
#include "allDefs.h"
#include "allStructs.h"
#include "Weights.h"
#include "SpikeStat.h"
#include "TabWeight.h"

class carte;
class carte_base;
class carte_modul;
class carte_decay;
class carte_modul_decay;

// this class handle the afferent map and convolution for a map
// ------------------------------------------------------------

class SpikeInputs
{
public:
	// to handle afferences
	int				nb_aff;							// number of afferent map to the current map
	int				convLockMap;					// wether or not at least one convolution is unlocked
	double			total_poids;					// total of the positive synaptic weights
	carte_base		*carte_aff[MAX_CARTE_FROM];		// pointer to the afferent maps
	int				group[MAX_CARTE_FROM];			// group associated with each map
    short	        zoom_afferent[MAX_CARTE_FROM];	// zoom associated with each afferent map (the difference of zoom between the afferent and the current map is usefull for computation)
    Weights 		*tab_tab_poids[MAX_CARTE_FROM]; // pointer to the convolutions

	// pour gerer les groups
	int 			nb_groups;							// number of groups
	FORMAT_COMPUTE 	*tab_modulation[MAX_GROUP];			// arrays of modulaion associated with each group
	FORMAT_COMPUTE 	*tab_modulation_cumul[MAX_GROUP];	// arrays of modulaion to compute accuratelly modulation associated with multiple spikes
	char			*fact_modulation[MAX_GROUP];		// foctor of mudulation (char version)
	char 			*name[MAX_GROUP];					// name of the groups
	int 			relevant_afferences[MAX_GROUP];		// number of relevant afference associated with each group (can speed up the computation if it's associated with number under the number of synaptic weights for this group)
	int 			somme_poids[MAX_GROUP];				// Total of the positive synaptic weights for a group
	SpikeStat		*spikeStat[MAX_GROUP];				// statistic of spikes within a group
	TabWeight		*tabWeight[MAX_GROUP];				// complex class regrouping convolution belonging to the same group

public:	
	SpikeInputs() { nb_groups = 0; nb_aff = 0; convLockMap = 1; total_poids = 0; // ensuite il faudra le faire pour les differents groups
		for (int i=0; i<MAX_GROUP; i++) { spikeStat[i] = NULL;  fact_modulation [i] = NULL; tabWeight[i] = NULL; }}
	void 		detect_convolution(carte_base *une_carte_from, char *name_convolution, short zoom_afferent0, int convLock
			       , double randFact, double multFact, double decFact, char *modFact, char *nameGroup, int *border_l0, int *border_h0, int duplicator, char *latency, int noZoom, double renorm);
	void 		add_convolutions		( int largeur_tot, int hauteur_tot);
	double 		compute_modulation		();
	void 		add_SpikeInputs			( int total_afferences, int SpikeInputsNum);
	int 		detect_SpikeInputs		( char *name0);
	TabWeight   *computeTabWeight		( char *groupName, char *mapName, int sorting);
	TabWeight 	*computeTabWeight		( int groupNum, char *mapName, int sorting);
	FORMAT_POIDS compute_max_activite	( char *groupName, char *mapName,  double percent);
	double 		compute_somme			();
	double 		compute_somme			( char *groupName );
	void 		affiche					();
	double 		computeMean				( int groupNum, double percentDech);
	double 		computeVar				( int groupNum, double percentDech);
	void		renormConvos			( double val);
};

#endif
