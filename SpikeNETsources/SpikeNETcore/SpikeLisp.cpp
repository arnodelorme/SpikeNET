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

// $Log: SpikeLisp.cpp,v $

#include "SpikeLisp.h"
#include "GlobalImgTreat.h"

SpikeLisp::SpikeLisp() 
{
  numberOper = 0;		    
  res[0] = NULL;
}
SpikeLisp::~SpikeLisp() 
{
  delete [] tmpImgConvo;
  for (int i=0; i<numberOper; i++)
    delete res[i];
}

void SpikeLisp::addConvo(char *fileName, int posConvo )
{
	char newName[1000];
	char *ptrName = newName;
	strcpy( newName,  CURRENT_DIR);
	
	printf("test\n");
	printf("%s\n", fileName);

	strcat( newName, fileName);
	while ((ptrName = strpbrk(ptrName, "/")) != NULL)
		*(ptrName++) = SEPARATEUR;
	convos[ posConvo ] = new GlobalImgTreat(newName, 1);
}

void SpikeLisp::addOper(int oper_0, int arg1_0, int arg2_0)
{
  oper[numberOper] = oper_0;
  arg1[numberOper] = arg1_0;
  arg2[numberOper] = arg2_0;
  thres[numberOper] = DEFAULT_THRESHOLD;
  numberOper++;
}

void SpikeLisp::addOper(int oper_0, int arg1_0, int arg2_0, int threshold)
{
  oper[numberOper] = oper_0;
  arg1[numberOper] = arg1_0;
  arg2[numberOper] = arg2_0;
  thres[numberOper] = threshold;
  numberOper++;
}

GlobalImgTreat *SpikeLisp::run(GlobalImgTreat *imageList[MAX_IMG_LIST])
{
  
  for (int i=0; i<numberOper; i++)
    {
      GlobalImgTreat *ptrArg1, *ptrArg2;
      
      if (arg1[i]>=0) ptrArg1 = imageList	[ arg1[i]   ];
      else 			ptrArg1 = res		[ -arg1[i]-1 ];
      
      if (oper[i]>0 && oper[i]<10)
	if (arg2[i]>=0) ptrArg2 = imageList	[ arg2[i]   ];
	else 			ptrArg2 = res		[ -arg2[i]-1 ];
      
      switch (oper[i])
	{
	case 0 : res[i]->copy(ptrArg1->ImInt); break;
	case 1 : res[i]->add( ptrArg1->ImInt, ptrArg2->ImInt, thres[i]); break;
	case 2 : res[i]->sub( ptrArg1->ImInt, ptrArg2->ImInt, thres[i]); break;
	case 3 : res[i]->times( ptrArg1->ImInt, ptrArg2->ImInt, thres[i]); break;
	case 4 : res[i]->divid( ptrArg1->ImInt, ptrArg2->ImInt, thres[i]); break;
	case 11 : res[i]->add( ptrArg1->ImInt, arg2[i], thres[i]); break;
	case 12 : res[i]->sub( ptrArg1->ImInt, arg2[i], thres[i]); break;
	case 13 : res[i]->times( ptrArg1->ImInt, arg2[i], thres[i]); break;
	case 14 : res[i]->divid( ptrArg1->ImInt, arg2[i], thres[i]); break;
	case 20 : res[i]->convo( ptrArg1->ImInt, tmpImgConvo, convos[arg2[i]], (FORMAT_POIDS)thres[i]); break;
	case 21 : res[i]->makepos( ptrArg1->ImInt, thres[i]); break;
	}
    }
  return res[numberOper-1];
}

void SpikeLisp::copy(SpikeLisp &sp)
{
  numberOper = sp.numberOper;
  for (int i=0; i<numberOper; i++)
    {
      oper[i]  = sp.oper[i];
      arg1[i]  = sp.arg1[i];
      arg2[i]  = sp.arg2[i];
      thres[i] = sp.thres[i];
      if (oper[i] == 20) 
	convos[arg2[i]] = sp.convos[arg2[i]];  
    }
}

void SpikeLisp::reset()
{
  numberOper = 0;
}

void SpikeLisp::reserv(int zoom0, int size_x0, int size_y0)
{
  int width;
  int height;
  if (zoom0 >0)
    {
      width 	= (int)ceil((double)size_x0 / zoom0);
      height 	= (int)ceil((double)size_y0 / zoom0);
    }
  else
    {
      width  = size_x0 * -zoom0;
      height = size_y0 * -zoom0;
    }
  
  // temporary array for computation
  tmpImgConvo			= new FORMAT_POIDS		  [ width * height ];
  if (tmpImgConvo == NULL) ERROR(1,("out of memory"));

  
  int zoom1 = 1, zoom2 = 1;
  for (int i=0; i<numberOper; i++)
    {
      if (arg1[i]>=0) zoom1 = zoom0; else zoom1 = 1;
      if (arg2[i]>=0) zoom2 = zoom0; else zoom2 = 1;
      
      res[i] = new GlobalImgTreat( width, height, zoom1, zoom2);
    }
}

SpikeLispSpike::SpikeLispSpike( double spikePC0, int norm0, int MAX_NORM0) : SpikeLisp()
{
  MAX_NORM = MAX_NORM0;
  norm	 = norm0;
  spikePC  = spikePC0;
}

void SpikeLispSpike::reserv(int zoom0, int size_x0, int size_y0)
{
  int width;
  int height;
  
  SpikeLisp::reserv( zoom0, size_x0, size_y0);
  
  if (zoom0 >0)
    {
      width 	= (int)ceil((double)size_x0 / zoom0);
      height 	= (int)ceil((double)size_y0 / zoom0);
    }
  else
    {
      width  = size_x0 * -zoom0;
      height = size_y0 * -zoom0;
    }
  
  spike_RET   		    = new coord_array [MAX_NORM + MAX_COUCHE + 40];
  spike_RET[0].array 	= new coord		  [ width * height ];
  if (spike_RET[0].array == NULL)
  	ERROR(1,("out of memory"));
}


SpikeLispSpike::~SpikeLispSpike()
{
  //parent automatically called
  //delete [] spike_RET[0].array;
  //delete [] spike_RET;		 
}


// **************************************************
// **************************************************

/*void main()
{

	SpikeLispSpike sp(0.3, 100, 50000);
	//SpikeLisp sp;
	GlobalImg *listImg[MAX_IMG_LIST];
*/	
	
	/*
	//(+ img1 (- img2 img3))	
	sp.addOper(2, 1, 2);
	sp.addOper(1, 0, -1);*/
	
	//(- (* 2 img1) (* 2 img1))
	/*sp.addOper(13, 0, 2);
	sp.addOper(13, 1, 2);
	sp.addOper(2 , -1, -2);*/
	
/*	sp.addConvo("Gauss", 0);	
	sp.addConvo("OnCenter2", 1);	
	sp.addOper(20, 0, 0);
	sp.addOper(20, -1, 1);
	sp.addOper(2, 1, 0);
	sp.addOper(3, -2, -3);

	//sp.addOper(2, 2, 1);	
	
	listImg[0] = new GlobalImg("2001Space001.PPM", 0);
	listImg[1] = new GlobalImg("2001Space002.PPM", 0);
	listImg[2] = new GlobalImg("2001Space003.PPM", 0);

	sp.reserv( -1, listImg[0]->width, listImg[0]->height);

	(sp.run( listImg ))->save(":totores");
	sp.makeSpikeList();
	sp.showSpikeList();
}
*/
