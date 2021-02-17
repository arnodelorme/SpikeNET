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

// $Log: inputlast.cc,v $

#include <stdio.h>
#include <string.h>
#include "SpikeNETClass.h"
#include "superSpikeNETClass.h"
#include "error.h"
#include "imageTransform.h"
#include "mapBuffer.h"

#if MODE == INTERNAL
#include "net_names.h"
#include "net_projs.h"
#endif


// ***********************************
// determination du numero d'une carte
// ***********************************

SpikeLispSpike *SpikeClass::treatLisp( char *str, double spikePC)
{
	//writing the information
	char fileName[50];
	strcpy(fileName, CURRENT_DIR);
	strcat(fileName, "tmp_SpikeNET_file");
	FILE *tmpFile = fopen(fileName, "w");
	fprintf(tmpFile, "%s", str);
	fclose(tmpFile);
	
	//reading the information
	tmpFile = fopen( fileName, "r");
	SpikeLispSpike *spL = new SpikeLispSpike( spikePC, EnvVar::MAX_TEMPS, EnvVar::MAX_NORM);
	yyLisp::yyrun( tmpFile, spL);
    fclose(tmpFile);
	return spL;
}	


carte_base * SpikeClass::findInside(char *name)
{
    carte_base *result = NULL;

    int i=0;
    while (i<nb_carte)
       if (strcmp(tab_carte[i++]->get_name(), name) == 0) result = tab_carte[i-1];
    
    return result;
}

// **********************************
// genere un fichier de configuration
// **********************************

/*void superSpikeClass::save_config()
{
	//ofstream   map_config("net_res");
	int			eof1;
	
	// sauve les variables d'enrironnement vitales
	//map_config << "RAISE_THRESHOLD\t" 	<< EnvVar::RAISE_THRESHOLD << "\n";
	//map_config << "LOWER_THRESHOLD\t" 	<< EnvVar::LOWER_THRESHOLD << "\n";
	//map_config << "MAX_TEMPS\t" 		<< EnvVar::MAX_TEMPS 		<< "\n";
	//map_config << "RET_PC\t" 			<< EnvVar::RET_PC 			<< "\n";
	//map_config << "MAX_NORM\t" 			<< EnvVar::MAX_NORM 		<< "\n";
	//map_config << "MOD_LINEAIRE\t" 		<< EnvVar::MOD_LINEAIRE 	<< "\n";
	//map_config << "MOD_CUT\t" 			<< EnvVar::MOD_CUT 		<< "\n";
	
	// recopi le net_env
	printf("ENVIRONNEMENT FILE\n");
	printf("------------------\n");
    FILESTREAM  envir(NET_ENV);	
    while ((eof1 = envir.get()) !=EOF)
		printf("%c",eof1);

	// recopi le net_names
	printf("\n\nMAP FILE\n");
	printf("--------\n");
    FILESTREAM  map_names(NET_NAMES);	
    while ((eof1 = map_names.get()) !=EOF)
		printf("%c",eof1);

	// recopi le net_proj
	printf("\n\nPROJECTION FILE\n");
	printf("---------------\n");
    FILESTREAM  map_projs(NET_PROJS);	
    while ((eof1 = map_projs.get()) !=EOF)
		printf("%c",eof1);

	printf("\n\nDEF FILE\n");
	printf("----------\n");
    FILESTREAM  def("all_def.h");	
    while ((eof1 = def.get()) !=EOF)
		printf("%c",eof1);
    
    // resultats
	printf("\n\nRESULTATS\n");
	printf("----------\n");
    
    //map_config.close();
}*/

// ******************
// lecture des cartes
// ******************

int superSpikeClass::readMulti(char *var, char *tmp, FILESTREAM &map_names)
{
	int 		 cmpt = 0;
	map_names >> (var+cmpt*30);
								
	//multiscaled and non nul threshold
	do
	{ 
		cmpt++;
		map_names >> (var+cmpt*30);
	} while( (atof(var+cmpt*30) != 0) && (var[cmpt*30]!=0) && (map_names.eof() != EOF));
			
	if ((cmpt != 1) && (cmpt != numberSpikeNET - (ONESCALE & networkType)))
		ERROR(104,("in %s file, there must be 1 or NB_SPIKENET arguments", NET_NAMES));
		
	if (map_names.eof() != EOF) strcpy(tmp, var+cmpt*30); else strcpy(tmp, "");	
	var[cmpt*30] = 0;

	// is there multi arguments
	return (cmpt != 1);
}

carte_base	*SpikeClass:: addMap(int couche, char *name, int mapNumber, double seuil, double mod, double estimer, int zoomer, double decay, int coordx, 
		int coordy, int zoomDsp, char *learn, proc *local, char *strLisp, char *object, int convSpeed)
{
	if (   EnvVar::DISPLAY  == OUI )
		if (coordx != -1) 			 SCREEN->create_window(mapNumber, zoomer, coordx, coordy, fact_zoom_SpikeNET, zoomDsp);
	if (EnvVar::SAVE_ON_DISK == OUI) DISK  ->create_window(mapNumber, zoomer, coordx, coordy, fact_zoom_SpikeNET, zoomDsp);

	if ((object[0] != 0) && (SCREEN != NULL)) SCREEN->affiche_image( new GlobalImg( object, 0), coordx-25, coordy+25 , 2 );
		
	if ((local != NULL))// || (nb_carte++ == 2) || (nb_carte++ == 3))
	{
        //proc *p = new proc( "si8000" );
		return tab_carte[nb_carte++] = new carte_virtual(mapNumber, couche, name, size_x, size_y, estimer, zoomer+fact_zoom_SpikeNET, local);
	}
	else if (couche == 0)
	  {
		if (strLisp[0] == 0) ERROR(105,("no input for map %s", name));
		if ((learn[0] != 0) && (!strcmp(learn, "no")))	 ERROR(106,("no learning possible for input map %s", name));
		SpikeLispSpike *spL =  treatLisp(strLisp, seuil);
		return tab_carte[nb_carte++]  = new carte_fixed(mapNumber, couche, name, size_x, size_y, estimer, zoomer+fact_zoom_SpikeNET, spL);
	  }
	  else
	  {	
		if (strLisp[0] != 0) ERROR(107,("input only possible in layer 0, thus not for map %s", name));
		if (seuil == 0)  	 ERROR(108,("no threshold for %s map\n", name));
		if ((learn[0] == 's') || (learn[0] == 'S'))
	    	return tab_carte[nb_carte++]  = new carte_learn_superv( mapNumber, couche, name, size_x, size_y, seuil , mod, estimer, zoomer+fact_zoom_SpikeNET, decay, convSpeed);
		else if (mod == 0)
				if (decay == 0)
	    				return tab_carte[nb_carte++]  = new carte( mapNumber, couche, name, size_x, size_y, seuil , mod, estimer, zoomer+fact_zoom_SpikeNET, decay, convSpeed);
				else	return tab_carte[nb_carte++]  = new carte_decay( mapNumber, couche, name, size_x, size_y, seuil , mod, estimer, zoomer+fact_zoom_SpikeNET, decay, convSpeed);
			else 
				if (decay == 0)
	    				return tab_carte[nb_carte++]  = new carte_modul( mapNumber, couche, name, size_x, size_y, seuil , mod, estimer, zoomer+fact_zoom_SpikeNET, decay, convSpeed);
				else	return tab_carte[nb_carte++]  = new carte_modul_decay( mapNumber, couche, name, size_x, size_y, seuil , mod, estimer, zoomer+fact_zoom_SpikeNET, decay, convSpeed);
	    }
}

void superSpikeClass::read_maps()
{

   char new_net_names[100];
   sprintf(  new_net_names, "%s%s%c%s", CURRENT_DIR, EnvVar::MAIN_DIRECTORY, SEPARATEUR, NET_NAMES);
   FILESTREAM   map_names(new_net_names);
    
    //char nb_carte_strng     [30];
    char name     			[30];
    char tmp     			[30];
    char zoom	   			[30];
    char object	   			[100];
    char couche   			[30];
    char coordx   			[30];
    char coordy   			[30];
    char zoomDsp   			[30];
    char conv        		[30];
    char bewulf   			[MAX_SPIKENET*30];
    char seuil	   			[MAX_SPIKENET*30];
    char mod  	   			[MAX_SPIKENET*30];
    char estim    			[MAX_SPIKENET*30];
    char decay   			[MAX_SPIKENET*30];
    char learn   			[MAX_SPIKENET*30];
    char strLisp			[200];
    SpikeLispSpike			*spL[MAX_SPIKENET];
    int  eof1	,not_end_param, nb_carte=0;

	// factor of multiplication for arguments number in multiscaled maps
	int  decf;
	int  thrf;
	int  estf;
	int  modf;
	int  lrnf;
	int  bewf;
	
	nbMaps = 0;
	
    	//map_names >> nb_carte_strng;
	    map_names >> tmp;
    	do
      	{
			// initialisation
	  		name[0]   = 0;
	  		couche[0] = 0;
	  		zoom[0]   = 0;
	  		object[0] = 0;
	  		conv[0]   = 0;
	  		
	  		seuil[0]  = 0;	thrf = 0;
	  		mod[0]	  = 0;  modf = 0;
	  		estim[0]  = 0;  estf = 0;
	  		decay[0]  = 0;  decf = 0;
	  		learn[0]  = 0;  lrnf = 0;
	  		bewulf[0] = 0;  bewf = 0;
	  		
	  		strLisp[0]  = 0;
	  		coordx[0]	= 0;
	  		coordy[0]	= 0;
	  		for (int i=0; i< numberSpikeNET; i++)	spL[i] = NULL;
	  		char c;
	  		
	  		not_end_param = OUI;
			while (not_end_param && (tmp[0] != 0))
			{
				while (tmp[0] == '#')
				{ 
					while (
						((c = map_names.get()) != '\n')  
						&& (c!=EOF)) { }; 
					map_names >> tmp; 
				} // commentaires
				
				switch (tmp[0])
				{
					case 'I' :
					case 'i' : { not_end_param = NON; map_names.getline(strLisp, 200, '\n'); map_names >> tmp; break; }
					case 'L' :
					case 'l' : { if (couche[0] != 0) not_end_param = NON;  else { map_names >> couche;  map_names >> tmp; } break; }
					case 'N' :
					case 'n' : { if (name[0] != 0) not_end_param = NON; else { map_names >> name;  map_names >> tmp; } break; }
					case 'P' :
					case 'p' : { if (coordx[0] != 0) not_end_param = NON; else { map_names >> coordx;  map_names >> coordy; map_names >> zoomDsp; map_names >> tmp; } break; }
					case 'Z' :
					case 'z' : { if (zoom[0] != 0) not_end_param = NON;  else { map_names >> zoom;  map_names >> tmp; } break; }
					case 'O' :
					case 'o' : { if (object[0] != 0) not_end_param = NON; else { map_names >> object;  map_names >> tmp; } break; }					
					case 'B' :
					case 'b' : { if (bewulf[0] != 0) not_end_param = NON;  else bewf = readMulti( bewulf, tmp, map_names); break; }
					case 'D' :
					case 'd' : { if (decay[0] != 0) not_end_param = NON;  else decf = readMulti( decay, tmp, map_names); break; }
					case 'S' :
					case 's' : { if (learn[0] != 0) not_end_param = NON;  else lrnf = readMulti( learn, tmp, map_names); break; }
					case 'C' :
					case 'c' : { if (conv[0] != 0) not_end_param = NON;  else { map_names >> conv;  map_names >> tmp; } break; }
					case 'T' :
					case 't' : { if (seuil[0] != 0) not_end_param = NON; else thrf = readMulti( seuil, tmp, map_names); break; }
					case 'M' :
					case 'm' : { if (mod[0] != 0) not_end_param = NON; else modf = readMulti( mod, tmp, map_names); break; }
					case 'E' :
					case 'e' : { if (estim[0] != 0) not_end_param = NON; else estf = readMulti( estim, tmp, map_names);  break; }
					default : ERROR(3,("unknown/redeclared type definition symbol '%s' in file declaration map", tmp));
				}
	    		//if (not_end_param) map_names >> tmp;
	    		//if (not_end_param) map_names >> tmp;
			}
			if (name[0] == 0) ERROR(109,("missing name in file declaration map"));
			if (couche[0] == 0) ERROR(110,("no layer for %s map", name));
			if (estim[0] == 0)  ERROR(111,("no estimer for %s map", name));
			if (zoom[0] == 0)  ERROR(112,("no zoom factor for %s map", name));
			
			// multiscale maps
			if (((name[0] == 'x') || (name[0] == 'X')) && (name[1] == '_'))
					for (int i=0; i< numberSpikeNET - (networkType & ONESCALE); i++)
						SpikeNet[i]->addMap(
							atoi(couche),
							name,
							nbMaps++, 
							atof(seuil+i*thrf*30),
							atof(mod+i*modf*30),
							atof(estim+i*estf*30),
							atoi(zoom),
							atof(decay+i*decf*30),
							coordx[0] ? atoi(coordx) : -1,
							atoi(coordy), 
							atoi(zoomDsp),
							learn+i*lrnf*30,
							//*(bewulf+i*bewf*30) ? strcmp(server->getHostName(), bewulf+i*bewf*30) : 1,
							*(bewulf+i*bewf*30) ? procManager->addTabProc( bewulf+i*bewf*30) : (proc *) NULL,
							strLisp, 
							object,
							atoi(conv));
			else
				{
					if (decf || thrf || modf || estf || bewf)
						ERROR(113,("useless parameters for non multiscaled map %s", name));
					SpikeNet[numberSpikeNET-1]->addMap(
							atoi(couche),
							name,
							nbMaps++, 
							atof(seuil),
							atof(mod),
							atof(estim),
							atoi(zoom),
							atof(decay),
							coordx[0] ? atoi(coordx) : -1,
							atoi(coordy),
							atoi(zoomDsp),
							learn,
							bewulf[0] ? procManager->addTabProc( bewulf) : (proc *)NULL,
							strLisp, 
							object,
							atoi(conv));
				}

	  		eof1 = map_names.get();
	  		if (nbMaps > MAX_CARTE)
	  			ERROR(203,("maximum map overflow"));

      	} while (eof1 != EOF);
};

// ************************
// lecture des projections
// ************************
void superSpikeClass::read_projs()
{

   char new_net_projs[100];
   sprintf(  new_net_projs, "%s%s%c%s", CURRENT_DIR, EnvVar::MAIN_DIRECTORY, SEPARATEUR, NET_PROJS);
  FILESTREAM map_projs(new_net_projs);

    char   name1 	 [100];
    char   name2 	 [100];
    char   tmp       [100];
    char   convType  [100];
    char   file_proj [100];
    char   *file_proj_tmp;
    char   randfact  [100];
    char   multfact  [100];
    char   decfact   [100];
    char   modfact	 [100];
    char   renorm    [100];
    char   groupe    [100]; groupe[0] = 0;
    char   latency	 [100]; latency[0] = 0;
    double  randFact  = 0;
    double  multFact  = 1.0;
    double  decFact   = 0;
    double  modFact   = 0;
    double  renormFact= 0;
    int    duplicator = 0;
    int    noZoom	  = 0;
    int    not_end_param, eof1;
    carte_base  *carteAff;
    carte_base  *carteEff;
    carte_base  *tabAff[MAX_SPIKENET];

    do
        {
	  	map_projs >> name1; 
	  	map_projs >> name2; 
	  	map_projs >> tmp; 
	  	map_projs >> convType;
	  	not_end_param = OUI;
		while (not_end_param)
		{
	    	map_projs >> tmp;
	    	if (!strcmp(tmp, "MULT"))
	      		{ map_projs >> multfact; multFact = atof(multfact); }
	      	else if (!strcmp(tmp, "RAND"))
	      			{ map_projs >> randfact; randFact = atof(randfact); }
	      		 else if (!strcmp(tmp, "DEC"))
	      				{ map_projs >> decfact; decFact  = atof(decfact); }
	      			  else if (!strcmp(tmp, "GROUP"))
	      					{ map_projs >> groupe; }
	      			  		else if (!strcmp(tmp, "DUPLICATE"))
	      						{ duplicator = 1; }
	      			  		else if (!strcmp(tmp, "MOD"))
	      						{ map_projs >> modfact; modFact  = atof(modfact);}
	      			  			else if (!strcmp(tmp, "LATENCY"))
	      							{ map_projs >> latency; }
	      			  				else if (!strcmp(tmp, "RENORM"))
	      								 { map_projs >> renorm;  renormFact = atof( renorm ); }
	      			  					 else if (!strcmp(tmp, "NOZOOM"))
	      								 	  { noZoom = 1; }
					  	   		 		 	  else { strcpy( file_proj, tmp); not_end_param = NON; }
		}            
        eof1 = map_projs.get(); 
        if (eof1!=EOF)
            {   
                // change separator depending on computer type
            	strcpy(tmp, file_proj);
                strcpy(file_proj, CURRENT_DIR);
                strcat(file_proj, tmp);
				file_proj_tmp = file_proj;                
                while ((file_proj_tmp = strpbrk(file_proj_tmp, "/")) != NULL)
                	*(file_proj_tmp++) = SEPARATEUR;
 
            	// find maps
            	//int mapNumber;
            	int begSpikeNET = 0;
            	int endSpikeNET = numberSpikeNET;
            	
            	//efferent map
            	if ((name2[0] == 'x') || (name2[0] == 'X'))
            	{
            		if ((name2[1] >= '0') && (name2[1] <= '9') && (name2[2] == '_'))
            	  	{ 
						if (networkType == ONESCALE) ERROR( 117,("multiscale map %s in a non multiscale network", name2));
            	  		begSpikeNET = int(name2[1]) - int('0'); 
            	  		endSpikeNET = begSpikeNET+1;
            	  		char c1 = name2[0];
            	  		strcpy(name2, name2+1);
            	  		name2[0] = c1;
            	  	}
            	  	else if (name2[1] == '_')
            			{
							if (networkType == ONESCALE) ERROR( 117,("multiscale map %s in a non multiscale network", name2));
            				endSpikeNET -= (networkType & ONESCALE);
            			}
             	  		else ERROR( 118,("map %s not found", name2));
             	}
             	else
             	{
 					if (!(networkType & ONESCALE)) ERROR( 130,("non multiscale map %s in an only multiscale network", name2));
 					begSpikeNET = numberSpikeNET-1;
 				}            	

            	for (int i=begSpikeNET; i< endSpikeNET; i++)
            	{
            	  int mapNumber = i;
                  if ((carteEff = SpikeNet[mapNumber]->findInside(name2)) != NULL)
                  {
                	if ((carteAff = SpikeNet[mapNumber]->findInside(name1)) == NULL)
                		findMap(name1, mapNumber, tabAff);
                	else { tabAff[0] = carteAff; tabAff[1] = NULL; }
                	               		
                	int cmpt = 0;
                	while (tabAff[cmpt] != NULL)
                	{
                		carteAff = tabAff[cmpt];
						if (groupe[0] == 0)
						{
							sprintf( groupe, "%s_%d", carteAff->get_name(), cmpt);
                			carteEff->detect_convolution (carteAff, file_proj,
								!strcmp(convType, "LOCK"), randFact, multFact, decFact, modFact, groupe, duplicator, latency, noZoom, renormFact);
							groupe[0] = 0;
						}
						else carteEff->detect_convolution (carteAff, file_proj, 
								!strcmp(convType, "LOCK"), randFact, multFact, decFact, modFact, groupe, duplicator, latency, noZoom, renormFact);
						if ((carteEff->get_virtual() != NULL) && (carteAff->get_virtual() == NULL))
                            {
                                carteAff->addEfferentMap( carteEff->get_virtual() );
                                
                            }
						if ((carteEff->get_virtual() == NULL) && (carteAff->get_virtual() != NULL))
                                carteAff->add_virtual();

						cmpt++;
					}
				   }
				}
   				randFact     = 0;
    			multFact     = 1.0;
    			decFact      = 0;
    			modFact		 = 0;
    			renormFact   = 0;
    		    noZoom       = 0;
    			groupe[0]	 = 0;
    			duplicator   = 0;
    			latency[0]   = 0;
    			groupe [0]   = 0;
            }
        } while (eof1!=EOF);
    //map_projs.close();
    procManager->affProc();
    
};


// ***************************
// initialisation des couches
// ***************************

void SpikeClass::init_couche	()
{
    double NB_COLOR   = EnvVar::NB_COLOR;
    int   tab_nbneur_couche	[MAX_COUCHE];
    
    // initialisation de la  couche 0
    //tab_couche[0] = 0;
    //tab_couche[1] = 0;
    //tab_nbneur_couche[0] = 2 * size_img;
    //tab_estim[0] = EnvVar::RET_PC;
    
    // initialisation des autres couches
    for (int i=0; i<MAX_COUCHE; i++) tab_nbneur_couche[i] = 0;
    for (int i=0; i<nb_carte; i++)
      {
          int couche 						= tab_carte[i]->get_couche();
          tab_couche			[i] 		= couche;
          tab_estim			[couche]		= tab_carte[i]->get_estim();
		  //printf("%s -> %3.2f\n", tab_carte[i]->name, tab_carte[i]->estim);
          tab_nbneur_couche	[couche]	   += tab_carte[i]->get_largeur_norm() * tab_carte[i]->get_hauteur_norm();
      }
    
    for (int i=0; i<  MAX_COUCHE; i++) 
      {
          ordre_couche[i] = 0;
          tab_estim   [i] = NB_COLOR / (tab_estim[i] * (double)tab_nbneur_couche[i]);
      }	
};
