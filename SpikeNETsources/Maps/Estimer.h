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

// $Log: Estimer.h,v $

#include "allDefs.h"

#ifndef _ESTIMER
#define _ESTIMER

// estimation of the number of spike within layers of map
// only for display

class Estimer
{
  	long		order_layer  		[MAX_COUCHE];		// number of spikes in the layer at the previous time step
  	long		order_layer_tmp  	[MAX_COUCHE];		// number of spikes in the layer at the current time step
  	long		tab_nbneur_layer	[MAX_COUCHE];		// number of neuron in the layer
  	double		estim				[MAX_COUCHE];		// estimation of the color to be associated with the spikes
  	double		real_estim			[MAX_COUCHE];		// estimator in percentage

public:
	
	Estimer()
	{
		for (int i=0; i<MAX_COUCHE; i++)
		{
			estim 		     [i] = 0;
			tab_nbneur_layer [i] = 0;
		}
	}
	
	void addMap( int layer, int nbNeur, double estim0 )
	{
		//if ( estim[ layer ] == 0)
			estim [layer] = estim0;
		/*else if ( estim[ layer ] != estim0 )
				ERROR( 131, ("estim parameter not homogenous within layer %d", layer))
		*/	
        tab_nbneur_layer [layer] += nbNeur;
        real_estim   	 [layer]  = EnvVar :: NB_COLOR / ( estim[ layer ] * (double)tab_nbneur_layer[ layer ] );
    }
    
    void reInit()
    {
		for (int i=0; i<MAX_COUCHE; i++)
		{
    		order_layer		[ i ]  = 0;
    		order_layer_tmp [ i ]  = 0;
    	}
    }
    
    void update()
    {
		for (int i=0; i<MAX_COUCHE; i++)
		{
    		order_layer		[ i ]  += order_layer_tmp [ i ];
    		order_layer_tmp [ i ]	= 0;
    	}
    }
    
	int getEstimerAndUpdate( int layer, int nbSpikes )
	{
		order_layer_tmp [ layer ] += nbSpikes;
		return (int)(order_layer [ layer ] * real_estim[ layer ]);
	}
	int getNbSpike (int layer )
	{
		return 	order_layer [ layer ];
	}	    	
};

#endif