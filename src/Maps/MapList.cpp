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

// $Log: MapList.cpp,v $

//variables d''environement
#include "MapList.h"
#include "MapResult.h"

// -----------------
// les constructeurs
// -----------------
MapList::MapList() {
  	scaleType = -1;
  	pappy     = NULL;
  	spikeNET_num = 0;
  	fact_zoom_SpikeNET = 0;
  
  	size_x = 0;
  	size_y = 0;
  	//nb_carte = 0;
  
  	estimer = new Estimer();

	// looping
	nbCartes = 0;
	nbMapList = 0;
	temps = 0;
}

MapList::MapList(int largeur, int hauteur, int fact_zoom, int OffsetEcran, EnvVar *env0, int scaleType0, SuperSpikeClass	*pappy0)
{
  	scaleType = scaleType0;
  	pappy     = pappy0;
  	spikeNET_num = fact_zoom;
  	if (scaleType == MULTISCALE)  fact_zoom_SpikeNET = fact_zoom;
  	else							fact_zoom_SpikeNET = 0;
  
  	size_x = largeur;
  	size_y = hauteur;
  	//nb_carte = 0;
  
  	estimer = new Estimer();

	// looping
	nbCartes = 0;
	nbMapList = 0;
	temps = 0;
};

// ---------
// fin a map
// ---------
carte_base * MapList::findInside(char *name) {
    carte_base *result = NULL;

    int i=0;
    while (i<nbCartes)
       if (strcmp(tab_carte[i++]->get_name(), name) == 0) result = tab_carte[i-1];
    
    if (result == NULL) {
    	i = 0;
    	while ((result == NULL) && (i < nbMapList)) { result = tab_mapList[i]->findInside( name); i++; }
	}
    return result;
}

// ----------------
// main initializer
// ----------------
void MapList::SpikeClassInit()
{

  if (!EnvVar::NO_SHOW) {
      printf("Spikenet NetWork  %d    -----------------------\n", fact_zoom_SpikeNET);
      printf("Reading image and neural model initialization ok, input size: %d %d\n",size_x,size_y);
  }    
  
  for (int i=0; i< nbCartes; i++) {
     	if (!EnvVar::NO_SHOW) printf("Initialising map %s\n", tab_carte[i]->get_name());
  		tab_carte[i]->add_convolutions();
  }
  if (!EnvVar::NO_SHOW) printf("Initialization of all map projections ok\n\n");
  
  // compute thresholds
  {for (int i=0; i<nbCartes;i++)
  	tab_carte[i]->seuil_mod();}
  	
  // display
  if ((!EnvVar::NO_SHOW) && nbCartes) {
      printf("\n     MAP   NUM THRESHOLD       MOD     X*Y CONV <-- PROJECTIONS\n");
      for (int i=0; i<nbCartes;i++)
	  	tab_carte[i]->affiche();
      printf("\n");
    }

  // remove virtual map with 0 efferent in this computer
  {for (int i=0; i < nbCartes; i++)
  	if  (tab_carte[i]->get_eff_virtual() == 1) {
  		delete tab_carte[i];
		for (int j=i; j < nbCartes; j++)
			tab_carte[j] = tab_carte[j+1];
		nbCartes--;
	}
  }
  
  
  // add all other the list to the loop
  {for (int i=0; i < nbMapList; i++)
		tabMapChained[ i ].map = tab_mapList[ i ];}
		
  // add all other the maps to the loop
  {for (int i=0; i < nbCartes; i++)
		tabMapChained[ i+nbMapList ].map = tab_carte[ i ];}

  if (!EnvVar::NO_SHOW)
  	printf("Number of maps : %d\n", getMapNb());

  // output file
  char fileOutputName[30];
  sprintf(fileOutputName, "%soutputFile%d", CURRENT_DIR, spikeNET_num);
  fileOutput = fopen(fileOutputName, "w");
    
};
