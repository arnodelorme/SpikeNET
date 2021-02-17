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

// $Log: allStructs.h,v $

#include "allDefs.h"

#ifndef _ALL_STRUCT
#define _ALL_STRUCT

// ******************
// structure d''entier
// ******************
typedef struct
{
    int size;
    int *array;
} gen_array;

// ***********************
// structure de coordonnes
// ***********************
typedef struct
{
#if LARGE_MAPS == OUI
    unsigned short x;
    unsigned short y;
#else
    unsigned char x;
    unsigned char y;
#endif
    FORMAT_POIDS  val;
} coord;

typedef struct
{
    int   size;
    coord *array;
} coord_array, *ptr_coord_array;

/*typedef struct 
{
    int            size;
    poids          *array;
} poids_array, *ptr_poids_array;
*/

// *******************
// structure de neuron
// *******************
class neuron;

typedef neuron *ptr_neuron;

typedef struct
{
    int    size;
    neuron **array;
} neur_array, *ptr_neur_array;

// *******************
// structure de carte
// *******************
class carte;

typedef carte *ptr_carte;

typedef struct
{
    int    size;
    carte  **array;
    int    *group;
} carte_array, *ptr_carte_array;

// ***********************
// structure de processeur
// ***********************
class proc
{
  public:
    
    char name[50];
    int  sock;

    proc( char *name0)
        { strcpy(name, name0);
        sock = 0;
        }
};

#endif


