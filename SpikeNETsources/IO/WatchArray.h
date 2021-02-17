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

// $Log: WatchArray.h,v $

#ifndef _WATCH_ARRAY
#define _WATCH_ARRAY

#define MAX_WATCH_ACT	1000
#include "allDefs.h"

// cette classe est utilisŽ pour l'apprentissage, evolution des poids synaptiques
// pour les cartes non verouillŽes, utilise TabWeight pour determiner l'ordre du poids
// et sauve dans des fichiers les statistiques sur les poids
// ----------------------------------------------------------------------------------------

class WatchArray
{
	class NamedArray
	{
		protected :
			char 			*name;
			TabWeight		*data;
		public:
		NamedArray	( char *name0, TabWeight *data0)
		{ 
		    name  = new char[strlen(name0)+1];
    		strcpy(name, name0);
			data = data0;
		}
		char			*getName()		{ return name; }
		FORMAT_POIDS	getData(int i)	{ return data->getItem(i)->getValue(); }
		int				getSize()		{ return data->getNbItems(); }
		void			sort()		    { data->sort(); }
	};
	typedef NamedArray	*ptrNamedArray;

	ptrNamedArray			*tabWatch;
	int						maxWatch;
	int						currentWatch;

public:	
	WatchArray()
	{
		maxWatch = MAX_WATCH_ACT;
		tabWatch = new ptrNamedArray[ MAX_WATCH_ACT ];
		currentWatch = 0;
	}
	
	void addWatch( char *name0, TabWeight *data0)
	{	
		if (currentWatch < MAX_WATCH_ACT)
			tabWatch[ currentWatch++ ] = new NamedArray( name0, data0);
		else
			ERROR( 215, ("array overflow"))
	}
	
	void writeAll( char *fileName)
	{
		if (currentWatch)
		{
			FILE *f = fopen( fileName, "w");
			if (f == NULL) ERROR(13, ("unable to open file %s for writing", fileName))
		
			//write	Names and sort data
			for (int i=0; i<currentWatch; i++)
			{
				fprintf( f, "%s\t", tabWatch[i]->getName());
				tabWatch[i]->sort();
			}
			fprintf( f, "\n");
		
			//write Datas
			for (int k=0; k< tabWatch[0]->getSize(); k++)
			{
				for (int j=0; j<currentWatch; j++)
					fprintf( f, "%f\t", tabWatch[j]->getData(k));
				fprintf( f, "\n");
			}	
			fclose(f);
		}
	}
};

#endif