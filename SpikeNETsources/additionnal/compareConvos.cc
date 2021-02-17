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

// $Log: compareConvos.cc,v $

#include "weights.h"

class WeightStat : public Weights
{
protected:
	double	var;
	double	mean;
	
public:
	double getMean() { return mean; }
	double getVar()  { return var;  }
	
	WeightStat() : Weights() { }
	void computeStat()
	{
		mean = 0;
		var  = 0;
		for (int i=0; i<width*height; i++)
		{
			mean += ImInt[i];
			var  += ImInt[i] * ImInt[i];
		}
		var = sqrt(mean*mean - var)	/ ((double)(width * height));
		mean /= ((double)(width * height));
	}
};

class compareConvos
{
	Weights	*tabWeight[100];
	int		nbTabWeights;
	
	compareConvos()
	{
		nbTabWeights = 0;
	}
};

int main( int argc, char *argv[])
{

	FILE *f1 = fopen(":net_names", "w");
	FILE *f2 = fopen(":net_projs", "w");
	int nbOfMaps = 64;
	
	fprintf( f1, "Name	ON	Layer	0	Threshold	0.5	Zoom	0	Estim	1	Pos	10	100	0	Input	(Convo	Convolutions/OnCenter	Image1	-10000)\nName	OFF	Layer	0	Threshold	0.5	Zoom	0	Estim	1	Pos	10	200	0	Input	(Convo	Convolutions/OffCenter	Image1	-10000)\n\n");

	for (int i=0; i<nbOfMaps; i++)
	{
		fprintf( f1, "Name ALEARN%d Layer 1 Zoom	0	Threshold	0.999	MOD \"networkUnsuperv/modReal\"	Conv	95	Estim	0\n", i+1);
		fprintf( f2, "ON  ALEARN%d	FILE	UNLOCK	GROUP RET RAND	0.5	RENORM	1000	Convosnew/newGauss1\n", i+1);
		fprintf( f2, "OFF ALEARN%d	FILE	UNLOCK	GROUP RET RAND	0.5	RENORM	1000	Convosnew/newGauss1\n", i+1);
	}
	fprintf( f2 , "\n");
	fclose(f1);
	
	for (int j=0; j<nbOfMaps; j++)
	{
		for (int k=0; k<nbOfMaps; k++)
			fprintf( f2, "ALEARN%d	ALEARN%d	FILE	LOCK	GROUP	INHIB MOD 1 MULT 2	Convosnew/newGauss23inhib\n", j+1, k+1);
		fprintf( f2 , "\n");
	}
	fclose(f2);
}