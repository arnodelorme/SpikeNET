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

// $Log: MapList.h,v $

#include "Map.h"
#include "MapBuffer.h"

#ifndef _MAPLIST
#define _MAPLIST

//class MapList;
// include global
#include <stdlib.h>
#include <string.h>
#include <math.h>

// include local
#include "allDefs.h"
#include "allStructs.h"
#include "Map.h"
#include "MapList.h"
#include "Estimer.h"

class SuperSpikeClass;
class SpikeLispSpike;
class GlobalImg;

// Classe gŽrant les diffŽrents SpikeNET (pour le multiscale)
// son utilitŽ est trs limitŽ avec SuperSpikeClass (uniquement les Estimateurs et les Maplist pour chaque echelle)

class MapList : public UniversalMap
{

class CarteChained
{
	public:
	 
	UniversalMap 	*map;		// current map
	CarteChained 	*suiv;		// previous CarteChained
	CarteChained 	*prec;		// next CarteChained
};

private:

  	// pour les variables d'environnement
  	SuperSpikeClass		*pappy;
  	int					scaleType;						// type de carte (multiscale ou pas
  	int					fact_zoom_SpikeNET;				// facteur de zoom du spikeNET courant (sert ˆ l'initialisation)
  	int					spikeNET_num;					// numero du spikeNET courant
   
  	//  variables essentielles	
  	int 				size_x, size_y;							// taille de base de l'image

public:
  	FILE          		*fileOutput;							// fichier de sortie contenant la configuration du reseau
  	carte_base			*tab_carte[MAX_CARTE];					// tableau des cartes (redondant avec celui de SuperSpikeClass
	MapList				*tab_mapList[MAX_SPIKENET];				// recursive computation
  	Estimer				*estimer;								// estimateur du nombre de dŽcharge dans chaque couche

private:

  	// apprentissage
  	int					layer_learn;							// couche d'apprentissage

  	// chaining the maps for multiprocessing computation
  	// -------------------------------------------------
	int nbCartes;							// number of maps to be treated
	int nbMapList;							// number of maps to be treated
    int temps;								// current time step
	CarteChained tabMapChained [MAX_CARTE]; // all the maps

    
public:
    
  	MapList(int largeur, int hauteur, int fact_zoom, int OffsetEcran, EnvVar *env0, int scaleType0, SuperSpikeClass	*pappy0);
  	MapList();
  
  	// a terme il faudrait supprimer ces methodes pour les replacer dans SuperSpikeClass
  	carte_base *findInside(char *name);
  	void SpikeClassInit ( );
  	void addMap( carte_base *ptr) { tab_carte[ nbCartes++ ] = ptr; }
  	void addMapList( MapList *ptr) { tab_mapList[ nbMapList++ ] = ptr; }

	// virtual functions
    virtual void init_neuron(GlobalImgTreat **listImg, int supervParam) {
  		estimer->reInit();
    	for (int i=0; i<nbMapList; i++)
            tab_mapList[i]->init_neuron(listImg, supervParam);
    	for (int j=0; j<nbCartes; j++)
            tab_carte[j]->init_neuron(listImg, supervParam);
    }

    virtual void 			detect_convolution(carte_base *une_carte_from, char *name_convolution, int convLock, double randFact, double multFact, double decFact, char *modFact, char *nameGroup, int duplicator, char *latency, int noZoom, double renorm)  { }
   	virtual void 			add_convolutions() 			 { }
   	virtual FORMAT_COMPUTE	seuil_mod() 				 { return 0; }
   	virtual double 			seuil_mod( double d)		 { return 0; }

    virtual void 			affiche()  { 
      	int i = 0;
  		if (!EnvVar::NO_SHOW) {  
    		while ( estimer->getNbSpike ( i++ ) ) {
  				printf(" : %d -> %d", i-1, estimer->getNbSpike ( i-1 ));
  				//fprintf(fileOutput, " : %d -> %d", i-1, estimer->getNbSpike ( i-1 ));
  			}
  			printf("\n");
  			//fprintf(fileOutput,"\n");
  		}
  	}

	void reChain() {
		//inverse first two maps at each time step to avoid that one is favored
		if (nbCartes > 1) {
 			UniversalMap *c = tabMapChained[ 0 ].map;
			tabMapChained[ 0 ].map = tabMapChained[ 1 ].map;
			tabMapChained[ 1 ].map = c;
		}
		
		for (int i=0; i<nbCartes+nbMapList; i++) {
			tabMapChained[i].suiv = tabMapChained + ((i+1) % (nbCartes+nbMapList));
			tabMapChained[(i+1) % (nbCartes+nbMapList)].prec = tabMapChained + i;
		}
	}
	
	virtual void resetComputeSync()
	{
		reChain();
		for (int i=0; i<nbCartes+nbMapList; i++)
            tabMapChained[i].map->resetComputeSync();
	}

	virtual void resetComputeSyncLearn()
	{ 
		reChain();
		for (int i=0; i<nbCartes+nbMapList; i++)
            tabMapChained[i].map->resetComputeSyncLearn();
	}
	
	virtual int compute_all()
	{
		CarteChained *ptrCarte = tabMapChained;
        int looping = 0;
        temps++;
        
		while (ptrCarte != NULL)
		{
			if (ptrCarte->map->compute_all())
				if ( ptrCarte->suiv == ptrCarte) ptrCarte = NULL;
				else 
				{ ptrCarte->prec->suiv = ptrCarte->suiv; ptrCarte->suiv->prec = ptrCarte->prec; ptrCarte = ptrCarte->suiv; } //ptrCarte->prec = ptrCarte->prec->prec; ptrCarte->prec->suiv = ptrCarte; }
			else 
			{
            	ptrCarte = ptrCarte->suiv; looping++;
                //if (looping % 100) printf("looping maps\n");   
            }
            //server->treatExternal();
		}
 	 	estimer->update();
 	 	return 1;
	}
	
	virtual int learn_all()
	{
		CarteChained *ptrCarte = tabMapChained;
        int looping = 0;
        temps++;
        
		while (ptrCarte != NULL)
		{
			if (ptrCarte->map->learn_all())
				if ( ptrCarte->suiv == ptrCarte) ptrCarte = NULL;
				else 
				{ ptrCarte->prec->suiv = ptrCarte->suiv; ptrCarte->suiv->prec = ptrCarte->prec; ptrCarte = ptrCarte->suiv; } //ptrCarte->prec = ptrCarte->prec->prec; ptrCarte->prec->suiv = ptrCarte; }
			else 
			{
            	ptrCarte = ptrCarte->suiv; looping++;
                //if (looping % 100) printf("looping maps\n");   
            }
            //server->treatExternal();
		}
 	 	return 1;
	}
	
    int getMapNb() { return nbCartes; }
};

#endif
