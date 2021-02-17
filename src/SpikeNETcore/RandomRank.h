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

// $Log: RandomRank.h,v $

#ifndef _RANDOM_RANK
#define _RANDOM_RANK

#include <math.h>

class RandomRank
{
protected:
	int			*tabInt;
	int			nbItems;
	int			currentNb;
		
public:
	RandomRank( int nb)
	{
		nbItems = nb;
		tabInt = new int[ nb ];
		reInit();
	}
	void reInit()
	{
		for (int i=0; i< nbItems; i++) tabInt[i] = i;
		currentNb = nbItems;
	}
	int getRank()
	{
		int tmpRank = 0;
		if (currentNb)
		{
			int pos = floor((double)rand()/RAND_MAX * currentNb * 0.99);
			if (pos >= currentNb) exit(-1);
			tmpRank = tabInt[pos];
			for (int i=pos+1; i < currentNb; i++)
				tabInt[i-1] = tabInt[i];
			currentNb--;
		}
		else tmpRank = -1;
		return tmpRank;
	}
};	

#endif