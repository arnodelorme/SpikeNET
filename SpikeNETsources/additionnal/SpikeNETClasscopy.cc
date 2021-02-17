//variables d''environement
#include "SpikeNETClass.h"

// -----------------
// les constructeurs
// -----------------
SpikeClass::SpikeClass(int largeur, int hauteur, int fact_zoom, int OffsetEcran, EnvVar *env0, int scaleType0, SuperSpikeClass	*pappy0, disk *DISK0, screen *SCREEN0)
{
  scaleType = scaleType0;
  pappy     = pappy0;
  spikeNET_num = fact_zoom;
  if (scaleType == MULTISCALE)  fact_zoom_SpikeNET = fact_zoom;
  else							fact_zoom_SpikeNET = 0;
  
  size_x = largeur;
  size_y = hauteur;
  nb_carte = 0;
  
  SCREEN = SCREEN0;
  DISK	 = DISK0;
  /*if (EnvVar::SAVE_ON_DISK == OUI)
  		Ecran = new ecran_disk		(size_x, size_y, fact_zoom_SpikeNET, 1);
  else 	Ecran = new ecran_display	(size_x, size_y, fact_zoom_SpikeNET, BYTES_PER_PIXEL);*/

  //SpikeClassInit(fact_zoom, OffsetEcran);
};

/*SpikeClass::SpikeClass(char *ImageName0, int fact_zoom, int OffsetEcran, EnvVar *env0, int scaleType0, SuperSpikeClass	*pappy0)
{
  //env = new EnvVar();
  env = env0;
  scaleType = scaleType0;
  pappy     = pappy0;
  spikeNET_num = fact_zoom;
  if (scaleType == MULTISCALE)  fact_zoom_SpikeNET = fact_zoom;
  else							fact_zoom_SpikeNET = 0;
  
  //decharges dans la retine
  //retina = new retine( ImageName0, &size_x, &size_y, EnvVar::MAX_TEMPS, fact_zoom, "newConvos/retine3", EnvVar::RET_PC, EnvVar::MAX_NORM);
  //retina  		  = new retine  ( ImageName0, &size_x, &size_y, EnvVar::MAX_TEMPS, fact_zoom_SpikeNET, RETINA_CONV, EnvVar::RET_PC, EnvVar::MAX_NORM, EnvVar::RET_THRESHOLD);
  //retinaTransient = new retine_T( ImageName0, &size_x, &size_y, EnvVar::MAX_TEMPS, fact_zoom_SpikeNET, RETINA_CONV, EnvVar::RET_PC, EnvVar::MAX_NORM, EnvVar::RET_THRESHOLD_T);
  SpikeClassInit(fact_zoom, OffsetEcran);
};

SpikeClass::SpikeClass(int largeur, int hauteur, int fact_zoom, EnvVar *env0, int scaleType0, SuperSpikeClass	*pappy0)
{
  //env = new EnvVar();
  env = env0;
  scaleType = scaleType0;
  pappy     = pappy0;
  spikeNET_num = fact_zoom;
  if (scaleType == MULTISCALE)  fact_zoom_SpikeNET = fact_zoom;
  else							fact_zoom_SpikeNET = 0;
  
  size_x = largeur;
  size_y = hauteur;
  //retina 		  = new retine  ( largeur, hauteur, EnvVar::MAX_TEMPS, fact_zoom_SpikeNET, RETINA_CONV, EnvVar::RET_PC, EnvVar::MAX_NORM, EnvVar::RET_THRESHOLD);
  //retinaTransient = new retine_T( largeur, hauteur, EnvVar::MAX_TEMPS, fact_zoom_SpikeNET, RETINA_CONV, EnvVar::RET_PC, EnvVar::MAX_NORM, EnvVar::RET_THRESHOLD_T);
  
  SpikeClassInit(0, 0);
};*/

void SpikeClass::SpikeClassInit()
{
  // nb de carte non elues
  nonElues = 0;
  //if (!EnvVar::NO_SHOW)  save_config();

  // creation de l'ecran
  //Ecran = new ecran(size_x >> fact_zoom_SpikeNET, size_y >> fact_zoom_SpikeNET, fact_zoom_SpikeNET, OffsetEcran);
  //Ecran->affiche_image(ImageName0);

  if (!EnvVar::NO_SHOW) 
    {
      printf("Spikenet NetWork  %d    -----------------------\n", fact_zoom_SpikeNET);
      printf("lecture image et initialisation ok, taille %d %d\n",size_x,size_y);
    }    
  
  // lecture des noms des cartes et des seuils
  //nb_carte = read_maps();
  //if (!EnvVar::NO_SHOW) printf("initialisation des cartes ok\n");
 
  // initialisation des couches
  init_couche();
  
  // initialisation des fenetres
  //for (int i=0; i< nb_carte; i++)
  // Ecran->create_window( i, tab_couche[i], tab_carte[i]->get_largeur_norm(), tab_carte[i]->get_hauteur_norm());
  
  //construction des liste de spike IN et OUT
  //liste_in  = new ptr_coord_array[nb_carte];
  //liste_out = new ptr_coord_array[nb_carte];
  /*for (int i=0; i< nb_carte; i++)
  	if (!(tab_carte[i]->get_FIXED()))  
    	{
    		tab_carte[i]->fix_io( liste_in, liste_out );
      		//liste_in[i]  = &( ((carte *)tab_carte[i])->tab_spike1 );
      		//liste_out[i] = &( ((carte *)tab_carte[i])->tab_spike2 );
    	}*/
  
  //lecture des projections
  //read_projs();
  for (int i=0; i< nb_carte; i++) 
  	tab_carte[i]->add_convolutions();
  
  //read_projs(tab_carte, nb_carte, RESERV);
  if (!EnvVar::NO_SHOW) printf("initialisation des projection ok\n\n");
  
  // affichage debug et compute du seuil et tout
  for (int i=0; i<nb_carte;i++)
  	tab_carte[i]->seuil_mod();
  	
  if (!EnvVar::NO_SHOW)
    {
      printf("\n     CARTE  NUM SEUIL           MOD     X*Y CONV <-- PROJECTIONS\n");
      for (int i=0; i<nb_carte;i++)
	  	tab_carte[i]->affiche();
      printf("\n");
    }

/*#if SAVE_LEARN == OUI
  layer_learn = 0;

  for (int i=0; i < nb_carte; i++)
   	{
    	int largeur, hauteur, borderl, borderh;
    	tab_carte[i]->sizeLearn(largeur, hauteur, borderl, borderh);
    }

  for (int couch=1; couch < 3; couch++)
  {
    int first_to_learn=0;
  	for (int i=0; i < nb_carte; i++)
    	if (!(tab_carte[i]->get_FIXED()))
      		if (!(tab_carte[i]->get_convLockMap()) && (tab_carte[i]->get_couche() == couch))
      		{ first_to_learn = i; layer_learn = couch; break; }
    if (first_to_learn != 0)
    {
    	// pour bien faire il faudrait scanner toutes les cartes
    	int largeur, hauteur, borderl, borderh;
    	tab_carte[first_to_learn]->sizeLearn(largeur, hauteur, borderl, borderh);
      	int couche = tab_carte[first_to_learn]->get_couche();
  		if (SCREEN != NULL) SCREEN->init_learn( couche, tab_couche[couche], largeur, hauteur);
  		if (DISK   != NULL) DISK  ->init_learn( couche, tab_couche[couche], largeur, hauteur);
  	}
  }
#endif*/

  // init the network but with all maps
  // do not add to list if no reference found to this map

  // put first fixed maps
  for (int i=0; i < nb_carte; i++)
  	if (tab_carte[i]->get_FIXED() == 1) //1 = virtual map with 0 efferent in this computer
		mapList.addMapToList( tab_carte[i] );

  // then put virtual maps
  for (int i=0; i < nb_carte; i++)
  	if (tab_carte[i]->get_eff_virtual() > 1) //1 = virtual map with 0 efferent in this computer
		mapList.addMapToList( tab_carte[i] );

  // at last others maps
  for (int i=0; i < nb_carte; i++)
  	if ((tab_carte[i]->get_eff_virtual() == 0) && (tab_carte[i]->get_FIXED() != 1)) //1 = virtual map with 0 efferent in this computer
		mapList.addMapToList( tab_carte[i] );

  printf("Number of map : %d\n", mapList.getMapNb());

  // output file
  char fileOutputName[30];
  sprintf(fileOutputName, "%soutputFile%d", CURRENT_DIR, spikeNET_num);
  fileOutput = fopen(fileOutputName, "w");
    
};


void SpikeClass::actualiseSession( int modulator)
{
  //adjust threshold : unsupervised


  for (int i=0;i<nb_carte;i++)
  	if (! tab_carte[i]->get_convLockMap() && tab_carte[i]->ordre_decharge)
  			printf("raised  %f\n", tab_carte[i]->seuil_mod( 0));
  		
      	/*	printf("raised  %s : %f\n", tab_carte[i]->get_name(), tab_carte[i]->seuil_mod(EnvVar::RAISE_THRESHOLD));
  		else
     		printf("lowered %s : %f\n", tab_carte[i]->get_name(), tab_carte[i]->seuil_mod(EnvVar::LOWER_THRESHOLD));*/			
}

/*
void SpikeClass::actualiseSession( int modulator)
{
    // adjust threshold : supervised
	double newT;
	
	for (int i=0; i< (nb_carte); i++)
	{ 
		int res = 10 - tab_carte[i]->modulateThreshold(0);
		if (modulator && res)
		{
			if (res < 0)	newT = tab_carte[i]->seuil_mod( 0.02 / (double) modulator * (1 + (double)abs(res)/10));
			else			newT = tab_carte[i]->seuil_mod( -0.02 / (double) modulator * (1 + (double)abs(res)/10));
			
      		printf("%s : %f\n", tab_carte[i]->get_name(), newT);
      		fprintf(fileOutput, "%s : %f\n", tab_carte[i]->get_name(), newT );
	    }
	    else
		{
      		printf("%s : untouched\n", tab_carte[i]->get_name());
      		fprintf(fileOutput, "%s : untouched\n", tab_carte[i]->get_name());
	    }
	}
}*/

// ----------------------
// chargement de l'image
// -----------------------

void SpikeClass::LoadImage(GlobalImg **lstImg)
{
  //retina->loadImageContrast(ImageName, &(tab_carte[0]->spike_list),  &(tab_carte[1]->spike_list));
  /*if (scaleType != 0) // a voir 
  {
  	retina->loadImageContrast(ImageName1);
  	retinaTransient->substractImages(ImageName1, ImageName2, retina->result_ON,  retina->result_OFF);
  	retina->computeSpike();
  	tab_carte[0]->fix_spike_list( retina->get_spike_ON() );
  	tab_carte[1]->fix_spike_list( retina->get_spike_OFF());
  	tab_carte[2]->fix_spike_list( retinaTransient->get_spike_ON() );
  	tab_carte[3]->fix_spike_list( retinaTransient->get_spike_OFF());

  	#if SYSTEME == MAC
  	SCREEN->affiche_image(ImageName1, 5, 110);
  	SCREEN->affiche_image(ImageName2, 5, 300);
  	#endif
  }*/
  
  if (SCREEN != NULL) SCREEN->affiche_image(lstImg[0], 0, 0 ,0);
  //SCREEN->affiche_image(retina->ptr_image, size_x, size_y);
  for (int i=0; i< MAX_COUCHE; i++)	ordre_couche[i] = 0;
  for (int i=0; i< (nb_carte); i++) tab_carte[i]->init_neuron(lstImg, 0);
  for (int i=0; i< nb_carte; i++) 
  	{
  		if (SCREEN != NULL) SCREEN->init_windows(tab_carte[i]->get_number());
  		if (DISK   != NULL) DISK  ->init_windows(tab_carte[i]->get_number());
  	}
};

void SpikeClass::ResetImageLearn(GlobalImg **lstImg)
{
  //for (int i=0; i< MAX_COUCHE; i++)				ordre_couche[i] = 0;
  for (int i=0; i< nb_carte;   i++) 
    	tab_carte[i]->init_neuron( lstImg, 1);
};

void SpikeClass::Compute( int temps )
{
/*  liste_tmp = liste_in;
  liste_in  = liste_out;
  liste_out = liste_tmp;*/
  
  //cout << liste_in[0]->size << endl;  

  // computation des cartes
/*  if (!(temps % 20))
  	for (int l=0; l<20; l++)
  	{ 	
  		for (int i=0; i<2; i++)
    	{
    		tab_carte[i]->compute_all();
  			int   counter[MAX_COUCHE];
  			for (int k=0; k< MAX_COUCHE; k++) counter[k] = ordre_couche[k];
      		double estimer = tab_estim[tab_couche[i]];
      		for (int j=0; j < tab_carte[i]->current_list->size; j++)
	    		SCREEN->put_pixel(i, tab_carte[i]->current_list->array[j].x, tab_carte[i]->current_list->array[j].y, tab_carte[i]->get_zoom(),(int)(counter[tab_couche[i]] * estimer));
      		ordre_couche[tab_couche[i]] += tab_carte[i]->current_list->size;
    	}
  	}
  */
  	//for (int k=2;k<nb_carte;k++)
  	//	if (!tab_carte[k]->compute_all()) ERROR(0,("computation unterminated in map %s\n", tab_carte[k]->get_name()));

//  	for (int j=0;j<nb_carte;j++)
//  		if (!tab_carte[j]->compute_all()) ERROR(0,("computation unterminated in map %s\n", tab_carte[j]->get_name()));
  
  mapList.reInit();
  mapList.compute();
  
  //if (!(temps % 10)) printf("%d\n", temps);

  // affiche les listes de spike
  /*if ( temps == 0)
  {
  	for (int i=0; i<nb_carte; i++)
    {
      	for (int j=0; j < liste_in[i]->size; j++)
			printf("%d_%d\t", liste_in[i]->array[j].x, liste_in[i]->array[j].y);
      	printf("\n");
    }
 	printf("\n");
 	getchar();
  }*/

  //for (int i=0;i<nb_carte;i++)
  //	if (!(tab_carte[i]->FIXED))
  //		tab_carte[i]->teste_si_decharge( liste_out, temps);
/*
  static int firstToSpike;
  if (temps == 0) firstToSpike = 0;
  int inLoop = firstToSpike;
  static char res[100];

  for (int i=0; i<nb_carte; i++)
  	if ((tab_couche[i] == 2) && tab_carte[i]->current_list->size && !firstToSpike)
  	{
  		tab_carte[i]->modulateThreshold(1);
  		if (inLoop) strcpy(	res, tab_carte[ i ]->get_name() );
  		else  		strcat(	res, tab_carte[ i ]->get_name() );
		inLoop = 1;
	}
  firstToSpike = inLoop;

  if (temps == EnvVar::MAX_TEMPS+MAX_COUCHE-1)
  	{	
  		fprintf(fileOutput, "Map : %s\t", res);
  		printf("Map : %s\t", res);
  	}*/

  // affichage des spikes           
  int   counter[MAX_COUCHE];
  for (int i=0; i< MAX_COUCHE; i++) counter[i] = ordre_couche[i];
  for (int i=0; i<nb_carte; i++)
      if  (tab_carte[i]->get_virtual() == NULL)
          {
              double estimer = tab_estim[tab_couche[i]];
              coord_array *current_list = tab_carte[i]->spike_list.getSpikeList(0);
              int number				= tab_carte[i]->get_number();
              if (SCREEN != NULL)
              	  for (int j=0; j < current_list->size; j++)
                  	{
                  		SCREEN->put_pixel(number, current_list->array[j].x, current_list->array[j].y, tab_carte[i]->get_zoom(),255 - (int)(counter[tab_couche[i]] * estimer));
                  		//if ((current_list->array[j].y > 150) && number == 2) exit(-1);
                  	}
                  		
              if (DISK != NULL)
              	  for (int j=0; j < current_list->size; j++)
                  	DISK->put_pixel(number, current_list->array[j].x, current_list->array[j].y, tab_carte[i]->get_zoom(),255 - (int)(counter[tab_couche[i]] * estimer));              
              
              //SCREEN->put_pixel(i, liste_in[i]->array[j].x, liste_in[i]->array[j].y, tab_carte[i]->get_zoom(),(int)(counter[tab_couche[i]] * estimer));
              //if (liste_in[2]->size) printf("%d -> %d %d\n", temps, liste_in[2]->array[0].x, liste_in[2]->array[0].y);
              ordre_couche[tab_couche[i]] += current_list->size;
          }

};

void SpikeClass::Learn( int temps )
{
  
/*  liste_tmp = liste_in;
  liste_in  = liste_out;
  liste_out = liste_tmp;*/

  // if (temps == 0) exit(-1);
  // compute les premieres cartes

  mapList.reInit();
  mapList.learn();


  /*for (int i=0;i<nb_carte;i++)
  	if (tab_carte[i]->get_convLockMap())
    	tab_carte[i]->compute_all();
    else
      	tab_carte[i]->learn_all(); // si pas de decharges on propage qd meme*/


    	//if ((tab_carte[i]->ordre_decharge > 0))// && (tab_carte[i]->ordre_decharge < 10))
		//else if ((tab_carte[i]->ordre_decharge >= 10) &&  (temps == (EnvVar::MAX_TEMPS + 3)))
		//	 { printf("\tcarte %d NOLEARN -> %d spikes\n", i, tab_carte[i]->ordre_decharge); }
	
  // au dernier pas de temps on ajuste les seuils
  /*if (temps == (EnvVar::MAX_TEMPS + MAX_COUCHE - 1))
    {
      for (int i=0;i<nb_carte;i++)
   		if (!(tab_carte[i]->get_FIXED()))
   		#if ABS_CONV == NON
			tab_carte[i]->modif_seuil(EnvVar::RAISE_THRESHOLD, EnvVar::LOWER_THRESHOLD);
		#else
			tab_carte[i]->modif_seuil(0, 0);
		#endif
    }*/
    
  // détermine la carte qui a le plus de decharge (spikeNet 0)
  /*int num_carte = 0;
  for (int i=1;i<nb_carte;i++)
  	if (tab_carte[num_carte]->ordre_decharge && tab_carte[i]->ordre_decharge)
  		{ if (tab_carte[i]->ordre_decharge < tab_carte[num_carte]->ordre_decharge) num_carte = i; }
  	else if (tab_carte[i]->ordre_decharge) num_carte = i;	
  
  // computation des poids et des seuils
  if (tab_carte[num_carte]->ordre_decharge)
  {
  	if (tab_carte[num_carte]->MOD_SEPARE)	tab_carte[num_carte]->learn_sep( liste_in, liste_out);
	else									tab_carte[num_carte]->learn_com( liste_in, liste_out);
  	for (int i=1;i<nb_carte;i++)
  		if (i != num_carte)
  			if (tab_carte[i]->ordre_decharge)
  				if (tab_carte[i]->MOD_SEPARE)	tab_carte[i]->learn_sep( liste_in, liste_out);
				else							tab_carte[i]->learn_com( liste_in, liste_out);
	
	if (temps == (EnvVar::MAX_TEMPS + 2)) 
		{
  			for (int i=1;i<nb_carte;i++)
  				if (i != num_carte)
  					if (tab_carte[i]->ordre_decharge)
  						{
							tab_carte[i]->convSpeed ++;
							tab_carte[i]->compute_somme();
						}
		
			tab_carte[num_carte]->convSpeed ++;
			tab_carte[num_carte]->compute_somme();
		}
  	//if (temps == 0)
  	//	{ printf("carte elue : %s\n", tab_carte[num_carte]->name);
	//	tab_carte[num_carte]->tab_tab_poids[0].affiche();}
  }
  else if (temps == 0) nonElues++; //printf("aucunne carte elue\n");

  int repond_moyenne = 0;
  if (temps == (EnvVar::MAX_TEMPS + 2))
  	{	
  		if (tab_carte[num_carte]->ordre_decharge)
  			{	// si il y en a un qui a decharge on monte le seuil de tous ceux qui ont decharge
  				//printf(".\n");
  				for (int i=0;i<nb_carte;i++)
  					if (tab_carte[i]->ordre_decharge)
  						{
  							tab_carte[i]->modif_seuil(0.2);
  							repond_moyenne++;
  						}
  			}
  		else 
  			{   // si il y en a aucun qui decharge, je baisse le seuil de tous le monde
  				//printf("lowering all threshold\n");
  				for (int i=0;i<nb_carte;i++)
  					tab_carte[i]->modif_seuil(-0.001);
  			}
  		//printf("nb de reponses %d\n", repond_moyenne);
  	}*/

  
};

void SpikeClass::saveAllConvos(int count_image)
{
  //printf("nb d'images non Elues : %d\n\n", nonElues);
  //printf("taux de convergence des cartes\n");
  int change;
  for (int i=0; i<nb_carte; i++)
      change = tab_carte[i]->saveAllConvos( !((count_image - 1) % 10), SCREEN);
      //change = tab_carte[i]->saveAllConvos( 1, SCREEN);

  
  //change = tab_carte[nb_carte-1]->saveAllConvos(1, SCREEN);


    if ((EnvVar::LEARN) && !((count_image-1) % 5) )
    {
		layer_learn = 1;
  		char fileName[50];
  		PicturePPM pict(SCREEN->Base_save_convo[layer_learn], SCREEN->sizeX_learn_convo[layer_learn], SCREEN->sizeY_learn_convo[layer_learn]);
  		sprintf(fileName, "%s%s%clearn%d_", CURRENT_DIR, EnvVar::DIR_SAVE_LEARN, SEPARATEUR, layer_learn);
  		int cmpt = strlen(fileName);
  		if (count_image >=1000) fileName[cmpt++] = 48 + (count_image / 1000); else fileName[cmpt++] = 48;
  		if (count_image >=100 ) fileName[cmpt++] = 48 + ((count_image / 100) % 10); else fileName[cmpt++] = 48;
  		if (count_image >=10 ) fileName[cmpt++] = 48 + ((count_image / 10) % 10); else fileName[cmpt++] = 48;
  		fileName[cmpt++] = 48 + (count_image % 10);
  		fileName[cmpt] 	 = 0;
  		//printf("%d %d large %d %d hauteur_norm\n", SCREEN->sizeY_learn_convo, pict.Height, SCREEN->sizeX_learn_convo, pict.Width);
  		pict.write_ppm( fileName);
  		//SCREEN->affiche_image( new GlobalImg(strcat(fileName, ".ppm"), 0));
  	}
};

void  SpikeClass::modifMap( double value, int layer)
{
  double newT;
  for (int i=0; i< nb_carte; i++)
      if ( tab_carte[i]->get_couche() == layer )
      {
      		printf("%s : %f\n", tab_carte[i]->get_name(), newT = tab_carte[i]->seuil_mod( value));
      		fprintf(fileOutput, "%s : %f\n", tab_carte[i]->get_name(), newT );
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
	fprintf(fileOutput, "%d traitees\tspikes", count_image+1);		  
    while (ordre_couche[i++])
    {
  		printf(" : %d -> %d", i-1, ordre_couche[i-1]);
  		fprintf(fileOutput, " : %d -> %d", i-1, ordre_couche[i-1]);
  	}
  	printf("\n");
  	fprintf(fileOutput,"\n");
  }

  // at last others maps
  //for (i=0; i < nb_carte; i++)
  //	if (tab_carte[i]->get_eff_virtual() != 1) //1 = virtual map with 0 efferent in this computer
	//	printf( "map %s : %d\n", tab_carte[i]->get_name(), tab_carte[i]->ordre_decharge);
  
  /*if (!EnvVar::NO_SHOW)   
   printf("retine : %d spikes\n", ordre_couche[0]);
   for (int i=0; i< nb_carte; i++)
      printf("carte %d : temps %d\n", i, tab_carte[i]->ordre_decharge);*/
  
  if (DISK != NULL)
  {
  	DISK->create_dir(ImageName, EnvVar::RESULTS);
  	for (i=0; i< nb_carte; i++)
      if ( tab_carte[i]->get_virtual() == NULL )
          DISK->save(ImageName, tab_carte[i]->get_name(), fact_zoom_SpikeNET, i);
  }
  //for (long long u=0; u< 30000000; u++);
  //printf("quitting\n");		  
 
};
