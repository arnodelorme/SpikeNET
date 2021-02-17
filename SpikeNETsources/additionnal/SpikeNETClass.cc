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

// $Log: SpikeNETClass.cc,v $

//variables d''environement
#include "SpikeNETClass.h"
#include "MapResult.h"

//extern ScreenSpike *screen1;
//extern MapResult mapRes;

// -----------------
// les constructeurs
// -----------------
SpikeClass::SpikeClass(int largeur, int hauteur, int fact_zoom, int OffsetEcran, EnvVar *env0, int scaleType0, SuperSpikeClass	*pappy0)
{
  scaleType = scaleType0;
  pappy     = pappy0;
  spikeNET_num = fact_zoom;
  if (scaleType == MULTISCALE)  fact_zoom_SpikeNET = fact_zoom;
  else							fact_zoom_SpikeNET = 0;
  
  size_x = largeur;
  size_y = hauteur;
  nb_carte = 0;
  
  estimer = new Estimer();

};

carte_base * SpikeClass::findInside(char *name)
{
    carte_base *result = NULL;

    int i=0;
    while (i<nb_carte)
       if (strcmp(tab_carte[i++]->get_name(), name) == 0) result = tab_carte[i-1];
    
    return result;
}

void SpikeClass::SpikeClassInit(MapList *mapList)
{
  // nb de carte non elues
  //nonElues = 0;
  //if (!EnvVar::NO_SHOW)  save_config();

  if (!EnvVar::NO_SHOW) 
    {
      printf("Spikenet NetWork  %d    -----------------------\n", fact_zoom_SpikeNET);
      printf("lecture image et initialisation ok, taille %d %d\n",size_x,size_y);
    }    
  
   for (int i=0; i< nb_carte; i++)
   {
     	if (!EnvVar::NO_SHOW)
  		printf("initialising map %s\n", tab_carte[i]->get_name());

  		tab_carte[i]->add_convolutions();
  }
  //read_projs(tab_carte, nb_carte, RESERV);
  if (!EnvVar::NO_SHOW) printf("initialisation des projection ok\n\n");
  
  // affichage debug et compute du seuil et tout
  {for (int i=0; i<nb_carte;i++)
  	tab_carte[i]->seuil_mod();}
  	
  if (!EnvVar::NO_SHOW)
    {
      printf("\n     CARTE  NUM SEUIL           MOD     X*Y CONV <-- PROJECTIONS\n");
      for (int i=0; i<nb_carte;i++)
	  	tab_carte[i]->affiche();
      printf("\n");
    }

  // put first fixed maps
  {for (int i=0; i < nb_carte; i++)
  	if (tab_carte[i]->get_FIXED() == 1) //1 = virtual map with 0 efferent in this computer
		mapList->addMapToList( tab_carte[i] );}

  // then put virtual maps
  {for (int i=0; i < nb_carte; i++)
  	if (tab_carte[i]->get_eff_virtual() > 1) //1 = virtual map with 0 efferent in this computer
		mapList->addMapToList( tab_carte[i] );}

  // at last others maps
  {for (int i=0; i < nb_carte; i++)
  	if ((tab_carte[i]->get_eff_virtual() == 0) && (tab_carte[i]->get_FIXED() != 1)) //1 = virtual map with 0 efferent in this computer
		mapList->addMapToList( tab_carte[i] );}

  if (!EnvVar::NO_SHOW)
  	printf("Number of map : %d\n", mapList->getMapNb());

  // output file
  char fileOutputName[30];
  sprintf(fileOutputName, "%soutputFile%d", CURRENT_DIR, spikeNET_num);
  fileOutput = fopen(fileOutputName, "w");
    
};


void SpikeClass::actualiseSession( double modulator)
{
  //adjust threshold : unsupervised

  fprintf(fileOutput, "\t\t\t\t");
  for (int i=0;i<nb_carte;i++)
  	if ( (!tab_carte[i]->get_convLockMap()) )
  			//{ tab_carte[i]->get_name(), tab_carte[i]->seuil_mod_dicot( modulator ); }
  			fprintf(fileOutput, "%f\t", tab_carte[i]->seuil_mod_dicot( modulator ));
  fprintf(fileOutput, "\n");
  		
      	/*	printf("raised  %s : %f\n", tab_carte[i]->get_name(), tab_carte[i]->seuil_mod(EnvVar::RAISE_THRESHOLD));
  		else
     		printf("lowered %s : %f\n", tab_carte[i]->get_name(), tab_carte[i]->seuil_mod(EnvVar::LOWER_THRESHOLD));*/			
}

// ----------------------
// chargement de l'image
// -----------------------

void SpikeClass::LoadImage()
{
  estimer->reInit();
};

/*void SpikeClass::Compute( int temps )
{	
  mapList.reInit();
  mapList.compute();

  //if (temps < 20) printf("%d - %d %d\n", temps, tab_carte[0]->ordre_decharge + tab_carte[1]->ordre_decharge, tab_carte[2]->ordre_decharge);
  estimer->update();
}*/

void SpikeClass::Compute( int temps )
{
  if (EnvVar::CONVERGE)
  {
  	/*static int firstToSpike = 0;
  	static char res[100];

  	if ( !temps || !firstToSpike)
  	{
  		mapList.reInit();
 	 	mapList.compute();

  		//if (temps < 20) printf("%d - %d %d\n", temps, tab_carte[0]->ordre_decharge + tab_carte[1]->ordre_decharge, tab_carte[2]->ordre_decharge);
  		estimer->update();
  
  		if (temps == 0) { firstToSpike = 0; strcpy(	res, ""); }
  		int inLoop = firstToSpike;

  		for (int i=0; i<nb_carte; i++)
  			if ((tab_carte[i]->get_couche() == 2) && tab_carte[i]->ordre_decharge && !firstToSpike)
  			{
  				tab_carte[i]->modulateThreshold(1);
  				if (inLoop) strcpy(	res, tab_carte[ i ]->get_name() );
  				else  		strcat(	res, tab_carte[ i ]->get_name() );
				inLoop = 1;
			}
  		firstToSpike = inLoop;
  	}
    if (temps == EnvVar::MAX_TEMPS+MAX_COUCHE-1)
  	{	
  		//fprintf(fileOutput, "Map : %s\t", res);
  		if (!EnvVar::NO_SHOW) printf("Map : %s\t", res);
  	}*/
  }
  else
  {
 	 //mapList.reInit();
  	 //mapList.compute();
	 //static discharged =0;
	 //if (temps == 0) if (discharged == 0) { printf("0\n"); } else discharged =0;
	 //if ((tab_carte[nb_carte-1]->ordre_decharge) && !discharged) { printf("%d\n", temps); discharged = 1; }

 	 estimer->update();
  }
};

void SpikeClass::Learn( int temps )
{
  
//  mapList.reInitLearn();
//  mapList.learn();

};

void  SpikeClass::modifMap( double value, int layer)
{
  double newT;
  for (int i=0; i< nb_carte; i++)
      if ( tab_carte[i]->get_couche() == layer )
      {
      		printf("%s : %f\n", tab_carte[i]->get_name(), newT = tab_carte[i]->seuil_mod( value));
      		//fprintf(fileOutput, "%s : %f\n", tab_carte[i]->get_name(), newT );
	  }
}


void SpikeClass::SaveOnDisk(char *ImageName, int count_image)
{			        
  //static int nb = 0;
  //static int total = 0;
  // **********************
  // fin de la computation
  // **********************
  //total++;
  //if (!ordre_couche[1]) nb++;
  //printf("couche %d : %d spikes -> %d/%d\n", 2, ordre_couche[1], nb, total);

  // nb de decharge dans les couches
  //for (int i=0; i< 4; i++)
  int i = 0;
  
  //if (ordre_couche[1])
  //for (long long u=0; u< 300000000; u++);
 
  if (!EnvVar::NO_SHOW) 
  {
	printf("%d traitees\tspikes", count_image+1);		  
	//fprintf(fileOutput, "%d traitees\tspikes", count_image+1);		  
    while ( estimer->getNbSpike ( i++ ) )
    {
  		printf(" : %d -> %d", i-1, estimer->getNbSpike ( i-1 ));
  		//fprintf(fileOutput, " : %d -> %d", i-1, estimer->getNbSpike ( i-1 ));
  	}
  	//tab_carte[2]->affiche();
  	printf("\n");
  	//fprintf(fileOutput,"\n");
  }
  

  // at last others maps
  //for (i=0; i < nb_carte; i++)
  //	if (tab_carte[i]->get_eff_virtual() != 1) //1 = virtual map with 0 efferent in this computer
	//	printf( "map %s : %d\n", tab_carte[i]->get_name(), tab_carte[i]->ordre_decharge);
  
  /*if (!EnvVar::NO_SHOW)   
   printf("retine : %d spikes\n", ordre_couche[0]);
   for (int i=0; i< nb_carte; i++)
      printf("carte %d : temps %d\n", i, tab_carte[i]->ordre_decharge);*/
  
  /*if (DISK != NULL)
  {
  	DISK->create_dir(ImageName, EnvVar::RESULTS);
  	for (i=0; i< nb_carte; i++)
      if ( tab_carte[i]->get_virtual() == NULL )
          DISK->save(ImageName, tab_carte[i]->get_name(), fact_zoom_SpikeNET, i);
  }*/
  //for (long long u=0; u< 30000000; u++);
  //printf("quitting\n");		  
 
};
