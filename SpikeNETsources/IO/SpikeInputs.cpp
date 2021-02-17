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

// $Log: SpikeInputs.cpp,v $


#include "SpikeInputs.h"
#include "Map.h"
#include "Function.h"

void SpikeInputs::renormConvos(double val) {
	compute_somme();
	for (int j=0; j< nb_aff; j++)
		if (tab_tab_poids[j]->totalPoids() > 0)
			tab_tab_poids[j]->renormalizeLinesAbsolute( val/total_poids );  

/*		for (int j=0; j< nb_aff; j++)
		if (tab_tab_poids[j]->totalPoids() > 0)
			tab_tab_poids[j]->renormalizeLines( val );  */
}

void SpikeInputs::detect_convolution(carte_base *une_carte_from, char *name_convolution, short zoom_afferent0, int convLock
	 , double randFact, double multFact, double decFact, char *modFact, char *nameGroup, int *border_l0, int *border_h0, int duplicator, char *latency, int noZoom, double renorm)
{
  int maxLatency = 0;
  // initialisation des convolutions
  carte_aff  	  [ nb_aff ] = une_carte_from;
  zoom_afferent   [ nb_aff ] = zoom_afferent0;
  group		  [ nb_aff ] = detect_SpikeInputs(nameGroup);
  if (modFact != NULL)
  {
  	if (fact_modulation [ group[nb_aff] ] == NULL) fact_modulation [ group[nb_aff] ] = new char[100];
  	if ((atof(modFact)==0) && (strcmp(modFact, "0")))
  	{
  		strcpy(fact_modulation [ group[nb_aff] ], modFact+1);
  		fact_modulation [ group[nb_aff] ][ strlen(fact_modulation [ group[nb_aff] ]) -1] = 0;
  	}
  	else strcpy(fact_modulation [ group[nb_aff] ], modFact);
  }
  else fact_modulation [ group[nb_aff] ] = NULL;
  
  // pas de copie
  /*tab_tab_poids[nb_aff] = new Weights;
  tab_tab_poids[nb_aff]->detect_convo( border_l0, border_h0, name_convolution, convLock, randFact, multFact, decFact);
  */
  
  if (nb_aff)
  	if (convLock && !(strcmp(name_convolution, tab_tab_poids[nb_aff-1]->getFileName())) && (randFact == 0.0) 
  		&& (group[ nb_aff ] == group[ nb_aff-1 ]) && (zoom_afferent[ nb_aff ] == zoom_afferent[ nb_aff-1 ]))
  	{
  		tab_tab_poids[nb_aff] = tab_tab_poids[nb_aff-1];
  		*border_l0 = 0; // pas de border plus large pour une copie
  		*border_h0 = 0;
  	}
  	else
  	{
  		tab_tab_poids[nb_aff] = new Weights();
  		maxLatency = tab_tab_poids[nb_aff]->detect_convo( border_l0, border_h0, name_convolution, convLock, randFact, multFact, decFact, duplicator, latency, noZoom, renorm);
  	}
  else
  	{
  		tab_tab_poids[nb_aff] = new Weights();
  		maxLatency = tab_tab_poids[nb_aff]->detect_convo( border_l0, border_h0, name_convolution, convLock, randFact, multFact, decFact, duplicator, latency, noZoom, renorm);
  	}
  	
  une_carte_from->maxLatency( maxLatency );
  if (convLock == 0) convLockMap = 0;
  nb_aff++;
  if (nb_aff > MAX_CARTE_FROM)
	 ERROR(208,("maximum afferent maps overflow"));
};


void SpikeInputs::add_convolutions(int largeur_tot, int hauteur_tot)
{
	if (nb_aff == 0) ERROR( 116,("map with no connexion, check"));
	
  	for (int i=0; i < nb_aff; i++)
    {
      Weights *tmp_poids = tab_tab_poids[i];
      
      //initialise la convo que si ce n''est pas une copie
      if (i == 0) tmp_poids->init_convo( largeur_tot, hauteur_tot, zoom_afferent[i]);
      else if (tab_tab_poids[i] != tab_tab_poids[i-1])
      			tmp_poids->init_convo( largeur_tot, hauteur_tot, zoom_afferent[i]);
      		/*else // juste pour la beaute et que ce soit le meme random tout le temps
      			for (int j=0;j<tab_tab_poids[i]->hauteur;j++)
      				for (int ii=0;ii<tab_tab_poids[i]->largeur;ii++)
						rand();*/
	//tab_tab_poids[i]->affiche();
	//printf("\nconvo %d\n", i);
    }
	//tab_tab_poids[0]->affiche();

};

double SpikeInputs::compute_modulation()
{
  
  // reordone les convos si necessaire
  {for (int i=1; i < nb_aff; i++)
  	for (int j=i; j < nb_aff; j++)
  		if (group[j-1] > group[j])
  		{
  			int 			tmp 		= group[j]; 		group[j] 		= group[j-1]; 			group[j-1]			= tmp;
  			Weights *tmp_poids 	= tab_tab_poids[j]; tab_tab_poids[j]= tab_tab_poids[j-1]; 	tab_tab_poids[j-1]  = tmp_poids;
  			carte_base 		*carte_tmp  = carte_aff[j]; 	carte_aff[j] 	= carte_aff[j-1]; 		carte_aff[j-1] 		= carte_tmp;
  			short 			zoom_tmp 	= zoom_afferent[j]; zoom_afferent[j]= zoom_afferent[j-1];	zoom_afferent[j-1]	= zoom_tmp;
  		}}	
  
  // calcul le seuil et tout
  int i=0;
  total_poids = 0;
  FORMAT_COMPUTE seuil = 0;
  int grouptmp;
  while (i<nb_aff)
  	{
  		grouptmp = group[i];
  		int total_afferences = 0;
  		double somme_poids = 0;
  		while(grouptmp == group[i])
  		{
	  		somme_poids 			+= tab_tab_poids[ i ]->totalPoids();
      		total_afferences        += tab_tab_poids[ i ]->getNbPoids(); //* ( 1 << (2*zoom_afferent[i]));
      		i++;
      		if (i == nb_aff) break;
    	}
		if (zoom_afferent[i-1] < 0) somme_poids /= (1 << - zoom_afferent[i-1]) * (1 << - zoom_afferent[i-1]); // divide weight for negative zoom
    	total_poids += somme_poids;
  		add_SpikeInputs(total_afferences, grouptmp);
  	}
  
  //tab_tab_poids[ 0 ]->affiche( );
  //tab_tab_poids[ 0 ]->conv_order( NULL, 0.9 );
  return total_poids;
 };

void SpikeInputs::add_SpikeInputs(int total_afferences, int SpikeInputsNum)
{
  int i;
  relevant_afferences[SpikeInputsNum] = (int)((double)total_afferences * EnvVar::MOD_CUT);
  
  // construction d'un tableau de modulation
  tab_modulation[SpikeInputsNum]       = new FORMAT_COMPUTE[total_afferences+1];
  tab_modulation_cumul[SpikeInputsNum] = new FORMAT_COMPUTE[total_afferences+1];

  if (EnvVar::MOD_LINEAIRE == OUI) //  ATTENTION MODULATION LINEAIRE N'IMPORTE QUOI
    for (i=0; i< relevant_afferences[SpikeInputsNum]+1; i++)               
      tab_modulation[SpikeInputsNum][i] = (FORMAT_COMPUTE)(RENORM * (double)(relevant_afferences[SpikeInputsNum]+1-i)/(double)(relevant_afferences[SpikeInputsNum]+1));
  else
    {
      // geometric modulation with numbers
      if (fact_modulation[SpikeInputsNum] == NULL)
      	for (i=0; i< relevant_afferences[SpikeInputsNum]+1; i++)
		{  
	  		tab_modulation[SpikeInputsNum][i]       = (FORMAT_COMPUTE)(1 << RENORM);
	  		tab_modulation_cumul[SpikeInputsNum][i] = (FORMAT_COMPUTE)(1 << RENORM);
		}
      else if ((atof(fact_modulation[SpikeInputsNum]) != 0) || (!strcmp(fact_modulation[SpikeInputsNum], "0")))
      {
      	FORMAT_COMPUTE maxNorm = (FORMAT_COMPUTE)(1 << RENORM);
      	if (((atof(fact_modulation[SpikeInputsNum]) == 1) || (!strcmp(fact_modulation[SpikeInputsNum], "0"))))
      		for (i=relevant_afferences[SpikeInputsNum]/200; i>=0 ; i--)
			{  
	  			tab_modulation[SpikeInputsNum][i]       = maxNorm;
	  			tab_modulation_cumul[SpikeInputsNum][i] = (FORMAT_COMPUTE)(1 << RENORM);
			}
      	else
      	{
      		double cumul_modulation = 1;
     		double real_modulation;
      		double modTmp = atof(fact_modulation[SpikeInputsNum]);
      		//  ATTENTION AUX MODULATION RELATIVE OU ABSOLUE ********
      		if (EnvVar::MOD_TYPE == NON)
      		{
      			modTmp = exp( -log(2.0)/((double)total_afferences * modTmp )); // modulation relatives
      			delete fact_modulation[SpikeInputsNum];
      			fact_modulation[SpikeInputsNum] = new char[20];
      			sprintf( fact_modulation[SpikeInputsNum], "%1.3f", modTmp);
      		}    
      		for (i=0; i< relevant_afferences[SpikeInputsNum]+1; i++)
			{  
	  			real_modulation  = exp(i*log(modTmp));
	  			tab_modulation[SpikeInputsNum][i]       = (FORMAT_COMPUTE)rint((1 << RENORM) * real_modulation);
	  			cumul_modulation = (cumul_modulation * (double)i + real_modulation)/(double)(i+1); 
	  			tab_modulation_cumul[SpikeInputsNum][i] = (FORMAT_COMPUTE)rint((1 << RENORM) * cumul_modulation);
			}
		}
	  }
	  else // modulation with file
	  {
	  	 FILESTREAM f(fact_modulation[SpikeInputsNum]);
	  	 char tmp[100];
		 f >> tmp; 
		 double firstMod = atof(tmp);
		 tab_modulation[SpikeInputsNum][0] = (FORMAT_COMPUTE)(1 << RENORM);
      	 for (i=1; i< relevant_afferences[SpikeInputsNum]+1; i++)
		 { 
			f >> tmp; 
	  	 	tab_modulation[SpikeInputsNum][i]       = (FORMAT_COMPUTE)((double)(1 << RENORM)*(atof(tmp)/firstMod));
	  	 	// no cumul possible
	  	 }
 	  }
    }
  
  //for (int i=relevant_afferences[SpikeInputsNum]; i<(total_afferences+1); i++) tab_modulation[SpikeInputsNum][i] = 0;
	};

	int SpikeInputs::detect_SpikeInputs(char *name0)
	{
		int i=0;
		
		for (i=0; i<nb_groups; i++)
			if (!strcmp(name[i], name0)) break;
		if (i == nb_groups)
		{
			name[nb_groups] = new char[strlen(name0)+1];
			strcpy(name[nb_groups++], name0);
		}
		if (nb_groups > MAX_GROUP)
			ERROR( 209,("maximum SpikeInputs overflow"));
		
		return i;
	};

double SpikeInputs::compute_somme()
{
  int i;
  double somme_poids;
  total_poids = 0;
  for (i=0; i<nb_aff; i++)
    {
      somme_poids = tab_tab_poids[ i ]->totalPoids();
	  if (zoom_afferent[i] < 0) somme_poids /= (1 << - zoom_afferent[i]) * (1 << - zoom_afferent[i]); // divide weight for negative zoom
      if (somme_poids > 0) total_poids += somme_poids;
    }
  return total_poids;
};

double SpikeInputs::compute_somme( char *groupName )
{
  int groupNum = -1;
  for (int j=0; j < nb_groups; j++)
  	if (!strcmp( groupName, name[ j ])) groupNum = j;
  if (groupNum == -1) ERROR(213, ("maximum individual modulation overflow"));

  double somme_poids;
  total_poids = 0;
  for (int i=0; i<nb_aff; i++)
  	if (group[i] == groupNum) 
    {
      somme_poids = tab_tab_poids[ i ]->totalPoids();
	  if (zoom_afferent[i] < 0) somme_poids /= (1 << - zoom_afferent[i]) * (1 << - zoom_afferent[i]); // divide weight for negative zoom
      if (somme_poids > 0) total_poids += somme_poids;
    }
  return total_poids;
}

void SpikeInputs::affiche()
{
	for (int j=0; j< nb_aff; j++)
		if (!j)
	    	printf("%15s\t%c%c%c \t%10s\t%s\t%d\t%d\t%d\t%s\n", carte_aff[j]->get_name(), tab_tab_poids[j]->getConvLock() ? ' ' : 'U', 
	     	(tab_tab_poids[j]->getRandFact() != 0.0) ? 'R' : ' ', (tab_tab_poids[j]->getRenorm() != 0.0) ? 'N' : ' ',
	     	name[group[j]], fact_modulation[group[j]], tab_tab_poids[j]->getNbPoids(), tab_tab_poids[j]->getTotalLines(),tab_tab_poids[j]->getNbZooms(),
	     	 tab_tab_poids[j]->getFileName());
	    else
	    	printf("%15s\t%c%c%c%c\t%10s\t%s\t%d\t%d\t%d\t%s\n", carte_aff[j]->get_name(), tab_tab_poids[j]->getConvLock() ? ' ' : 'U', 
	     	(tab_tab_poids[j]->getRandFact() != 0.0) ? 'R' : ' ', (tab_tab_poids[j]->getRenorm() != 0.0) ? 'N' : ' ', (tab_tab_poids[j] == tab_tab_poids[j-1]) ? 'C' : ' ',
	     	name[group[j]], fact_modulation[group[j]], tab_tab_poids[j]->getNbPoids(), tab_tab_poids[j]->getTotalLines(),tab_tab_poids[j]->getNbZooms(),
	     	 tab_tab_poids[j]->getFileName());


}

TabWeight *SpikeInputs::computeTabWeight	(char *groupName, char *mapName, int sorting)
{
	int groupNum = -1;
  	for (int i=0; i < nb_groups; i++)
  		if (!strcmp( groupName, name[ i ])) groupNum = i;
  	if (groupNum == -1) ERROR(213, ("maximum individual modulation overflow"));
  	
    return computeTabWeight	( groupNum, mapName, sorting);
}

TabWeight *SpikeInputs::computeTabWeight	(int groupNum, char *mapName, int sorting)
{
    if (tabWeight[groupNum] == NULL)
    {
    	tabWeight[groupNum] = new TabWeight( ceil (relevant_afferences[groupNum] / EnvVar::MOD_CUT) );
    	for (int i=0; i < nb_aff; i++)
    		if (group[i] == groupNum)
			{
 	  			Weights *tmp_tab_tab_poids = tab_tab_poids[i];
	  			tmp_tab_tab_poids->saveConvos( mapName, carte_aff[i]->get_name(), !tmp_tab_tab_poids->getConvLock()); // to put weights in array
  	  			int largeurConvo = tmp_tab_tab_poids->getLargeur(); 
		  		for (int a =0; a < tmp_tab_tab_poids->getLargeur(); a++)
		  			for (int b=0; b < tmp_tab_tab_poids->getHauteur(); b++)
		  				if ( *tmp_tab_tab_poids->getImIntPtr(a+b*largeurConvo) != 0 )
							tabWeight[groupNum]->addPoids( i, a, b, tmp_tab_tab_poids->getImIntPtr(a+b*largeurConvo));
			}
  	}
  	else
    	for (int i=0; i < nb_aff; i++)
    		if (group[i] == groupNum)
	  			tab_tab_poids[i]->saveConvos( mapName, carte_aff[i]->get_name(), !tab_tab_poids[i]->getConvLock());
  
  if (sorting) tabWeight[groupNum]->sort();
  return tabWeight[groupNum];
}

double SpikeInputs::computeMean( int groupNum, double percentDech)
{
	double mean = 0;
	for (int i=0; i< (int)(relevant_afferences[groupNum] * percentDech); i++)
		mean += tab_modulation[groupNum][i];
	return mean / (int)(relevant_afferences[groupNum] * percentDech);
}

double SpikeInputs::computeVar( int groupNum, double percentDech)
{
	int nbRel = (int)(relevant_afferences[groupNum] * percentDech);
	double mean = computeMean( groupNum, percentDech);
	double var = 0;
	for (int i=0; i< nbRel; i++)
		var += tab_modulation[groupNum][i] * tab_modulation[groupNum][i];
	return (var - mean*mean*nbRel) / nbRel;
}

#include "RandomRank.h"

FORMAT_POIDS SpikeInputs::compute_max_activite( char *groupName, char *mapName, double percent)
{
	int groupNum = -1;
  	for (int i=0; i < nb_groups; i++)
  		if (!strcmp( groupName, name[ i ])) groupNum = i;
  	if (groupNum == -1) ERROR(213, ("maximum individual modulation overflow"));

	TabWeight *tabWeight = computeTabWeight	( groupNum, mapName, 0);
	
	FORMAT_POIDS maxAct = 0;
	FORMAT_POIDS minAct = 0;

	Function erfinv("erfinv");
	Function normInv("normInv");

	if (fact_modulation [ groupNum ] != NULL)
	{
		/*for (int i=0; i< relevant_afferences[groupNum]/2; i++)
		{
			maxAct += tabWeight->getItem( i )->getValue() * tab_modulation[groupNum][i];
			minAct += tabWeight->getItem( i )->getValue() * tab_modulation[groupNum][ relevant_afferences[groupNum] - 1 - i];
		}*/
		
		//tirages aleatoires
		/*FILE *f = fopen(":tmp", "w");
		randomRank rnd( relevant_afferences[groupNum] );
		for (int j=0; j<10000; j++)
		{
			 int posM = 0, posW = 0;
			 double sum =0;
			 rnd.reInit();
			 while( (posM = rnd.getRank()) != -1)
			 	sum += tabWeight->getItem( posW++ )->getValue() * tab_modulation[groupNum][posM];			 
			 fprintf( f, "%.0f\n", sum);
		}
		fclose( f);
		exit(-1);*/
		
		//printf( "mean %f\t%f\n", (minAct + maxAct)/2, computeMean( groupNum ) * tabWeight->getMean() * relevant_afferences[groupNum]);
		//return minAct + (maxAct-minAct)*percent;

		double mean2 = computeMean( groupNum, 1) * tabWeight->getMean() * relevant_afferences[groupNum];
		double var2  = computeVar ( groupNum, 1)  * tabWeight->getVar()  * (relevant_afferences[groupNum]-1);

		//printf("%.0f\t%.0f\t->%.0f\t%.0f\n", mean2, var2, mean2 + sqrt(2*var2)*erfinv.getValue( 2 * percent - 1), mean2 + sqrt(var2)*normInv.getValue( percent ));
		//printf("%f\t%f\t->%f     ou %f\t%f->\t%f\n", mean, var,  mean + sqrt(2*var)*erfinv.getValue( 2 * percent - 1), mean2, var2, mean2 + sqrt(2*var2)*erfinv.getValue( 2 * percent - 1));
		//return mean + sqrt(2 * var) * erfinv.getValue( 2 * percent - 1);
		return mean2 + sqrt(var2)*normInv.getValue( percent );
	}
	else
	{
		for (int i=0; i< relevant_afferences[groupNum]; i++)
			maxAct += tabWeight->getItem( i )->getValue() * (1 << RENORM);
		return maxAct * percent;
	}
}

	
	