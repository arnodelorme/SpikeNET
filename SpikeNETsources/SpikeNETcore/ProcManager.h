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

// $Log: ProcManager.h,v $


#ifndef _PROC_MANAGER
#define _PROC_MANAGER

class ProcManager
{
	proc *tabProc[MAX_CARTE];
	char *localName;
	int nbProc;
	
public:	
	ProcManager( char *localName0)
	{ nbProc = 0; 
		if (localName0 == NULL) localName = NULL;
		else 
			if (strlen(localName0) < 30) 
				{ localName = new char[30]; strcpy(localName, localName0); }
			else ERROR(9,("processor string too long"));
	}

	proc *addTabProc( char *name )
	{
		if (!EnvVar::NO_SHOW)
        	if (name != NULL)
            	printf("\nTrying to add proc : %s", name);
        	else
            	printf("\nTrying to add NULL proc");
            
		int i=0;
		proc *res = NULL;
		if (localName == NULL) res = NULL; // for mac
		else if (strcmp(localName, name))
		  {
			while (i < nbProc)
			{
				if (!strcmp( tabProc[i]->name, name)) res = tabProc[i];
				i++;
			}
			if (res == NULL) { tabProc[ nbProc++ ] = new proc(name); res = tabProc[nbProc-1]; }
		  }
		return res;
	}
    void affProc()
        {
		  if (nbProc > 0) {
			  printf("\nProcessor list : ");
                for (int i=0; i<nbProc; i++)
                    printf("%s\t", tabProc[ i ]->name);
				printf("\n\n");
		  }
		}        
};

#endif
