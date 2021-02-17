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

// $Log: EnvVarTest.h,v $

//#include <fstream.h>
#include <string.h>
#include <stdlib.h>
#include "allDefs.h"

#ifndef _ENVVAR
#define _ENVVAR

/* **************************************** */
/* pour le traitement des varibles globales */
/* **************************************** */
static class EnvVar2
{
public :
  // variables d''environnement
  	int    NB_COLOR      ;
  	int    MAX_TEMPS     ;
  	int    SAVE_ON_DISK  ;
  	int    DISPLAY       ;
  	int    NO_SHOW       ;
  	int    MAX_NORM      ;
  	char   RESULTS  [50]   ;
  	int    MOD_LINEAIRE  ;
  	int    NB_SPIKENET   ;
  	int    WAIT_ENTER    ;
  	int    NET_NAME_VERSION   ;
  	int	   CONVERGENCE_CONVO	;
  	int    STEP;
  	double  MOD_CUT       ;
  	double  RAISE_THRESHOLD;
  	double  LOWER_THRESHOLD;
  	char   MAIN_DIRECTORY[50];
  	int	   MOD_TYPE;
  	int	   ZOOM_CONVO;
  	char   DIR_SAVE_CONVOS[50];
  	char   DIR_SAVE_LEARN[50];
  	char   DIR_SAVE_REBUILD[50];
  	char   DIR_SAVE_STATS[50];
  	int	   PURE_ORDER;
  	int	   BYTES_PER_PIXEL;
  	int	   BYTES_FILE;
  	int	   SIZEX;
  	int	   SIZEY;
  	int	   REPLACE0_1;
  	int	   LEARN;
  	int	   FOND;
  	int	   RESCUE;
  	double TIME_BASE;
  	double SPONTANEOUS;
  	int	   BRIME_LIB;
  	int    THRESHOLD_TYPE;
  	int    WATCH_ACTIVITY;
  	int    LEARN_SUPERV;
  	int    RECONSTRUCT_MOD;
  	int    CONVERGE;
  	int	   NEW_IMAGE_LIST;

private : 

void defaultValues()
{
      NB_COLOR      	= DEFAULT_NB_COLOR;
      MAX_NORM      	= DEFAULT_MAX_NORM*MAX_TEMPS;
      SAVE_ON_DISK  	= DEFAULT_SAVE;
      DISPLAY  			= 1;
      NO_SHOW       	= DEFAULT_SHOW;
      MOD_LINEAIRE 	 	= DEFAULT_MOD;
      NB_SPIKENET   	= 1;
      WAIT_ENTER    	= 0;
      CONVERGENCE_CONVO = DEFAULT_CONV_CONVO;
      NET_NAME_VERSION	= DEFAULT_NAME_VERSION;
	  STEP			  	= 1;
	  MOD_TYPE	      	= DEFAULT_MODABS;       // modulation absolue ou relative
	  ZOOM_CONVO 	  	= DEFAULT_ZOOM_CONVO;
	  PURE_ORDER 	  	= DEFAULT_PURE_ORDER;
	  BYTES_PER_PIXEL 	= DEFAULT_BYTES_PER_PIXEL;
	  BYTES_FILE 	  	= 1;
	  SIZEX	  		  	= DEFAULT_SIZEX;
	  SIZEY	          	= DEFAULT_SIZEY;
	  REPLACE0_1	  	= DEFAULT_REPLACE0_1;
	  LEARN			  	= DEFAULT_LEARN;
	  FOND			  	= 1;
	  RESCUE		  	= 0;
	  RECONSTRUCT_MOD 	= 0;
	  CONVERGE		  	= 0;
	  NEW_IMAGE_LIST  	= 0;
	  THRESHOLD_TYPE  	= 0;
      MAX_TEMPS       	= -1;
	  SPONTANEOUS     	= 0;
	  BRIME_LIB		  	= 0;
	  LEARN_SUPERV	  	= NO;
	  strcpy( RESULTS, DEFAULT_RESULTS);

}

void assign( char *varName, char *varValue)
{
	// integer
      if (!strcmp(varName, "NB_COLOR")) 			NB_COLOR = atoi( varValue );
      if (!strcmp(varName, "MAX_NORM")) 			MAX_NORM = atoi( varValue );
      if (!strcmp(varName, "SAVE_ON_DISK")) 		SAVE_ON_DISK = atoi( varValue );
      if (!strcmp(varName, "DISPLAY")) 				DISPLAY = atoi( varValue );
      if (!strcmp(varName, "NO_SHOW")) 				NO_SHOW = atoi( varValue );
      if (!strcmp(varName, "MOD_LINEAIRE")) 		MOD_LINEAIRE = atoi( varValue );
      if (!strcmp(varName, "NB_SPIKENET")) 			NB_SPIKENET = atoi( varValue );
      if (!strcmp(varName, "WAIT_ENTER")) 			WAIT_ENTER = atoi( varValue );
      if (!strcmp(varName, "CONVERGENCE_CONVO")) 	CONVERGENCE_CONVO = atoi( varValue );
      if (!strcmp(varName, "NET_NAME_VERSION")) 	NET_NAME_VERSION = atoi( varValue );
	  if (!strcmp(varName, "STEP")) 				STEP = atoi( varValue );
	  if (!strcmp(varName, "MOD_TYPE")) 			MOD_TYPE = atoi( varValue );       // modulation absolue ou relative
	  if (!strcmp(varName, "ZOOM_CONVO")) 			ZOOM_CONVO = atoi( varValue );
	  if (!strcmp(varName, "PURE_ORDER")) 			PURE_ORDER = atoi( varValue );
	  if (!strcmp(varName, "BYTES_PER_PIXEL")) 		BYTES_PER_PIXEL = atoi( varValue );
	  if (!strcmp(varName, "BYTES_FILE")) 			BYTES_FILE = atoi( varValue );
	  if (!strcmp(varName, "SIZEX")) 				SIZEX = atoi( varValue );
	  if (!strcmp(varName, "SIZEY")) 				SIZEY = atoi( varValue );
	  if (!strcmp(varName, "REPLACE0_1")) 			REPLACE0_1 = atoi( varValue );
	  if (!strcmp(varName, "LEARN")) 				LEARN = atoi( varValue );
	  if (!strcmp(varName, "FOND")) 				FOND = atoi( varValue );
	  if (!strcmp(varName, "RESCUE")) 				RESCUE = atoi( varValue );
	  if (!strcmp(varName, "RECONSTRUCT_MOD")) 		RECONSTRUCT_MOD = atoi( varValue );
	  if (!strcmp(varName, "CONVERGE")) 			CONVERGE = atoi( varValue );
	  if (!strcmp(varName, "NEW_IMAGE_LIST")) 		NEW_IMAGE_LIST = atoi( varValue );
	  if (!strcmp(varName, "THRESHOLD_TYPE")) 		THRESHOLD_TYPE = atoi( varValue );	  
      if (!strcmp(varName, "MAX_TEMPS")) 			MAX_TEMPS = atoi( varValue );
	// float      
      if (!strcmp(varName, "MOD_CUT")) 				MOD_CUT = atof( varValue );      
      if (!strcmp(varName, "RAISE_THRESHOLD")) 		RAISE_THRESHOLD = atof( varValue );
      if (!strcmp(varName, "LOWER_THRESHOLD")) 		LOWER_THRESHOLD = atof( varValue );
	  if (!strcmp(varName, "TIME_BASE")) 			TIME_BASE = atof( varValue );
	  if (!strcmp(varName, "SPONTANEOUS")) 			SPONTANEOUS = atof( varValue );
	// string
	  if (!strcmp(varName, "DIR_SAVE_CONVOS")) 		strcpy( DIR_SAVE_CONVOS, varValue );
	  if (!strcmp(varName, "RESULTS")) 				strcpy( RESULTS, varValue );
	  if (!strcmp(varName, "DIR_SAVE_LEARN")) 		strcpy( DIR_SAVE_LEARN, varValue );
};

void load_env_var( char *net_env_name)
{
  FILESTREAM   f(net_env_name);
  char descript[100], value[30];
  int  eof1;
    
  f >> descript;
  do
  {
     f >> value;
    //cout << descript;
            
     eof1 = f.get();
     if ((eof1!=EOF) && ((!strcmp(descript, "set") || (!strcmp(descript, "SET")) || (!strcmp(descript, "Set"))) ))
     {
     	strcpy(descript, value);
     	f >> value;
     	assign( descript, value);
     	if (!NO_SHOW) printf("Variable %s\tassigned to %s\n", descript, value);
     }
     else if (!STRICT_LOAD_ENV) assign( descript, value);
     strcpy( descript, value);
  } while ((eof1!=EOF));
  do
  {
     f >> descript;
     //cout << descript;
            
     eof1 = f.get();
     if ((eof1!=EOF) && ((!strcmp(descript, "set") || (!strcmp(descript, "SET")) || (!strcmp(descript, "Set"))) ))
     {
     	f >> descript;
     	f >> value;
     	assign( descript, value);
     }
  } while ((eof1!=EOF));
}

public :
 EnvVar2()
    {
      FILESTREAM console(CONSOLE);
      console >> MAIN_DIRECTORY;
      char new_net_env_name[100];
      sprintf(  new_net_env_name, "%s%s%c%s", CURRENT_DIR, MAIN_DIRECTORY, SEPARATEUR, "net_env" );
      
      load_env_var( new_net_env_name );
    }
} EnvVars;

#endif

