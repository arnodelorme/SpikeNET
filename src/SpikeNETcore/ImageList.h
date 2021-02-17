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

// $Log: ImageList.h,v $

#include <stdlib.h>
#include <string.h>

#include "allDefs.h"
#include "GlobalImgTreat.h"

#ifndef _IMAGE
#define _IMAGE

#if MODE == INTERNAL
#include "lst_img.h"
#endif

typedef char * ptr_char;

class ImageList
{
  protected:
    ptr_char  *tab_img;
    GlobalImgTreat *ptrImg[MAX_IMG_LIST];
	char	  *allimages;
    int       nb_image;
    int		  imgAtOnce;

	int 	  countImage;
	int		  step;
	
  public:
    ImageList(int step0)
      {
 		countImage = EnvVar::RESCUE;
    	step = step0;
    	// read the first image
      }

    ImageList(char *s, int step0)
      {
	
		char *list_dir = new char[100];
		strcpy(list_dir,"ls ");
		strcat(list_dir,s);
		strcat(list_dir," > lst_img");
		system(list_dir);
	
		read_all_images(s);
 		countImage = 0;

		// initialisation			
    	step = step0;
    	// read the first image
    	ptrImg[0] = new GlobalImgTreat(tab_img[0], 0);
    	
       }
    
    int getNbImages() { return nb_image; }
    
	virtual void reserv()
	  {
		read_all_images(DIRECTORY);		
    	ptrImg[0] = new GlobalImgTreat(tab_img[0], 0);
	  }
      
    virtual void reset()
	  { countImage = 0; countImage = EnvVar::RESCUE; }

    virtual void reset( GlobalImgTreat *im)
	  { 
	  	countImage = 0;
	  	step	   = 1;
	  	nb_image   = 1;
	  	ptrImg[0]  = im; 
	  }
	  		
	virtual void init( int imgAtOnce0)
	{
		// initialisation			
    	imgAtOnce = imgAtOnce0;
    	for (int i=1; i<imgAtOnce; i++)
    		ptrImg[i] = new GlobalImgTreat(tab_img[0], 0);
	}
	
	int getWidth()
	{ return ptrImg[0]->getWidth(); }
	
	int getHeight()
	{ return ptrImg[0]->getHeight(); }

	char *getCurrentImg()
	{	return tab_img[countImage-1]; }
	
	int getCount()
	{	return countImage; }

    virtual void read_all_images(char *s)
      {
		tab_img = new ptr_char[MAX_IMAGE];
	
    	char new_net_lst[100];
    	sprintf(  new_net_lst, "%s%s%c%s", CURRENT_DIR, EnvVar::MAIN_DIRECTORY, SEPARATEUR, LST_IMG );
	
		FILESTREAM f(new_net_lst);
		int eof1;
		nb_image=0;
		char image[100];
		char *newimage;
	
		allimages = new char[60*MAX_IMAGE];
		tab_img[0] = allimages;
	    
		do
		{
	  		f >> image; 

	  
	  		eof1 = f.get(); 
	  		if (eof1!=EOF)
	    	{
	      		if (nb_image) tab_img[nb_image] = tab_img[nb_image-1] + strlen(tab_img[nb_image-1]) + 1;
	      		strcpy(tab_img[nb_image], CURRENT_DIR);
	      		newimage = image;
	      		while ((newimage = strpbrk(newimage, "/")) != NULL)
					*(newimage++) = SEPARATEUR;
	      		strcat(tab_img[nb_image], image);
		   
	      		nb_image++;
	      		if (nb_image > MAX_IMAGE)
             		ERROR(0,("maximum of images overflow"));
	    	}
	  	} while (eof1!=EOF);
      }
     	
 	virtual GlobalImgTreat **revert()
 	{
 		if (countImage+imgAtOnce < nb_image+1)
 			{
 				for (int i=0; i<imgAtOnce; i++)
 				ptrImg[i]->revertImg(tab_img[countImage+i]);
 				countImage += step;
				return ptrImg;
			}
		return NULL;
	}	
 	
    ~ImageList() {delete []tab_img; delete [] allimages;  for (int i=0; i< imgAtOnce; i++) delete ptrImg[i]; }
};

class ImageListLearn  : public ImageList
{
protected:
	class MapDescLearn
	{
		public:
			char map[30];
			int x;
			int y;
		
		MapDescLearn *suiv;
		MapDescLearn( char *mapName, int coordx, int coordy, MapDescLearn *suiv0)
		{	strcpy(map, mapName); x = coordx; y = coordy; suiv = suiv0; }
	};
	
	MapDescLearn *tabLearn[MAX_IMAGE];
	MapDescLearn *currentLearn;
	
public:
	void reserv()
	  {
		read_all_images(DIRECTORY);		
    	ptrImg[0] = new GlobalImgTreat(tab_img[0], 0);
	  }

	  ImageListLearn( int step0) : ImageList( step0)
	  { }
      
 	GlobalImgTreat **revert()
 	{
 		currentLearn = tabLearn[countImage];
		return ImageList::revert();
	}	

    void read_all_images(char *s)
      {
      	
		char image[100];
      	char map[30];
      	char coordx0[30];
      	char coordy0[30];
      	char line[150];
      	
		tab_img = new ptr_char[MAX_IMAGE];
	
    	char new_net_lst[100];
    	sprintf(  new_net_lst, "%s%s%c%s", CURRENT_DIR, EnvVar::MAIN_DIRECTORY, SEPARATEUR, LST_IMG );
	
		FILESTREAM f(new_net_lst);
		int eof1, eof2;
		nb_image=0;
		char *newimage;
	
		allimages = new char[60*MAX_IMAGE];
		tab_img[0] = allimages;
	    
		do
		{
			eof1 = f.getline( line, 150, '\n');
			istrngstream ff(line);

	  		if ((eof1!=EOF) && (line[0] != 0))
	    	{
	    		tabLearn[nb_image] = NULL;
	  			ff >> image;
	      		if (nb_image) tab_img[nb_image] = tab_img[nb_image-1] + strlen(tab_img[nb_image-1]) + 1;
	      		strcpy(tab_img[nb_image], CURRENT_DIR);
	      		
     			newimage = image;
      			while ((newimage = strpbrk(newimage, "/")) != NULL)
					*(newimage++) = SEPARATEUR;
      			strcat(tab_img[nb_image], image);
	  			
	  			do
	  			{
	  				ff >> map;
	  				ff >> coordx0;
	  				ff >> coordy0;

	  				eof2 = ff.get(); 
	  				if (map[0] != 0)
	  					tabLearn[nb_image] = new MapDescLearn( map, atoi(coordx0), atoi(coordy0), tabLearn[nb_image]);
	    		} while  (eof2!=EOF);
		   
	      		nb_image++;
	      		if (nb_image > MAX_IMAGE)
             		ERROR(201,("maximum of images overflow"));
	    	}
	  	} while (eof1!=EOF);
      }

 	//char *getMapLearn() { return tab_map[countImage-1]; }	
 	//int getCoordx() { return coordx[countImage-1]; }	
 	//int getCoordy() { return coordy[countImage-1]; }	

	char *firstMapLearn( )
	{	return tabLearn[ countImage-1 ]->map; }

	void firstMapLearn( char **mapLearn, int *coordx, int *coordy)
	{
		*mapLearn 	= tabLearn[ countImage-1 ]->map;
		*coordx		= tabLearn[ countImage-1 ]->x;
		*coordy		= tabLearn[ countImage-1 ]->y;
	}
	
	int getMapLearn( char **mapLearn, int *coordx, int *coordy)
	{
		if (currentLearn != NULL)
		{
			*mapLearn 	= currentLearn->map;
			*coordx		= currentLearn->x;
			*coordy		= currentLearn->y;
			currentLearn = currentLearn->suiv;
			return 1;
		}
		return 0;
	}
};
#endif
