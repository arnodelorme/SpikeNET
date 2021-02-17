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

// $Log: Map.h,v $


#ifndef _MAP
#define _MAP

#include "MapBase.h"
#include "SpikeStat.h"

// ******************
// classe carte
// ******************

typedef short *ptr_short;

class carte : public carte_base
{

//friend carte;

  public:
  
  	// variables d'ordre general
  	double                seuil_relatif;		// seuil en %
  	FORMAT_COMPUTE       seuil;					// seuil reel commun a toute la carte
  	char                 *fact_modulation;		// facteur de modulation
  	double				 realDecay;				// decay de la carte en % je crois
    int				     modulT;

	// reseau
	int					 mapTreated;			// nb de carte affŽrentes traitŽes (reseau)
    int                  nbLooping;				// nb de bouclage dans le compute (attente d'une info)
	
    // dimensions
    short       		largeur_tot;			// largeur totale de la carte
    short       		hauteur_tot;			// hauteur totale de la carte
    short       		offset_debut;			// offset du dŽbut des neurones non dŽsactivŽs
    short				separate;				// seuils separes

	// tableaux
 //   coord_array          tab_spike1;
 //   coord_array          tab_spike2;
    
    // pour la computation
    short 		 		 *state[MAX_GROUP]; 	// etats pour la modulations (groups = modulation separŽe)
    FORMAT_COMPUTE       *threshold;			// seuils separes pour tous les neurones
    unsigned short       *nb_decharge_from;		// nb de decharge affŽrente pour chaque neurone
    FORMAT_COMPUTE       *act_tmp;				// activite temporaire (inutilise je crois)
    FORMAT_COMPUTE       *activite;				// activite des neurones
    unsigned short       *update;				// derniere update du neurone (pour les seuils variables)
    FORMAT_COMPUTE       *decay;				// tableau pour le decay
    FORMAT_POIDS		 *learning;				// tableau pour l'apprentissage non supervise
    FORMAT_POIDS		 *learningPtr;			// raccourci dans le tableau precedent
    
#if LARGE_MAPS == OUI
    unsigned short       *tab_x;				// coordonne x du neurone en fonction de son offset
    unsigned short       *tab_y;				// coordonne y du neurone en fonction de son offset
    unsigned int         tab_modified_MMX_size; // nombre de neurones modifiŽ en ce moment
    unsigned int         *tab_modified_MMX;		// tableau contenant les offsets des neurones modifiŽ en ce moment
#else
    unsigned char        *tab_x;
    unsigned char        *tab_y;
    unsigned short       tab_modified_MMX_size; 
    unsigned short       *tab_modified_MMX;
#endif

    //pour l''apprentissage
    int      			convSpeed;				// vitesse de convergence
    int					convLockConvo;			// existe-t'il des convolutions non verouillŽes
	    
  public:
    // methodes proprietaires
    carte(int numero0, OutputDisplay *disp0, char *name0, int largeur_norm0, int hauteur_norm0, double seuil0, char *fact_modulation0, int zoom0, double decay0, int convSpeed0);
	double printState(double act, int t, int n, int s,double p,  double v);
    void *recopie(carte *objet);
    void add_convolutions();
    void detect_convolution(carte_base *une_carte_from, char *name_convolution, int convLock, double randFact, double multFact, double decFact, char *modFact, char *nameGroup, int duplicator, char *latency, int noZoom, double renorm);
    void write();
	void compute_decay(double decay0, int max_time);
	//~carte();

   	// methodes virtuelles
   	virtual FORMAT_COMPUTE 	seuil_mod();
	virtual double 			seuil_mod( double new_seuil_relatif );
	virtual double 			seuil_mod_dicot( double new_seuil_relatif );
    virtual int  			modulateThreshold(int inc) { int lastMod = modulT; if (inc) modulT+=inc; else modulT = 0; return lastMod; }
	virtual FORMAT_COMPUTE 	getActivite( int x, int y) { return activite[offset_debut + x + y *largeur_tot]; }
    virtual void 			init_neuron(GlobalImgTreat **listImg, int supervParam);
	virtual int  			compute_all();
    virtual	int  			learn_all();
	virtual int  			compute_pos();
	virtual void 			affiche();
    virtual void 			resetComputeSync();
    virtual void 			resetComputeSyncLearn();
   	virtual void 			mult_seuil( double value ) { seuil = (FORMAT_COMPUTE)(seuil * value); }
   	virtual FORMAT_COMPUTE 	getMaxAct();
   	virtual void 			renormConvos(double val=0) { map_SpikeInputs.renormConvos( val); }
    virtual void 			writeStats() { 
    	if (map_SpikeInputs.spikeStat[0] != NULL)
    		if (((name[0] == 'x') || (name[0] == 'X')) && (name[1] == '_'))
				 { char fileName[100]; sprintf( fileName, "%s%s%cLUT%d%s", CURRENT_DIR, EnvVar::DIR_SAVE_STATS, SEPARATEUR, zoom, name+1); map_SpikeInputs.spikeStat[0]->writeStats( fileName); }	
    		else { char fileName[100]; sprintf( fileName, "%s%s%cLUT%s", CURRENT_DIR, EnvVar::DIR_SAVE_STATS, SEPARATEUR, name); map_SpikeInputs.spikeStat[0]->writeStats( fileName); }
	}
	virtual void reconstructConvo( int force);

	// special apprentissage
	void 		 sizeLearn(int &sizeX_rebuild, int &sizeY_rebuild, int &BORDER_IMGL, int &BORDER_IMGH);
	void 		 teste_si_decharge(coord_array **liste_out, int temps);
	//void init_neuron_learn();
 	//void modif_seuil(double raise_threshold, double lower_threshold);
	//virtual void sizeLearn(int &sizeX_rebuild, int &sizeY_rebuild, int &BORDER_IMGL, int &BORDER_IMGH);
};

// ces autres cartes sont des versions ou seul change le compute
// peut etre faudrait il par exemple mettre la modulation dans la carte qui gere cela plutot que dans la carte de base

class carte_modul : public carte
{
public:
  carte_modul(int numero0, OutputDisplay *disp0, char *name0, int largeur_norm0, int hauteur_norm0, double seuil0, char *fact_modulation0, int zoom0, double decay0, int convSpeed0) : 
    carte(numero0, disp0, name0, largeur_norm0, hauteur_norm0, seuil0, fact_modulation0, zoom0, decay0, convSpeed0) {}
  virtual int  compute_all();
};	

class carte_modul_decay : public carte
{
public:
    carte_modul_decay(int numero0, OutputDisplay *disp0, char *name0, int largeur_norm0, int hauteur_norm0, double seuil0, char *fact_modulation0, int zoom0, double decay0, int convSpeed0) : 
		carte(numero0, disp0, name0, largeur_norm0, hauteur_norm0, seuil0, fact_modulation0, zoom0, decay0, convSpeed0) {}
	virtual int  compute_all(); 
};	

class carte_decay : public carte
{
public:
    carte_decay(int numero0, OutputDisplay *disp0, char *name0, int largeur_norm0, int hauteur_norm0, double seuil0, char *fact_modulation0, int zoom0, double decay0, int convSpeed0) : 
		carte(numero0, disp0, name0, largeur_norm0, hauteur_norm0, seuil0, fact_modulation0, zoom0, decay0, convSpeed0) {}
	virtual int  compute_all(); 
};		

class carte_learn_superv : public carte
{
	int convergingFactor;

public:
    carte_learn_superv(int numero0, OutputDisplay *disp0, char *name0, int largeur_norm0, int hauteur_norm0, double seuil0, char *fact_modulation0, int zoom0, double decay0, int convSpeed0) : 
		carte(numero0, disp0, name0, largeur_norm0, hauteur_norm0, seuil0, fact_modulation0, zoom0, decay0, convSpeed0) { convergingFactor = -1; }

	virtual void setLearnCoord( int x, int y);
    virtual void init_neuron(GlobalImgTreat **listImg, int supervParam);
	virtual int  compute_all(); 
};

// les cartes buffers sont des cartes qui gerent les liste de decharge en provenance d'un autre processus
//#include "mapBuffer.h"
#endif
