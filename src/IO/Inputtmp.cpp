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

// $Log: Input.cpp,v $

#include <stdio.h>
#include <string.h>
//#include "SpikeNETClass.h"
#include "SuperSpikeNETClass.h"
#include "MapBuffer.h"
#include "SpikeLisp.h"
#include "MapFixed.h"
#include "y.tab.h"
#include "SpikeOrder.h"

#if MODE == INTERNAL
#include "net_names.h"
#include "net_projs.h"
#endif

#include "OutputDisplay.h"
extern ScreenSpikeAuto *screen1;
extern ScreenSpikeAuto *screen2;

// ***********************************
// determination du numero d'une carte
// ***********************************

SpikeLispSpike *SuperSpikeClass::treatLisp( char *str, double spikePC)
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
	y_tab::yyrun( tmpFile, spL);
    fclose(tmpFile);
	return spL;
}	

int SuperSpikeClass::determineMultiscale()
{
	int			 filetype=0;	// 1 = non multiscale  (bit 1)
								// 2 = only multiscale (bit 2)
								// 3 = both			   (bit 1 & 2)
    char new_net_names[100];
    sprintf(  new_net_names, "%s%s%c%s", CURRENT_DIR, EnvVar::MAIN_DIRECTORY, SEPARATEUR, NET_NAMES);
    FILESTREAM   map_names(new_net_names);

    char 		 tmp [30];
	int 		 eof1;

    if (!map_names)
      ERROR( 2,("unable to open file %s", new_net_names));
	
    do
    {
		map_names >> tmp;
		eof1 = map_names.get();
		if (eof1!=EOF)
	  	{
			if ((tmp[0] == 'N') || (tmp[0] == 'n'))
			{
				map_names >> tmp; // name of the map
				if (((tmp[0] == 'x') || (tmp[0] == 'X')) && (tmp[1] == '_')) filetype |= MULTISCALE; // bit 2 set
				else														 filetype |= ONESCALE;   // bit 1 set
			}
		}
    } while (eof1 != EOF);

	if (filetype == 0) 							ERROR(122,("no map in %s", new_net_names));
	if ((filetype>1) && (EnvVar::NB_SPIKENET < 2)) WARN( 150,("x_map useless in %s file for a not multiscale network", new_net_names));
	if ((filetype<2) && (EnvVar::NB_SPIKENET > 1)) ERROR(123,("x_map required in %s file for a multiscale network", new_net_names));
	if (EnvVar::NB_SPIKENET >= MAX_SPIKENET)		ERROR(210,("maximum of multiscale SpikeNET overflow"));
	if (!EnvVar::NO_SHOW)
	{
		if (filetype == 1) 		printf("Non multiscale network\n");
		else if (filetype == 2) printf("Only multiscale network\n");
			  else 				printf("mixed multiscale & non multiscale network\n");
	}
	return filetype;
}

/*void SuperSpikeClass::findMap(char *name, int spikeClassNb, carte_base **tabRes)
{
	//what kind of map is it
	if (((name[0] == 'x') || (name[0] == 'X')) && ((name[1] == '_') || (name[2] == '_')))
	{
		if (networkType == ONESCALE) ERROR( 117,("multiscale map %s in a non multiscale network", name));
		if (name[1] == '_')
		{
			// error if the last spikeNET is multiscale
			int numberMultiscale = numberSpikeNET - (networkType & ONESCALE);
			//if ((spikeClassNb != ( numberSpikeNET-1)) && (networkType & ONESCALE)) ERROR(118,("map %s not found", name));			
			for (int nbRes=0; nbRes<numberMultiscale; nbRes++)
				if ((tabRes[nbRes] = SpikeNet[nbRes]->findInside(name)) == NULL) ERROR(119,("multiscale map %s not found", name));
			tabRes[numberMultiscale] = NULL;
		}
		else
		{
			char newname[50];
			sprintf(newname,"x_%s", name+2);
 			if ((name[1] == '-') && (spikeClassNb > 0))	{ tabRes[0] = SpikeNet[spikeClassNb-1]->findInside(newname); tabRes[1] = NULL; }
			if ((name[1] == '+') && (spikeClassNb < (EnvVar::NB_SPIKENET-1))) { tabRes[0] = SpikeNet[spikeClassNb+1]->findInside(newname); tabRes[1] = NULL; }
			if (name[1] == '*')
			{
				for (int nbRes=0; nbRes<(numberSpikeNET-1); nbRes++)
					if (nbRes != spikeClassNb)
						if ((tabRes[nbRes] = SpikeNet[nbRes]->findInside(newname)) == NULL) ERROR(119,("multiscale map %s not found", newname));
				tabRes[numberSpikeNET-2] = NULL;
			}
			int valuemap = (int)name[1]-48;
			if ((valuemap > 0) && (valuemap < EnvVar::NB_SPIKENET)) { tabRes[0] = SpikeNet[valuemap]->findInside(newname); tabRes[1] = NULL; }
		}
	}
	else //simple map
	{
		if (!(networkType & 0x2)) ERROR( 118,("map %s not found", name));
		tabRes[0] = SpikeNet[numberSpikeNET-1]->findInside(name);
		tabRes[1] = NULL; 
	}
}*/

// **********************************
// genere un fichier de configuration
// **********************************

/*void SuperSpikeClass::save_config()
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

int SuperSpikeClass::readMulti(char *var, char *tmp, FILESTREAM &map_names)
{
	int 		 cmpt = 0;
	map_names >> (var+cmpt*30);
								
	//multiscaled and non nul threshold
	do
	{ 
		cmpt++;
		map_names >> (var+cmpt*30);
	} while( (atof(var+cmpt*30) != 0) && (var[cmpt*30]!=0) && (var[cmpt*30] != '\"') && (map_names.eof() != EOF));
			
	if ((cmpt != 1) && (cmpt != numberSpikeNET - (ONESCALE & networkType)))
		ERROR(104,("in %s file, there must be 1 or NB_SPIKENET arguments", NET_NAMES));
		
	if (map_names.eof() != EOF) strcpy(tmp, var+cmpt*30); else strcpy(tmp, "");	
	var[cmpt*30] = 0;

	// is there multi arguments
	return (cmpt != 1);
}

void SuperSpikeClass::read_maps( BaseImg *baseImg)
{

   char new_net_names[100];
   sprintf(  new_net_names, "%s%s%c%s", CURRENT_DIR, EnvVar::MAIN_DIRECTORY, SEPARATEUR, NET_NAMES);
   FILESTREAM   map_names(new_net_names);
    
    //char nb_carte_strng     [30];
    char name     			[30];
    char tmp     			[200];
    char zoom	   			[30];
    char object	   			[100];
    char couche   			[30];
    char coordx   			[30];
    char coordy   			[30];
    char zoomDsp   			[30];
    char conv        		[30];
    char freq       		[30];
    char bewulf   			[MAX_SPIKENET*30];
    char seuil	   			[MAX_SPIKENET*30];
    char mod  	   			[MAX_SPIKENET*30];
    char estim    			[MAX_SPIKENET*30];
    char decay   			[MAX_SPIKENET*30];
    char learn   			[MAX_SPIKENET*30];
    char color   			[MAX_SPIKENET*30];
    char strLisp			[200];
    SpikeLispSpike			*spL[MAX_SPIKENET];
    int  eof1	,not_end_param;
    //int  nomultisc;
	int  size_x = baseImg->getWidth();
	int  size_y = baseImg->getHeight();	

	// factor of multiplication for arguments number in multiscaled maps
	int  decf;
	int  thrf;
	int  estf;
	int  modf;
	int  lrnf;
	int  bewf;
	int  colf;
	
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
		freq[0]    = 0;
	  		
		seuil[0]  = 0;	thrf = 0;
		mod[0]	  = 0;  modf = 0;
		estim[0]  = 0;  estf = 0;
		decay[0]  = 0;  decf = 0;
		learn[0]  = 0;  lrnf = 0;
		bewulf[0] = 0;  bewf = 0;
		color[0]  = 0;  colf = 0;
	  		
		strLisp[0]  = 0;
		coordx[0]	= 0;
		coordy[0]	= 0;
		{for (int i=0; i< numberSpikeNET; i++)	spL[i] = NULL;}
		char c;
	  		
		not_end_param = OUI;
		while (not_end_param && (tmp[0] != 0))
		  {
			//if (tmp[0] != 0) printf("%s\n", tmp);
			while (tmp[0] == '#')
			  { 
				while (
					   ((c = map_names.get()) != '\n')  
					   && (c!=EOF)) { }; 
				map_names >> tmp;
				while (tmp[0] == 0) map_names >> tmp;
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
			  case 'F' :
			  case 'f' : { if (freq[0] != 0) not_end_param = NON;  else { map_names >> freq;  map_names >> tmp; } break; }					
			  case 'B' :
			  case 'b' : { if (bewulf[0] != 0) not_end_param = NON;  else bewf = readMulti( bewulf, tmp, map_names); break; }
			  case 'D' :
			  case 'd' : { if (decay[0] != 0) not_end_param = NON;  else decf = readMulti( decay, tmp, map_names); break; }
			  case 'S' :
			  case 's' : { if (learn[0] != 0) not_end_param = NON;  else lrnf = readMulti( learn, tmp, map_names); break; }
			  case 'C' :
			  case 'c' : if ((tmp[2] == 'l') || (tmp[2] == 'l'))
				{ if (color[0] != 0) not_end_param = NON;  else colf = readMulti( color, tmp, map_names); break; }
			  else								
				{ if (conv[0] != 0)  not_end_param = NON;  else { map_names >> conv;  map_names >> tmp; } break; }
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
		if (zoom[0] == 0)  strcpy(zoom, "0");
		if (!strcmp(mod, "0")) mod[0] = 0;

		// non multiscale and multiscale map reservation
		int begL, endL;
		if (((name[0] == 'x') || (name[0] == 'X')) && (name[1] == '_'))	{ begL = 1; endL = numberSpikeNET; }
		else  {
		  begL = 0; 
		  endL = 1;
		  if (decf || thrf || modf || estf || bewf) ERROR(113,("useless parameters for non multiscaled map %s", name));
		}
			
		for (int i= begL; i< endL ; i++) {
			
		  // determine name and scale for multiscale maps
		  int scaling = 0;
		  if (begL != (endL-1)) { name[0] = (i-1)+48; scaling = i-1; ms_index = i-1; }
          else ms_index = i; // ms_index is the multiscale index
     
		  // Output Display Of the Map
		  OutputDisplay *outputTmp;
		  if (coordx[0] != 0)
			{
			  int coordXtmp = 0;
			  int coordYtmp = 0;
			  double factZoom;
			  if (atoi(zoomDsp) > 0) factZoom = 1/(1 << atoi(zoomDsp)); else factZoom = 1 << -atoi(zoomDsp);
			  switch ( scaling )
				{
				case 3 : coordXtmp +=  (int)(((double)size_y * factZoom) / 4) + (INTER_HAUT >> 2) ;
				case 2 : coordXtmp +=  (int)(((double)size_x * factZoom) / 2) + INTER_LARG;
				case 1 : coordYtmp +=  (int)((double)size_y * factZoom) + INTER_HAUT;
				case 0 : coordXtmp +=  atoi(coordx); coordYtmp += atoi(coordy);
				}
			  outputTmp = new OutputDisplay( coordXtmp, coordYtmp, size_x, size_y, atoi(zoomDsp), atoi(couche), EnvVar::SAVE_ON_DISK, screen0, SpikeNet[i]->estimer, strtoul(color+ms_index*colf*30, (char **) NULL, 0));
			}
		  else outputTmp = new OutputDisplay( 0, 0, size_x, size_y, atoi(zoomDsp), atoi(couche),  EnvVar::SAVE_ON_DISK, NULL, SpikeNet[i]->estimer, strtoul(color+ms_index*colf*30, (char **) NULL, 0));
		  if (object[0] != 0)	outputTmp->addImgAssociated( (BaseImg *) new PicturePPM( object ) );
				
				
		  // local or remote map
		  proc *local = *(bewulf+i*bewf*30) ? procManager->addTabProc( bewulf+ms_index*bewf*30) : (proc *) NULL;
		  if ((local != NULL)) // remote map
			SpikeNet[i]->addMap( tab_carte[nbMaps++] = new carte_virtual( nbMaps, outputTmp, name, size_x, size_y, atoi(zoom) + scaling, local) );
					
		  else if (atoi(couche) == 0) { // local map layer 0
			if (strLisp[0] == 0) ERROR(105,("no input for map %s", name));
			if ((learn[0] != 0) && (!strcmp(learn, "no")))	 ERROR(106,("no learning possible for input map %s", name));
			SpikeLispSpike *spL =  treatLisp(strLisp, atof(seuil+ms_index*thrf*30));
			SpikeNet[i]->addMap( tab_carte[ nbMaps++]  = new carte_fixed( nbMaps, outputTmp, name, size_x, size_y, atoi(zoom) + scaling, spL) );
		  }
		  else {	 // local map layer non 0
			if (strLisp[0] != 0) ERROR(107,("input only possible in layer 0, thus not for map %s", name));
			if (seuil[0] == 0)  	 ERROR(108,("no threshold for %s map\n", name));
			if ((learn[0] == 's') || (learn[0] == 'S')) {
			  SpikeNet[i]->addMap( tab_carte[nbMaps++]  = new carte_learn_superv( nbMaps, outputTmp, name, size_x, size_y, atof(seuil+ms_index*thrf*30) , mod+ms_index*modf*30, atoi(zoom) + scaling, atof(decay+i*decf*30), atoi(conv)) );
			  EnvVar::LEARN_SUPERV = YES;
			}
			else if (mod[0] == 0)
			  if (decay[0] == 0)
				SpikeNet[i]->addMap( tab_carte[nbMaps++]  = new carte( nbMaps, outputTmp, name, size_x, size_y, atof(seuil+i*thrf*30) , NULL, atoi(zoom) + scaling, atof(decay+i*decf*30), atoi(conv)) );
			  else	SpikeNet[i]->addMap( tab_carte[nbMaps++]  = new carte_decay( nbMaps, outputTmp, name, size_x, size_y, atof(seuil+i*thrf*30) , mod+i*modf*30, atoi(zoom) + scaling, atof(decay+i*decf*30), atoi(conv)) );
			else 
			  if (decay[0] == 0)
				SpikeNet[i]->addMap( tab_carte[nbMaps++]  = new carte_modul( nbMaps, outputTmp, name, size_x, size_y, atof(seuil+i*thrf*30) , mod+i*modf*30, atoi(zoom) + scaling, atof(decay+i*decf*30), atoi(conv)) );
			  else	SpikeNet[i]->addMap( tab_carte[nbMaps++]  = new carte_modul_decay( nbMaps, outputTmp, name, size_x, size_y, atof(seuil+i*thrf*30) , mod+i*estf*30, atoi(zoom) + scaling, atof(decay+i*decf*30), atoi(conv)) );
		  }				
			
		  //estimer
		  SpikeNet[i]->estimer->addMap( atoi(couche), tab_carte[nbMaps-1]->get_largeur_norm() * tab_carte[nbMaps-1]->get_hauteur_norm(), atof(estim+i*estf*30) );
		  number2discharge[ tab_carte[nbMaps-1]->get_number() ] = atoi(freq);
		}
			
		eof1 = map_names.get();
		if (nbMaps > MAX_CARTE)
		  ERROR(203,("maximum map overflow"));

	  } while (eof1 != EOF);
};

// ************************
// lecture des projections
// ************************
void SuperSpikeClass::read_projs()
{

   char new_net_projs[100];
   FILE *saveLearn = NULL;
   if (EnvVar::RESCUE)
   {
   		sprintf(  new_net_projs, "%s%s%c%s", CURRENT_DIR, EnvVar::MAIN_DIRECTORY, SEPARATEUR, "net_projs_backup" );
   		if (!EnvVar::NO_SHOW) printf("Rescue mode, starting at step %d\n", EnvVar::RESCUE);
   }
   else
   {
   		sprintf(  new_net_projs, "%s%s%c%s", CURRENT_DIR, EnvVar::MAIN_DIRECTORY, SEPARATEUR, NET_PROJS);
   		if ((EnvVar::LEARN) || (EnvVar::LEARN_SUPERV))
   		{
   			char 		fileSaveName[100];
   			sprintf		( fileSaveName, "%s%s%c%s", CURRENT_DIR, EnvVar::MAIN_DIRECTORY, SEPARATEUR, "net_projs_backup" );
   			saveLearn = fopen( fileSaveName, "w");
   		}
   	}
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
    char   modfact	 [100]; modfact[ 0 ] =0;
    char   renorm    [100];
    char   group    [100]; group[0] = 0;
    char   latency	 [100]; latency[0] = 0;
    double  randFact  = 0;
    double  multFact  = 1.0;
    double  decFact   = 0;
    double  modFact   = 0;
    double  renormFact= 0;
    int    duplicator = 0;
    int    noZoom	  = 0;
    int    not_end_param, eof1;
    carte_base  *tabAff[MAX_SPIKENET];
    carte_base  *tabEff[MAX_SPIKENET];

    do
        {
	  	map_projs >> name1;
		while (name1[0] == '#') {
		  map_projs.getline(name1, 100, '\n');
		  map_projs >> name1;
		}

	  	map_projs >> name2; 
	  	//map_projs >> tmp; 
	  	//map_projs >> convType;
	  	not_end_param = OUI;
		while (not_end_param)
		{
	    	map_projs >> tmp;
	    	if (!strcmp(tmp, "MULT"))
			  { map_projs >> multfact; multFact = atof(multfact); }
	      	else if (!strcmp(tmp, "RAND"))
			  { map_projs >> randfact; randFact = atof(randfact); }
	      	else if (!strcmp(tmp, "FILE"))
			  { map_projs >> convType; }
			else if (!strcmp(tmp, "DEC"))
			  { map_projs >> decfact; decFact  = atof(decfact); }
			else if (!strcmp(tmp, "GROUP"))
			  { map_projs >> group; }
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
		if (convType[0] == 0) strcpy(convType, "LOCK");

        eof1 = map_projs.get(); 
        if (eof1!=EOF)
		  {   
			//generate a new line in the file for saving
			if (saveLearn != NULL)
			  {
				if (strcmp(convType, "LOCK"))
				  if (group[0] != 0)
					fprintf( saveLearn, "%s\t%s\tFILE\tUNLOCK\tGROUP\t%s\t%s/%s_%s\n", name1, name2, group, EnvVar::DIR_SAVE_CONVOS, name2, name1);
				  else
					fprintf( saveLearn, "%s\t%s\tFILE\tUNLOCK\t%s/%s_%s\n",  name1, name2, EnvVar::DIR_SAVE_CONVOS, name2, name1);
				else
				  if (group[0] != 0)
					fprintf( saveLearn, "%s\t%s\tFILE\tLOCK\tGROUP\t%s\tMULT\t%f\t%s\n",  name1, name2, group, multFact, file_proj);
				  else
					fprintf( saveLearn, "%s\t%s\tFILE\tLOCK\tMULT\t%f\t%s\n",  name1, name2, multFact, file_proj);
			  }	
			
			//determine if the first character is not nul
			char tmpchar = file_proj[1];
			file_proj[1]=0;
			if (atoi(file_proj) != 0) { //learn, generate a file of the right size
			  //determine the size
			  file_proj[1]= tmpchar;
			  strcpy(tmp, file_proj);
			  char *tmp2 = strpbrk( tmp, "x"); *tmp2 = 0;
			  int sizeX = atoi(tmp);
			  int sizeY = atoi(tmp2+1);
			  sprintf(file_proj, "%stmp%c%s", CURRENT_DIR, SEPARATEUR, BRIMEFILE);
			  FILE *f = fopen( file_proj, "w");
			  if (f == NULL) { ERROR(13,("Unable to open file %s for writing", file_proj)); }
			  fprintf( f, "%d %d %d\n", sizeX, sizeY, 0);
			  for (int i=0; i<sizeX* sizeY; i++) fprintf( f, "%d\t", 1);
			  fclose(f);
			  strcpy( convType, "UNLOCK");
			  sprintf(file_proj, "tmp/%s", BRIMEFILE);
			} else file_proj[1]= tmpchar;
 
			// for the Brime lib
			if (EnvVar::BRIME_LIB == 0) {
			  // change separator depending on computer type
			  strcpy(tmp, file_proj);
			  strcpy(file_proj, CURRENT_DIR);
			  strcat(file_proj, tmp);
			  file_proj_tmp = file_proj;                
			  while ((file_proj_tmp = strpbrk(file_proj_tmp, "/")) != NULL)
				*(file_proj_tmp++) = SEPARATEUR;
			}
			if ((EnvVar::BRIME_LIB == 1) && (file_proj[0] == '*')) { //compute, add the directory name
			  strcpy(tmp, strpbrk( file_proj, "_") + 1);
			  sprintf(file_proj, "%s%s%c%s", CURRENT_DIR, EnvVar::DIR_SAVE_CONVOS, SEPARATEUR, tmp);
			}
			if ((EnvVar::BRIME_LIB == 2) && (file_proj[0] == '*')) { //learn, generate a file of the right size
			  //determine the size
			  strcpy(tmp, file_proj+ 1);
			  char *tmp2 = strpbrk( tmp, "x"); char *tmp3 = strpbrk( tmp2, "_"); *tmp2 = 0; *tmp3 =0;
			  int sizeX = atoi(tmp);
			  int sizeY = atoi(tmp2+1);
			  sprintf(file_proj, "%stmp/%s", CURRENT_DIR, BRIMEFILE);
			  FILE *f = fopen( file_proj, "w");
			  fprintf( f, "%d %d %d\n", sizeX, sizeY, 0);
			  for (int i=0; i<sizeX* sizeY; i++) fprintf( f, "%d\t", 1);
			  fclose(f);
			  strcpy( convType, "UNLOCK");
			}
			
            //afferent map(s)
            if (((name1[0] == 'X') || (name1[0] == 'x')) && (name1[1] == '_')) {
			  if (networkType == ONESCALE) ERROR( 117,("multiscale map %s in a non multiscale network", name2));
			  
			  tabAff[ numberSpikeNET - 1 ] = NULL;
			  for (int i=0; i< numberSpikeNET - 1; i++) {
				name1[ 0 ] = i+48;
				tabAff[i] = SpikeNet[0]->findInside( name1 );
			  }
            }
            else { tabAff[ 0 ] = SpikeNet[0]->findInside( name1 ); tabAff[ 1 ] = NULL; }
			
			
          	//efferent map(s)
  			if (((name2[0] == 'X') || (name2[0] == 'x')) && (name2[1] == '_')) {
			  if (networkType == ONESCALE) ERROR( 117,("multiscale map %s in a non multiscale network", name2));
			  
					tabEff[ numberSpikeNET - 1 ] = NULL;
            		for (int i=0; i< numberSpikeNET - 1; i++) {
					  name2[ 0 ] = i+48;
					  tabEff[i] = SpikeNet[0]->findInside( name2 );
            		}
            }
            else for (int i=0; i< numberSpikeNET; i++) tabEff[ i ] = SpikeNet[0]->findInside( name2 );         
            	
            //scan all maps
            int cmpt1 = 0;
            while (tabAff[cmpt1] != NULL) {
 				if (group[0] == 0) {
					sprintf( group, "%s_%d", tabAff[cmpt1]->get_name(), cmpt1);
                	tabEff[cmpt1]->detect_convolution (tabAff[cmpt1], file_proj, !strcmp(convType, "LOCK"), randFact, multFact, decFact, modfact, group, duplicator, latency, noZoom, renormFact);
					group[0] = 0;
				}
				else tabEff[cmpt1]->detect_convolution ( tabAff[cmpt1], file_proj, !strcmp(convType, "LOCK"), randFact, multFact, decFact, modfact, group, duplicator, latency, noZoom, renormFact);
				
				// virtual maps		
				if ((tabEff[cmpt1]->get_virtual() != NULL) && ( tabAff[cmpt1]->get_virtual() == NULL))
                               		tabAff[cmpt1]->addEfferentMap( tabEff[cmpt1]->get_virtual() );
				if ((tabEff[cmpt1]->get_virtual() == NULL) && (tabAff[cmpt1]->get_virtual() != NULL)) tabAff[cmpt1]->add_virtual();
				cmpt1++;
			}
   	
			randFact     = 0;
			multFact     = 1.0;
    		decFact      = 0;
    		modFact		 = 0;
    		renormFact   = 0;
    		noZoom       = 0;
    		group[0]	 = 0;
    		duplicator   = 0;
    		modfact[ 0 ] =0;
    		latency[0]   = 0;
    		group [0]   = 0;
			convType[0] = 0;
     	} // end of if not EOF
     	
     } while (eof1!=EOF);
    //map_projs.close();
    
    if (!EnvVar::NO_SHOW)
    	procManager->affProc();
    if (saveLearn != NULL) fclose( saveLearn);
    
};
