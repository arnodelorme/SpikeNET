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

// $Log: MapSimple.cc,v $

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.h"
#include "PicturePPM.h"
#include "computeMap.cc"

extern GlobalImg *originalImg;
extern ScreenSpikeAuto *screen2;

#define BORDER 5

carte::carte(int numero0, OutputDisplay *disp0, char *name0, int largeur_norm0, int hauteur_norm0, double seuil0, char *fact_modulation0, int zoom0, double decay0, int convSpeed0) :
		carte_base(numero0, disp0, name0, largeur_norm0, hauteur_norm0, zoom0)
{
    //printf("creating card %s number %d\n",name0, numero);
    border_l = 0;
    border_h = 0;
        
    seuil_relatif   = seuil0;
	if (fact_modulation0 == NULL) fact_modulation = NULL;
	else
	{
    	fact_modulation = new char[100];
    	strcpy(fact_modulation, fact_modulation0);
    	ordre_decharge  = 0;
    }

	// pour l'apprentissage
    convSpeed   = convSpeed0;
	convLockMap = 1; // verouille par defaux
	convLockConvo = EnvVar::CONVERGENCE_CONVO;
	    
    //variables d'environnement
	FIXED 		 = NON;
    
    // afferent map list initialisation
	map_SpikeInputs.nb_groups = 0;

    // arrays initialisation
    state[0]			= NULL;
    threshold           = NULL;
    nb_decharge_from	= NULL;	
    act_tmp				= NULL;
    activite			= NULL;
    update				= NULL;
    tab_x				= NULL;
    tab_y				= NULL; 
    tab_modified_MMX	= NULL;
    learning			= NULL;
   decay				= new FORMAT_COMPUTE[EnvVar::MAX_TEMPS + MAX_COUCHE];
    compute_decay( decay0, EnvVar::MAX_TEMPS);
    modulT = 0;
    
	if (EnvVar::LEARN)
	{
		rebuild = new Rebuild( 21, 21);
		screen2->addBaseImg( rebuild);
	}
};

void carte::affiche()
{
	printf("%10s %4d\t(%d)%dx%d(%d)\t%6.2f\t%6.2f\t%d\t%.0f\t%d\t%d\n", name, numero, border_l, largeur_norm, hauteur_norm, border_h,
		 		(double) seuil, realDecay, 1 << zoom, map_SpikeInputs.total_poids, latencyReserv, convSpeed);
	if (map_SpikeInputs.nb_aff > 20)
		printf("\t\t\t\t%d afferent maps - not shown\n",map_SpikeInputs.nb_aff);
	else map_SpikeInputs.affiche();
    
    //if (d_IP != NULL) d_IP->affiche(); else printf("\t\t\tno efferents\n");
	//printf("\n");
}

FORMAT_COMPUTE carte::getMaxAct()
{
	FORMAT_COMPUTE maxAct = 0;
	for (int j=0; j< hauteur_norm; j++)
		for (int i=0; i< largeur_norm; i++)
			if ( maxAct < activite[ offset_debut + j*largeur_tot + i ]) maxAct = activite[ offset_debut + j*largeur_tot + i ];
	return maxAct;
}
			
			
// *************************************************************************************************
// ******************************************* convolution  ****************************************
// *************************************************************************************************

void carte::detect_convolution(carte_base *une_carte_from, char *name_convolution, int convLock
			       , double randFact, double multFact, double decFact, char *modFact, char *nameGroup, int duplicator, char *latency, int noZoom, double renorm)
{
  short zoom_afferent0 = zoom - une_carte_from->get_zoom();
  int border_l0;
  int border_h0;

  // initialisation des convolutions
  char *modFact2 = modFact;
  if (modFact2[0] == 0) modFact2 = fact_modulation;
  map_SpikeInputs.detect_convolution(une_carte_from, name_convolution, zoom_afferent0, convLock
			       , randFact, multFact, decFact, modFact2, nameGroup, &border_l0, &border_h0, duplicator, latency, noZoom, renorm);

  if (convLock == 0) convLockMap = 0;

  // initialisation des border
  if (zoom_afferent0 <= 0)
  { // additives border for negatives zoom
  	border_l0 = (int)(( ceil ( (double)border_l0 ) +1)/2) - zoom_afferent0;
 	border_h0 = (int)(( ceil ( (double)border_h0 ) +1)/2) - zoom_afferent0;
  }
  else
  { // finer borders for positive zoom
  	border_l0 = (int)(( ceil ( (double)border_l0 / (1 << zoom_afferent0 )) +1)/2);
 	border_h0 = (int)(( ceil ( (double)border_h0 / (1 << zoom_afferent0 )) +1)/2);
  }
  
  if (border_l0 > border_l) border_l = border_l0;
  if (border_h0 > border_h) border_h = border_h0;
  
  largeur_tot 	= largeur_norm + 2*border_l;
  hauteur_tot 	= hauteur_norm + 2*border_h;
  offset_debut 	= border_h*largeur_tot + border_l;

#if LARGE_MAPS == NON
  if ((largeur_tot > 255) || (hauteur_tot > 255))
  	ERROR(204,("large map needed for map %s", name));
#endif

};

void carte::add_convolutions()
{
	map_SpikeInputs.add_convolutions( largeur_tot, hauteur_tot); 
	map_SpikeInputs.compute_modulation();
};

// *************************************************************************************************
// ******************* calcul du seuil, des afferences, de la modulation ***************************
// *************************************************************************************************
       	
FORMAT_COMPUTE carte::seuil_mod()
{
	double new_seuil;
	if (EnvVar::THRESHOLD_TYPE != 0)
	{
	 	new_seuil = map_SpikeInputs.compute_max_activite( "RET", name, seuil_relatif);
	 	//new_seuil += map_SpikeInputs.compute_max_activite( "EXIT", name, seuil_relatif);
	}
	else	new_seuil   = (seuil_relatif * map_SpikeInputs.compute_somme()) * (double)(1 << RENORM);
	
#if COMPUTE_CUT == NON
 		new_seuil   *= (1 << RENORM);
#endif
 	//if (new_seuil > ((1 << (8*sizeof(FORMAT_COMPUTE)-1))-1))
 	//	ERROR(301, ("arithmetic overflow in threshold calculationin map %s", name));

  	return seuil = (FORMAT_COMPUTE) rint(new_seuil);	
}

double carte::seuil_mod( double new_seuil_relatif )
{
		seuil_relatif += new_seuil_relatif;
		seuil_mod();
		return seuil_relatif;
/*double total_poids = map_SpikeInputs.compute_somme();
 	double new_seuil   = (seuil_relatif * total_poids) * (double)(1 << RENORM);
 	
#if COMPUTE_CUT == NON
 	new_seuil   *= (1 << RENORM);
#endif
	int toto = (1 << (8*sizeof(FORMAT_COMPUTE)-1))-1;
 	if (new_seuil > ((1 << (8*sizeof(FORMAT_COMPUTE)-1))-1))
 		ERROR(301, ("arithmetic overflow in threshold calculationin map %s", name));
  	seuil = (FORMAT_COMPUTE) rint(new_seuil);
  	
  	return seuil_relatif;*/
}

double carte::seuil_mod_dicot( double percent_seuil )
{
	float ideal_decharge = 5.0;
	double oldseuil = seuil;
	
	if (last_ordre_decharge < ideal_decharge)	seuil += (FORMAT_COMPUTE) ( percent_seuil* seuil * ( last_ordre_decharge - ideal_decharge )/ideal_decharge);
//	else								seuil += (FORMAT_COMPUTE) ( percent_seuil* seuil*( last_ordre_decharge/ideal_decharge));
	else								seuil += (FORMAT_COMPUTE) ( percent_seuil* seuil*log((last_ordre_decharge/ideal_decharge)*(last_ordre_decharge/ideal_decharge) + 1) );
	
	return seuil/oldseuil;

/*double total_poids = map_SpikeInputs.compute_somme();
 	double new_seuil   = (seuil_relatif * total_poids) * (double)(1 << RENORM);
 	
#if COMPUTE_CUT == NON
 	new_seuil   *= (1 << RENORM);
#endif
	int toto = (1 << (8*sizeof(FORMAT_COMPUTE)-1))-1;
 	if (new_seuil > ((1 << (8*sizeof(FORMAT_COMPUTE)-1))-1))
 		ERROR(301, ("arithmetic overflow in threshold calculationin map %s", name));
  	seuil = (FORMAT_COMPUTE) rint(new_seuil);
  	
  	return seuil_relatif;*/
}

void carte::compute_decay(double decay0, int max_time)
{
	if (decay0 != 0)
	{
		realDecay =  (double)max_time * decay0 / (double)EnvVar::TIME_BASE;
		
		// resting potential = 0
		for (int i=0; i<max_time+ MAX_COUCHE; i++)
			decay[i] = (FORMAT_COMPUTE)(exp(i*log(realDecay)) * (1 << RENORM));
	}
	else
	{
		realDecay = 0;
		for (int i=0; i<max_time+ MAX_COUCHE; i++)
			decay[i] = (FORMAT_COMPUTE)(1 << RENORM);
	}
}

// *************************************************************************************************
// ************************************* initialisation neurones ***********************************
// *************************************************************************************************

void carte::init_neuron(GlobalImg **listImg, int supervParam)
{
  int i,j,k;

  // pour la computation MMX
  int pos;
  
  //if (state[0] 	        == NULL) for (i=0; i< map_SpikeInputs.nb_groups; i++) state[i] = new short[largeur_tot * hauteur_tot]; 
  if (state[0] 	        == NULL) for (i=0; i< map_SpikeInputs.nb_groups; i++) state[i] = new short[largeur_tot * hauteur_tot]; 
  if (nb_decharge_from 	== NULL) nb_decharge_from	= new unsigned short[largeur_tot * hauteur_tot];	
  //if (threshold 		== NULL) threshold		= new FORMAT_COMPUTE[largeur_tot * hauteur_tot];
  //if (act_tmp 			== NULL) act_tmp		= new FORMAT_COMPUTE[largeur_tot * hauteur_tot];
  if (activite 			== NULL) activite		= new FORMAT_COMPUTE[largeur_tot * hauteur_tot];
  //if (update 			== NULL) update			= new unsigned short[largeur_tot * hauteur_tot];
#if LARGE_MAPS == OUI
  if (tab_x 			== NULL) tab_x				= new unsigned short[largeur_tot * hauteur_tot];
  if (tab_y 			== NULL) tab_y		        = new unsigned short[largeur_tot * hauteur_tot]; 
  if (tab_modified_MMX 	== NULL) tab_modified_MMX	= new unsigned int[largeur_tot * hauteur_tot]; 
#else
  if (tab_x 			== NULL) tab_x				= new unsigned char[largeur_tot * hauteur_tot];
  if (tab_y 			== NULL) tab_y		        = new unsigned char[largeur_tot * hauteur_tot]; 
  if (tab_modified_MMX 	== NULL) tab_modified_MMX	= new unsigned short[largeur_tot * hauteur_tot]; 
#endif

  if (!convLockMap && learning == NULL)
  	 {
  	 	learning    = new FORMAT_POIDS[2 * (EnvVar::MAX_TEMPS + MAX_COUCHE)];
  	 	learningPtr = learning + EnvVar::MAX_TEMPS + MAX_COUCHE;
  	 	
  	 	double tau  =  (double)EnvVar::MAX_TEMPS * 0.01 / (double)EnvVar::TIME_BASE; //tau in s
  	 	double orderModif = 8.10511;
  	 	
  	 	for (int i=0; i < 2 * (EnvVar::MAX_TEMPS + MAX_COUCHE); i++) {
  	 		double xaxis = (i- (EnvVar::MAX_TEMPS + MAX_COUCHE));
  	 		if (xaxis > 0)	learning[ i ] = (FORMAT_POIDS)( exp( - xaxis / tau / orderModif));	//initialy 20.0
  	 		else			learning[ i ] = (FORMAT_POIDS)( exp( xaxis / tau / orderModif));
  	 	}
  	 	// save
  	 	/*FILE *saveLearn = fopen( ":saveLearn", "w");
  	 	for (int ii=0; ii < 2 * (EnvVar::MAX_TEMPS + MAX_COUCHE); ii++)
  	 		fprintf(saveLearn,  "%4.3f\n", learning [ i ]);  	 	
  	 	fclose(saveLearn);*/
  	 }

  //if (tot_poids_afferent== NULL) tot_poids_afferent	= new FORMAT_COMPUTE[largeur_tot * hauteur_tot]; (convolutions dupliquees)
  tab_modified_MMX_size = 0;

    // initialisation des pseudos neurones
  for (i=0;i<largeur_tot;i++)
    for (j=0;j<hauteur_tot;j++)
#if BORDER_RETINA != 0
      if (((i<(border_l+BORDER)) || (i>=(largeur_tot - border_l - BORDER))) ||
	  		((j<(border_h+BORDER)) || (j>=(hauteur_tot - border_h - BORDER))))
#else
      if (((i<border_l) || (i>=(largeur_tot - border_l))) ||
	  		((j<border_h) || (j>=(hauteur_tot - border_h))))
#endif
				for (k=0; k< map_SpikeInputs.nb_groups; k++)
	  				state[k]		[i + j*largeur_tot] = INACTIF;
      else    
	  {
	  	// init de thresold et tot_poids_afferent plus loin
	  	pos = i + j*largeur_tot;
	  	if ((!supervParam) || convLockMap) {	
	  		activite			[pos] = 0;
	  		nb_decharge_from	[pos] = 0;
	    		for (k=0; k< map_SpikeInputs.nb_groups; k++)
	        			state[k]		[pos] = map_SpikeInputs.relevant_afferences[k];
	  	}
	  	else {
	  	  	if (!nb_decharge_from	[pos])
	  	  		for (k=0; k< map_SpikeInputs.nb_groups; k++)
	        				state[k][pos] = 0;
//	      else
//	    	for (k=0; k< map_SpikeInputs.nb_groups; k++)
//	        	state[k]		[pos] = map_SpikeInputs.relevant_afferences[k];
	      		else {
	    			for (k=0; k< map_SpikeInputs.nb_groups; k++)
	        				//state[k][pos] = -state[k][pos];
	        				state[k][pos] = 1;
				rebuild->addSpike( tab_x[pos], tab_y[pos], originalImg, 1.0);//ordre_decharge );
	        			//printf("neuron concerned %s %d %d\n", name, pos, nb_decharge_from	[pos]);
		  	}
	  	}	  	
	    //state[k]		[pos] = relevant_afferences_carte[k];
	  	//act_tmp		[pos] = 0;
	  	//update		[pos] = 0;
	  	tab_x			[pos] = i-border_l;
	  	tab_y			[pos] = j-border_h;
	  }

  // initialisation de thresold et de tot_poids_afferent
  /*for (i=0;i<largeur_tot;i++)
    for (j=0;j<hauteur_tot;j++)
      {
		threshold           [i+j*largeur_tot] = seuil;
		//tot_poids_afferent  [i+j*largeur_tot] = somme_poids;
      }*/
  
   last_ordre_decharge = ordre_decharge;
   ordre_decharge = 0;
   temps		  = 0;
   mapTreated = 0;
   nbLooping = 0;
   
   // spike list reinitialisation
   spike_list.reInit( latencyReserv + 1 );
   if (disp != NULL)  disp->clear( 0 );

#if SPIKESTAT == OUI
	if (map_SpikeInputs.spikeStat[0] == NULL)
    	map_SpikeInputs.spikeStat[0] = new SpikeStat( map_SpikeInputs.relevant_afferences[0]);
#else
	if (map_SpikeInputs.spikeStat[0] == NULL)
    	map_SpikeInputs.spikeStat[0] = new SpikeStat( map_SpikeInputs.relevant_afferences[0], "LUTLEARN");
#endif



}

void carte::resetComputeSync()  
{
    //change spike_list
    spike_list.nextSpikeList( YES );
    if (d_IP != NULL) d_IP->sendSpikes( spike_list.getSpikeList( 0 ), numero, temps);
    if (disp != NULL) disp->writeSpikes( spike_list.getSpikeList( 0 ) );
        
}

void carte::resetComputeSyncLearn()  
{
    //change spike_list
    spike_list.nextSpikeList( YES );
    if (d_IP != NULL) d_IP->sendSpikes( spike_list.getSpikeList( 0 ), numero, temps);
        
}         


// *************************************************************************************************
// **************************************  COMPUTATION LOOPS ***************************************
// *************************************************************************************************

/*int  carte::compute_all() 				COMPUTE_MAP( activite[tmp_neur] += FORMAT_COMPUTE(*tmp_val) << RENORM )
int  carte_modul::compute_all()			COMPUTE_MAP( activite[tmp_neur] += *(tab_modulation_tmp - tmp_state[tmp_neur]--) * *tmp_val )
int  carte_modul_decay::compute_all()	COMPUTE_MAP( activite[tmp_neur]  = (temps - update[tmp_neur]) ? (activite[tmp_neur] = ((activite[tmp_neur] * decay[temps - update[tmp_neur]]) >> RENORM) + *(tab_modulation_tmp - tmp_state[tmp_neur]--) * *tmp_val, update[tmp_neur] = temps, activite[tmp_neur]) : activite[tmp_neur] + *(tab_modulation_tmp - tmp_state[tmp_neur]--) * *tmp_val )
int  carte_decay::compute_all()			COMPUTE_MAP( activite[tmp_neur]  = (temps - update[tmp_neur]) ? (activite[tmp_neur] = ((activite[tmp_neur] * decay[temps - update[tmp_neur]]) >> RENORM) + *tmp_val, update[tmp_neur] = temps, activite[tmp_neur]) : activite[tmp_neur] + *tmp_val )
*/

int  carte::compute_all() 				COMPUTE_MAP( activite[tmp_neur] += FORMAT_COMPUTE(*tmp_val) << RENORM )

#if SPIKESTAT == OUI
/*********************** stat relative */
int  carte_modul::compute_all()			COMPUTE_MAP( (((groupTreated == 0) ? (map_SpikeInputs.spikeStat[0]->addValue( map_SpikeInputs.relevant_afferences[0] - tmp_state[tmp_neur], tmp_spike->val, temps),0) : 0), activite[tmp_neur] += *(tab_modulation_tmp - tmp_state[tmp_neur]--) * *tmp_val))
#else
/*********************** no stat */
int  carte_modul::compute_all()			COMPUTE_MAP( activite[tmp_neur] += *(tab_modulation_tmp - tmp_state[tmp_neur]--) * *tmp_val )
#endif

int  carte_modul_decay::compute_all()	COMPUTE_MAP( activite[tmp_neur] += *(tab_modulation_tmp - tmp_state[tmp_neur]--) * *tmp_val )
int  carte_decay::compute_all()			COMPUTE_MAP( activite[tmp_neur] += *(tab_modulation_tmp - tmp_state[tmp_neur]--) * *tmp_val )


int  carte_learn_superv::compute_all()  COMPUTE_MAP( (*tmp_val = *tmp_val + *(tab_modulation_tmp - tmp_state[tmp_neur]--), 0) )
int  carte::compute_pos()				COMPUTE_MAP( (nb_decharge_from[tmp_neur] ? ((*tmp_val > 0) ? (( activite[tmp_neur] += *(tab_modulation_tmp - tmp_state[tmp_neur]--) * *tmp_val), 0) : 0) : 0) )

#define LEARNING_MODELE 	10

int  carte::learn_all() 				
{
		if (convLockMap)
			return compute_all();
		else
#if LEARNING_MODELE == 0
/*********************** stop discharge after a spike */
		COMPUTE_MAP( (nb_decharge_from[tmp_neur] ? (exit(-1), 0) : 0 , 0) )
#endif

#if LEARNING_MODELE == 1
/*********************** dummy learning W += "1 or -1" */
		COMPUTE_MAP( (nb_decharge_from[tmp_neur] ? (*tmp_val += ((temps - activite[tmp_neur]) > 0) ? - 1 : + 1): 0 , 0) )
#endif
		
#if LEARNING_MODELE == 2
/*********************** order add : W += mod/alpha */
		COMPUTE_MAP( ((nb_decharge_from[tmp_neur] ? (*tmp_val = *tmp_val + (*(tab_modulation_tmp - tmp_state[tmp_neur]--)) / (FORMAT_POIDS)(last_ordre_decharge * convSpeed)) : 0), 0) )
#endif

#if LEARNING_MODELE == 3
/*********************** order mean : W = (alpha*W + mod)/( alpha+1 ) */	
		COMPUTE_MAP( ((nb_decharge_from[tmp_neur] ? (*tmp_val = (((convSpeed * last_ordre_decharge)* *tmp_val + *(tab_modulation_tmp - tmp_state[tmp_neur]--))/(last_ordre_decharge * convSpeed + 1))) : 0), 0) ) 
#endif

#if LEARNING_MODELE == 10
/*********************** markram pure add : W += ("0" or 1000") then renorm convolution*/
 		COMPUTE_MAP( (nb_decharge_from[tmp_neur] ? (*tmp_val += ((temps - activite[tmp_neur]) < 0) ? (1000)/convSpeed : 0): 0) )
//		COMPUTE_MAP( (nb_decharge_from[tmp_neur] ? (*tmp_val += ((temps - activite[tmp_neur]) < 0) ?  (printf("learn\n")) : (printf("bug\n"))): 0) )
 //		COMPUTE_MAP( (nb_decharge_from[tmp_neur] ? (seuil+1): 0) )
#endif

#if LEARNING_MODELE == 11
/*********************** markram add : W += (alpha*W + "0 or 1000")/( alpha+1 ) */
 		COMPUTE_MAP( (nb_decharge_from[tmp_neur] ? (*tmp_val = ((temps - activite[tmp_neur]) > 0) ? (convSpeed * last_ordre_decharge * *tmp_val+0)/(convSpeed * last_ordre_decharge+ 1) : (convSpeed * last_ordre_decharge * *tmp_val+1000)/(convSpeed * last_ordre_decharge+ 1)) : 0, 0) )
#endif

#if LEARNING_MODELE == 12
/*********************** markram mean : W += ("0 or 1000" - W)/(alpha*ndech) */
		COMPUTE_MAP( ((nb_decharge_from[tmp_neur] ? (*tmp_val += 
									((temps - activite[tmp_neur]) > 0) ? - *tmp_val / ((double)last_ordre_decharge*convSpeed) 
																					: (1000.0 - *tmp_val) / ((double)last_ordre_decharge*convSpeed)) : 0), 0) )
#endif

#if LEARNING_MODELE == 13
/*********************** markram mean : W += ("0 or 1000" - W)/alpha */  /* ********************** ********************** ********************** */
		COMPUTE_MAP( ((nb_decharge_from[tmp_neur] ? (*tmp_val += 
									((temps - activite[tmp_neur]) > 0) ? - *tmp_val / ((double)convSpeed) 
																					: (1000.0 - *tmp_val) / ((double)convSpeed)) : 0), 0) )
#endif

#if LEARNING_MODELE == 14
/*********************** markram mean2 (nothing when t=discharge) : W += ("0 or 1000" - W)/alpha */
		COMPUTE_MAP( ((nb_decharge_from[tmp_neur] ? (*tmp_val += 
											((temps - activite[tmp_neur]) != 0) ? (((temps - activite[tmp_neur]) >= 0) ? - *tmp_val / ((double)last_ordre_decharge*convSpeed) 
																					: (1000.0 - *tmp_val) / ((double)last_ordre_decharge*convSpeed)) : 0) : 0), 0) )
#endif

#if LEARNING_MODELE == 20
/*********************** markram realadd : W = W + learning (saturation between 0 and 1000) */	
		COMPUTE_MAP( ((nb_decharge_from[tmp_neur] ? ((*tmp_val = abs(*tmp_val + *(learningPtr + temps - activite[tmp_neur]))) > 1000) ? (*tmp_val = 1000) : 0 : 0), 0) )
#endif

#if LEARNING_MODELE == 21
/*********************** markram realmean : W = ( W*alpha - learning )/(alpha+1) */
		COMPUTE_MAP( ((nb_decharge_from[tmp_neur] ? (*tmp_val = (*tmp_val * last_ordre_decharge * convSpeed + *(learningPtr + (temps - activite[tmp_neur])))/(last_ordre_decharge*convSpeed + 1)) : 0), 0) )
#endif

#if LEARNING_MODELE == 22
/*********************** markram realconv : W += ( "0 or 1000" - W )/(alpha * learning) GOOD GOOD */
	  	COMPUTE_MAP( ((nb_decharge_from[tmp_neur] ? (*tmp_val += (((temps - activite[tmp_neur]) <= 0) ? (1000 - *tmp_val) / (last_ordre_decharge * convSpeed + *(learningPtr + temps - activite[tmp_neur])) : - *tmp_val / (last_ordre_decharge * convSpeed + *(learningPtr + temps - activite[tmp_neur])))) : 0), 0) )
#endif

#if LEARNING_MODELE == 23
/*********************** markram realconv 2 : W += ( "0 or 1000" - W )/alpha * learning */
		    COMPUTE_MAP( (nb_decharge_from[tmp_neur] ? (((temps - activite[tmp_neur]) == 0) ? ((*tmp_val =  *tmp_val + ( 1000.0- *tmp_val) / convSpeed * *(learningPtr)),tmp_state[tmp_neur] = 1) : 0),
		    								(*tmp_val = (((temps - activite[tmp_neur]) <= 0) ? 
		    									*tmp_val + (1000.0- *tmp_val) / convSpeed * *(learningPtr - tmp_state[tmp_neur]--) : 
		    									*tmp_val + (     0- *tmp_val) / convSpeed * *(learningPtr + tmp_state[tmp_neur]++))) : 0) )
#endif

#if LEARNING_MODELE == 24
/*********************** markram realconv 3 : W += ( "0 or 1000" - W )/(alpha*ndech) * learning */
		    COMPUTE_MAP( (nb_decharge_from[tmp_neur] ? (((temps - activite[tmp_neur]) == 0) ? ((*tmp_val =  *tmp_val + ( 1000.0- *tmp_val) / (convSpeed*last_ordre_decharge) * *(learningPtr)),tmp_state[tmp_neur] = 1) : 0),
		    								(*tmp_val = (((temps - activite[tmp_neur]) <= 0) ? 
		    									*tmp_val + (1000.0- *tmp_val) / (convSpeed*last_ordre_decharge) * *(learningPtr - tmp_state[tmp_neur]--) : 
		    									*tmp_val + (     0- *tmp_val) / (convSpeed*last_ordre_decharge) * *(learningPtr + tmp_state[tmp_neur]++))) : 0) )
#endif

#if LEARNING_MODELE == 25
/*********************** markram order lateral inhibition : warning bug within inhibtion because tmp_state reach 0 and then we cannot enter the test for inhibition */
		    COMPUTE_MAP( ( (nb_decharge_from[tmp_neur] ? (((temps - activite[tmp_neur]) == 0) ? (tmp_spike2->x = tab_x[tmp_neur], tmp_spike2->y = tab_y[tmp_neur], tmp_spike2++, tmp_size++) : 0) , 
		    							(*tmp_val = (((temps - activite[tmp_neur]) <= 0) ? 
		    									*tmp_val + (((groupTreated == 0) ? (1000.0- *tmp_val) : (250.0-*tmp_val)) ) / ( convSpeed) * *(learningPtr - tmp_state[tmp_neur]--) : 
		    									*tmp_val + (((groupTreated == 0) ? (0 - *tmp_val) : (*tmp_val-1.0))) / (convSpeed)  * *(learningPtr + tmp_state[tmp_neur]++))) : 0), 0) )
#endif
}
