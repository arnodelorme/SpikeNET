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

// $Log: Sender.h,v $

#ifndef _SENDER
#define _SENDER

#include "allDefs.h"

class Sender
{
  proc *tabProc[MAX_CARTE_FROM];
  int nbProc;
	
public:
	Sender() { nbProc=0; }

	void addEfferent( proc *remoteProc )
	{
        if (remoteProc == NULL)
            ERROR(305,("trying to add null proc"));
		int i = 0;
        int found = 0;
		for (i=0; i<nbProc; i++) 
			if (tabProc[i] == remoteProc) found = 1; 
		if (!found) tabProc[nbProc++] = remoteProc;
	}
#if SYSTEME == UNIX
    void sendSpikes( volatile coord_array *c,int MapNumber,int TimeStep) { }
#else
    void sendSpikes( volatile coord_array *c,int MapNumber,int TimeStep)
    { ERROR(212,("unable to send spikes throught the networks on this machine")); }
#endif
    void sendSpikesInternal(coord_array *c,int MapNumber,int TimeStep);
    
    void UCanWrite(int sock, volatile coord_array *c,int MapNumber,int TimeStep);
    void affiche() 
        {
            for (int i=0; i<nbProc; i++)
                printf("\t\t\t%s\n", tabProc[i]->name);
        }
};

#endif