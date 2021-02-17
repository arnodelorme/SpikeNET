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

// $Log: ImageModifieroldd.h,v $


#ifndef _IMGMODIF
#define _IMGMODIF

#include "TreatTxt.h"
#include "newstream.h"
#include "GlobalImg.h"

#define	LEARN_DESACTIVATED		-102
#define	LEARN				-2
#define	IMAGE				-1
#define	CONTRAST				0
#define	RANDOM				1
#define	LUMINANCE			2
#define	RORATE90				3
#define	HORZ_MIROR			4
#define	VERT_MIROR			5
#define	RESIZE				6
#define	GRAB					7
#define	PUT					8
#define	SMOOTH				9
#define	SAVEPPM				10
#define	INVERT				11
#define	SCALE				12
#define	SAVETARGA				13

   class ImageModifier : public TreatTxt
   {
   	protected:

		GlobalImg  *ImToOper; 	// The image to be treated
		
	public :

   		ImageModifier() : TreatTxt() { }
   		
   		/// inherited methods
		/// void addOper( int codeOper, char *valueOper)
		/// char *getIndex( int oper0) 
		/// char *getValue( int oper0)
		
		/// get the Image Name
		char *getName() {
			char *res = getValue( IMAGE );
			if (res == NULL) ERROR( 0, ("Image Name not found"))
			return res;
		}
		
		/// get the Image
		GlobalImg *getImg() { return ImToOper; }
		/// clean the object 
		void cleanImg() { delete ImToOper; ImToOper = NULL; }
		
		/// treat a line of text
		void treatLine( char *textLine) {
			char tmp[100], value0[100];
			
			istrngstream ff(textLine);
	  			
	  		do
	  		{
	  			ff >> tmp;
	  			switch( tmp[ 0 ] )
	  			{
	  				case 0   : 	break;
	  				case 'L' : 
	  				case 'l' : 	if ( (tmp[1]=='e')||(tmp[1]=='E') ) { ff >> value0; addOper( LEARN, value0 ); }
	  						else						      { ff >> value0; addOper( LUMINANCE, value0 ); }
	  						break;
	  				case 'R' :
	  				case 'r' : 	if ( (tmp[1]=='a')||(tmp[1]=='A') ) { ff >> value0; addOper( RANDOM, value0 ); } 
							else if ( (tmp[1]=='e')||(tmp[1]=='E') ) { ff >> value0; addOper( RESIZE, value0 ); } 
	  							else	{ 
	  								ff >> value0;
	  								int intval = (atoi(value0) + 360) % 360;
	  								if ((intval % 90) != 0) 	ERROR( 136, ("invalid value for rotation %d in file lst_img", intval))
	  								if ((intval / 90) >= 1)	addOper( RORATE90, NULL );
	  								if ((intval / 90) >= 2)	addOper( RORATE90, NULL );
	  								if ((intval / 90) >= 3)	addOper( RORATE90, NULL );	  									
	  							}
	  						break; 
	  				case 'C' :
	  				case 'c' : 	ff >> value0; addOper( CONTRAST, value0 ); break; 
	  				case 'P' :
	  				case 'p' : 	ff >> value0; addOper( PUT, value0 ); break; 
	  				case 'H' : 
	  				case 'h' : 	addOper( HORZ_MIROR, NULL ); break;
	  				case 'G' : 
	  				case 'g' :	ff >> value0; addOper( GRAB, value0 ); break;
	  				case 'V' : 
	  				case 'v' :	addOper( VERT_MIROR, NULL ); break;
	  				case 'I' :
	  				case 'i' :	if ( (tmp[1]=='n')||(tmp[1]=='N') ) { addOper( INVERT, NULL ); }
	  						else  						       {  ff >> value0; addOper( IMAGE, value0 ); }
	  						break;
	  				case 'S' :
	  				case 's' :  	if ( (tmp[1]=='m')||(tmp[1]=='M') ) {  ff >> value0; addOper( SMOOTH, value0 ); }
	  			           		else 	if ( (tmp[1]=='c')||(tmp[1]=='C') ) 	{ ff >> value0; addOper( SCALE, value0 ); }
	  			           		else 	if ( (tmp[4]=='a')||(tmp[4]=='a') ) 	{ ff >> value0; addOper( SAVEPPM, value0 ); }
	  			           			else  							{ ff >> value0; addOper( SAVETARGA, value0 ); }
	  						break;
	  				default  :	ERROR( 134, ("undefined symbol '%s' in file lst_img", tmp))
	  			}
	  		} while (tmp[0]!=0);
		}		
		
		/// apply operators
		void applyOper()
		{
			// get the image
			ImToOper = new GlobalImg( getName(), 0);
		
			char *tmp, *tmp1, *tmp2, *tmp3, *tmp4, *tmp5, *tmp6, *tmp7, *tmp15;
			for (int i=0; i< nbOper; i++)
			{
				switch( oper[ i ] )
				{
					case LEARN_DESACTIVATED	: break;
					case LEARN	: break;
					case IMAGE	: break;
					case CONTRAST : ImToOper->recalculateContrast( atof(value[ i ]) ); break;
					case RANDOM : tmp15 = strpbrk( value[ i ], "_"); if (tmp15 != NULL) { *tmp15 =0; ImToOper->random( atof(value[ i ]),  atoi(tmp15+1)); *tmp15 ='_'; } else ImToOper->random( atof(value[ i ])); break;
					case LUMINANCE : ImToOper->shiftLum( atoi(value[ i ]) ); break;
					case RORATE90 : ImToOper->rotate90(); break;
					case HORZ_MIROR : ImToOper->horzMiror(); break;
					case VERT_MIROR : ImToOper->vertMiror(); break;
					case RESIZE : tmp = strpbrk( value[ i ], "x"); *tmp =0; ImToOper->resize( atoi(value[ i ]), atoi(tmp+1)); *tmp = 'x'; break;
					case GRAB : tmp2 = strpbrk( value[ i ], "x"); tmp3 = strpbrk( tmp2, "-"); tmp4 = strpbrk( tmp3, "x"); *tmp4=(*tmp3=(*tmp2=0)); ImToOper->grab( atoi(value[ i ]), atoi(tmp2+1), atoi(tmp3+1), atoi(tmp4+1)); *tmp2='x'; *tmp3='-'; *tmp4='x'; break;
					case PUT : tmp5 = strpbrk( value[ i ], "x"); tmp6 = strpbrk( tmp5, "-"); tmp7 = strpbrk( tmp6, "x"); *tmp7=(*tmp6=(*tmp5=0)); ImToOper->replaceInNewImage( atoi(value[ i ]), atoi(tmp5+1), atoi(tmp6+1), atoi(tmp7+1)); *tmp5='x'; *tmp6='-'; *tmp7='x'; break;
					case SMOOTH : ImToOper->smooth(atoi(value[ i ])); break;
					case SAVEPPM : { PicturePPM tmpPict( ImToOper->Im, ImToOper->getWidth(), ImToOper->getHeight()); tmpPict.write_ppm( basestream::conform( value[ i ] ) ); } break;
					case SAVETARGA : { PictureTARGA tmpPict( ImToOper->Im, ImToOper->getWidth(), ImToOper->getHeight()); tmpPict.writeTARGA( basestream::conform( value[ i ] ) ); } break;
					case INVERT : ImToOper->invert( ); break;
					case SCALE : ImToOper->scale( atof(value[i]) ); break;
				}
			}
			ImToOper->updateInt();
		}

		/// Learning phylosophy
		/// **************
		/// - many maps can be selected for learning
		/// - when a map is selected, its operator (-2) is set to -102
		/// - then other map can be extrated thereafter
		/// - the reInit Learn procedure, reinitialize learning maps to be extracted
		
		/// get the maps to learn
		void getLearn(char *mapName, int *posx, int *posy) {
			// warning: pointer must be reserved
			int res = getIndex( LEARN);
			oper[ res ] = LEARN_DESACTIVATED; //inative state
			char *tmp5 = strpbrk( value[ res ], "-");
			char *tmp6 = strpbrk( tmp5, "x");
			*tmp5 = 0; *tmp6 = 0;
			
			strcpy( mapName, value[res]);
			*posx = atoi( tmp5+1);
			*posy = atoi( tmp6+1);
		}
		
		/// reset extraction of learning maps
		void resetGetLearn() {
			for (int i=0; i< nbOper; i++)
				if (oper[i] == LEARN_DESACTIVATED) oper[i] = LEARN;
		}	
};
#endif