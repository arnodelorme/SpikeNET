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

// $Log: SuperSpikeNETClass.h,v $


#ifndef _SUPERSPIKECLASS
#define _SUPERSPIKECLASS

#include "MapList.h"
#include "ImageList.h"
#include "ImageListRich.h"
#include "ProcManager.h"
#include "ScreenSpike.h"
#include "WatchAct.h"
#include "SpikeNETlib.h"

#if SYSTEME == MAC
#include "mytimer.h"
#endif

// *****************
// classe principale
// *****************

// amélioration : faire une carte parente pour regarder l'activité (wactAct)
//				: faire une carte parente pour gerer l'apprentissage supervisé
//				: faire une carte parente pour gerer le réseau

class SuperSpikeClass
{

	//all maps
	carte_base			*tab_carte[MAX_CARTE];			// tableau des cartes
	int					nbMaps;							// nb de cartes

	// multiechelle
	//SpikeClass 			*SpikeNet[MAX_SPIKENET];		// les spikeNETs - for NIH static is better
	MapList 			*SpikeNet[MAX_SPIKENET];		// les spikeNETs - for NIH static is better
	int 				networkType;					// type de résau (bit 1 = 
	int					numberSpikeNET;					// nb de spikeNET en parrallèle	
	
	// reseau
	ProcManager			*procManager;					// processus manager pour disribuer les cartes dans le cas d'un reseau
	Server				*server;						// caracteristiques du serveur pour le réseau

	// divers
	ImageListLearn		*img_list;						// gestionnaire d'image	
	ScreenSpikeAuto  	*screen0;						// écran principal
	
	// apprentissage supervisé
	int 				increaseT;						// pour propager plusieurs fois les images, optimisation des seuils dans l'apprentissage supervisé
	int					number2discharge[MAX_CARTE];	// nb d'images sur laquelle une carte doit décharger
	int					numberGoodIn[MAX_CARTE];		// nb d'images sur laquelle la carte devait décharger et a decharge dans un carre predefini
	int					numberBadIn [MAX_CARTE];		// nb d'images sur laquelle la carte ne devait pas décharger et a decharge dans un carre predefini
	int					numberGoodOut[MAX_CARTE];		// nb d'images sur laquelle la carte devait décharger et a decharge a l'exterieur d'un carre predefini
	int					numberBadOut [MAX_CARTE];		// nb d'images sur laquelle la carte ne devait pas décharger et a decharge a l'exterieur d'un carre predefini

	//WatchAct			*watchAct;						// optionnel pour étudier l'activité des cartes

  	// la mapList sert a la propagation
  	//MapList				mapList;

#if SYSTEME ==MAC
	timer				Timer;							// timer pour le mac (mais maintenant outdated car il y a le Profiler	
#endif

// private : initialisation des cartes...
  	void   				read_projs     ();
  	void    			read_maps      ( BaseImg *baseImg );
	int					readMulti(char *var, char *tmp, FILESTREAM &map_names);

public:

	SuperSpikeClass(int argc, char *argv[]);
	SuperSpikeClass( char *directoryExec, char *directory, BaseImg *baseImg, int mode );
	
	// fonction vitales
    void init( BaseImg *baseImg);
	void setMapLearn();
	int determineMultiscale();
	void findMap(char *name, int spikeClassNb, carte_base **tabRes);
  	SpikeLispSpike *treatLisp(char *str, double spikePC);
	void compute();
	void learn( BaseImg *Im, int coordx, int coordy);
	void saveOnDisk(char *ImageName, int count_image);
	void run();	
	~SuperSpikeClass() {
        //for (int i=0; i< numberSpikeNET; i++) delete SpikeNet[i];
    	//delete img_list, env, procManager;
    }
    
    // fonction moins vitales à supprimer eventuellement
	void out_of_memory() {	ERROR(1,("out of memory")); }
	carte_virtual *getMap( int number) { return (carte_virtual *)((void *)tab_carte[number]); }
    void resetResBrime();
    void testResBrime(int temps);
	BaseImg *compute( BaseImg *Im);
	void actualiseSession( int modulator);
	void changeInFile( char *strName, double newT );
	void testResults();
	void initTextWindow();
	
	// non necessaire a une propagation et une compilation normale 
	//void modifyThreshold( double value) { tab_carte[ nbMaps-1 ]->seuil_mod( value ); }
	//void propagateOnly() { for (int i=0; i<nbMaps; i++) tab_carte[ i ]->mult_seuil( 2 ); }
};

#endif
