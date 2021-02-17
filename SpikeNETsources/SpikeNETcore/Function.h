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

// $Log: Function.h,v $


#ifndef _FUNCTION
#define _FUNCTION

#include "allDefs.h"

class Function
{
	double	*tabx;
	double	*taby;
	int		nbItems;
	
public:	
	Function( char *fileName)
	{
		int eof = 0;
		nbItems = 0;
		char tmp[30];
		
		//count the items
		FILESTREAM *f = new FILESTREAM( fileName);
		
		do { (*f) >> tmp; (*f) >> tmp; nbItems++; }
		while ( (eof = (*f).get()) != EOF );

		tabx = new double[ nbItems ];
		taby = new double[ nbItems ];
		
		delete f;
		
		FILESTREAM g( fileName);
		int ptr = 0;
		do { g >> tmp; tabx[ptr] = atof( tmp ); g >> tmp; taby[ptr++] = atof( tmp );}
		while ( (eof = g.get()) != EOF );
	}
	
	double getValue( double x)
	{
		if ((x < tabx[0]) && (x > tabx[nbItems-1]))
			ERROR( 310, ("value not defined inside a function"));
		
		int i=0;
		while (x > tabx[i]) i++;
		
		if (x == tabx[i]) return taby[i];
		else return taby[i-1] + (taby[i]-taby[i-1])/(tabx[i]-tabx[i-1]) * (x - tabx[i-1]);
	}
		
};

#endif