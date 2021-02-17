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

// $Log: MapFixed.h,v $

#include "MapBase.h"

#ifndef _MAPFIXED
#define _MAPFIXED

class carte_fixed : public carte_base
{
private:
	SpikeLispSpike *spL;	
	
public:
	carte_fixed(int numero0, OutputDisplay *disp0, char *name0, int largeur_norm0, int hauteur_norm0, int zoom0, SpikeLispSpike *spL0) :
		carte_base(numero0, disp0, name0, largeur_norm0, hauteur_norm0, zoom0) 
	{ 	
		FIXED 		 = OUI;
		spL = spL0;
		int factZoom = (zoom0>=0)?(1<<zoom0):-(1<<(-zoom0));
		spL->reserv( factZoom, largeur_norm0, hauteur_norm0);

	}

	virtual void reconstructConvo( int force)
	{
		// recog map
		if (recog == NULL)
		{
			//if (!strcmp(name, "ON"))	recog = new OutputDisplay( disp->pos_x + disp->getWidth() + INTER_LARG, disp->pos_y, 3, 3, EnvVar::ZOOM_CONVO, disp->screen, "Convolutions/OnCenter", 1);
			//if (!strcmp(name, "OFF"))	recog = new OutputDisplay( disp->pos_x + disp->getWidth() + INTER_LARG, disp->pos_y, 3, 3, EnvVar::ZOOM_CONVO, disp->screen, "Convolutions/OffCenter", 1);
			if (!strcmp(name, "ON"))	recog = new GlobalImg( "Convolutions/OnCenter", 1);
			if (!strcmp(name, "OFF"))	recog = new GlobalImg( "Convolutions/OffCenter", 1);
 			recog->convert2char();
 			disp->writeRebuilt( (BaseImg *) recog );
		}
	}
	
	virtual void init_neuron(GlobalImgTreat **listImg, int supervParam) 
	{ 
		// treat image
		spL->run(listImg);
		
		// compute a spike list and reinit
		int nbLists = spike_list.makeSpikeList( spL->getTreatedImg(), spL->MAX_NORM, spL->norm, spL->spikePC);
		spike_list.reInit( nbLists, NULL); // NULL means to keep internal pointer array of spike lits

		// retinit internal variables
        ordre_decharge = 0;
        temps = 0;
        if (disp != NULL)  disp->clear( 0 );
	}

	virtual void resetComputeSync()
	{
		spike_list.nextSpikeList( NO );
    	ordre_decharge += spike_list.getSpikeList( 0 )->size;
    	if (d_IP != NULL) d_IP->sendSpikes( spike_list.getSpikeList( 0 ), numero, temps);
    	if (disp != NULL) disp->writeSpikes( spike_list.getSpikeList( 0 ) );
	}
	virtual void resetComputeSyncLearn()
	{
		spike_list.nextSpikeList( NO );
    	if (d_IP != NULL) d_IP->sendSpikes( spike_list.getSpikeList( 0 ), numero, temps);
    	//if (disp != NULL) disp->writeSpikes( spike_list.getSpikeList( 0 ) );
	}

	virtual void affiche()
	{ 
		printf("%10s %4d\t%d * %d\t%d\n", name, numero, border_l, border_h, latencyReserv);
    	if (d_IP != NULL) d_IP->affiche(); else printf("\t\t\tno afferent\n");
    }
	
	virtual void detect_convolution(carte_base *une_carte_from, char *name_convolution, int convLock,
			    double randFact, double multFact, double decFact, char *modFact, char *nameGroup, int duplicator, char *latency, int noZoom, double renorm) {}	
    virtual void add_convolutions() {}
	virtual int  saveAllConvos(int force, ScreenSpike *Ecran) { if (force) reconstructConvo( force); return 0; }
 	virtual void modif_seuil(double raise_threshold, double lower_threshold) {}
};

#endif
