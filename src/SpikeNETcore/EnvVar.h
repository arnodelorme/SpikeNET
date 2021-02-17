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

// $Log: EnvVar.h,v $

//#include <fstream.h>
#include <string.h>
#include <stdlib.h>
#include "allDefs.h"

#ifndef _ENVVAR
#define _ENVVAR

static int strcasesame( char *c1, char *c2)
{
	return strcmp( c1, c2 );
/*	int res = 1;
	int len = strlen(c1);
	if (strlen(c1) != strlen(c2)) return 0;
	int count = 0;
	while (count < len) {
		if (c1[count] == c2[count]) count++;
		else { res = 0; break; }
	}
	return !res;*/
}	

/* **************************************** */
/* pour le traitement des varibles globales */
/* **************************************** */
class EnvVar
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
  static    int	   CONVERGE_VALUE;
  static    int	   LAYER_CONVERGE;
  static    int	   TIME_STOP;
  static    int	   TESTRES;
  static    int	   SIZE_RESP_ZONE;
  static    int	   CHRONO;
  static	char   SAVE_TMP[];
  
public :
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

//static void newscreen( char *screenName, int coordx, int coordy, int sizex, int sizey) { }


static int assign( char *varName, char *varValue)
{
	int found = 0;
	// integer
  	  if (!strcasesame(varName, "CHRONO")) 			    { found = 1; CHRONO = atoi( varValue ); }
      if (!strcasesame(varName, "NB_COLOR")) 			{ found = 1; NB_COLOR = atoi( varValue ); }
      if (!strcasesame(varName, "MAX_NORM")) 			{ found = 1; MAX_NORM = atoi( varValue ); }
      if (!strcasesame(varName, "SAVE_ON_DISK")) 		{ found = 1; SAVE_ON_DISK = atoi( varValue ); }
      if (!strcasesame(varName, "DISPLAY")) 			{ found = 1; DISPLAY = atoi( varValue ); }
      if (!strcasesame(varName, "NO_SHOW")) 			{ found = 1; NO_SHOW = atoi( varValue ); }
      if (!strcasesame(varName, "MOD_LINEAIRE")) 		{ found = 1; MOD_LINEAIRE = atoi( varValue ); }
      if (!strcasesame(varName, "NB_SPIKENET")) 		{ found = 1; NB_SPIKENET = atoi( varValue ); }
      if (!strcasesame(varName, "WAIT_ENTER")) 			{ found = 1; WAIT_ENTER = atoi( varValue ); }
      if (!strcasesame(varName, "CONVERGENCE_CONVO")) 	{ found = 1; CONVERGENCE_CONVO = atoi( varValue ); }
      if (!strcasesame(varName, "NET_NAME_VERSION")) 	{ found = 1; NET_NAME_VERSION = atoi( varValue ); }
	  if (!strcasesame(varName, "STEP")) 				{ found = 1; STEP = atoi( varValue ); }
	  if (!strcasesame(varName, "MOD_TYPE")) 			{ found = 1; MOD_TYPE = atoi( varValue ); }       // modulation absolue ou relative
	  if (!strcasesame(varName, "ZOOM_CONVO")) 			{ found = 1; ZOOM_CONVO = atoi( varValue ); }
	  if (!strcasesame(varName, "PURE_ORDER")) 			{ found = 1; PURE_ORDER = atoi( varValue ); }
	  if (!strcasesame(varName, "BYTES_PER_PIXEL")) 	{ found = 1; BYTES_PER_PIXEL = atoi( varValue ); }
	  if (!strcasesame(varName, "BYTES_FILE")) 			{ found = 1; BYTES_FILE = atoi( varValue ); }
	  if (!strcasesame(varName, "SIZEX")) 				{ found = 1; SIZEX = atoi( varValue ); }
	  if (!strcasesame(varName, "SIZEY")) 				{ found = 1; SIZEY = atoi( varValue ); }
	  if (!strcasesame(varName, "REPLACE0_1")) 			{ found = 1; REPLACE0_1 = atoi( varValue ); }
	  if (!strcasesame(varName, "LEARN")) 				{ found = 1; LEARN = atoi( varValue ); }
	  if (!strcasesame(varName, "FOND")) 				{ found = 1; FOND = atoi( varValue ); }
	  if (!strcasesame(varName, "RESCUE")) 				{ found = 1; RESCUE = atoi( varValue ); }
	  if (!strcasesame(varName, "RECONSTRUCT_MOD")) 	{ found = 1; RECONSTRUCT_MOD = atoi( varValue ); }
	  if (!strcasesame(varName, "CONVERGE")) 			{ found = 1; CONVERGE = atoi( varValue ); }
	  if (!strcasesame(varName, "NEW_IMAGE_LIST")) 		{ found = 1; NEW_IMAGE_LIST = atoi( varValue ); }
	  if (!strcasesame(varName, "THRESHOLD_TYPE")) 		{ found = 1; THRESHOLD_TYPE = atoi( varValue ); }	  
      if (!strcasesame(varName, "MAX_TEMPS")) 			{ found = 1; MAX_TEMPS = atoi( varValue ); }
      if (!strcasesame(varName, "CONVERGE_VALUE")) 		{ found = 1; CONVERGE_VALUE = atoi( varValue ); }
      if (!strcasesame(varName, "LAYER_CONVERGE")) 		{ found = 1; LAYER_CONVERGE = atoi( varValue ); }
      if (!strcasesame(varName, "TIME_STOP"))	 		{ found = 1; TIME_STOP = atoi( varValue ); }
      if (!strcasesame(varName, "TESTRES"))	 			{ found = 1; TESTRES = atoi( varValue ); }
      if (!strcasesame(varName, "SIZE_RESP_ZONE"))	 	{ found = 1; SIZE_RESP_ZONE = atoi( varValue ); }
	// float      
      if (!strcasesame(varName, "MOD_CUT")) 			{ found = 1; MOD_CUT = atof( varValue ); }      
      if (!strcasesame(varName, "RAISE_THRESHOLD")) 	{ found = 1; RAISE_THRESHOLD = atof( varValue ); }
      if (!strcasesame(varName, "LOWER_THRESHOLD")) 	{ found = 1; LOWER_THRESHOLD = atof( varValue ); }
	  if (!strcasesame(varName, "TIME_BASE")) 			{ found = 1; TIME_BASE = atof( varValue ); }
	  if (!strcasesame(varName, "SPONTANEOUS")) 		{ found = 1; SPONTANEOUS = atof( varValue ); }
	// string
	  if (!strcasesame(varName, "DIR_SAVE_CONVOS")) 	{ found = 1; strcpy( DIR_SAVE_CONVOS, varValue ); }
	  if (!strcasesame(varName, "RESULTS")) 			{ found = 1; strcpy( RESULTS, varValue ); }
	  if (!strcasesame(varName, "DIR_SAVE_LEARN")) 		{ found = 1; strcpy( DIR_SAVE_LEARN, varValue ); }
	  
	  return found;
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
     	if (!NO_SHOW) printf("Value %s\tassigned to variable %s\n", value, descript);
     }
     else if (!STRICT_LOAD_ENV) assign( descript, value);
     strcpy( descript, value);
  } while ((eof1!=EOF));
  
  if (TIME_STOP == -1) TIME_STOP = MAX_TEMPS+MAX_COUCHE;
}

 static void init(char *net_env_name)
    {
      FILESTREAM console(CONSOLE);
      console >> MAIN_DIRECTORY;
      char new_net_env_name[100];
      sprintf(  new_net_env_name, "%s%s%c%s", CURRENT_DIR, MAIN_DIRECTORY, SEPARATEUR, net_env_name );
      
      load_env_var( new_net_env_name );
    }
 };

#endif

