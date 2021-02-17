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

// $Log: Sockets.cpp,v $

// UNIX FUNCTION ONLY
#include "Server.h"
#include "Sender.h"
#include "SuperSpikeNETClass.h"

#if SYSTEME == MAC
//#include <sys/errno.h>
//#include <fcntl.h>
//#include <GUSI.h>
#endif

#if SYSTEME == UNIX
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <fcntl.h>

#endif

// trash
/*	int colorT[] =
	{
		0xFFFFFFFF,
		0xFFBFFFFF,
		0xFF7FFFFF,

		0xFF00FFFF, //GB
		0xFF00FFCF,
		0xFF00FF9F,
		0xFF00FF7F,

		0xFF00FF00, //G
		0xFF00CF00,
		0xFF009F00,
		0xFF007F00,
		0xFF004F00,

		0xFF00004F, //B
		0xFF00007F,
		0xFF00009F,
		0xFF0000CF,
		0xFF0000FF,

		0xFF7F00FF, //BR
		0xFF9F00FF,
		0xFFCF00FF,
		0xFFFF00FF, 

		0xFF9F009F,
		0xFF7F007F,
		0xFF4F004F,

		0xFF4F4F4F, //grey
		0xFF7F7F7F,
		0xFF9F9F9F,
		0xFFCFCFCF,

		0xFF4F4F4F,
		0xFF7F7F7F,
		0xFF9F9F9F,
		0xFFCFCFCF,

		0xFF00FFFF,
		0xFF00FF3F,
		0xFF003FFF,
		0xFF003F3F,
		
		0xFF00FFFF,
		0xFF00FF3F,
		0xFF003FFF,
		0xFF003F3F
 };*/
/*	int colorT[] =
	{
		0xFF00FFFF, //GB
		0xFF00FFAF,
		0xFF00FF6F,
		0xFF00AFFF,
		0xFF006FFF,
		0xFF00AFAF,
		0xFF006F6F,
		0xFF006FAF,
		0xFF00AF6F,
		0xFF00FF00, //G
		0xFF00CF00,
		0xFF009F00,
		0xFF007F00,
		0xFF004F00,
		0xFF00004F, //B
		0xFF00007F,
		0xFF00009F,
		0xFF0000CF,
		0xFF0000FF,
		0xFFFFFF00, //RG
		0xFFEFEF00,
		0xFFFFAF00,
		0xFFFF6F00,
		0xFFAFFF00,
		0xFF6FFF00,
		0xFFAFAF00,
		0xFF6F6F00,
		0xFF6FAF00,
		0xFFAF6F00,
		0xFFFF00FF, //RB
		0xFFFF00AF,
		0xFFFF006F,
		0xFFAF00FF,
		0xFF6F00FF,
		0xFFAF00AF,
		0xFF6F006F,
		0xFF6F00AF,
		0xFFAF006F,
		0xFFAFAFAF,
		0xFF5F5F5F, //grey
 };
	
	int posx = 25; int posy =100;
	for (int ii=0; ii<1000; ii++)
		screen0->drawRect(ii, 70, ii+1, 230, 0xFF7F7F7F);
	for (int yy=0; yy<40; yy++)
	{
		screen0->drawRect( posx-10, posy-13, posx+10, posy+13, colorT[yy]);
		posx += 30;
		if (yy == 19) { posx = 25; posy+=50; }
	}
	//screen0->drawRect( posx-10, posy-13, posx+10, posy+13, 0xFFFF0000);
	//screen0->saveTARGA( ":test"); 
	exit(-1);
*/