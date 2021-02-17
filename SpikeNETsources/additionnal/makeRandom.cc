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

// $Log: makeRandom.cc,v $

/*
 *  Hello World for the CodeWarrior
 *  © 1997-1998 Metrowerks Corp.
 *
 *  Questions and comments to:
 *       <mailto:support@metrowerks.com>
 *       <http://www.metrowerks.com/>
 */

#include <stdio.h>
#include <math.h>
#include "allDefs.h"
#include "ImageListRich.h"

#define WIDTH	2020
#define HEIGHT	1000
//#define WIDTH	910
//#define HEIGHT	700
#define PI		3.14159

    char 	EnvVar::MAIN_DIRECTORY[100] = "testing";
    int	    EnvVar::RESCUE   			= 0;


int test( unsigned char *img, int xi, int yi, int xf, int yf, int width);
int test( unsigned char *img, int xi, int yi, int xf, int yf, int width)
{
	int value = 0;
	for (int y=yi; y<yf; y++)
		for (int x=xi; x<xf; x++)
			value += img[x+y*width];
	return value;
}

void slicing();
void slicing()
{
	// slicing the big image
	PicturePPM pict(":Images:megapict.ppm");
	pict.write_ppm_grab_abs( 0, 0  , 320, 360, ":testing:megapict1");
	pict.write_ppm_grab_abs( 0, 340, 320, 700, ":testing:megapict2");
	pict.write_ppm_grab_abs( 300, 0  , 620, 360, ":testing:megapict3");
	pict.write_ppm_grab_abs( 300, 340, 620, 700, ":testing:megapict4");
	pict.write_ppm_grab_abs( 600, 0  , 910, 360, ":testing:megapict5");
	pict.write_ppm_grab_abs( 600, 340, 910, 700, ":testing:megapict6");
	exit(-1);
}

int main()
{
	//slicing();
	//exit(-1);

	unsigned char *rawData  = new unsigned char[WIDTH*HEIGHT];
	unsigned char *rawData2  = new unsigned char[WIDTH*HEIGHT];
	
	//initialise
	for (int i=0; i<WIDTH*HEIGHT; i++) rawData[i] = 0;
	
	int posx, posy;
	int loop = 0;
	int numb = 0;
	
	char newName[100], tmp[100], *imageName, *newImageName;	
	
	printf("TREAT IMAGES\n");
		
	// image list reading
	ImageListRich		*img_list;
	GlobalImg			**currentImg;
	char				*mapLearn;
	int					coordx, coordy;
	FILE				*fout = fopen(":fout", "w");
	
	img_list = new ImageListRich( 1 );

	img_list->reserv();
	img_list->init( 1 );

	for (int gg=0; gg<10000; gg++) rand();
	while((currentImg = img_list->revert()) != NULL)
	{
		printf("%d - treating image %s\n", img_list->getCount(), img_list->getCurrentImg());

		numb++;

		//currentImg[0]->div2 ();
		//currentImg[0]->div2 ();

		// find a position
		loop = 0;
		int sizex, sizey;
		do
		{
			double rand1 = (double)rand()/RAND_MAX;
			double rand2 = (double)rand()/RAND_MAX;
			sizex = (rand2 * 69 + 23);
			sizey = (rand2 * 84 + 28);
			posx = (double)rand1 * (WIDTH-sizex-3);
			posy = (double)rand2 * (HEIGHT-sizey-3);
			loop++;
		}
		while (test(rawData, posx-3, posy-3, posx+sizex+3, posy+sizey+3, WIDTH) && (loop < 1000000));
		if (loop == 1000000)	printf("position not found %d : quiting\n", numb);
		else
		{
			currentImg[0]->resize( sizex, sizey );
			currentImg[0]->recopi_ecran( rawData + posx + posy*WIDTH, WIDTH);
			img_list->getMapLearn( &mapLearn, &coordx, &coordy);
			fprintf( fout, "Image\t%s\tResult\t%s\t%dx%d-3x3\n", img_list->getCurrentImg(), mapLearn, posx + (coordx>>2), posy + (coordy>> 2));
		}
	}
/*	while((currentImg = img_list->revert()) != NULL)
	{
		printf("treating image %s\n", img_list->getCurrentImg());

		numb++;

		currentImg[0]->div2 ();
		currentImg[0]->div2 ();

		// find a position
		loop = 0;
		do
		{
			posx = (double)rand()/RAND_MAX * (WIDTH-29);
			posy = (double)rand()/RAND_MAX * (HEIGHT-34);
			loop++;
		}
		while (test(rawData, posx-3, posy-3, posx+29, posy+34, WIDTH) && (loop < 1000000));
		if (loop == 1000000)	printf("position not found %d : quiting\n", numb);
		else
		{
			currentImg[0]->recopi_ecran( rawData + posx + posy*WIDTH, WIDTH);
			img_list->getMapLearn( &mapLearn, &coordx, &coordy);
			fprintf( fout, "Image\t%s\tResult\t%s\t%dx%d-3x3\n", img_list->getCurrentImg(), mapLearn, posx + (coordx>>2), posy + (coordy>> 2));
		}
	}
*/	PicturePPM megaPictTst( rawData, WIDTH, HEIGHT);
	megaPictTst.write_ppm(":testing:megapictTst");
	//exit(-1);
	fclose( fout );
	
	//compute the interpolation
	// -----------------------
	for (int ii=0; ii<WIDTH*HEIGHT; ii++) rawData2[ii] = rawData[ii];
	
	for (int y=0; y< HEIGHT; y++)
	{
		printf("Processing line %d \n", y);
		for (int x=0; x< WIDTH; x++)
		{
			//printf("Processing pos %d %d\n", x, y);
			int tsum = 0;
			int realColor = 0;
			if (!rawData[x + y*WIDTH])
			{
				for (int teta=0; teta<360; teta+=3)
				{
					int costeta = (int)(cos(teta*PI/180) * (double)(1 << 16));
					int sinteta = (int)(sin(teta*PI/180) * (double)(1 << 16));
					int newx, newy, t=0;
					
					do
					{	t++;
						newx = x + ( costeta*t >> 16);
						newy = y + ( sinteta*t >> 16);
						if ((newx < 0) || (newy < 0) || (newx >= WIDTH) || (newy >= HEIGHT)) break;
					} while (!rawData[newx+newy*WIDTH]);
					
					if ((newx < 0) || (newy < 0) || (newx >= WIDTH) || (newy >= HEIGHT)) realColor+= (128 * 10000) / t;
					else realColor += (rawData[newx+newy*WIDTH] * 10000) / t;
					tsum+= 10000 / t;
				}
				realColor /= tsum;
				if (realColor > 255) 
					{ printf("Error : color %d at pos %d %d\n", realColor, x, y); exit(-1); }
				rawData2[x + y*WIDTH] = (unsigned char)realColor;
			}
		}
	}
	
	PicturePPM megaPict( rawData2, WIDTH, HEIGHT);
	megaPict.write_ppm(":megapict");
		
		
	
}

