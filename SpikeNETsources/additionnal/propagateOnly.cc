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

// $Log: propagateOnly.cc,v $


#include "SuperSpikeNETClass.h"

	// environment variables		
    int 	EnvVar::NB_COLOR      		= DEFAULT_NB_COLOR;
    int 	EnvVar::MAX_TEMPS     		= 0;
    int 	EnvVar::MAX_NORM      		= DEFAULT_MAX_NORM;
    int	 	EnvVar::SAVE_ON_DISK  		= DEFAULT_SAVE;
    int	 	EnvVar::DISPLAY  			= DISPLAY;
    int 	EnvVar::NO_SHOW       		= DEFAULT_SHOW;
    int		EnvVar::MOD_LINEAIRE  		= DEFAULT_MOD;
    int		EnvVar::NB_SPIKENET   		= 1;
    int		EnvVar::WAIT_ENTER    		= 0;
    int		EnvVar::CONVERGENCE_CONVO 	= DEFAULT_CONV_CONVO;
    int		EnvVar::NET_NAME_VERSION	= DEFAULT_NAME_VERSION;
	int		EnvVar::STEP			  	= 1;
    double	EnvVar::MOD_CUT       		= atof(DEFAULT_MOD_CUT);
    double	EnvVar::RAISE_THRESHOLD 	= atof(DEFAULT_RAISE_THRE);
    double	EnvVar::LOWER_THRESHOLD 	= atof(DEFAULT_LOWER_THRE);
    char 	EnvVar::RESULTS [40] 		= DEFAULT_RESULTS;
    char 	EnvVar::MAIN_DIRECTORY[100] = ".";
 	int	    EnvVar::MOD_TYPE			= DEFAULT_MODABS;
  	int	    EnvVar::ZOOM_CONVO			= DEFAULT_ZOOM_CONVO;
  	char    EnvVar::DIR_SAVE_CONVOS[40]	= DEFAULT_DIR_SAVE_CONVOS;
  	char    EnvVar::DIR_SAVE_LEARN[40]	= DEFAULT_DIR_SAVE_LEARN;
  	char    EnvVar::DIR_SAVE_REBUILD[40]= DEFAULT_DIR_SAVE_REBUILD;
  	char    EnvVar::DIR_SAVE_STATS[40]	= DEFAULT_DIR_SAVE_STATS;
 	int	    EnvVar::PURE_ORDER			= DEFAULT_PURE_ORDER;
  	int	    EnvVar::BYTES_PER_PIXEL		= DEFAULT_BYTES_PER_PIXEL;
  	int	    EnvVar::BYTES_FILE			= 1;
  	int	    EnvVar::SIZEX				= DEFAULT_SIZEX;
    int	    EnvVar::SIZEY				= DEFAULT_SIZEY;
    int	    EnvVar::REPLACE0_1			= DEFAULT_REPLACE0_1;
    int	    EnvVar::LEARN   			= DEFAULT_LEARN;
    int	    EnvVar::FOND    			= 1;
    int	    EnvVar::RESCUE   			= 0;
    double  EnvVar::TIME_BASE  			= atof(DEFAULT_TIME_BASE);
    double  EnvVar::SPONTANEOUS  		= 0.0;
    int 	EnvVar::BRIME_LIB     		= 0;
    int 	EnvVar::THRESHOLD_TYPE     	= 0;
    int		EnvVar::WATCH_ACTIVITY     	= 0;
    int		TabWeight::sizetabtmp		= 0;
    int		**TabWeight::tabGlobal		= NULL;

#define MAX_WATCH	200


/*#include "weights.h"
class WeightStat : public Weights
{
protected:
	double	var;
	double	mean;
	
public:
	double getMean() { return mean; }
	double getVar()  { return var;  }
	
	WeightStat() : Weights() { }
	void computeStat()
	{
		mean = 0;
		var  = 0;
		for (int i=0; i<width*height; i++)
		{
			mean += ImInt[i];
			var  += ImInt[i] * ImInt[i];
		}
		var = sqrt(mean*mean - var)	/ ((double)(width * height));
		mean /= ((double)(width * height));
	}
};

class compareConvos
{
	Weights	*tabWeight[100];
	int		nbTabWeights;
	
	compareConvos()
	{
		nbTabWeights = 0;
	}
};*/


static SuperSpikeClass *mainObject = NULL;

int main(int argc, char *argv[])
{
    
	mainObject = new SuperSpikeClass(argc, argv);
    EnvVar::WATCH_ACTIVITY     	= 1;
    EnvVar::LEARN				= 0;
	mainObject->propagateOnly();
	mainObject->compute();
}
