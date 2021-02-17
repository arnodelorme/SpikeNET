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

// $Log: NewImageList.cpp,v $

#include <stdlib.h>
#include <string.h>
#include "ImageModifier.h"
#include "NewImageList.h"

/// this class deals with array of ImageModfifier
/// - it convert a file to an array
/// - it provides sequential access to the array
/// - it can load several items at the same time

    /// create a simple imageList
    NewImageList::NewImageList( char *file, int step0, int imgAtOnce0, int countImage0) {
    	step = step0;
    	imgAtOnce = imgAtOnce0;
 		countImage = countImage0-1;
    	
    	if (file == NULL) 	readInLstImg();
    	else				readInFile( file);
    }
        
    /// reset the image list to its begginning
    void NewImageList::reset (int pointer) { 
		countImage = pointer;
    	for (int i=countImage; i<countImage + imgAtOnce-1; i++)
    		tabModif[ i ]->applyOper();
	}
	
	/// get the next Image
	GlobalImg *NewImageList::goAndGetNext() {
		if (countImage+imgAtOnce-1 >= nb_image-1) return NULL;
		else {
			countImage++;
			if (countImage != 0) tabModif[ countImage-1 ]->cleanImg();
			tabModif[ countImage+imgAtOnce-1 ]->applyOper();
			return tabModif[ countImage ]->getImg();
		}
	}
	
   /// load all the data from a file
   void NewImageList::readInFile(char *fileName)
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


