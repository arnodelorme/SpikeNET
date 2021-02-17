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

// $Log: NewImageListold.h,v $

#include <stdlib.h>
#include <string.h>

#ifndef _IMAGELIST
#define _IMAGELIST

#include "allDefs.h"
#include "ImageModifier.h"
typedef char * ptr_char;

/// this class deals with array of ImageModfifier
/// - it convert a file to an array
/// - it provides sequential access to the array
/// - it can load several items at the same time

class NewImageList
{
  protected:
   	ImageModifier *tabModif[MAX_IMAGE];	// array of ImageModifier
   	ImageModifier *currentModif;		// current Image treated

    int       nb_image;					// total number of images
    int		  imgAtOnce;				// number of images to be loaded at once

	int 	  countImage;				// pointer onto the current image
	int		  step;						// increment step
	
  public:
  	/// access methods
    int getNbImages()		{ return nb_image; }
	int getWidth()			{ return tabModif[ countImage ]->getImg()->getWidth(); }
	int getHeight()			{ return tabModif[ countImage ]->getImg()->getHeight(); }
	int getCount()			{ return countImage; }
	GlobalImg *getCurrentImg()		{ return tabModif[ countImage ]->getImg(); }
	char	  *getCurrentImgName()	{ return tabModif[ countImage ]->getName(); }
  
    /// create a simple imageList
    NewImageList( char *file = NULL, int step0 = 1, int imgAtOnce0 = 1, int countImage0 = 0) {
    	step = step0;
    	imgAtOnce = imgAtOnce0;
 		countImage = countImage0-1;
    	
    	if (file == NULL) 	readInLstImg();
    	else				readInFile( file);
    }
    
    /// reset imgAtOnce
    void setImgAtOnce( int imgAtOnce0 ) {
    	imgAtOnce = imgAtOnce0;
	}	
    
    /// reset the image list to its begginning
    void reset (int pointer) { 
		countImage = pointer;
    	for (int i=countImage; i<countImage + imgAtOnce-1; i++)
    		tabModif[ i ]->applyOper();
	}
	
	/// get the next Image
	GlobalImg *goAndGetNext() {
		if (countImage+imgAtOnce-1 >= nb_image-1) return NULL;
		else {
			countImage++;
			if (countImage != 0) tabModif[ countImage-1 ]->cleanImg();
			tabModif[ countImage+imgAtOnce-1 ]->applyOper();
			return tabModif[ countImage ]->getImg();
		}
	}
	
   /// load all the data from a file
   void readInFile(char *fileName)
   {
      	char line[150];
      	int eof1;

		FILESTREAM f(fileName);
		nb_image=0;

		do
		{
			// treat a line of text
			eof1 = f.getline( line, 150, '\n');
	  		if (strlen(line) > 1) {
      			tabModif[ nb_image ] = new ImageModifier();
      			tabModif[ nb_image ]->treatLine( line );
 	      		
 	      		nb_image++;
 	      		if (nb_image > MAX_IMAGE)	ERROR(201,("maximum of images overflow"));
      		}
	  	} while (eof1!=EOF);
   }
   
   /// load all the data from the lst_img_file
   void readInLstImg() {
    	char new_net_lst[100];
    	sprintf(  new_net_lst, "%s%s%c%s", CURRENT_DIR, EnvVar::MAIN_DIRECTORY, SEPARATEUR, LST_IMG );
    	readInFile( new_net_lst);
   }
   
   /// clean up this mess  	
    ~NewImageList() { }
};

#endif


