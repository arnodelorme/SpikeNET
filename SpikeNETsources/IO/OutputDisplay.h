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

// $Log: OutputDisplay.h,v $

#include "allDefs.h"
#include "PicturePPM.h"
#include "ScreenSpike.h"
#include "Estimer.h"

#ifndef _OUTPUTDISPLAY
#define _OUTPUTDISPLAY

// dealing with displaying the results of the current map

class OutputDisplay : public GlobalImg
{
public:
	int		pos_x;			// x position to be displayed
	int		pos_y;			// y position to be displayed
	int     widthZoom;		// zoom in width
	int     heightZoom;		// zoom in height
	int		zoomDsp;		// zoom for display only
	int		zoomWrt;		// zoom for writing only
	int		layer;			// layer of the map (together with the estimer, this gives the color of pixels)
	int		disk;			// 0 or 1 for saving
	int		update;			// does this map need to be updated (0 or 1)
	int		colorCst;		// color of the background
	ScreenSpike 	*screen;		// pointer to the screen with deal with this map
	Estimer 		*estimer;		// pointer to the estimer of the map
	BaseImg 		*imgAssociated;	// pointer to the additionnal image associated with the map

	int countUpdate;		// count the number of update of this map
	
	OutputDisplay(  int pos_x0, int pos_y0, int size_x0, int size_y0, int zoomDsp0, ScreenSpike *screen0, char *fileName, int type, int color0) : GlobalImg( fileName, type)
	{
		pos_x 		 = pos_x0;
		pos_y 		 = pos_y0;
		width  		 = size_x0;
		height 		 = size_y0;
		if (zoomDsp0 < 0) 	{ widthZoom = size_x0 << (-zoomDsp0); heightZoom = size_y0 << (-zoomDsp0); }
		else    	       	{ widthZoom = size_x0 >>   zoomDsp0;  heightZoom = size_y0 >> zoomDsp0; }
		zoomDsp		 = zoomDsp0;
		layer		 = 0;
		estimer		 = NULL;
		screen		 = screen0;
		update		 = 1;
		bytePerPixel = EnvVar::BYTES_FILE;
		imgAssociated= NULL;
		colorCst	 = color0;
	}
		
	OutputDisplay( int pos_x0, int pos_y0, int size_x0, int size_y0, int zoomDsp0, int layer0, int disk0, ScreenSpike *screen0, Estimer *estimer0, int color0) : GlobalImg()
	{
		pos_x 		 = pos_x0;
		pos_y 		 = pos_y0;
		width  		 = size_x0;
		height 		 = size_y0;
		if (zoomDsp0 < 0) 	{ widthZoom = size_x0 << (-zoomDsp0); heightZoom = size_y0 << (-zoomDsp0); }
		else    	       	{ widthZoom = size_x0 >>   zoomDsp0;  heightZoom = size_y0 >> zoomDsp0; }
		zoomDsp		 = zoomDsp0;
		layer		 = layer0;
		estimer		 = estimer0;
		screen		 = screen0;
		disk		 = disk0;
		update		 = 1;
		bytePerPixel = EnvVar::BYTES_FILE;
		imgAssociated= NULL;
		colorCst	 = color0;
		
		if (disk)
		{
			Im = new unsigned char [ width * height * EnvVar::BYTES_FILE ];		
			if (Im == NULL) ERROR(4,("not enought memory to save files"));
		}
		countUpdate = 0;
	}

	void adjustSize( int width0, int height0 )
	{
		width  =  width0;
		height =  height0;
		if (zoomDsp < 0) 	{ widthZoom = width0 << (-zoomDsp); heightZoom = height0 << (-zoomDsp); }
		else    	       	{ widthZoom = width0 >>   zoomDsp;  heightZoom = height0 >> zoomDsp; }
	}
	
	int getLayer() { return layer; }
	
	void put_pixel( int pos_x0, int pos_y0, int color) {
		if (colorCst != 0) {
			if (zoomDsp < 0) {
				for (int j=0; j< (1 << -zoomDsp); j++)	
					for (int i=0; i< (1 << -zoomDsp); i++) {
						if (screen != NULL) screen ->put_RVB_pixel( pos_x + pos_x0*(1 << -zoomDsp) + i, pos_y + pos_y0*(1 << -zoomDsp) + j, colorCst);
						if (disk)			BaseImg::put_pixel( pos_x0*(1 << -zoomDsp) + i, pos_y0*(1 << -zoomDsp) + j, color);
					}
			}
			else {
				if (screen != NULL) screen->put_RVB_pixel( pos_x + (pos_x0 >> zoomDsp), pos_y + (pos_y0 >> zoomDsp), colorCst);
				if (disk)			BaseImg::put_pixel( pos_x0 >> zoomDsp, pos_y0 >> zoomDsp, color);
			}
		}
		else if ((color >= 0) && (color < 256))
		if ((color >= 0) && (color < 256))
			if (zoomDsp < 0)
			{
				for (int j=0; j< (1 << -zoomDsp); j++)	
					for (int i=0; i< (1 << -zoomDsp); i++)
					{
						if (screen != NULL) screen ->put_pixel( pos_x + pos_x0*(1 << -zoomDsp) + i, pos_y + pos_y0*(1 << -zoomDsp) + j, color);
						if (disk)			BaseImg::put_pixel( pos_x0*(1 << -zoomDsp) + i, pos_y0*(1 << -zoomDsp) + j, color);
					}
			}
			else 
			{
				if (screen != NULL) screen->put_pixel( pos_x + (pos_x0 >> zoomDsp), pos_y + (pos_y0 >> zoomDsp), color);
				if (disk)			BaseImg::put_pixel( pos_x0 >> zoomDsp, pos_y0 >> zoomDsp, color);
			}
	}
	
	void clear( int colorClear )
	{
		if ((imgAssociated != NULL) && (screen != NULL))
		{
			screen->putBaseImg( imgAssociated, pos_x - INTER_LARG - (imgAssociated->getWidth() >> 2), pos_y, 2);
			if (colorCst != 0) screen->drawRect( pos_x - INTER_LARG - (imgAssociated->getWidth() >> 2), pos_y, pos_x - INTER_LARG, pos_y + (imgAssociated->getHeight() >> 2), colorCst); 
		}
		for (int j=0;j<heightZoom; j++)
			for (int i=0; i<widthZoom; i++)
			{
				if (screen != NULL) screen->put_pixel( pos_x + i, pos_y + j, colorClear);
				if (disk)			BaseImg::put_pixel( i, j, colorClear);
			}
	}

	void flush()
	{
		double factZoom;
		if (screen != NULL) 
			if (zoomDsp <= 0)
			{
				int factZoom = 1 << (-zoomDsp);
				for (int j=0;j<height; j++)
					for (int i=0; i<width; i++)
						for (int k=0; k<factZoom; k++)
							for (int l=0; l<factZoom; l++)
								screen->put_pixel( pos_x + i*factZoom + k, pos_y + j*factZoom + l, Im[ i + j*width]);
			}
	}		
	/*void reInit( int color )
	{
		clear( color );
		if ((imgAssociated != NULL) && (screen != NULL)) screen->putBaseImg( imgAssociated, pos_x - INTER_LARG - imgAssociated->getWidth(), pos_y, -1);
	}*/
		
	void writeSpikes( coord_array *spikeList )
	{
		int count = estimer->getEstimerAndUpdate( layer, spikeList->size );

		//static int line = 0;
		//for (int i=0; i<50; i++)
		//	put_pixel( i+100, line / 48, 255 - count);
		//line++;
	
		for (int j=0; j < spikeList->size; j++)
		{
        	put_pixel( spikeList->array[j].x, spikeList->array[j].y, 255 - count);
        	//printf("%d %d->%3.2f", 	spikeList->array[j].x, spikeList->array[j].y, spikeList->array[j].val);
        }
        //printf("\n");
	}
	
	void writeRebuilt( BaseImg *reBuilt, int shifter = 0)
	{ if (screen != NULL) screen->putBaseImg( reBuilt, pos_x + width + shifter + INTER_LARG, pos_y, EnvVar::ZOOM_CONVO); }
	
	void writeOnDisk( char *name, char *fileName )
	{
		if (disk)
		{
			char newname[100];
		
         	while (strpbrk(fileName, SEPARATEURSTR) != NULL)
            	 fileName = strpbrk(fileName, SEPARATEURSTR)+1;
		
    		sprintf( newname, "%s%s%c%s_%s%d.out", CURRENT_DIR, EnvVar::RESULTS, SEPARATEUR, name, fileName, countUpdate);
    		PicturePPM tmpPict( Im, width, height);
    		tmpPict.write_ppm(newname);
    	}
    }
    
    void addImgAssociated( BaseImg *img0 )	{ imgAssociated = img0; }
};

#endif