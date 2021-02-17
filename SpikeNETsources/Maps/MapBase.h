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

// $Log: MapBase.h,v $

#include <stdlib.h>
#include <math.h>
#include "allDefs.h"
#include "allStructs.h"
#include "Weights.h"
#include "SpikeInputs.h"
#include "Sender.h"
#include "SpikeListsExtended.h"
#include "OutputDisplay.h"
#include "Rebuild.h"
#include "GlobalImgTreat.h"
#include "UniversalMap.h"

#ifndef _MAPBASE
#define _MAPBASE

// carte de base desquelles toute les autres dŽcoulent
// ---------------------------------------------------

class carte_base : public UniversalMap
{
	// int temps (inherited from universal map
	// all the virtual functions of UniversalMap are implemented (some of them as pure virtual)

public:
	SpikeListsExtended	spike_list;				// objet de gestion de liste de spike
  	long                ordre_decharge;			// nombre de dŽcharge dans la carte a un pas de temps donnŽ
  	long                last_ordre_decharge;	// nombre de dŽcharge dans la carte a un pas de temps prŽcŽdent

protected:
	// pour le motif reconnu
	GlobalImg   		*recog;					// gestion des images des convolution
	Rebuild				*rebuild;				// gestion des images sur laquelle la carte a dechargŽe

	SpikeInputs			map_SpikeInputs;		// gestion des spikes afferents (classe vitale)
	int					latencyReserv;			// nb de pas de temps ˆ rŽserver dans les liste de spike (peut etre non necessaire)
	int 				numero;					// numero de la carte
	char				name[30];				// nom de la carte
    short				zoom;					// facteur de zoom (/image initiale)
	int					FIXED;					// carte fixe (c'est a dire retine)
	short				convLockMap;    		// possede une convolution non verrouillee (qui apprend)

	//for network computation
	int 				virtMap;				// carte virtuelle (sur un autre ordinateur
	Sender    			*d_IP;					// reseau
	OutputDisplay		*disp;					// gestionnaire d'affichage de la carte

	// dimention sans les borders
    short				border_l;				// bordure de la carte en largeur
    short				border_h;				// bordure de la carte en largeur
    short       		largeur_norm;			// largeur sans les bordeur
    short       		hauteur_norm;			// hauteur sans les bordeur

	carte_base(int numero0, OutputDisplay *disp0, char *name0, int largeur_norm0, int hauteur_norm0, int zoom0)
	{
   		numero 	= numero0;
    	disp	= disp0;
    	strcpy(name, name0);
		d_IP = NULL;
		virtMap = 0;
		    	
    	zoom			= zoom0;
    	if (zoom >0)
    	{
    		largeur_norm 	= (int)ceil((double)largeur_norm0 / ( 1 << zoom));
    		hauteur_norm 	= (int)ceil((double)hauteur_norm0 / ( 1 << zoom));
    	}
    	else
    	{
    		largeur_norm = largeur_norm0 << -zoom;
    		hauteur_norm = hauteur_norm0 << -zoom;
    	}
    	
    	// fixe les borders pour la retine
    	recog    = NULL;
    	rebuild  = NULL;
    	border_l = 1;
    	border_h = 1;
    	convLockMap = 1;
        temps = 0;
        latencyReserv = 1;
        disp->adjustSize( largeur_norm, hauteur_norm );
   }
  
       
public:
	// fonction virtuelles pures communes ˆ toutes les cartes
    virtual void 			init_neuron(GlobalImgTreat **listImg, int supervParam) = 0;
    virtual void 			affiche() = 0;    
    virtual void 			detect_convolution(carte_base *une_carte_from, char *name_convolution, int convLock, double randFact, double multFact, double decFact, char *modFact, char *nameGroup, int duplicator, char *latency, int noZoom, double renorm) =0;
   	virtual void 			add_convolutions() = 0;
    virtual void 			resetComputeSync() = 0;
    virtual void 			resetComputeSyncLearn() = 0;
	virtual int 			compute_all() 		{	return 1;	} 
	virtual int 			learn_all()   		{ 	return 1;	}
	virtual int 			compute_pos()  	 	{ 	return 1;	}	
   	virtual FORMAT_COMPUTE	seuil_mod() 		{ 	return 0; 	}
   	virtual double 			seuil_mod( double d){ 	return 0;	}
   	virtual double 			seuil_mod_dicot( double d) { return 0;}
   	virtual void 			mult_seuil(double value) { }
   	virtual FORMAT_COMPUTE 	getMaxAct() { return 0; }
    virtual int  			modulateThreshold(int inc) { return 0; }
	virtual void 			setLearnCoord( int x, int y) { ERROR( 115,("cannot learn on this map %s", name)); }
	virtual void 			sizeLearn(int &sizeX_rebuild, int &sizeY_rebuild, int &BORDER_IMGL, int &BORDER_IMGH) {}
	virtual void 			putSpikeInBuffer(int sock0,  int mapNumber) { ERROR( 302,("cannot put spike in buffer, %s is not a virtual map", name)); }
	virtual proc 			*get_virtual()		{ return NULL; }
	virtual void 			reconstructConvo(int force) { ERROR( 307,("unreacheadable code")) }
	virtual FORMAT_COMPUTE 	getActivite( int x, int y) { ERROR( 307,("unreacheadable code")) return 0; }
	virtual void 			writeStats() { }
	virtual void 			writeRebuild() { if (rebuild != NULL) { char fileName[100]; sprintf( fileName, "%s%s%cRebuild%s", CURRENT_DIR, EnvVar::DIR_SAVE_REBUILD, SEPARATEUR, name); rebuild->write(fileName);} }
   	virtual void 			renormConvos(double val=0) { }
   	
	// fonction prorietaire   	
	int  saveAllConvos(int force);
	void add_virtual() { virtMap+=1; }
    int  get_eff_virtual() { return virtMap; }
    void maxLatency( int maxLatency0 ) { if ( latencyReserv < maxLatency0 ) latencyReserv = maxLatency0; }
    void saveOnDisk( char *imageName ) { if (EnvVar::SAVE_ON_DISK) disp->writeOnDisk( name, imageName ); }
    int getColor() { if (disp != NULL) return disp->colorCst; else return 0; }
    BaseImg *getImageAssociated() { if (disp != NULL) return disp->imgAssociated; else return NULL; }

	// fonction pour ecrite les liste de spike pour le logiciel NEURON
	void write_spikelists( int imgNb ) { char fileName[100]; sprintf( fileName, "%s%s%c%sVect_%s_%.4d", CURRENT_DIR, EnvVar::DIR_SAVE_REBUILD, SEPARATEUR, EnvVar::SAVE_TMP, name, imgNb); spike_list.write( EnvVar::MAX_TEMPS, fileName); sprintf( fileName, "%s%s%cSpikeList_%s", CURRENT_DIR, EnvVar::DIR_SAVE_REBUILD, SEPARATEUR, name); spike_list.writeCount( EnvVar::TIME_STOP, fileName); }
	//void write_spikelists( int imgNb ) { char fileName[100]; sprintf( fileName, "%s%s%cSpikeList_%s", CURRENT_DIR, EnvVar::DIR_SAVE_REBUILD, SEPARATEUR, name); spike_list.writeCount( EnvVar::TIME_STOP, fileName); }
    
	// methodes d'acces
	int		get_couche()    	{ return disp->getLayer(); }
	int		get_number()    	{ return numero; }
	int		get_largeur_norm() 	{ return largeur_norm; }
	int		get_hauteur_norm() 	{ return hauteur_norm; }
	int     get_zoom()			{ return zoom; }
	int		get_border_l()		{ return border_l; }
	int		get_border_h()		{ return border_h; }
	int		get_size_x_recog()	{ return recog->getWidth(); }
	int		get_size_y_recog()	{ return recog->getHeight(); }
    BaseImg			*getResult()		{ return (BaseImg *) disp; }
	char 			*get_name()     	{ return name; }
	int 			get_FIXED() 		{ return FIXED; }
	int 			get_convLockMap() 	{ return convLockMap; }
	unsigned char   *get_recog()		{ return recog->Im; }
	void			addEfferentMap(proc *remoteProc)
	{
		if (d_IP == NULL) d_IP = new Sender();
		d_IP->addEfferent( remoteProc );
	}

};

#endif