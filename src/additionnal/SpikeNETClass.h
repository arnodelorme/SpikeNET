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

// $Log: SpikeNETClass.h,v $

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

// $Log: SpikeNETClass.h,v $


#ifndef _SPIKECLASS
#define _SPIKECLASS

// include global
#include <stdlib.h>
#include <string.h>
#include <math.h>

// include local
#include "allDefs.h"
#include "allStructs.h"
#include "Map.h"
#include "MapList.h"
#include "Estimer.h"

class SuperSpikeClass;
class SpikeLispSpike;
class GlobalImg;

// Classe gérant les différents SpikeNET (pour le multiscale)
// son utilité est très limité avec SuperSpikeClass (uniquement les Estimateurs et les Maplist pour chaque echelle)

class SpikeClass
{

private:

  // pour les variables d'environnement
  SuperSpikeClass		*pappy;
  int					scaleType;						// type de carte (multiscale ou pas
  int					fact_zoom_SpikeNET;				// facteur de zoom du spikeNET courant (sert à l'initialisation)
  int					spikeNET_num;					// numero du spikeNET courant
   
  //  variables essentielles	
  int 					size_x, size_y;							// taille de base de l'image
  int					nb_carte;								// nombre de cartes

public:
  FILE          		*fileOutput;							// fichier de sortie contenant la configuration du reseau
  carte_base			*tab_carte[MAX_CARTE];					// tableau des cartes (redondant avec celui de SuperSpikeClass
  Estimer				*estimer;								// estimateur du nombre de décharge dans chaque couche

private:

  // apprentissage
  int					layer_learn;							// couche d'apprentissage
    
public:
    
  void   		SpikeClassInit ( MapList *mapList);
  SpikeClass(int largeur, int hauteur, int fact_zoom, int OffsetEcran, EnvVar *env0, int scaleType0, SuperSpikeClass	*pappy0);

  // a terme il faudrait supprimer ces methodes pour les replacer dans SuperSpikeClass
  carte_base *findInside(char *name);
  void LoadImage();
  void Compute( int temps );
  void Learn( int temps );
  void SaveOnDisk(char *ImageName, int count_image);
  void ResetImageLearn();
  void modifMap( double value, int layer);
  void actualiseSession( double modulator);
  void addMap( carte_base *ptr) { tab_carte[ nb_carte++ ] = ptr; }

  //SpikeClass(char *ImageName0, int fact_zoom, int OffsetEcran, EnvVar *env0, int scaleType0, SuperSpikeClass *pappy0);
  //SpikeClass(int largeur, int hauteur, int fact_zoom, EnvVar *env0, int scaleType0, SuperSpikeClass	*pappy0);
  //void LoadImage(unsigned char *addresse);
  //carte_base *addMap(int couche, char *name, int mapNumber, double seuil, double mod, double estimer, int zoomer, double decay, int coordx, int coordy, int zoomDsp, char *learn, proc *local, char *strLisp, char *object, int convSpeed);
  //SpikeLispSpike *treatLisp(char *str, double spikePC);

};

#endif
