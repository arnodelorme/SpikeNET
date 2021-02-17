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

// $Log: treatImgs.cc,v $

#include "allDefs.h"
//#include "ImageListRich.h"
#include "NewImageList.h"

    char 	EnvVar::MAIN_DIRECTORY[100] = "testing";
    int	    EnvVar::RESCUE   			= 0;

int main()
{
	NewImageList *imgList = new NewImageList("testing:lst_img_col_target");
	int number = 0;
	printf("salut\n");
	
	GlobalImg *currentImg;
	GlobalImg *outputImg = new GlobalImg( 192, 128, 3);
	outputImg->clear( 0 );
	
	while((currentImg = imgList->goAndGetNext()) != NULL) {
		printf("treating image %s\n", imgList->getCurrentImgName());
		if (currentImg->getWidth() == 192) {
			for (int i=0; i< 128*192*3; i++)
				outputImg->ImInt[i] =  (outputImg->ImInt[i]*number + currentImg->ImInt[i])/(number+1);
			number++;
		}
	}
	
	//renormalisation
	outputImg->convert2char( 0 );
	
	// saving the output
	PictureTARGA outputTarga( outputImg->Im, 192, 128 );
	outputTarga.writeTARGA(":testing:col_target");
	
	/*GlobalImg gim(":test.ppm", 0);
	gim.resize( 39, 28);
	gim.write_ppm(":test2");
	
	gim.revertImg(":test.ppm");
	gim.resize( 38, 28);
	gim.write_ppm(":test3");

	gim.revertImg(":test.ppm");
	gim.resize( 37, 28);
	gim.write_ppm(":test4");*/

/*	ImageListRich		*img_list;
	img_list = new ImageListRich( 1 );

	img_list->reserv();
	img_list->init( 1 );
	GlobalImg **currentImg;
	PictureTARGA *outputImg = new PictureTARGA( 128, 192);
	outputImg->clear( 255 ) ;
	int count = 0;
	int absc = 0;
	int ordo = -35;

	while((currentImg = img_list->revert()) != NULL)
	{
		printf("treating image %s\n", img_list->getCurrentImg());
	}
	outputImg->writeTARGA( ":testing:test"); 
	printf("wrote\n");*/
}
/*
int main()
{

	//GlobalImg gim(":test.ppm", 0);
	//gim.resize( 39, 28);
	//gim.write_ppm(":test2");
	
	//gim.revertImg(":test.ppm");
	//gim.resize( 38, 28);
	//gim.write_ppm(":test3");

	//gim.revertImg(":test.ppm");
	//gim.resize( 37, 28);
	//gim.write_ppm(":test4");

	ImageListRich		*img_list;
	img_list = new ImageListRich( 1 );

	img_list->reserv();
	img_list->init( 1 );
	GlobalImg **currentImg;
	PicturePPM *outputImg = new PicturePPM( 240, 30);
	outputImg->clear( 255 ) ;
	int count = 0;
	int absc = 0;
	int ordo = -35;

	while((currentImg = img_list->revert()) != NULL)
	{
		printf("treating image %s\n", img_list->getCurrentImg());
	//	currentImg[0]->div2();
	//	currentImg[0]->div2();
		
	//	absc += 30;
	//	if (!(count % 8)) { absc = 0; ordo+= 35; }
		
		//currentImg[0]->recopi_ecran( outputImg->Im + absc + ordo*outputImg->getWidth(), outputImg->getWidth());
	//	count++;
	}
	outputImg->write_ppm( ":test"); 
	printf("wrote\n");
}*/
