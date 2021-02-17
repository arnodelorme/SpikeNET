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

// $Log: Rebuild.h,v $

#ifndef _REBUILD
#define _REBUILD

#include "allDefs.h"
#include "PicturePPM.h"
#include "GlobalImg.h"

// this class deals with reconstructing the average image onto with the map discharge
// ----------------------------------------------------------------------------------

class Rebuild : public GlobalImg
{
	double	*pixCount;		// for each positionin the image, determine how many pixels have been applied

public:
	Rebuild( int sizex, int sizey) : GlobalImg(sizex, sizey)
	{
		if (Im == NULL)
			Im 		 = new unsigned char [ sizex * sizey ];
		ImInt 	 = new FORMAT_POIDS  [ sizex * sizey ];
		pixCount = new double        [ sizex * sizey ];
		bytePerPixel = 1;
		
		clear();
	}
	
	void addSpike( int posx, int posy, GlobalImg *originalImg, double inc)
	{
		/*unsigned char min = 100000;
		unsigned char max = 0;		
		{for (int j = 0; j<height ; j++)
			for (int i = 0; i<width ; i++)
			{
				int oriy = j + posy - height/2;
				int orix = i + posx - width/2;
				if ((orix>=0) && (orix<originalImg->getWidth()) && (oriy>=0) && (oriy<originalImg->getHeight()))
				{
					unsigned char val = originalImg->Im[ i + posx - width/2 + (posy - height/2)* originalImg->getWidth()];
					if (val> max) max = val;
					else if (val<min) min = val;
				}
			}}*/
			
		for (int j = 0; j<height ; j++)
			for (int i = 0; i<width ; i++)
			{
				int oriy = j + posy - height/2;
				int orix = i + posx - width/2;
				
				if ((orix>=0) && (orix<originalImg->getWidth()) && (oriy>=0) && (oriy<originalImg->getHeight()))
				{
					ImInt    [ i + j*width ] += originalImg->Im[ orix + oriy* originalImg->getWidth()] * inc;
					pixCount [ i + j*width ] +=inc;
					//ImInt    [ i + j*width ] = (ImInt[ i+j*width ] * pixCount [ i+j*width ] + 128.0 * (originalImg->Im[ orix + oriy* originalImg->getWidth()]-min)/(double)(max-min) )/ ( pixCount [ i + j*width ]+1 );
					//pixCount [ i + j*width ] +=inc;
				}
			}
	}
	
	void write( char *fileName)
	{
		update();
		PicturePPM pict( Im, width, height);
		pict.write_ppm( fileName);
	}
	
	virtual int update()
	{	
		{for (int i=0; i< width * height; i++) if (pixCount [ i ] !=0) ImInt [ i ] /= (double)pixCount [ i ];}
		minmax();
		for (int i=0; i<width * height; i++)
    		Im[i] = (unsigned char)( ((ImInt[i]-min) * 255.0)/(max - min) );
		{for (int i=0; i< width * height; i++)
			if (pixCount [ i ] !=0) ImInt    [ i ] *= pixCount [ i ];}
		//for (int i=0; i<width * height; i++)
    	//	Im[i] = (unsigned char)ImInt[i];		
		return 0;
	}
	
	virtual void clear(int val = 0)
	{
		for (int i=0; i<width * height; i++)
		{
			ImInt[ i ] = 0;
			pixCount[ i ] = 0;
		}
	}	
};

/*class Rebuild : public GlobalImg
{
	double	*pixCount;

public:
	Rebuild( int sizex, int sizey) : GlobalImg( NULL, sizex, sizey)
	{
		Im = new unsigned char[ sizex * sizey ];
		ImInt 	 = new FORMAT_POIDS[ sizex * sizey ];
		pixCount = new double      [ sizex * sizey ];
		bytePerPixel = 1;
		
		for (int i=0; i<sizex * sizey; i++) { ImInt[i] = 0; pixCount[i] = 0; }
	}
	
	void addSpike( int posx, int posy, GlobalImg *originalImg, double inc)
	{
		for (int j = 0; j<height ; j++)
			for (int i = 0; i<width ; i++)
			{
				int oriy = j + posy - height/2;
				int orix = i + posx - width/2;
				
				if ((orix>=0) && (orix<originalImg->getWidth()) && (oriy>=0) && (oriy<originalImg->getHeight()))
				{
					ImInt    [ i + j*width ] += (originalImg->Im[ orix + oriy* originalImg->getWidth()])*inc;
					pixCount [ i + j*width ] += inc;
				}
			}
	}
	
	void write( char *fileName)
	{
		update();
		PicturePPM pict( Im, width, height);
		pict.write_ppm( fileName);
	}
	
	virtual int update()
	{	
		{for (int i=0; i< width * height; i++) ImInt [ i ] /= pixCount [ i ];}
		minmax();	
		for (int i=0; i<width * height; i++)
    		Im[i] = (unsigned char)( ((ImInt[i]-min) * 255)/(max - min) );
    		
		{for (int i=0; i< width * height; i++) ImInt [ i ] *= pixCount [ i ];}
		return 0;
	}
};*/

#endif
