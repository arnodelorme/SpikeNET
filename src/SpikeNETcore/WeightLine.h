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

// $Log: WeightLine.h,v $

// *********************
// weight base structure
// *********************
#ifndef _WEIGHT_LINE
#define _WEIGHT_LINE
#include "allDefs.h"

// this class deals with contiguous synaptic weights
// -------------------------------------------------

class WeightLine
{
  public:

  int        	taille;		// size of the line
  long          offset;		// offset of the beginning of the line IN THE MAP (depend on the size of the map) compared to center of the convolution
  short         offsetX;	// X offset in the convolution
  short         offsetY;	// Y offset in the convolution
  FORMAT_POIDS  *tabPoids;	// array of weight
  FORMAT_POIDS  firstPoids; // easy acces to the first synaptic weight
    
  void constructeur(int taille0, int offset0, short offsetX0, short offsetY0, FORMAT_POIDS *poidsDeLigne) {
      taille       = taille0;
      offset       = offset0;
      offsetX      = offsetX0;
      offsetY      = offsetY0;
      tabPoids = &firstPoids;
      for (int i = 0; i < taille; i++) tabPoids[i] = poidsDeLigne[i];
    }
  void affiche()
    {
      printf("%d %d\t->%d :", offsetX, offsetY, taille);
      //for (int i = 0; i < taille; i++) printf("%3.2f\t", tabPoids[i] );
      for (int i = 0; i < taille; i++) printf("%4d\t", tabPoids[i] );
      printf("\n");
      //for (int i = 0; i < taille; i++) cout << tabPoids[i] << "\t";
      //cout << endl;
    }
  void reset() { for (int i = 0; i < taille; i++) tabPoids[i] = 0; taille = 0; offset = 0;}
  void add(WeightLine *pl) { for (int i = 0; i < taille; i++) tabPoids[i] += *(&(pl->firstPoids) + i); }
//  void alea(double randFact) { for (int i = 0; i < taille; i++) tabPoids[i] = (FORMAT_POIDS)((double)tabPoids[i] * (1 + (2*(double)rand()/RAND_MAX-1) * randFact)); }
  void alea(double randFact) { for (int i = 0; i < taille; i++) tabPoids[i] = (FORMAT_POIDS)((double)tabPoids[i] * (1 + (double)rand()/RAND_MAX * randFact)/(1 + randFact)); }
  void recopi(WeightLine *source) { 
  	taille = source->taille;
  	offset = source->offset;
    tabPoids = &firstPoids;
  	for (int i = 0; i < taille; i++) tabPoids[i] = source->tabPoids[i]; 	
  }
};

#endif