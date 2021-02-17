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

// $Log: SpikeLisp.h,v $

#ifndef _SPIKELISP
#define _SPIKELISP

//#include "GlobalImg.h"
#include "allDefs.h"
class GlobalImgTreat;

class SpikeLisp
{

 public:
  int 				oper[MAX_OPER];
  int 				arg1[MAX_OPER];
  int 				arg2[MAX_OPER];
  int 				thres[MAX_OPER];
  GlobalImgTreat 	*res[MAX_OPER];
  GlobalImgTreat	*convos[MAX_OPER];
  FORMAT_POIDS		*tmpImgConvo;
  int       		numberOper;
  
  SpikeLisp();
  ~SpikeLisp();       
  void addConvo(char *fileName, int posConvo );            
  void addOper(int oper_0, int arg1_0, int arg2_0);              
  void addOper(int oper_0, int arg1_0, int arg2_0, int threshold);              
  GlobalImgTreat *run(GlobalImgTreat *imageList[MAX_IMG_LIST]);
  GlobalImgTreat *getTreatedImg() { return res[numberOper-1]; }
  void reserv( int zoom0, int size_x0, int size_y0);
  void reset();
  void copy(SpikeLisp &sp);
};

class SpikeLispSpike : public SpikeLisp
{
 public:
  double 		spikePC;
  int 			norm;
  int 			MAX_NORM;
  coord_array 	*spike_RET;
  
  SpikeLispSpike( double spikePC0, int norm0, int MAX_NORM0);
  ~SpikeLispSpike();       
  void makeSpikeList( int border = 0);
  void showSpikeList();
  void reserv( int zoom0, int size_x0, int size_y0);
    
};

#endif