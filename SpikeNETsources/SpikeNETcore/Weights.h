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

// $Log: Weights.h,v $


#ifndef _POIDS_ARRAY
#define _POIDS_ARRAY

#include "WeightLine.h"
#include "GlobalImg.h"
#include "TabWeight.h"

class SpikeInputs;

// this class represent a receptive field between two maps
// -----------------------------------------------------

class Weights : public GlobalImg
{
//friend SpikeInputs;
// to do for the latency
// 1 - put the number of latency into nbLatency
// 2 - make an array of latency (0, 3, 6) for instance
// 3 - update the function getLatency with these values
// 3 - reserv the convolutions
// make the maps send latencyReserv number of spikes

protected:
  int             nbPoids;		// number of weight in the convolution
  int			  nbZooms;		// number of sub convolution between the two maps (depend on the zoom)
  int			  zoom;			// zoom octave difference between the two maps (afferent and efferent)
  int			  totalLines;	// total number of WeightLines
  int             convLock;		// 1 if the convolution is locked (no learning) 0 otherwise
  int			  duplicator;	// 1 if there 1 convolution per neuron 0 otherwise (does not work anymore)
  int             nbLatency;	// latency associated with the convolution (does not work)
  int			  noZoom;		// flag (0 or 1) 
  double          randFact;
  double          multFact;
  double          decFact;
  double		  renorm;

  WeightLine      *tabPoidsLine[MAX_LINE_POIDS];
  
  unsigned short tabOffset[MAX_LINE_POIDS];
  WeightLine	 *zoomOffset[MAX_ZOOM*MAX_ZOOM];
  unsigned short *tabZoomOffset[MAX_ZOOM*MAX_ZOOM];
  int            nbLines[MAX_ZOOM*MAX_ZOOM];
  WeightLine     *allTabPoidsLine;
  int            sizeTabPoidsLine;
  char	         *fileName;
  int            noModif;
  char 			 newname[100];
  GlobalImg	 	 *latencyMap;

  // for calculus
  WeightLine 	 *ptrArrayLine;
  int			 watchAdded;
  TabWeight		*tabWeight;

  // access
public:
  int inline 			getConvLock() { return convLock; }
  int inline 			getDuplicator() { return duplicator; }
  int inline            getNbPoids() { return nbPoids; }
  int inline			getSizeTabPoidsLine() { return sizeTabPoidsLine; }
  int inline			getTotalLines() { return totalLines; }
  int inline			getNbZooms() { return nbZooms; }
  int inline			getLargeur() { return width; }
  int inline			getHauteur() { return height; }
  int inline            getNbLatency() { return nbLatency; }
  double				getRenorm()    { return renorm; }
  double				getRandFact() { return randFact; }
  char					*getFileName() { return fileName; }
  WeightLine inline		*getZoomOffset(int tmp_offset, int latency) { return zoomOffset[tmp_offset]; }
  unsigned short inline	*getTabZoomOffset(int tmp_offset) { return tabZoomOffset[tmp_offset]; }
  FORMAT_POIDS			getTabTemp(int tmp_offset) { return ImInt[tmp_offset]; }
  FORMAT_POIDS			*getImIntPtr(int tmp_offset) { return ImInt+tmp_offset; }
  int inline            getNbLines(int tmp_offset) { return nbLines[tmp_offset]; }

  Weights() : GlobalImg() {     ImInt = NULL; 	Im = NULL; }
  int  detect_convo( int *largeur, int *hauteur, char *fileName0, int convLock0, double randFact0, double multFact0, double decFact0, int duplicator, char *latency, int noZoom0, double renorm0);
  FORMAT_POIDS totalPoids();
  void init_convo  ( int largeur_carte, int hauteur_carte, int factZoomConv);
  int  getLatency( int lat ) { return 0; }
  void affiche() { /*printf("%d\n", nbLines); */for (int i=0; i<totalLines; i++) tabPoidsLine[i]->affiche(); }
  void duplicate(int largeur_tot, int hauteur_tot, int largeurCarte, int hauteurCarte);
  void duplicate2(int largeur_tot, int hauteur_tot);
  void saveConvos( char *name, char *dest, int force);
  void saveAllConvos(int largeurTot, int hauteurTot, int borderL, int borderH);
  void decrementeAll( double bottom, int largeurTot, int hauteurTot );
  void zoomPos(int largeur_carte);
  void zoomNegIter();
  void conv_order(char *name, double fact_modulation);
  int  addToConvo(FORMAT_POIDS *tab, int tabLarg, int tabHaut, int tabCentX, int tabCentY, int largeur_carte);
  void unZoom(FORMAT_POIDS *tab, int tabLarg, int tabHaut, int tabCentX, int tabCentY, int tmp_zoom);
  void renormalizeLines( FORMAT_POIDS tot);
  void renormalizeLinesAbsolute( FORMAT_POIDS renorm );
  double computeVar();
  double computeMean();
  void watchConvo( char *source, char *dest);
  virtual int update() { return 0; }
};

typedef Weights *ptr_Weights;
#endif
