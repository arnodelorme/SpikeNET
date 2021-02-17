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

// $Log: learning.cpp,v $

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.h"
#include "PicturePPM.h"
#include "TabWeight.h"

extern ScreenSpikeAuto *screenLearned;

void carte::sizeLearn(int &sizeX_rebuild, int &sizeY_rebuild, int &BORDER_IMGL, int &BORDER_IMGH)
{
  BORDER_IMGL = 0;
  BORDER_IMGH = 0;
  sizeX_rebuild = 0;
  sizeY_rebuild = 0;
 
  for (int i=0; i < map_SpikeInputs.nb_aff; i++)
    if (map_SpikeInputs.carte_aff[i]->get_couche() != get_couche()) 
 	{
 		if (map_SpikeInputs.carte_aff[i]->get_border_l() > BORDER_IMGL) BORDER_IMGL = map_SpikeInputs.carte_aff[i]->get_border_l();
 		if (map_SpikeInputs.carte_aff[i]->get_border_h() > BORDER_IMGH) BORDER_IMGH = map_SpikeInputs.carte_aff[i]->get_border_h();
 		int reallargeur = map_SpikeInputs.tab_tab_poids[ i ]->getLargeur() +  (map_SpikeInputs.carte_aff[ i ]->get_size_x_recog() >> 1)*2;
 		int realhauteur = map_SpikeInputs.tab_tab_poids[ i ]->getHauteur() +  (map_SpikeInputs.carte_aff[ i ]->get_size_y_recog() >> 1)*2;
 		if (reallargeur > sizeX_rebuild) sizeX_rebuild = reallargeur;
 		if (realhauteur > sizeY_rebuild) sizeY_rebuild = realhauteur;
  	}
}

void carte_learn_superv::init_neuron(GlobalImgTreat **listImg, int supervParam)
{
    //inactive = 1;
	carte::init_neuron( listImg, 0);

	convergingFactor++;
	
	// put all the neurons to an inactive state
  	for (int i=0;i<largeur_tot;i++)
    	for (int j=0;j<hauteur_tot;j++)
			for (int k=0; k< map_SpikeInputs.nb_groups; k++)
	  			state[k]		[i + j*largeur_tot] = INACTIF;
}

void carte_learn_superv::setLearnCoord( int x, int y)
{
    //inactive = 0;

	int xx, yy; 
    if (zoom >0)	{	xx = x >> zoom; yy = y >> zoom; }
	else			{	xx = x << -zoom; yy = y << -zoom; }

	for (int k=0; k< map_SpikeInputs.nb_groups; k++)
	  	state[k]		[xx + yy*largeur_tot + offset_debut] = map_SpikeInputs.relevant_afferences[k];
}

int carte_base::saveAllConvos(int force)
{
	double last_poids = map_SpikeInputs.total_poids;
	if ((last_poids != map_SpikeInputs.compute_somme()) || force) {
    	    	for (int i=0; i < map_SpikeInputs.nb_aff; i++)
   				if (force) if (! map_SpikeInputs.tab_tab_poids[ i ]->getConvLock())
					map_SpikeInputs.tab_tab_poids[ i ]->saveConvos( name, map_SpikeInputs.carte_aff[i]->get_name(), force); // reconvstruction des convolutions
   			return 1;
		}
	return 0;
};

void carte::reconstructConvo( int force)
{
  // on se base sur la premiere carte pour determiner les border
  int BORDER_IMGL = 0;
  int BORDER_IMGH = 0;
  int sizeX_rebuild = 0;
  int sizeY_rebuild = 0;
  
  sizeLearn( sizeX_rebuild, sizeY_rebuild, BORDER_IMGL, BORDER_IMGH);

  if (recog == NULL) {
   	recog = new GlobalImg( sizeX_rebuild, sizeY_rebuild, 1 );
  	if (EnvVar::LEARN) {
  		screenLearned->addBaseImg( recog );
   		screenLearned->addBaseImg( map_SpikeInputs.tab_tab_poids[ 0 ] );
  		screenLearned->addBaseImg( map_SpikeInputs.tab_tab_poids[ 1 ] );
	}	 		
  }

  for (int i=0; i < sizeX_rebuild; i++)
  	for (int j=0; j < sizeY_rebuild; j++)
  		recog->ImInt[ i + j*sizeX_rebuild ] = 0;

  TabWeight *tabWeight = map_SpikeInputs.computeTabWeight( 0, name, EnvVar::RECONSTRUCT_MOD);
  
  for (int posItem=0; posItem < tabWeight->getNbItems(); posItem++)
  {
  		int a = tabWeight->getItem( posItem )->getX();
  		int b = tabWeight->getItem( posItem )->getY();
  		int i = tabWeight->getItem( posItem )->getAfferent();
		FORMAT_POIDS modulate = 0.2;
		
		if (EnvVar::RECONSTRUCT_MOD == OUI)
  			modulate = map_SpikeInputs.spikeStat[0]->getValue( posItem ); 

	    if (modulate > 0.1)
	    {
	    	Weights *tmp_tab_tab_poids = map_SpikeInputs.tab_tab_poids[i];
  	    	int borderXconvo = sizeX_rebuild/2 - tmp_tab_tab_poids->getLargeur()/2 - map_SpikeInputs.carte_aff[i]->get_size_x_recog()/2;
  	    	int borderYconvo = sizeY_rebuild/2 - tmp_tab_tab_poids->getHauteur()/2 - map_SpikeInputs.carte_aff[i]->get_size_y_recog()/2;

	    	int posImg       = 0;
			for (int jj=0; jj< map_SpikeInputs.carte_aff[i]->get_size_y_recog(); jj++)
	       		for (int ii=0; ii< map_SpikeInputs.carte_aff[i]->get_size_x_recog(); ii++)
		   		{	
		      		int posRebuild = a + ii + borderXconvo + (b + jj + borderYconvo) * sizeX_rebuild;
			  		if (posRebuild < sizeX_rebuild * sizeY_rebuild)
//			  			recog->ImInt[posRebuild] += (*(map_SpikeInputs.carte_aff[i]->get_recog() + posImg++)-127) * ptr->getValue() * modulation;	
						if (EnvVar::RECONSTRUCT_MOD == NON)
			  				recog->ImInt[posRebuild] += (*(map_SpikeInputs.carte_aff[i]->get_recog() + posImg++)-127) * tabWeight->getItem( posItem )->getValue(); 
						else
			  				recog->ImInt[posRebuild] += (*(map_SpikeInputs.carte_aff[i]->get_recog() + posImg++)-127) * modulate;
			  		else 
						ERROR(303,("index out of bound exception in reconstruction %d", posRebuild))
		   		}
		}
  }
  
/*  for (int i=0; i < map_SpikeInputs.nb_aff; i++)
  {
    if ((map_SpikeInputs.carte_aff[i]->recog != NULL) && (map_SpikeInputs.carte_aff[i]->get_couche() != get_couche()))
	{
	  Weights *tmp_tab_tab_poids = map_SpikeInputs.tab_tab_poids[i];
	  tmp_tab_tab_poids->saveConvos( name, map_SpikeInputs.carte_aff[i]->get_name(), !tmp_tab_tab_poids->getConvLock());
	  
  	  int largeurConvo = tmp_tab_tab_poids->getLargeur();
  	  int borderXconvo = sizeX_rebuild/2 - tmp_tab_tab_poids->getLargeur()/2 - map_SpikeInputs.carte_aff[i]->get_size_x_recog()/2;
  	  int borderYconvo = sizeY_rebuild/2 - tmp_tab_tab_poids->getHauteur()/2 - map_SpikeInputs.carte_aff[i]->get_size_y_recog()/2;
	  
	  for (int a =0; a < tmp_tab_tab_poids->getLargeur(); a++)
	  	for (int b=0; b < tmp_tab_tab_poids->getHauteur(); b++)
	    {
	    	int posImg       = 0;
		  	for (int jj=0; jj< map_SpikeInputs.carte_aff[i]->get_size_y_recog(); jj++)
		      for (int ii=0; ii< map_SpikeInputs.carte_aff[i]->get_size_x_recog(); ii++)
		      {
		      	int posRebuild = a + ii + borderXconvo + (b + jj + borderYconvo) * sizeX_rebuild;
				if (posRebuild < sizeX_rebuild * sizeY_rebuild)
			  		recog->ImInt[posRebuild] += (*(map_SpikeInputs.carte_aff[i]->get_recog() + posImg++)-127) * (tmp_tab_tab_poids->getTabTemp(a+b*largeurConvo));	
				else 
					ERROR(303,("index out of bound exception in reconstruction %d", posRebuild))
		      }
		}
	}
  }*/

  // cherche le max dans le tableau
  recog->convert2char();

  //if (!force)
  //if (!convLockMap)
  disp->writeRebuilt( (GlobalImg *)recog );
  //disp->writeRebuilt( (GlobalImg *)map_SpikeInputs.tab_tab_poids[ 0 ], 28 );
  //disp->writeRebuilt( (GlobalImg *)map_SpikeInputs.tab_tab_poids[ 1 ], 52 );
}

/*void carte::modif_seuil(double raise_threshold, double lower_threshold)
{
    //seuil_relatif *= (1.0 + factor/(sqrt((double)convSpeed)));
    //seuil_relatif *= (1.0 + factor/((double)convSpeed));

	if (!convLockMap)
	{
	    if (ordre_decharge)
	    {
			seuil_relatif += ordre_decharge * raise_threshold;
			convSpeed ++;		//on ne modifie pas la convergence
	    }
	    else seuil_relatif -= lower_threshold;

  		double total_poids = map_SpikeInputs.compute_somme();
 	 	seuil = (FORMAT_COMPUTE) ((1 << RENORM) * seuil_relatif * total_poids);
 	}
}*/

/*void carte::teste_si_decharge(coord_array **liste_out, int temps)
{
	int newborderL = 0; //border_l - 1;
	int newborderH = 0; //border_h - 1;
	int abs, ord, tmp_neur, j;
	
     for (j=0; j<liste_out[ numero ]->size; j++)
        { //scanne les spikes de la carte courante
        	abs = liste_out[ numero ]->array[j].x + border_l;
        	ord = liste_out[ numero ]->array[j].y + border_h;
        	tmp_neur = abs + ord * largeur_tot;
 	  		
 	  		//if (!ordre_decharge) { ordre_decharge = temps; }
 	  		//else { for (int l=0; l < carte_from.size; l++) state[l][tmp_neur] = 0; } //inactivation
 	  		
 	  		// pour que le neurone soit centre
     		if (((abs<newborderL) || (abs>=(largeur_tot - newborderL))) ||
	  			((ord<newborderH) || (ord>=(hauteur_tot - newborderH))))
 	  			{ for (int l=0; l < map_SpikeInputs.nb_groups; l++) state[l][tmp_neur] = 0; }
 	  		else
 	  			if (!ordre_decharge) { ordre_decharge = temps; }
 	  			else { for (int l=0; l < map_SpikeInputs.nb_groups; l++) state[l][tmp_neur] = 0; }
 	  	}

}*/

/*
carte::~carte(){
  	delete tab_neuron_modified.array;
  	delete tab_spike1.array;
  	delete tab_spike2.array;
	for (int i=0; i<carte_from.size; i++)
		delete tab_tab_poids[ carte_from.array[i] ].array;
	delete tab_tab_poids;
	delete carte_from.array;
	delete tab_neuron;
};
*/


