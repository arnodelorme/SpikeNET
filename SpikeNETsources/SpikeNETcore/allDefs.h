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

// $Log: allDefs.h,v $

// tous les define
// ***************
//#include <fstream.h>
//#include <iostream.h>

#ifndef _ALLDEFINE
#define _ALLDEFINE

#include "operatingsystem.h"

// pour les convolutions
#define DIRECTORY       ""
#define BOOLEAN         unsigned char
#define	DETECT			1
#define RESERV			0
#define INACTIF         0
#define ACTIF           1
#define OUT_LISTE       0
#define IN_LISTE        1
#define OUI				1
#define NON				0
#define YES				1
#define NO				0
#define GEOMETRIQUE		0
#define LINEAIRE        1
#define INTERNAL        1
#define EXTERNAL        0
#define MULTISCALE		2
#define ONESCALE		1

/* *********************************** */
/*     les defines commencent ici      */
/* *********************************** */

// mode de fonctionnement de SpikeNET
#define MODE    		EXTERNAL
#define DEBUG			NON
#define STRICT_LOAD_ENV NON

// calculus precision
#define RENORM           8         // decalage en bit
#define FORMAT_COMPUTE   long      // mode de calcul
#define FORMAT_POIDS     double    // mode de stockage des poids
#define COMPUTE_CUT		 OUI	   // calcule en mode cut
#define LARGE_MAPS       OUI	   // pour des image de plus de 200
#define BORDER_RETINA	 1

// parametre divers
//#define DISPLAY			 OUI

// limites de SpikeNET
#define MAX_IMAGE       16200       // nombre maximum d image traitable (ok : 300)
#define MAX_SPIKE       10000       // nombre maximum de spike par pas de temps et pas carte (ok : 10000)
#define MAX_MODIFIED    60000       // nombre maximum de neurones mis a jour par pas de temps et par carte (ok : 60000)
#define MAX_CARTE_FROM  150         // nombre maximum de projection par carte (ok : 30)
#define MAX_GROUP		60
#define MAX_CARTE		1100         // nombre maximum de carte (ok : 30)
#define MAX_COUCHE		8          // nombre maximum de couche (ok : 8)
#define MAX_ZOOM		4
#define MAX_LINE_POIDS  500        // nombre maximum de ligne de poids
#define MAX_SPIKENET	6          // nombre maximum de SpikeNET
#define MAX_SPIKE_LIST	10
#define MAX_IMG_AFFICH	1000
#define MAX_SCREEN		10

// pour l''affichage
//#define ZOOM_DISP 		1         // zoom pour l'afichage des cartes
//#define MAP_PER_ROW 	2          // nombre de carte par colonne (ici decalage <<)
#define INTER_HAUT		1          // espacement des cartes en hauteur
#define INTER_LARG		1          // espacement des cartes en largeur
#define INTER_MENU		1          // distance au menu

//aprentissage
#define SAVE_LEARN 		NON
#define SPIKESTAT		NON
#define EXTERNAL_GRAB	OUI
#define REAL_RETINAL_CONTRAST	OUI
//#define MARKRAM			OUI
//#define ABS_CONV		OUI
//#define REPLACE0_1      NON
//#define NORMALISE_LEARN 1000

//parametres par defaut
#define DEFAULT_NB_COLOR    	256
#define DEFAULT_MAX_NORM     	4
#define DEFAULT_SAVE         	NON
#define DEFAULT_SHOW         	NON
#define DEFAULT_MAX_TEMPS       1000
#define DEFAULT_RESULTS      	"Results"
#define DEFAULT_MOD          	GEOMETRIQUE
#define DEFAULT_MOD_CUT      	"1"
#define DEFAULT_NAME_VERSION 	0
#define DEFAULT_CONV_CONVO   	30
#define DEFAULT_RAISE_THRE    	"0.5"
#define DEFAULT_LOWER_THRE    	"0.1"
#define DEFAULT_MODABS		  	NON        // modulation absolue ou relative
#define DEFAULT_ZOOM_CONVO 	  	0          // zoom pour l'afichage/sauvegarde des convos
#define DEFAULT_DIR_SAVE_CONVOS "save_convos"
#define DEFAULT_DIR_SAVE_LEARN  "save_learn"
#define DEFAULT_DIR_SAVE_STATS  "save_stats"
#define DEFAULT_DIR_SAVE_REBUILD "save_rebuild"
#define DEFAULT_PURE_ORDER       OUI       // traitement dans la retine
#define DEFAULT_CORRELATE_RET	 NON	   // correlation des ON et des OFF
#define DEFAULT_ON_EQ_OFF		 NON       // toujours le meme nb de spike en ON et en OFF meme si truncation
#define DEFAULT_BYTES_PER_PIXEL	 4
#define DEFAULT_SIZEX			 0
#define DEFAULT_SIZEY			 0
#define DEFAULT_REPLACE0_1		 NON
#define DEFAULT_LEARN			 NON
#define DEFAULT_TIME_BASE		 "0.1"
#define BRIMEFILE				 "brimelearn"
#define ARCHIVE_NAME			 "SpikeArchive"

// network
#define MAX_SPIKE_BUFFER	 10000          // nombre maximum de SpikeNET
#define BUFFER_NETWORK		 600000
#define MAX_MSG_SIZE         10000
#define PORT                 2001

//spikeLisp
#define MAX_OPER 			50
#define MAX_IMG_LIST		20
#define DEFAULT_THRESHOLD	-10000000

/* *********************************** */
/*     et ils se terminent la          */
/* *********************************** */
#define STRSTREAM	   istrngstream

#if SYSTEME == PC
#define rint(X)		   (int)(((X + 0.5) == (int)X) ? (int)X : (int)X+1)
#endif

#if LARGE_MAPS == OUI
#define INTEGERPOS		long
#else
#define INTEGERPOS		short
#endif

#if MODE == INTERNAL
//#define FILESTREAM STRSTREAM
#define FILESTREAM STRSTREAM
#define LST_IMG	  lst_img
#define NET_NAMES net_names
#define NET_PROJS net_projs
#define NET_ENV   net_env
#else
//#define FILESTREAM ifstream
#define FILESTREAM ifilestream
//#define LST_IMG	  "lst_img"
//#define NET_NAMES "net_names_face"
//#define NET_PROJS "net_projs_face"
//#define NET_ENV   "net_env_face"
/*#define LST_IMG	  "network2001/lst_img_2001Lune"
#define NET_NAMES "network2001/net_names_2001Lune"
#define NET_PROJS "network2001/net_projs_2001Lune"
#define NET_ENV   "network2001/net_env_2001Lune"*/
#define CONSOLE	  "console"
#define LST_IMG	  "lst_img"
#define NET_NAMES "net_names"
#define NET_PROJS "net_projs"
#define NET_ENV   "net_env"
#endif

#define REAL             double

//#include "GUSI.h"
#include "newstream.h"
#include "EnvVar.h"
#include "allStructs.h"

#define ERROR(X, Y)		{ printf("SpikeError %d: ", X); printf Y; printf("\n"); exit(-1); }
#define WARN(X,Y)		{  }
//#define WARN(X,Y)		{ printf("SpikeWarning %d: ", X); printf Y; printf("\n"); }

#endif

/*	
-------------------- VERSION CONTROL ----------------------

SpikeNET 1.0b		Doc terminated
					Zomming + and - fixed
					Supervised learning fixed
					SpikeLisp interpretation fixed
					dialog between spacial frequency

SpikeNET 1.01b		Sockets
					new computation structures
					processor declaration and handling
					virtual maps buffering
					
SpikeNET 1.02b		Errors handling
					initiation of spike list homogeneisation
					static environment class

SpikeNET 1.03b		UNIX basic display
					new unsupervised learning
					macros in critical loops
					initiation of latency control
									
SpikeNET 1.04b		new unsupervised parameters
					new unsupervised functions and retinit functions
					remove addmap function
					output inside map
					
very soon			homogenisation with sender
					latency termination
					recay for modulation, multiple modulation for weights
					socket repair
					spike list homgeneisation termination (sockets)

------------------------ ERROR LIST ------------------------
ENVIRONMENT ERROR
1	-	out of memory
2	-	unable to open file %s
3	-	unknown/redeclared type definition symbol '%s' in file declaration map
4	-	not enought memory to save files
5	-	unable to open a new file for PPM %s
6	-	read_ppm, Impossible to find the file %s
7	-	unknown PGM format: NOT A P5 PPM FILE %s
8	-	truncated PPM File %s
9	-	processor string too long
10	-	unable to determine host name
11	-	socket error
12  -   opening stream socket error
13  -	unable to open file %s for writing

CONFIGURATION ERROR
101	-	environement variable %s not found in file
102	-	Error : MAX_TEMPS higher than MAX_NORM\n
103	-	MAX_TEMPS and MAX_NORM must be equal in %s
104	-	in %s file, there must be 1 or NB_SPIKENET arguments
105	-	no input for map %s
106	-	no learning possible for input map %s
107	-	input only possible in layer 0, thus not for map %s
108	-	no threshold for %s map\n
109	-	missing name in file declaration map
110	-	no layer for %s map
111	-	no estimer for %s map
112	-	no zoom factor for %s map
113	-	useless parameters for non multiscaled map %s
114	-	map %s cannot learn anything
115	-	cannot learn on this map %s
116	-	map with no connexion, check
117	-	multiscale map %s in a non multiscale network
118	-	map %s not found
119	-	multiscale map %s not found
120	-	map %s not found for learning
121	-	cannot learn on multiscaled maps
122	-	no map in %s
123	-	x_map required in %s file for a multiscale network
124	-	negative latencies not allowed in %s
125	-	latency map width different of its convolution map in %s
126	-	latency map height different of its convolution map in %s
127 -	unrecognised symbol %s
128	-   lisp like language syntax error
129 -	no output specified
130 -	non multiscale map %s in an only multiscale network
131 -	estim parameter not homogenous within layer %d
132 -	learning map %s not found for supervised learning
133 -	parameter LEARN must be set in environment file for UNLOCK maps
134 -   undefined symbol '%s' in file lst_img
135 -   no image defined in line '%s' of file lst_img
136 -   invalid value for rotation %d in file lst_img
137 -   external grab not alowed in lst_img file
138 -	screen %s not defined
150	-	x_map useless in %s file for a not multiscale network


LIMITATION ERROR
201	-	maximum of images overflow
202	-	max latency overflow
203	-	maximum map overflow
204	-	large map needed for map %s
205	-	number in spike list overflow in server
206	-	buffer spike list overflow in server
207	-	display not yet available on this platform
208	-	maximum afferent maps overflow
209	-	maximum SpikeInputs overflow
210	-	maximum of multiscale SpikeNET overflow
211	-	max line per convolution exeeded in %s
212	-	unable to send spikes throught the networks on this machine
213 - 	maximum individual modulation overflow
214 -	weights number overflow, recompile
215 -	array overflow
216 -	maximun map for display overflow

EXECUTION ERROR
301	-	arithmetic overflow in threshold calculationin map %s
302	-	cannot put spike in buffer, %s is not a virtual map
303	-	index out of bound exception in reconstruction %d
304	-	dysplay error, impossible to allocate learning maps
305	-	trying to add null proc
306 -	error saving convolution, index out of bound exception %d %d in 0-%dx0-%d
307 -	unreacheadable code
308 -	null pointer for the image list
309 -	too much weights for threshold computation
310 -	value not defined inside a function
311 - 	array must have equal size when added to watchArray
*/

