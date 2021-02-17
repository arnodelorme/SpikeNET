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

// $Log: SpikeListsExtended.cpp,v $

#include <math.h>
#include "SpikeListsExtended.h"

// paramters to compute with IF neurons
//-------------------------------------
#define V0			-0.070		//V
#define VT			-0.043		//V
#define CM		2E-6			//microF/cm^2
#define INPUT		-0.256E-9	//A
#define GLEAK		0.0001		//S/cm^2
#define SURFACE	0.00015			//cm^2
#define NOISE		0.5

double SpikeListsExtended::computeIFthreshold() {
	double tau = CM/GLEAK;
	return -CM*SURFACE*(VT-V0)/tau;
}

double SpikeListsExtended::realIF( double I) {
		double tau = CM/GLEAK;
		return -tau * log(1 + CM*SURFACE*(VT-V0)/(I*tau) );
}
//-------------------------------------

void SpikeListsExtended::computeLatenciesRenorm( GlobalImg *ImTreat, int max_norm, int border) {

  	int width  = ImTreat->getWidth();
  	int height = ImTreat->getHeight();
  	FORMAT_POIDS *ImInt  = ImTreat->ImInt;
	
	// compute min and max of the image
  	FORMAT_POIDS max = -10000000;
  	FORMAT_POIDS min =  10000000;
  	for (int i=0; i< width*height; i++) {
      if ((ImInt[i]>max) && (ImInt[i]!=INACTIVE)) max = ImInt[i];
      if ((ImInt[i]<min) && (ImInt[i]!=INACTIVE)) min = ImInt[i];
    }

	// renormalise
  	for(int j=0; j< height;j++)
  		for(int i=0; i< width;i++)
  			if ( (j<border) || (j>=height-border) || (i<border) || (i>=width-border) || (ImInt[i+j*width] == INACTIVE))
				 latencyImg->ImInt[i+j*width] = INACTIVE;
  			else latencyImg->ImInt[i+j*width] = (int) (((double)max_norm - 1.0)*(max - (double)ImInt[i+j*width])/((double)(max-min)));
}

void SpikeListsExtended::computeLatenciesAbsolute( GlobalImg *ImTreat, int max_norm, int border) {

  	int width  = ImTreat->getWidth();
  	int height = ImTreat->getHeight();
  	FORMAT_POIDS *ImInt  = ImTreat->ImInt;

	// compute latencies
 	for(int j=0; j< height;j++)
  		for(int i=0; i< width;i++)
  			if ( (j<border) || (j>=height-border) || (i<border) || (i>=width-border) || (ImInt[i] == INACTIVE))
				 latencyImg->ImInt[i] = INACTIVE;
  			else	{
				// compute as NOISY integrate and fire neuron
      			latencyImg->ImInt[i] = computeIFthreshold() + (-ImInt[i] * 1E-12 )*(1-NOISE) - 148E-12*(double)rand()/RAND_MAX*NOISE;
 				latencyImg->ImInt[i] = realIF(latencyImg->ImInt[i]); // in ms
 				
 				// Inactivate extreme positions
 				if ((latencyImg->ImInt[i] < 0) || (latencyImg->ImInt[i] > 0.100)) latencyImg->ImInt[i] = INACTIVE;
 				else latencyImg->ImInt[i] =  latencyImg->ImInt[i] /0.1*((double)max_norm - 1.0);
 			}
}

void SpikeListsExtended::makeSpikeStat( GlobalImg *ImTreat, int max_norm) {
  FORMAT_POIDS	tmpval;
  int width  = ImTreat->getWidth();
  int height = ImTreat->getHeight();
  FORMAT_POIDS *ImInt  = ImTreat->ImInt;
  
  // reset sizes
  for (int k=1;k<max_norm;k++) allSpikeLists [k].size = 0;
 
  // attribute each spike with its value
/*  for (int j= 0 ; j< height ; j++)
    	for (int i= 0; i< width ; i++)
			if ((tmpval = latencyImg->ImInt[ i + j*width ]) != INACTIVE) {
	    		coord 	*coord_tmp = allSpikeLists [ (int) tmpval ].array + allSpikeLists [ (int)tmpval ].size;
	    		allSpikeLists [ (int)tmpval ].size++;
	    		coord_tmp->val = ImInt[ i + j*width ];
	    	}*/
}

int SpikeListsExtended::computeSpikeList( int max_norm ) {

  int width  = latencyImg->getWidth();
  int height = latencyImg->getHeight();
  
  // reservation of the spike lists
  for (int k=0;k<max_norm;k++)	allSpikeLists[k].size = 0;

	latencyImg->minmax();
  
  for (int j= 0 ; j< height ; j++)
    	for (int i= 0; i< width ; i++)
    			if (latencyImg->ImInt[i+j*width] != INACTIVE)
    				allSpikeLists[ (int)latencyImg->ImInt[ i+j*width ] ].size++;
  
  for (int i=1;i<max_norm;i++) {
      allSpikeLists [i].array = allSpikeLists [i-1].array + allSpikeLists [i-1].size;
      allSpikeLists [i-1].size = 0;
  }
  allSpikeLists [max_norm-1].size = 0;
  
  // write results into the array
  int 			count = 0;
  FORMAT_POIDS	tmpval;
  
  for (int jj= 0 ; jj< height ; jj++)
    	for (int i= 0; i< width ; i++)
			if ((tmpval = latencyImg->ImInt[ i + jj*width]) != INACTIVE) {
	    		count++;
	    		coord 	*coord_tmp = allSpikeLists [ (int) tmpval ].array + allSpikeLists [ (int)tmpval ].size;
	    		allSpikeLists [ (int)tmpval ].size++;
	    		coord_tmp->x = i;
	    		coord_tmp->y = jj;
	  		}
   return count;
}
 
 void SpikeListsExtended::nomalizeSize( int count, int norm, double spikePC) {
  
  	// build spike list of constant sizes
	int taille = (int)(((double)count * spikePC)/ (double)norm);
  	if (taille == 0) {
      //printf("Warning : on & off spiking list size are 1, truncation");
      taille = 1;
      count  = (int)((double)count * spikePC);
    }
  
  	allSpikeLists [0].size = taille;
  	for (int i=1;i<norm;i++) {
      allSpikeLists [i].size  = taille;
      allSpikeLists [i].array = allSpikeLists [0].array + taille * i;
      if ((taille == 1) && (i>=count)) allSpikeLists [i].size = 0;
    }
}

int SpikeListsExtended::makeSpikeList( GlobalImg *ImTreat, int max_norm, int norm, double spikePC, int border) {

	// reserv if necessary
	if (allSpikeLists == NULL) {
  		allSpikeLists   		= new coord_array [max_norm + MAX_COUCHE + 40];
  		allSpikeLists[0].array 	= new coord		  [ ImTreat->getWidth() * ImTreat->getHeight() ];
  		if (allSpikeLists[0].array == NULL) ERROR(1,("out of memory"));
  	}

	// create the latency image if necessary
	if (latencyImg == NULL) latencyImg = new GlobalImg( ImTreat->getWidth(), ImTreat->getHeight(), 1);

	if (EnvVar::PURE_ORDER == OUI) {
		// normalize the size of each time step if necessary
		computeLatenciesRenorm (ImTreat, max_norm, border);
		int count = computeSpikeList( max_norm );	
	 	nomalizeSize( count, norm, spikePC);
 	} 
  	else { 
  		// compute real latencies
  		if (norm != max_norm) ERROR(103,("MAX_TEMPS and MAX_NORM must be equal in %s", NET_ENV));
		computeLatenciesAbsolute (ImTreat, max_norm, border);
		computeSpikeList( max_norm );
	}

#if SPIKESTAT == OUI
  	// attribute each spike with its value in the original image
	makeSpikeStat( ImTreat, border);
#endif

  	// put the last 3 spike list to 0
  	for (int i=norm;i<(norm + MAX_COUCHE);i++)
  		allSpikeLists [i].size  = 0;
  		
  	// return the number of list
  	return max_norm+MAX_COUCHE;
}

// old save image and compute LUT
// ------------------------------
	/*imgTreated->convert2char( );
	PicturePPM ppm( imgTreated->Im, imgTreated->getWidth(), imgTreated->getHeight());
	ppm.write_ppm( "toto2");
	printf("%f %f %d\n", imgTreated->getMax(), imgTreated->getMin(), INACTIVE);
	exit(-1);*/

  /*PicturePPM pict( imgTreated->getWidth(), imgTreated->getHeight());
  pict.clear(0);
  for (int i=0;i<norm;i++)
  	for (int j=0; j< allSpikeLists [i].size; j++)
      {
  		  int posnorm = (int)allSpikeLists [i].array[j].x + allSpikeLists [i].array[j].y * imgTreated->getWidth();
		  pict.Im[posnorm] = (unsigned char)(i*255/norm);
  	}
  pict.write_ppm("toto");
  exit(-1);*/
  
  // compute LUT PART 2 -- see also PART 1, spike statistic in the retina 
  /*FILE *f = fopen( ":fileLUT", "w");
  for (int i=0;i<norm;i++)
  {
  	for (int j=0; j< allSpikeLists [i].size; j++)
      {
  		  int posnorm = (int)allSpikeLists [i].array[j].x + allSpikeLists [i].array[j].y * width;
  		  tabSpike[ i ] = (tabSpike[ i ]*tabSize[ i ] + imgtmp->ImInt[ posnorm ]) / (tabSize[ i ]+1);
  		  tabSize [ i ]++;
      }
    fprintf( f, "%f\t%d\n", tabSpike[ i ], tabSize [ i ]);
  }
  fclose(f);*/