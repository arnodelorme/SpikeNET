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

// $Log: defImg.h,v $

// tous les define
// ***************
//#include <fstream.h>
//#include <iostream.h>

#ifndef _DEFIMG
#define _DEFIMG

// pour les convolutions
#define MAC 			1
#define UNIX 			2
#define PC 				3

/* *********************************** */
/*     les defines commencent ici      */
/* *********************************** */

// for the image module
#define FORMAT_POIDS     double      // mode de stockage des poids
#define INACTIVE		 -3000000000

#ifndef SYSTEME
#define SYSTEME MAC
#endif
#include "newstream.h"

#ifndef ERROR
#define ERROR(X, Y)		{ printf("SpikeError %d: ", X); printf Y; printf("\n"); exit(-1); }
#endif
#ifndef WARN
#define WARN(X,Y)		{  }
#endif

#endif

