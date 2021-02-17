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

// $Log: SpikeNETlibInterface.cc,v $

// ******************************
// *					    	*
// *		SpikeNet            *
// *						    *
// ******************************

// include global
#include "SuperSpikeNETClass.h"

//#include <new.h>
	// environment variables		
 /*   int 	EnvVar::NB_COLOR      		= DEFAULT_NB_COLOR;
    int 	EnvVar::MAX_TEMPS     		= 0;
    int 	EnvVar::MAX_NORM      		= DEFAULT_MAX_NORM;
    int	 	EnvVar::SAVE_ON_DISK  		= DEFAULT_SAVE;
    int	 	EnvVar::DISPLAY  			= DISPLAY;
    int 	EnvVar::NO_SHOW       		= DEFAULT_SHOW;
    int		EnvVar::MOD_LINEAIRE  		= DEFAULT_MOD;
    int		EnvVar::NB_SPIKENET   		= 1;
    int		EnvVar::WAIT_ENTER    		= 0;
    int		EnvVar::CONVERGENCE_CONVO 	= DEFAULT_CONV_CONVO;
    int		EnvVar::NET_NAME_VERSION	= DEFAULT_NAME_VERSION;
	int		EnvVar::STEP			  	= 1;
    double	EnvVar::MOD_CUT       		= atof(DEFAULT_MOD_CUT);
    double	EnvVar::RAISE_THRESHOLD 	= atof(DEFAULT_RAISE_THRE);
    double	EnvVar::LOWER_THRESHOLD 	= atof(DEFAULT_LOWER_THRE);
    char 	EnvVar::RESULTS [40] 		= DEFAULT_RESULTS;
    char 	EnvVar::MAIN_DIRECTORY[100] = ".";
 	int	    EnvVar::MOD_TYPE			= DEFAULT_MODABS;
  	int	    EnvVar::ZOOM_CONVO			= DEFAULT_ZOOM_CONVO;
  	char    EnvVar::DIR_SAVE_CONVOS[40]	= DEFAULT_DIR_SAVE_CONVOS;
  	char    EnvVar::DIR_SAVE_LEARN[40]	= DEFAULT_DIR_SAVE_LEARN;
  	char    EnvVar::DIR_SAVE_REBUILD[40]= DEFAULT_DIR_SAVE_REBUILD;
  	char    EnvVar::DIR_SAVE_STATS[40]	= DEFAULT_DIR_SAVE_STATS;
 	int	    EnvVar::PURE_ORDER			= DEFAULT_PURE_ORDER;
  	int	    EnvVar::BYTES_PER_PIXEL		= DEFAULT_BYTES_PER_PIXEL;
  	int	    EnvVar::BYTES_FILE			= 1;
  	int	    EnvVar::SIZEX				= DEFAULT_SIZEX;
    int	    EnvVar::SIZEY				= DEFAULT_SIZEY;
    int	    EnvVar::REPLACE0_1			= DEFAULT_REPLACE0_1;
    int	    EnvVar::LEARN   			= DEFAULT_LEARN;
    int	    EnvVar::FOND    			= 1;
    int	    EnvVar::RESCUE   			= 0;
    double  EnvVar::TIME_BASE  			= atof(DEFAULT_TIME_BASE);
    double  EnvVar::SPONTANEOUS  		= 0.0;
    int 	EnvVar::BRIME_LIB     		= 0;
    int 	EnvVar::THRESHOLD_TYPE     	= 0;
    int		EnvVar::WATCH_ACTIVITY     	= 0;
    int		TabWeight::sizetabtmp		= 0;
    int		**TabWeight::tabGlobal		= NULL;*/
    int 	EnvVar::NB_COLOR      		= DEFAULT_NB_COLOR;
    int 	EnvVar::MAX_TEMPS     		= DEFAULT_MAX_TEMPS;
    int 	EnvVar::MAX_NORM      		= DEFAULT_MAX_NORM;
    int	 	EnvVar::SAVE_ON_DISK  		= DEFAULT_SAVE;
    int	 	EnvVar::DISPLAY  			= DISPLAY;
    int 	EnvVar::NO_SHOW       		= DEFAULT_SHOW;
    int		EnvVar::MOD_LINEAIRE  		= DEFAULT_MOD;
    int		EnvVar::NB_SPIKENET   		= 1;
    int		EnvVar::WAIT_ENTER    		= 0;
    int		EnvVar::CONVERGENCE_CONVO 	= DEFAULT_CONV_CONVO;
    int		EnvVar::NET_NAME_VERSION	= DEFAULT_NAME_VERSION;
	int		EnvVar::STEP			  	= 1;
    double	EnvVar::MOD_CUT       		= atof(DEFAULT_MOD_CUT);
    double	EnvVar::RAISE_THRESHOLD 	= atof(DEFAULT_RAISE_THRE);
    double	EnvVar::LOWER_THRESHOLD 	= atof(DEFAULT_LOWER_THRE);
    char 	EnvVar::RESULTS [40] 		= DEFAULT_RESULTS;
    char 	EnvVar::MAIN_DIRECTORY[100] = ".";
 	int	    EnvVar::MOD_TYPE			= DEFAULT_MODABS;
  	int	    EnvVar::ZOOM_CONVO			= DEFAULT_ZOOM_CONVO;
  	char    EnvVar::DIR_SAVE_CONVOS[40]	= DEFAULT_DIR_SAVE_CONVOS;
  	char    EnvVar::DIR_SAVE_LEARN[40]	= DEFAULT_DIR_SAVE_LEARN;
  	char    EnvVar::DIR_SAVE_REBUILD[40]= DEFAULT_DIR_SAVE_REBUILD;
  	char    EnvVar::DIR_SAVE_STATS[40]	= DEFAULT_DIR_SAVE_STATS;
 	int	    EnvVar::PURE_ORDER			= DEFAULT_PURE_ORDER;
  	int	    EnvVar::BYTES_PER_PIXEL		= DEFAULT_BYTES_PER_PIXEL;
  	int	    EnvVar::BYTES_FILE			= 1;
  	int	    EnvVar::SIZEX				= DEFAULT_SIZEX;
    int	    EnvVar::SIZEY				= DEFAULT_SIZEY;
    int	    EnvVar::REPLACE0_1			= DEFAULT_REPLACE0_1;
    int	    EnvVar::LEARN   			= DEFAULT_LEARN;
    int	    EnvVar::FOND    			= 1;
    int	    EnvVar::RESCUE   			= 0;
    double  EnvVar::TIME_BASE  			= atof(DEFAULT_TIME_BASE);
    double  EnvVar::SPONTANEOUS  		= 0.0;
    int 	EnvVar::BRIME_LIB     		= 0;
    int 	EnvVar::THRESHOLD_TYPE     	= 0;
    int		EnvVar::WATCH_ACTIVITY     	= 0;
    int     EnvVar::LEARN_SUPERV		= NO;
    int     EnvVar::RECONSTRUCT_MOD		= NO;
    int		EnvVar::CONVERGE 			= NO;
    int		EnvVar::NEW_IMAGE_LIST		= NO;
    int		EnvVar::LAYER_CONVERGE		= 2;
   int		EnvVar::CONVERGE_VALUE		= 8;    
    int		EnvVar::TIME_STOP			= -1;    
    int		EnvVar::TESTRES				= 0;    
    int		TabWeight::sizetabtmp		= 0;
    int		**TabWeight::tabGlobal		= NULL;


SuperSpikeClass::SuperSpikeClass(  char *directoryExec, char *directoryLearn, BaseImg *baseImg, int mode )
{
	char cons[100];
	sprintf( cons, "%sconsole", CURRENT_DIR);
	FILE *f2 = fopen( cons, "w");
	fprintf( f2, "%s\n", directoryExec);
	fclose(f2);

	EnvVar::init( NET_ENV );
	EnvVar::BRIME_LIB = mode;
	strcpy( EnvVar::DIR_SAVE_CONVOS, directoryLearn);
	
	img_list = NULL;
	init( baseImg );
}

BaseImg *SuperSpikeClass::compute( BaseImg *Im)
{
	GlobalImg *bsimg = new GlobalImg( Im->Im, Im->getWidth(), Im->getHeight());
	
	// init
    for (int kk=0; kk<numberSpikeNET; kk++) SpikeNet[kk]->LoadImage(&bsimg);
  	for (int i=0; i< nbMaps; i++)			tab_carte[i]->init_neuron(&bsimg, 0);

	// COMPUTATION
	for (int temps=0;temps< EnvVar::MAX_TEMPS + MAX_COUCHE;temps++)
	    for (int i=0; i<numberSpikeNET; i++)
            SpikeNet[i]->Compute(temps);

	// affichage
	{for (int kk=0; kk<numberSpikeNET; kk++)
     	SpikeNet[kk]->SaveOnDisk   ("", 0);}
     
     return tab_carte[ nbMaps-1]->getResult();
}

void SuperSpikeClass::learn( BaseImg *Im, int coordx, int coordy)
{
	GlobalImg *bsimg = new GlobalImg( Im->Im, Im->getWidth(), Im->getHeight());
	
	// init
    for (int kk=0; kk<numberSpikeNET; kk++) SpikeNet[kk]->LoadImage(&bsimg);
  	for (int i=0; i< nbMaps; i++)			tab_carte[i]->init_neuron(&bsimg, 0);
	tab_carte[nbMaps-1]->setLearnCoord( coordx, coordy);

	// COMPUTATION
	for (int temps=0;temps< EnvVar::MAX_TEMPS + MAX_COUCHE;temps++)
	    for (int i=0; i<numberSpikeNET; i++)
            SpikeNet[i]->Compute(temps);

	// sauvegarde sur le disque
	{for (int kk=0; kk<numberSpikeNET; kk++)
     	SpikeNet[kk]->SaveOnDisk   ( "", 0);}
	{for (int kk=0; kk<nbMaps; kk++)
	    tab_carte[kk]->saveAllConvos( 1);}
}

/////////////////////////////////

static SuperSpikeClass *mainObject = NULL;

ImageStruct *run(ImageStruct *Im, char *directoryLearn, char *network)
{
	BaseImg *baseImg = new BaseImg( Im->data, Im->width, Im->height);
	
	if 	(mainObject == NULL)
		mainObject = new SuperSpikeClass( network, directoryLearn, baseImg, 1 ); // mode 1 compute
	baseImg = mainObject->compute( baseImg );
	
	ImageStruct *imgStr = new ImageStruct;
	imgStr->width = baseImg->getWidth();
	imgStr->height = baseImg->getHeight();
	imgStr->data = baseImg->Im;
	return imgStr;
}

void learn(ImageStruct *Im, CoordStruct *coordvalues, CoordStruct *sizeValues, char *directoryLearn, char *network)
{
	BaseImg *baseImg = new BaseImg( Im->data, Im->width, Im->height);
		
	if 	(mainObject == NULL)
	{
		// build the projection file
		char file_proj[ 50];
		sprintf(file_proj, "%s%s", CURRENT_DIR, BRIMEFILE);
		FILE *f = fopen( file_proj, "w");
		fprintf( f, "%d %d %d\n", sizeValues->x, sizeValues->y, 0);
		for (int i=0; i<sizeValues->x; i++)
			for (int i=0; i<sizeValues->y; i++)
				fprintf( f, "%d\t", 1);
		fclose(f);
		mainObject = new SuperSpikeClass( network, directoryLearn, baseImg, 2 ); // mode 2 learn
	}

	mainObject->learn( baseImg, coordvalues->x, coordvalues->y);
}

void modifyThreshold( double val)
{ mainObject->modifyThreshold( val); }

