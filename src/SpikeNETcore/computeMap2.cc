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

// $Log: computeMap2.cc,v $

#define COMPUTE_MAP(X)																											\
{																																\
    int             j, k, l;																									\
    coord           *tmp_spike;																									\
    coord           *tmp_spike2;																								\
    WeightLine       *tmp_poids;																									\
    WeightLine       *tmp_poids_line;																							\
    int             tmp_size;																									\
    short			tmp_offset;																									\
    short  			*tmp_state;																									\
    short           tmp_zoom_afferent;																							\
    short           tmp_zoom_real;																								\
    INTEGERPOS    	tmp_pos;																									\
    INTEGERPOS    	tmp_neur;																									\
    unsigned char   tmp_conv_lock;																								\
    unsigned short  *tmp_tab_offset;																							\
    FORMAT_POIDS    *tmp_val;																									\
    FORMAT_COMPUTE  *tab_modulation_tmp;																						\
    Weights *tmp_tab_tab_poids;																							\
	coord_array		*output_list;																								\
	/*static toto = 0; printf("-%d-\n", toto++); */ 																		\
																																\
	int				mapMissingInGroup = 0;																						\
																																\
    output_list = spike_list.getNext();																							\
    tmp_spike2 = output_list->array;																							\
    tmp_size   = output_list->size;																								\
																																\
    /* application de toutes les convolutions*/																					\
    while((mapTreated < map_SpikeInputs.nb_aff) && !mapMissingInGroup)																\
    {																															\
        int groupTreated = map_SpikeInputs.group[mapTreated];																		\
	    tab_modulation_tmp = map_SpikeInputs.tab_modulation[groupTreated] + map_SpikeInputs.relevant_afferences[groupTreated];			\
																																\
																																\
        if (map_SpikeInputs.carte_aff[mapTreated]->spike_list.getSpikeList( 0 ) == NULL)												\
            mapMissingInGroup = 1;																								\
        																														\
        if (nbLooping++ > 1000000)																								\
            {																													\
                nbLooping = 1;																									\
                printf("hey unterminated computation from %s in map %s - computed:%d temps:%d\n", 								\
                       map_SpikeInputs.carte_aff[mapTreated]->get_name(), name, map_SpikeInputs.carte_aff[mapTreated]->temps, temps);		\
            }   																												\
																																\
   		if (!mapMissingInGroup)																									\
            while ((groupTreated == map_SpikeInputs.group[mapTreated]) && (mapTreated < map_SpikeInputs.nb_aff))							\
                {  					        																					\
                    tmp_spike = map_SpikeInputs.carte_aff[ mapTreated ]->spike_list.getSpikeList( 0 )->array;							\
                    																											\
                    tmp_state = state[ groupTreated ];																			\
                    tmp_zoom_afferent = map_SpikeInputs.zoom_afferent[ mapTreated ];													\
                    tmp_zoom_real     = (1 << tmp_zoom_afferent);																\
                    if (tmp_zoom_afferent < 0) tmp_zoom_real = 1;																\
																																\
                    tmp_tab_tab_poids = map_SpikeInputs.tab_tab_poids[ mapTreated ];													\
                    tmp_conv_lock     = tmp_tab_tab_poids->getConvLock();														\
                    /*printf("taille spike:%d\n", liste_in[ carte_from.array[i] ]->size);*/										\
                    																											\
                    for (int u=0; u< tmp_tab_tab_poids->getNbLatency(); u++)													\
                       for (j=0; j<map_SpikeInputs.carte_aff[mapTreated]->spike_list.getSpikeList( tmp_tab_tab_poids->getLatency( u ) )->size; j++)																\
                          { /* scanning map */																																								\
                            if (tmp_zoom_afferent >=0)																																						\
                                tmp_pos    = offset_debut + (tmp_spike->x >>  tmp_zoom_afferent) + (tmp_spike->y >>  tmp_zoom_afferent) * largeur_tot;														\
                            else tmp_pos    = offset_debut + (tmp_spike->x << -tmp_zoom_afferent) + (tmp_spike->y << -tmp_zoom_afferent) * largeur_tot;														\
                            tmp_offset = (tmp_spike->x % tmp_zoom_real) + ((tmp_spike->y % tmp_zoom_real) <<  tmp_zoom_afferent);																			\
                            /*if (tmp_tab_tab_poids->getDuplicator())																																			\
                                 tmp_poids = (WeightLine *)((char *)tmp_tab_tab_poids->getZoomOffset(tmp_offset, u) + tmp_pos * tmp_tab_tab_poids->getSizeTabPoidsLine());									\
                            else */tmp_poids = tmp_tab_tab_poids->getZoomOffset( tmp_offset, u);																												\
                            tmp_tab_offset = tmp_tab_tab_poids->getTabZoomOffset( tmp_offset);																												\
                            /*getchar();*/																																												\
                            																																												\
                            for (k = tmp_tab_tab_poids->getNbLines(tmp_offset); k>0 ; k--)																													\
                                {																																											\
                                    tmp_poids_line = (WeightLine *)((char *)tmp_poids + tmp_tab_offset[k-1]);																								\
                                    tmp_neur = tmp_pos + tmp_poids_line->offset;																															\
                                    tmp_val  = &(tmp_poids_line->firstPoids);																																\
                                    																																										\
                                    for (l=tmp_poids_line->taille; l>0; l--)																																\
                                        { /*scanning weigths */																																				\
                                            if (tmp_state[tmp_neur] > 0)																																	\
                                                {																																							\
/*DEBUG																																																		\
                                                    int x = tmp_neur % largeur_tot;																															\
                                                    int y = tmp_neur / largeur_tot;																															\
                                                    if (((x<border_l) || (x>=(border_l+largeur_norm))) ||																									\
                                                        ((y<border_h) || (y>=(border_h+hauteur_norm))))																										\
                                                        { printf("Error : map %s %dx%d coord %d-%d(%d)\n", name, largeur_tot, hauteur_tot, x, y, tmp_neur); exit(-1); }										\
                                                    int spike_x = tmp_neur % largeur_tot - largeur_tot + largeur_norm ;							\
                                                    int spike_y = tmp_neur / largeur_tot - hauteur_tot + hauteur_norm ;							\
                                                    if ((spike_x < 0) || (spike_y < 0) || (spike_x >= largeur_norm) || (spike_y >= hauteur_norm))															\
                                                        { } printf("Spike : map %s %dx%d pos %d-%d act %d zoom %d %d lat %d val %3.2f\n",																						\
                                                                 name, largeur_norm, hauteur_norm, spike_x, spike_y,  activite[tmp_neur], tmp_zoom_afferent, tmp_offset, u,*tmp_val); /*exit(-1); 												\
*/																																																			\
                                                    if ((X) > seuil)																			\
                                                        {																																					\
                                                            /* add the neuron only once */																													\
                                                            if (!(nb_decharge_from[tmp_neur]++))																											\
                                                                tab_modified_MMX[ tab_modified_MMX_size++ ] = tmp_neur;																						\
                                                            																																				\
/*DEBUG																																																		\
                                                            if ((tmp_spike2->x >= largeur_norm) || (tmp_spike2->y >= hauteur_norm))																			\
                                                                { printf("Spike Error : map %s %dx%d pos %d-%d(%d) act %d mod %d state %d \ncarte %s group %s convo %s\n",									\
                                                                         name, largeur_norm, hauteur_norm, tmp_spike2->x, tmp_spike2->y, tmp_neur, activite[tmp_neur], 										\
                                                                         *(tab_modulation_tmp - ++tmp_state[tmp_neur]), tmp_state[tmp_neur], map_SpikeInputs.carte_aff[mapTreated-1]->get_name()					\
                                                                         , map_SpikeInputs.name[groupTreated], map_SpikeInputs.tab_tab_poids[mapTreated-1]->getFileName()); exit(-1); }								\
                                                            if ((tmp_size+1) > MAX_SPIKE)																													\
                                                                ERROR(0,("MAX_SPIKE overflow in map %s\n", name));*/																						\
                                                        }																																					\
                                                }																																							\
                                            tmp_val  ++;																																					\
                                            tmp_neur ++;																																					\
                                        }																																									\
                                }																																											\
                            tmp_spike++;																																									\
                        }																																													\
                    mapTreated++;																																											\
                }																																															\
	}																																																		\
																																																			\
	if (!mapMissingInGroup)																																													\
	{																																																		\
		if (EnvVar::SPONTANEOUS)																																										\
			if ( (double)rand()/RAND_MAX < (double)EnvVar::TIME_BASE / EnvVar::MAX_TEMPS * EnvVar::SPONTANEOUS * largeur_norm * hauteur_norm )																\
			{ 																																												\
				tmp_neur = offset_debut + (int)(((double)rand()/RAND_MAX * largeur_norm)) + (int)(((double)rand()/RAND_MAX * hauteur_norm)) * largeur_tot;													\
        		if (!(nb_decharge_from[tmp_neur]++))																																						\
            		tab_modified_MMX[ tab_modified_MMX_size++ ] = tmp_neur;																																	\
            	activite[ tmp_neur ] = seuil+1;printf("Envar Spontaneous not nul\n"); exit(-1); \
			}																																																\
																																																			\
		/*check neurons for discharge*/																																										\
		for (int i=0; i<tab_modified_MMX_size; i++)																																							\
		{																																																	\
			if ( activite[ tmp_neur = tab_modified_MMX[ i ]  ] > seuil)																																		\
			{																																																\
		    	tmp_spike2->x = tab_x[tmp_neur];																																							\
		    	tmp_spike2->y = tab_y[tmp_neur];																																							\
 		    	tmp_spike2++;																																												\
		    	tmp_size++;					/*this figure is redundant */																																	\
 		    	for (l = map_SpikeInputs.nb_groups-1; l >= 0; l--) *(state[l]+tmp_neur) = 0; /*-map_SpikeInputs.relevant_afferences[l]; //considere le total*/															\
                                                    /*int spike_x = tmp_neur % largeur_tot - largeur_tot + largeur_norm ;							\
                                                    int spike_y = tmp_neur / largeur_tot - hauteur_tot + hauteur_norm ;							\
	    		printf("discharge : pos %d-%d act %d seuil %d\n", spike_x, spike_y, activite[tmp_neur], seuil); nb_decharge_from[tmp_neur] = 1;*/ /* 1 only if the neuron fired	*/																															\
	    		activite[tmp_neur] = temps;																																									\
			}																																																\
			else nb_decharge_from[tmp_neur] = 0;																																							\
		}																																																	\
																																																			\
		tab_modified_MMX_size = 0;																																											\
		output_list->size  = tmp_size;																																										\
    	ordre_decharge += tmp_size;																																											\
		if (tmp_size) modulateThreshold(tmp_size);													   																						\
		temps++;																																															\
																																																			\
		mapTreated = 0;																																														\
        nbLooping = 0;																																														\
        																																																	\
		return 1;																																															\
	}																																																		\
    else return 0;																																															\
																																																			\
};

// *************************************************************************************************
// *****************************************  slow version   ***************************************
// *************************************************************************************************
 
//void carte::compute_all()
/*{

    int             i, j, k, l;
    coord           *tmp_spike;
    WeightLine       *tmp_poids;
    WeightLine       *tmp_poids_line;
    int             tmp_size;
    short			tmp_offset;
    short           *tmp_state;
    short           tmp_zoom_afferent;
    short           tmp_zoom_real;
#if LARGE_MAP == OUI
    unsigned int    tmp_pos;
    unsigned int    tmp_neur;
#else
    unsigned short  tmp_pos;
    unsigned short  tmp_neur;
#endif
    unsigned char   tmp_conv_lock;
    unsigned short  *tmp_tab_offset;
    FORMAT_POIDS    *tmp_val;
    FORMAT_COMPUTE  current_modulation;
    FORMAT_COMPUTE  *tab_modulation_tmp; // lecture du tableau a l'envers
    FORMAT_COMPUTE  *tab_modulation_cumul_tmp;
    Weights *tmp_tab_tab_poids;
	coord_array		*output_list;

	// initialisation de la sortie
	if (current_list == &tab_spike1) output_list = &tab_spike2; else output_list = &tab_spike1;
	output_list->size = 0;

    // application de toutes les convolutions
    i=0;
    while(i<map_SpikeInputs.nb_aff)
    { 
        int group = map_SpikeInputs.group[i];
   		while ((group == map_SpikeInputs.group[i]) && (i < map_SpikeInputs.nb_aff))
  		{
  					        
        	// scanne les cartes
            tmp_spike = map_SpikeInputs.carte_aff[i]->current_list->array;
            tmp_state = state[ group ];
            tmp_zoom_afferent = map_SpikeInputs.zoom_afferent[i];
            tmp_zoom_real     = (1 << tmp_zoom_afferent);
            if (tmp_zoom_afferent < 0) tmp_zoom_real = 1;
	    	tmp_tab_tab_poids = map_SpikeInputs.tab_tab_poids[i];
	    	tmp_conv_lock     = tmp_tab_tab_poids->getConvLock();
            
            for (j=0; j<map_SpikeInputs.carte_aff[i]->current_list->size; j++)
                { //scanne les spikes de cette carte
		  			tmp_pos    = offset_debut + (tmp_spike->x >> tmp_zoom_afferent) + (tmp_spike->y >> tmp_zoom_afferent) * largeur_tot;
					tmp_offset = (tmp_spike->x % tmp_zoom_real) + ((tmp_spike->y % tmp_zoom_real) <<  tmp_zoom_afferent);
					
		  			//if (tmp_conv_lock)
		       		//tmp_poids = tmp_tab_tab_poids->tabPoidsLine[0];
		       		tmp_poids 		  = tmp_tab_tab_poids->getZoomOffset(tmp_offset);
	    			tmp_tab_offset    = tmp_tab_tab_poids->getTabZoomOffset(tmp_offset);
		       		//if (tmp_offset != 0) { ERROR(0,("tmp_offset different de 0")); }
		  			//else tmp_poids = (WeightLine *)((char *)tmp_tab_tab_poids->allTabPoidsLine + tmp_pos * tmp_tab_tab_poids->sizeTabPoidsLine);
		    
		  			for (k = tmp_tab_tab_poids->getNbLines(tmp_offset); k>0 ; k--)
		    		{
		      			tmp_poids_line = (WeightLine *)((char *)tmp_poids + tmp_tab_offset[k-1]);
		      			tmp_neur = tmp_pos + tmp_poids_line->offset;
		      			tmp_val  = &(tmp_poids_line->firstPoids);
		      
		      			for (l=tmp_poids_line->taille; l>0; l--)
						{ //scanne les poids de la convolution
			  				if (tmp_state[tmp_neur] > 0)
			    			{
		    					#if DEBUG == OUI
			      				int x = tmp_neur % largeur_tot;
			      				int y = tmp_neur / largeur_tot;
				  				if (((x<border_l) || (x>=(border_l+largeur_norm))) ||
	    							((y<border_h) || (y>=(border_h+hauteur_norm))))
			        			{ printf("Error : map %s %dx%d coord %d-%d(%d)\n", name, largeur_tot, hauteur_tot, x, y, tmp_neur); exit(-1); }
		    					int spike_x = (tmp_spike->x >> tmp_zoom_afferent);
		    					int spike_y = (tmp_spike->y >> tmp_zoom_afferent);
		    					if ((spike_x < 0) || (spike_y < 0) || (spike_x >= largeur_norm) || (spike_y >= hauteur_norm))
		    						{ printf("Spike Error : map %s %dx%d pos %d-%d(%d) zoom %d\n",
		    						name, largeur_norm, hauteur_norm, spike_x, spike_y, tmp_neur, tmp_zoom_afferent); exit(-1); }
								#endif
								
			      				act_tmp[tmp_neur] += *tmp_val;
			      				if (!(nb_decharge_from[tmp_neur]++))
									tab_modified_MMX[ tab_modified_MMX_size++ ] = tmp_neur;
			    			}
			  				tmp_val  ++;
			  				tmp_neur ++;
						}
		    		}
		  			tmp_spike++;
				}
			i++; //carte suivante
   	}
	    	
		#if DEBUG == OUI
		if (tab_modified_MMX_size > MAX_MODIFIED)
			ERROR(0,("MAX_MODIFIED overflow in map %s", name));
		#endif
	    	
	    // scanne tous les neurones ayant ete touches
	    tmp_size  = output_list->size;
	    tmp_spike = output_list->array + tmp_size;
	    tab_modulation_tmp = map_SpikeInputs.tab_modulation[group] + map_SpikeInputs.relevant_afferences[group];
	    tab_modulation_cumul_tmp = map_SpikeInputs.tab_modulation_cumul[group];
    
	    for (j=0; j<tab_modified_MMX_size; j++)
	    {
			tmp_neur = tab_modified_MMX[j];
		
			// modulation divise par deux 
			//current_modulation  = *(tab_modulation_tmp - tmp_state[tmp_neur] + (nb_decharge_from[tmp_neur] >> 1)); 
		    // modulation reele 
		    current_modulation  = (FORMAT_COMPUTE) ((int)*(tab_modulation_tmp - tmp_state[tmp_neur]) * tab_modulation_cumul_tmp[nb_decharge_from[tmp_neur] - 1]) >> RENORM;


			tmp_state[tmp_neur] -=  nb_decharge_from[tmp_neur];
			nb_decharge_from[tmp_neur] 	= 0;
		
			#if COMPUTE_CUT == OUI
				activite[tmp_neur] += (FORMAT_COMPUTE) ( (int)act_tmp[tmp_neur] * current_modulation );
			#else
				activite[tmp_neur] += (FORMAT_COMPUTE) ( (int)act_tmp[tmp_neur] * current_modulation >> RENORM );
			#endif
							
			#if DEBUG == OUI
 			if (((int)act_tmp[tmp_neur] * current_modulation >> RENORM) > ((1 << (8*sizeof(FORMAT_COMPUTE)-1))-1))
 				ERROR(0,("Arithmetic overflow in activity calculationin map %s", name));
			            
			if (abs(activite[tmp_neur]) > 5000000000)
				{ printf("erreur activite %s %d tmp %d mod %d pos %d state %d group %d\n", name, activite[tmp_neur], act_tmp[tmp_neur], 
					current_modulation, tmp_neur, tmp_state[tmp_neur], group); exit(-1); }
			#endif
			
			act_tmp[tmp_neur]			= 0;
		
			//if (activite[tmp_neur] > threshold[tmp_neur])
			if (activite[tmp_neur] > seuil)
		  	{
		    	tmp_spike->x = tab_x[tmp_neur];
		    	tmp_spike->y = tab_y[tmp_neur];
		    	#if DEBUG == OUI
		    	if ((tmp_spike->x < 0) || (tmp_spike->y < 0) || (tmp_spike->x >= largeur_norm) || (tmp_spike->y >= hauteur_norm))
		    		{ printf("Spike Error : map %s %dx%d pos %d-%d(%d) act %d mod %d state %d \ncarte %s group %s convo %s\n",
		    			name, largeur_norm, hauteur_norm, tmp_spike->x, tmp_spike->y, tmp_neur, activite[tmp_neur], 
					current_modulation, tmp_state[tmp_neur], map_SpikeInputs.carte_aff[i-1]->name, map_SpikeInputs.name[group], map_SpikeInputs.tab_tab_poids[i-1]->fileName); exit(-1); }
				if ((tmp_size+1) > MAX_SPIKE)
					ERROR(0,("MAX_SPIKE overflow in map %s", name));
				#endif
 		    	for (l=0; l < map_SpikeInputs.nb_groups; l++) state[l][tmp_neur] = -map_SpikeInputs.relevant_afferences[l]; //considere le total
		                                                     // + state[l][tmp_neur]; // signifie qu'il a decharge
 		    	tmp_spike++;
		    	tmp_size++;
	    		activite[tmp_neur] = 0;
		  	}
	     }

	    
	    tab_modified_MMX_size = 0;
	    output_list->size  = tmp_size;
    	ordre_decharge += tmp_size;
	}
	// swap spike_list ------------ care : parrallelisation problem -> need synchronisation
	current_list = output_list;
};*/
