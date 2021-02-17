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

// $Log: op_temps.h,v $

/*-------------------------------------------------------------------------------------------------------------------

                           OP_TEMPS.H

                           Philippe LAGADEC 4/05/1999
                           
                           primitives pour comparer, additionner et soustraire 2 temps
                           (basé sur struct timeval d'unix)
                           
-------------------------------------------------------------------------------------------------------------------*/

/*
REMARQUES:
*/

/*=== INCLUDES ====================================================================================================*/

/* pour la definition de struct timeval: */
#include <sys/time.h>



/*=== CONSTANTES ====================================================================================================*/



/*=== TYPES ====================================================================================================*/



/*=== VARIABLES GLOBALES ===========================================================================================*/


/*=== MACROS ===================================================================================================*/


/*=== PROTOTYPES ===================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------------------------------------------------------------------
  TPS_COMPARE
---------------------
-> compare 2 temps
retourne -1 si t1<t2, 0 si t1==t2, +1 si t1>t2
*/
int tps_compare (struct timeval *t1, struct timeval *t2);


/*---------------------------------------------------------------------------------------------------------------------
  TPS_AJOUTE
---------------------
-> ajoutee 1 temps a un autre
   au retour, t1=t1+t2
*/
void tps_ajoute (struct timeval *t1, const struct timeval *t2);


/*---------------------------------------------------------------------------------------------------------------------
  TPS_SOUSTRAIT
---------------------
-> soustrait 1 temps a un autre
   au retour, t1=t1-t2 si t1>t2 (delai positif non nul)
   retourne 0 si t1>t2, -1 si t1<=t2 (delai nul ou negatif)
*/
int tps_soustrait (struct timeval *t1, const struct timeval *t2);

#ifdef __cplusplus
}
#endif
