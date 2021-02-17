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

// $Log: GlobalImgTreat.cpp,v $

#include "GlobalImgTreat.h"

/* 0 = nothing
   1 = PLUS
   2 = MINUS
   3 = TIMES
   4 = DIVIDE
   5 = CONVO
   */

#include <math.h>

#define  GENOP2IMG(X) { if (zoom1 > 1) GENOP2IMG_ZOOM(X) else  GENOP2IMG_UNZOOM(X) }
#define  GENOP1IMG(X) { if (zoom1 > 1) GENOP1IMG_ZOOM(X) else  GENOP1IMG_UNZOOM(X) }
#define  GENOP2IMG_ZOOM(X)  { 																											\
	thres = threshold; int realfacty1 = zoom1*zoom1*width; int realfacty2 = zoom2*zoom2*width; 											\
	for (int j=0; j<height; j++) 																										\
		for (int i=0; i<width; i++)  																									\
			if ((img1[i+j*width]!=INACTIVE) && (img2[i+j*width]!=INACTIVE))   															\
				if((ImInt[i+j*width] = zoom(zoom1, img1+i*zoom1+j*realfacty1) X zoom(zoom2, img2+i*zoom2+j*realfacty2)) < threshold) 	\
					ImInt[i] = INACTIVE; else; else ImInt[i] = INACTIVE;}
					
#define  GENOP1IMG_ZOOM(X)  { 																											\
	thres = threshold; int realfacty1 = zoom1*zoom1*width; 																				\
	for (int j=0; j<height; j++) 																										\
		for (int i=0; i<width; i++) 																									\
			if (img1[i+j*width]!=INACTIVE)  																							\
				if((ImInt[i+j*width] = zoom(zoom1, img1+i*zoom1+j*realfacty1) X fact) < threshold)  									\
					ImInt[i] = INACTIVE; else; else ImInt[i] = INACTIVE; }

#define  GENOP2IMG_UNZOOM(X) {  																										\
	thres = threshold; int realfacty1 = width >> factZoom1; int realfacty2 = width >> factZoom2;  										\
	for (int j=0; j<height; j++)  																										\
		for (int i=0; i<width; i++)  																									\
			if ((img1[i+j*width]!=INACTIVE) && (img2[i+j*width]!=INACTIVE))  															\
				if((ImInt[i+j*width] = img1[(i >> factZoom1) + (j >> factZoom1)*realfacty1 ] X img2[(i >> factZoom2) + (j >> factZoom2)*realfacty2 ]	) < threshold) \
				ImInt[i] = INACTIVE; else; else ImInt[i] = INACTIVE; }

#define  GENOP1IMG_UNZOOM(X) {  																										\
	thres = threshold; int realfacty1 = width >> factZoom1;   																			\
	for (int j=0; j<height; j++)   																										\
		for (int i=0; i<width; i++)   																									\
			if (img1[i+j*width]!=INACTIVE)   																							\
				if((ImInt[i+j*width] = img1[(i >> factZoom1) + (j >> factZoom1)*realfacty1 ] X fact	) < threshold) 						\
				ImInt[i] = INACTIVE; else; else ImInt[i] = INACTIVE; }

GlobalImgTreat::GlobalImgTreat(int width0, int height0, int zoom1_0, int zoom2_0) : GlobalImg()
{
  max = -100000;
  min =  100000;
  width  = width0;
  height = height0;
  zoom1	= zoom1_0;
  zoom2	= zoom2_0;
  factZoom1 = (int)(log(abs(zoom1))/log(2.0));
  factZoom2 = (int)(log(abs(zoom2))/log(2.0));
  
  ImInt = new FORMAT_POIDS[width * height];
  Im = NULL;
  
}

void GlobalImgTreat::copy(FORMAT_POIDS *img)
{
  //version with real zoom
  //-----------------------
  /*int zoomer = (int)(sqrt(zoom1));
    
    int widthtModulo = img->width % zoom1;
    int widthtInt	 = img->width - widthModulo;
    int heightModulo = img->height % zoom1;
    int heightInt	 = img->height - heightModulo;
    
    //init
    for (int j=0; j<height; j++)
    for (int i=0; i<width; i++) Im[i+j*width] = 0;		
    
    //zoom
    for (int j=0; j< heightInt ; j++)
    {
    for (int i=0; i< widthInt; i++) 
    Im[(i >> zoomer) + (j >> zoomer)*width] += img->Im[ i + j*img->width ];
    for (int i=widthInt; i< widthInt + widthModulo; i++)
    Im[(i >> zoomer) + (j >> zoomer)*width] += (img->Im[ i + j*img->width ] * zoom1) / widthModulo;
    }
    for (int j= heightInt; j< heightInt+heightModulo ; j++)
    {
    for (int i=0; i< widthInt; i++) 
    Im[(i >> zoomer) + (j >> zoomer)*width] += (img->Im[ i + j*img->width ] << zoomer) / heightModulo;
    for (int i=widthInt; i< widthInt + widthModulo; i++)
    Im[(i >> zoomer) + (j >> zoomer)*width] += ((img->Im[ i + j*img->width ] << (2 * zoomer)) / widthModulo) / heightModulo;
    }*/
  
  if (zoom1 > 1)
    {
      int realfacty1 = zoom1*zoom1*width;
      for (int j=0; j<height; j++)
	for (int i=0; i<width; i++) ImInt[i+j*width] = zoom(zoom1, img+i*zoom1+j*realfacty1);
    }
  else
    {
      int realfacty1 = width >> factZoom1;
      for (int j=0; j<height; j++)
	for (int i=0; i<width; i++) ImInt[i+j*width] = img[(i >> factZoom1) + (j >> factZoom1)*realfacty1 ];
    }
}

void GlobalImgTreat::add(FORMAT_POIDS *img1, FORMAT_POIDS *img2, FORMAT_POIDS threshold)		GENOP2IMG(+)   
void GlobalImgTreat::add(FORMAT_POIDS *img1, FORMAT_POIDS fact, FORMAT_POIDS threshold)		GENOP1IMG(+)   
void GlobalImgTreat::sub(FORMAT_POIDS *img1, FORMAT_POIDS *img2, FORMAT_POIDS threshold)		GENOP2IMG(-)   
void GlobalImgTreat::sub(FORMAT_POIDS *img1, FORMAT_POIDS fact, FORMAT_POIDS threshold)			GENOP1IMG(-)	
void GlobalImgTreat::times(FORMAT_POIDS *img1, FORMAT_POIDS *img2, FORMAT_POIDS threshold)		GENOP2IMG(*)   
void GlobalImgTreat::times(FORMAT_POIDS *img1, FORMAT_POIDS fact, FORMAT_POIDS threshold)		GENOP1IMG(*)   
void GlobalImgTreat::divid(FORMAT_POIDS *img1, FORMAT_POIDS *img2, FORMAT_POIDS threshold)		GENOP2IMG(/)   
void GlobalImgTreat::divid(FORMAT_POIDS *img1, FORMAT_POIDS fact, FORMAT_POIDS threshold)		GENOP1IMG(/) 
void GlobalImgTreat::makepos(FORMAT_POIDS *img1, FORMAT_POIDS threshold)
{
  copy(img1);
  
  thres = threshold;
  for (int i=0; i<height*width; i++)
    if (ImInt[i]<min) min = ImInt[i];
  
  for (int j=0; j<height*width; j++) 
    ImInt[j] += min + threshold;
}

void GlobalImgTreat::convo(FORMAT_POIDS *image, FORMAT_POIDS *tmpImgConvo, GlobalImg *Convo, FORMAT_POIDS threshold)
{
  thres = threshold;
  
  // application de la convolution
  int 			absc, ord, i, j, k, l;
  FORMAT_POIDS *cursor;
  int 			larg_flt_div2 = Convo->getWidth()  / 2;
  int 			haut_flt_div2 = Convo->getHeight() / 2;
  int 			centre_filtre = Convo->getWidth() * Convo->getHeight() / 2;
  int 			val_filtre    = 0;
  
  for (i=0; i<height*width; i++) ImInt[i] = 0;
  
  //zooming
  if (zoom1 > 1)
    {
      int realfacty1 = zoom1*zoom1*width;
      for (j=0; j<height; j++)
		for (i=0; i<width; i++) tmpImgConvo[i+j*width] = zoom(zoom1, image+i*zoom1+j*realfacty1);
    }
  else
    {
      int realfacty1 = width >> factZoom1;
      for (j=0; j<height; j++)
		for (i=0; i<width; i++) tmpImgConvo[i+j*width] = image[(i >> factZoom1) + (j >> factZoom1)*realfacty1 ];
    }
  
  for(j=0;j<height;j++)
    for(i=0;i<width;i++)
      {
	cursor = ImInt  + i + j* width;
	for (k=-larg_flt_div2;k<(larg_flt_div2+1);k++)
	  for (l=-haut_flt_div2;l<(haut_flt_div2+1);l++)
	    {
	      absc = i+k; if (absc<0) absc = 0; if (absc>=width)  absc = width-1;
	      ord = j+l;  if (ord <0) ord  = 0; if (ord >=height) ord = height-1;
	      
	      *cursor += *(tmpImgConvo + absc + ord * width) * Convo->ImInt[k + centre_filtre + l*Convo->getWidth()];
	    }
	//if (*cursor<min) min = *cursor;
	//if (*cursor>max) max = *cursor;
      }

  /*//save the image
  convert2char();
  PicturePPM pict(Im, width, height);
  pict.write_ppm("tmppm");
  exit(-1);*/

  // threshold
  for(j=0;j<height;j++)
    for(i=0;i<width;i++)
    	if (*(ImInt + i+j*width) < thres) *(ImInt + i+j*width) = INACTIVE;
    	
}
