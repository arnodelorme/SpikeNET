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

// $Log: ImageModifier.h,v $


#ifndef _IMGMODIF
#define _IMGMODIF

#include "TreatTxt.h"
#include "GlobalImg.h"
#include "newstream.h"

   class ImageModifier : public TreatTxt
   {
   	protected:

		GlobalImg  *ImToOper; 	// The image to be treated
		
	public :
		// constructor; free memory
   		ImageModifier() : TreatTxt() { }
 		void cleanImg() { delete ImToOper; ImToOper = NULL; }			// clean the object 
  		
   		// inherited methods
		// void addOper( int codeOper, char *valueOper)
		// char *getIndex( int oper0) 
		// char *getValue( int oper0)
		
		// access methods
		char *getName();									// get the Image Name
		GlobalImg *getImg() { return ImToOper; }					// get the Image


		// treatment of images
		void treatLine( char *textLine);							// treat and store a line of text
		void applyOper();									// apply operators

		// learning parameters
		void getLearn(char *mapName, int *posx, int *posy);			// get the maps to learn; iterative (see .cc file for more comments)
		void resetGetLearn();									// reset extraction of learning maps
};
#endif