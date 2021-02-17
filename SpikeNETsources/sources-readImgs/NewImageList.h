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

// $Log: NewImageList.h,v $

#include <stdlib.h>
#include <string.h>

#ifndef _IMAGELIST
#define _IMAGELIST

#ifndef MAX_IMAGE
#define MAX_IMAGE 200
#endif

/// this class deals with array of ImageModfifier
/// - it convert a file to an array
/// - it provides sequential access to the array
/// - it can load several items at the same time


/// if is composed of 3 classes 
///		TreatTxt 		the token manager
///		ImageModifier	who apply treatment to individual images
///		NewImageList	who handle list of images
/// ONLY THIS CLASS IS ACCESSED

class NewImageList
{
#include "ImageModifier.h"
typedef char * ptr_char;

  protected:
   	ImageModifier *tabModif[MAX_IMAGE];	// array of ImageModifier
   	ImageModifier *currentModif;		// current Image treated

    int       nb_image;					// total number of images
    int		  imgAtOnce;				// number of images to be loaded at once

	int 	  countImage;				// pointer onto the current image
	int		  step;						// increment step

    void readInFile(char *fileName);	// load all the data from a file
   	void readInLstImg() { }				// MAYBE BUG HERE load all the data from the lst_img_file
	
  public:
  	/// access methods
    int getNbImages()		{ return nb_image; }
	int getWidth()			{ return tabModif[ countImage ]->getImg()->getWidth(); }
	int getHeight()			{ return tabModif[ countImage ]->getImg()->getHeight(); }
	int getCount()			{ return countImage; }
	GlobalImg *getCurrentImg()		{ return tabModif[ countImage ]->getImg(); }
	char	  *getCurrentImgName()	{ return tabModif[ countImage ]->getName(); }
  
  	// constructor
    NewImageList( char *file=NULL, int step0=1, int imgAtOnce0=1, int countImage0=0); 	// create a simple imageList
    void setImgAtOnce( int imgAtOnce0 ) {	imgAtOnce = imgAtOnce0; }    				// reset the number of images increment
    void reset (int pointer); 															// reset the image list to its begginning or to a pointer
	GlobalImg *goAndGetNext();															// get the next Image
    ~NewImageList() { }
};

#endif


