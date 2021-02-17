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

// $Log: EnvVarOld.h,v $

//#include <fstream.h>
#include <string.h>
#include <stdlib.h>
#include "allDefs.h"

#ifndef _ENVVAR
#define _ENVVAR

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

private : 
static char *load_env_str(char *s, char *net_env_name, char *defaultvalue)
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
}

public :
 static void init(char *net_env_name)
    {
      FILESTREAM console(CONSOLE);
      console >> MAIN_DIRECTORY;
      char new_net_env_name[100];
      if (console.get() != EOF) { console >> new_net_env_name; if (new_net_env_name[0]) strcpy( RESULTS, new_net_env_name); }
      strcpy( RESULTS, DEFAULT_RESULTS);
      if (console.get() != EOF) { console >> new_net_env_name; if (new_net_env_name[0]) strcpy( DIR_SAVE_CONVOS, new_net_env_name); }
      if (console.get() != EOF) { console >> new_net_env_name; if (new_net_env_name[0]) strcpy( DIR_SAVE_LEARN, new_net_env_name); }
      
      sprintf(  new_net_env_name, "%s%s%c%s", CURRENT_DIR, MAIN_DIRECTORY, SEPARATEUR, net_env_name );
      
      // variables d''environnement
      NB_COLOR      = load_env_int("NB_COLOR", new_net_env_name, DEFAULT_NB_COLOR);
      MAX_TEMPS     = atoi(load_env_str("MAX_TEMPS", new_net_env_name, (char*)NULL));
      MAX_NORM      = load_env_int("MAX_NORM", new_net_env_name, DEFAULT_MAX_NORM*MAX_TEMPS);
      SAVE_ON_DISK  = load_env_int("SAVE_ON_DISK", new_net_env_name, DEFAULT_SAVE);
      DISPLAY  		= load_env_int("DISPLAY", new_net_env_name, 1);
      NO_SHOW       = load_env_int("NO_SHOW", new_net_env_name, DEFAULT_SHOW);
      //RET_CONTRASTE = load_env_int("RET_CONTRASTE", new_net_env_name);
      //RESULTS       = load_env_str("RESULTS", new_net_env_name, DEFAULT_RESULTS);
      MOD_LINEAIRE  = load_env_int("MOD_LINEAIRE", new_net_env_name, DEFAULT_MOD);
      MOD_CUT       = atof(load_env_str("MOD_CUT", new_net_env_name, DEFAULT_MOD_CUT));
      NB_SPIKENET   = load_env_int("NB_SPIKENET", new_net_env_name, 1);
      WAIT_ENTER    = load_env_int("WAIT_ENTER", new_net_env_name, 0);
      CONVERGENCE_CONVO = load_env_int("CONVERGENCE_CONVO", new_net_env_name, DEFAULT_CONV_CONVO);
      RAISE_THRESHOLD = atof(load_env_str("RAISE_THRESHOLD", new_net_env_name, DEFAULT_RAISE_THRE));
      LOWER_THRESHOLD = atof(load_env_str("LOWER_THRESHOLD", new_net_env_name, DEFAULT_LOWER_THRE));
      NET_NAME_VERSION= load_env_int("NET_NAME_VERSION", new_net_env_name, DEFAULT_NAME_VERSION);
	  STEP			  = load_env_int("STEP", new_net_env_name, 1);
	  MOD_TYPE	      = load_env_int("MOD_TYPE", new_net_env_name, DEFAULT_MODABS);       // modulation absolue ou relative
	  ZOOM_CONVO 	  = load_env_int("ZOOM_CONVO", new_net_env_name, DEFAULT_ZOOM_CONVO);
	  PURE_ORDER 	  = load_env_int("PURE_ORDER", new_net_env_name, DEFAULT_PURE_ORDER);
	  BYTES_PER_PIXEL = load_env_int("BYTES_PER_PIXEL", new_net_env_name, DEFAULT_BYTES_PER_PIXEL);
	  BYTES_FILE 	  = load_env_int("BYTES_FILE", new_net_env_name, 1);
	  SIZEX	  		  = load_env_int("SIZEX", new_net_env_name, DEFAULT_SIZEX);
	  SIZEY	          = load_env_int("SIZEY", new_net_env_name, DEFAULT_SIZEY);
	  REPLACE0_1	  = load_env_int("REPLACE0_1", new_net_env_name, DEFAULT_REPLACE0_1);
	  LEARN			  = load_env_int("LEARN", new_net_env_name, DEFAULT_LEARN);
	  FOND			  = load_env_int("FOND", new_net_env_name, 1);
	  RESCUE		  = load_env_int("RESCUE", new_net_env_name, 0);
	  RECONSTRUCT_MOD = load_env_int("RECONSTRUCT_MOD", new_net_env_name, 0);
	  CONVERGE		  = load_env_int("CONVERGE", new_net_env_name, 0);
	  TIME_BASE		  = atof(load_env_str("TIME_BASE", new_net_env_name, DEFAULT_TIME_BASE));
	  SPONTANEOUS     = atof(load_env_str("SPONTANEOUS", new_net_env_name, "0"));
	  NEW_IMAGE_LIST  = load_env_int("NEW_IMAGE_LIST", new_net_env_name, 0);
	  BRIME_LIB		  = 0;
	  LEARN_SUPERV	  = NO;
	  THRESHOLD_TYPE  = load_env_int("THRESHOLD_TYPE", new_net_env_name, 0);
    }
  
/* void static check()
 {
 	if (MAX_TEMPS > MAX_NORM)
 		ERROR(0,("MAX_TEMPS higher than MAX_NORM"));
 }*/
};

#endif

