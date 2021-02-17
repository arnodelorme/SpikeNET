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

// $Log: GlobalImgTreat.h,v $

#include "GlobalImg.h"

#ifndef _GLOBALIMGTREAT
#define _GLOBALIMGTREAT

class GlobalImgTreat : public GlobalImg
{
 public:
  
  int thres;
  int factZoom1;
  int factZoom2;
  int zoom1;
  int zoom2;
  
  // constructors
  GlobalImgTreat() 										: GlobalImg() { }
  GlobalImgTreat(char *fileName, int type)				: GlobalImg( fileName, type) { }
  GlobalImgTreat(unsigned char *Im0, int w0, int h0) 	: GlobalImg( Im0, w0, h0) { }
  GlobalImgTreat( int w0, int h0, int depth0) 			: GlobalImg( w0, h0, depth0) { zoom1=1; zoom2=1; }
  GlobalImgTreat(int width0, int height0, int zoom1_0, int zoom2_0);  

  FORMAT_POIDS  zoom(int factor, FORMAT_POIDS *ptr) { FORMAT_POIDS tmp=0; for (int j=0; j<factor; j++) for (int i=0; i<factor; i++) tmp += *(ptr + i + j*width*factor); 
#if REAL_RETINAL_CONTRAST == OUI
  return tmp / (factor*factor); }
#else
  return tmp; }
#endif

  void copy(FORMAT_POIDS *img1);
  void add(FORMAT_POIDS *img1, FORMAT_POIDS *img2, FORMAT_POIDS threshold);
  void add(FORMAT_POIDS *img1, FORMAT_POIDS fact, FORMAT_POIDS threshold);
  void sub(FORMAT_POIDS *img1, FORMAT_POIDS *img2, FORMAT_POIDS threshold);
  void sub(FORMAT_POIDS *img1, FORMAT_POIDS fact, FORMAT_POIDS threshold);
  void times(FORMAT_POIDS *img1, FORMAT_POIDS *img2, FORMAT_POIDS threshold);
  void times(FORMAT_POIDS *img1, FORMAT_POIDS fact, FORMAT_POIDS threshold);
  void divid(FORMAT_POIDS *img1, FORMAT_POIDS *img2, FORMAT_POIDS threshold);
  void divid(FORMAT_POIDS *img1, FORMAT_POIDS fact, FORMAT_POIDS threshold);
  void convo(FORMAT_POIDS *image, FORMAT_POIDS  *tmpImgConvo, GlobalImg *Convo, FORMAT_POIDS threshold);
  void makepos(FORMAT_POIDS *img1, FORMAT_POIDS threshold);
};

#endif