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

// $Log: EnvVarOld2.h,v $

//#include <fstream.h>
#include <string.h>
#include <stdlib.h>
#include "allDefs.h"

#ifndef _ENVVAR
#define _ENVVAR

/* **************************************** */
/* pour le traitement des varibles globales */
/* **************************************** */
static class EnvVar
{
public :

  // variables d''environnement
  static	int    NB_COLOR      ;
  static	int    MAX_TEMPS     ;
  static	int    SAVE_ON_DISK  ;
  static	int    DISPLAY       ;
  static	int    NO_SHOW       ;
  static	int    MAX_NORM      ;
  static	char   RESULTS  []   ;
  static	int    MOD_LINEAIRE  ;
  static	int    NB_SPIKENET   ;
  static	int    WAIT_ENTER    ;
  static	int    NET_NAME_VERSION   ;
  static	int	   CONVERGENCE_CONVO	;
  static	int    STEP;
  static	double  MOD_CUT       ;
  static	double  RAISE_THRESHOLD;
  static	double  LOWER_THRESHOLD;
  static	char   MAIN_DIRECTORY[];
  static	int	   MOD_TYPE;
  static	int	   ZOOM_CONVO;
  static	char   DIR_SAVE_CONVOS[];
  static	char   DIR_SAVE_LEARN[];
  static	char   DIR_SAVE_REBUILD[];
  static	char   DIR_SAVE_STATS[];
  static	int	   PURE_ORDER;
  static	int	   BYTES_PER_PIXEL;
  static	int	   BYTES_FILE;
  static	int	   SIZEX;
  static	int	   SIZEY;
  static	int	   REPLACE0_1;
  static	int	   LEARN;
  static	int	   FOND;
  static	int	   RESCUE;
  static	double TIME_BASE;
  static	double SPONTANEOUS;
  static	int	   BRIME_LIB;
  static	int    THRESHOLD_TYPE;
  static	int    WATCH_ACTIVITY;
  static	int    LEARN_SUPERV;
  static	int    RECONSTRUCT_MOD;
  static	int    CONVERGE;
  static	int	   NEW_IMAGE_LIST;

private : 
/*static char *load_env_str(char *s, char *net_env_name, char *defaultvalue)
{
  FILESTREAM   f(net_env_name);
    
    char descript[100];
    char *resultat = new char[100];
    int eof1, find = 0;

    do
        {
            f >> descript;
            //cout << descript;
            
            eof1 = f.get(); 
            if (eof1!=EOF) if (!strcmp(s, descript)) find = 1;
        } while ((eof1!=EOF) && (!find));

    if (!find)
    	if (defaultvalue == NULL)
            ERROR(101,("environement variable %s not found in file", s))
        else strcpy(resultat, defaultvalue);
    else f >> resultat; 
    return resultat;

}

static int load_env_int(char *s, char *net_env_name, int defaultvalue) 
{
  char tmp[30];
  sprintf(tmp, "%d", defaultvalue);
  return atoi(load_env_str(s, net_env_name, tmp));
}*/

/*static defaultValues()
{
      NB_COLOR      	= DEFAULT_NB_COLOR;
      MAX_NORM      	= DEFAULT_MAX_NORM*MAX_TEMPS;
      SAVE_ON_DISK  	= DEFAULT_SAVE;
      DISPLAY  			= 1;
      NO_SHOW       	= DEFAULT_SHOW;
      MOD_LINEAIRE 	 	= DEFAULT_MOD;
      NB_SPIKENET   	= 1
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
      MOD_CUT         	= DEFAULT_MOD_CUT;
      RAISE_THRESHOLD 	= DEFAULT_RAISE_THRE;
      LOWER_THRESHOLD 	= DEFAULT_LOWER_THRE;
	  TIME_BASE		  	= DEFAULT_TIME_BASE;
	  SPONTANEOUS     	= 0;
	  BRIME_LIB		  	= 0;
	  LEARN_SUPERV	  	= NO;
	  strcpy( RESULTS, DEFAULT_RESULTS);

}*/

static void assign( char *varName, char *varValue)
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

static void load_env_var( char *net_env_name)
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
 static void init(char *net_env_name)
    {
      FILESTREAM console(CONSOLE);
      console >> MAIN_DIRECTORY;
      char new_net_env_name[100];
      sprintf(  new_net_env_name, "%s%s%c%s", CURRENT_DIR, MAIN_DIRECTORY, SEPARATEUR, net_env_name );
      
      load_env_var( new_net_env_name );
    }
} toto;

#endif

