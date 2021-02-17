// Copyright (C) 1997-2004 Arnaud Delorme, arno@salk.edu
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

// $Log: main.cpp,v $

// ******************************
// *					    	*
// *		SpikeNet            *
// *						    *
// ******************************

// include global
//#include <new.h>
#include "SuperSpikeNETClass.h"
#include "y.tab.h"

// cet include sert a la compatibilitŽ Reseau du Mac pour les commandes Reseau
//#include "GUSI.h"

//global variable to remove
//Server *server;

	// environment variables		
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
    int	    EnvVar::FOND    			= 0;
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
    int		EnvVar::SIZE_RESP_ZONE		= 2;
    int		EnvVar::CHRONO		        = 0;
  	char    EnvVar::SAVE_TMP[40]		= "vect1N:";
    int		TabWeight::sizetabtmp		= 0;
    int		**TabWeight::tabGlobal		= NULL;
	//FileFolder	*ifilestream::testing	= new FileFolder(ARCHIVE_NAME);

static SuperSpikeClass *mainObject = NULL;

int CRC(char *fileName)
{
	FILESTREAM cons(fileName);
	char tmp[40];
	cons >> tmp;
	strcat(tmp, "/net_names");
	FILESTREAM f(tmp);

	int CRCres = 0;
	int nb = 0;
	char c = 0, c1 = 0;
	
	while( ((c = f.get()) != EOF)) { CRCres += c * c1 * 123 + 3; nb++; c1 = c; }
	return (CRCres / nb);
}

int getCRC(char *fileName)
{
	FILESTREAM f(fileName);
	char tmp[40];
	f >> tmp;
	f >> tmp;	
	return atoi( tmp );
}

int main(int argc, char *argv[])
{

  if (argc < 2) {
	printf("\nSpikeNET: no directory specified, demo mode\n");
	printf("          to run a network which configuration files are contained in a folder,\n");
	printf("          provide the folder name as the first argument to SpikeNET\n\n");
	
	printf("SpikeNET demo:\n");
	printf("\tFace detection:1\n");
	printf("\tFace detection multiscale:2\n");
	printf("\tFace recognition (learning):3\n");
	printf("\tFace recognition (sequential testing):4\n");
	printf("\tFace recognition (random testing):5\n");
	printf("\nEnter your selection:");

	char resp[100];
	scanf("%s", resp);
	printf("\n");
	
	if (resp[0] == '1')
	  system("echo networkdemodetection > console");
	else if (resp[0] == '2')
	  system("echo networkdemodetectionms > console");
	else if (resp[0] == '3')
	  system("echo networkdemo40faceslearn  > console");
	else if (resp[0] == '4')
	  system("echo networkdemo40facesrun > console");
	else if (resp[0] == '5')
	  system("echo networkdemo40facesrandom > console");
	else { printf("\nUnrecognized selection\n"); exit(-1); }
  }
  else { // no demo mode
	char tmp[100];
	sprintf(tmp, "echo %s > console", argv[1]); 
	system( tmp );
  }

	
/*	char tmp[100];
	scanf("%s", tmp);
	int res = strtoul( tmp, (char **) NULL, 0);
	printf("%x\n", res);
	exit(-1);*/
    // creation des spikenet
    // ---------------------
    if (!EnvVar::NO_SHOW)
        {
	  		printf("SpikeNet version 1.02\n");
	  		printf("Copyrigth (C) Arnaud Delorme 1997-2004\n");
	  		printf("This version is for research purposes only\n");
	  		printf("For commercial purposes, see www.spikenet-technology.com\n\n");
        }

	mainObject = new SuperSpikeClass(argc, argv);
	//printf("res : %d\t%d\n", CRC( "console"), getCRC("console"));
	//if (CRC( "console") == getCRC("console"))
	mainObject->run();
	return 0;
}
