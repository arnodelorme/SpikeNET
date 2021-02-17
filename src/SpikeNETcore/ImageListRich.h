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

// $Log: ImageListRich.h,v $

#ifndef _IMAGELISTRICH
#define _IMAGELISTRICH

#include "ImageList.h"

class ImageListRich : public ImageListLearn
{
   class ImageModifier
   {
   		int		oper[10];
   		char	*value[10];
   		int		nbOper;

		public :
		   		
   		ImageModifier() { nbOper = 0; }
		void addOper( int codeOper, char *valueOper)
		{ oper[ nbOper ] = codeOper; if (valueOper != NULL) { value[ nbOper ] = new char[strlen(valueOper)+1]; strcpy( value[ nbOper ], valueOper); } nbOper++; }
		
		void applyOper( GlobalImgTreat *ImToOper )
		{
			char *tmp, *tmp1, *tmp2, *tmp3, *tmp4, *tmp5, *tmp6, *tmp7, *tmp15;
			
			//for (int i=0; i< nbOper; i++)
			//  printf("Apply ************************* %d:%s, %d\n", oper[ i ], value[ i ], ImToOper->Im);
			
			for (int i=0; i< nbOper; i++)
			//for (int i=0; i< 0; i++)
			{
				switch( oper[ i ] )
				{
					case 0 : ImToOper->recalculateContrast( atof(value[ i ]) ); break;
					case 1 : tmp15 = strpbrk( value[ i ], "_"); if (tmp15 != NULL) { *tmp15 =0; ImToOper->random( atof(value[ i ]),  atoi(tmp15+1)); *tmp15 ='_'; } else ImToOper->random( atof(value[ i ])); break;
					case 2 : ImToOper->shiftLum( atoi(value[ i ]) ); break;
					case 3 : ImToOper->rotate90(); break;
					case 4 : ImToOper->horzMiror(); break;
					case 5 : ImToOper->vertMiror(); break;
					case 6 : tmp = strpbrk( value[ i ], "x"); *tmp =0; ImToOper->resize( atoi(value[ i ]), atoi(tmp+1)); *tmp = 'x'; break;
					case 7 : tmp2 = strpbrk( value[ i ], "x"); tmp3 = strpbrk( tmp2, "-"); tmp4 = strpbrk( tmp3, "x"); *tmp4=(*tmp3=(*tmp2=0)); ImToOper->grab( atoi(value[ i ]), atoi(tmp2+1), atoi(tmp3+1), atoi(tmp4+1)); *tmp2='x'; *tmp3='-'; *tmp4='x'; break;
					case 8 : tmp5 = strpbrk( value[ i ], "x"); tmp6 = strpbrk( tmp5, "-"); tmp7 = strpbrk( tmp6, "x"); *tmp7=(*tmp6=(*tmp5=0)); ImToOper->replaceInNewImage( atoi(value[ i ]), atoi(tmp5+1), atoi(tmp6+1), atoi(tmp7+1)); *tmp5='x'; *tmp6='-'; *tmp7='x'; break;
					case 9 : ImToOper->smooth(atoi(value[ i ])); break;
					case 10 : { PicturePPM tmpPict( ImToOper->Im, ImToOper->getWidth(), ImToOper->getHeight()); tmpPict.write_ppm( basestream::conform( value[ i ] ) ); } break;
					case 11 : ImToOper->invert( ); break;
					case 12 : ImToOper->scale( atof(value[i]) ); break;
				}
			}
			ImToOper->updateInt();
		}
	};
	
   ImageModifier *tabModif[MAX_IMAGE];
   ImageModifier *currentModif;
				
/*   ImageListRich(char *s, int step0) : ImageListLearn( step0 )
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
   }	*/
   
public:

	ImageListRich( int step0) : ImageListLearn( step0)
    { }
 
   void read_all_images(char *s)
   {
		char image[200];
      	char map[100];
      	char coordx0[30];
      	char coordy0[30];
      	char value0[30];
      	char line[200];
    	char tmp[200];

		tab_img = new ptr_char[MAX_IMAGE];	
    	char new_net_lst[100];
    	sprintf(  new_net_lst, "%s%s%c%s", CURRENT_DIR, EnvVar::MAIN_DIRECTORY, SEPARATEUR, LST_IMG );
	
		FILESTREAM f(new_net_lst);
		int eof1, eof2;
		nb_image=0;
		char *newimage;
	
		allimages = new char[100*MAX_IMAGE];
		tab_img[0] = allimages;

		do
		{
			eof1 = f.getline( line, 150, '\n');

	  		if (strlen(line) > 1)
	    	{
				istrngstream ff(line);
	    		tabLearn[ nb_image ] = NULL;
      			tabModif[ nb_image ] = new ImageModifier();
      			image[0] = 0;
	  			
	  			do
	  			{
	  				ff >> tmp;
	  				switch( tmp[ 0 ] )
	  				{
	  					case 0   : 	break;
	  					case 'L' : 
	  					case 'l' : 	if ( (tmp[1]=='e')||(tmp[1]=='E') ) { ff >> map; ff >> coordx0; ff >> coordy0; tabLearn[nb_image] = new MapDescLearn( map, atoi(coordx0), atoi(coordy0), tabLearn[nb_image]); }
	  								else								{ ff >> value0; tabModif[ nb_image ]->addOper( 2, value0 ); }
	  								break;
	  					case 'R' :
	  					case 'r' : 	if ( (tmp[1]=='a')||(tmp[1]=='A') ) { ff >> value0; tabModif[ nb_image ]->addOper( 1, value0 ); } 
									else if ( (tmp[1]=='e')||(tmp[1]=='E') ) { ff >> value0; tabModif[ nb_image ]->addOper( 6, value0 ); } 
	  								else								
	  								{ 
	  									ff >> value0;
	  									int intval = (atoi(value0) + 360) % 360;
	  									if ((intval % 90) != 0) 	ERROR( 136, ("invalid value for rotation %d in file lst_img", intval))
	  									if ((intval / 90) >= 1)	tabModif[ nb_image ]->addOper( 3, NULL );
	  									if ((intval / 90) >= 2)	tabModif[ nb_image ]->addOper( 3, NULL );
	  									if ((intval / 90) >= 3)	tabModif[ nb_image ]->addOper( 3, NULL );	  									
	  								}
	  								break; 
	  					case 'C' :
	  					case 'c' : 	ff >> value0; tabModif[ nb_image ]->addOper( 0, value0 ); break; 
	  					case 'P' :
	  					case 'p' : 	ff >> value0; tabModif[ nb_image ]->addOper( 8, value0 ); break; 
	  					case 'H' : 
	  					case 'h' : 	tabModif[ nb_image ]->addOper( 4, NULL ); break;
	  					case 'G' : 
	  					case 'g' :	ff >> value0; tabModif[ nb_image ]->addOper( 7, value0 ); break;
	  					case 'V' : 
	  					case 'v' :	tabModif[ nb_image ]->addOper( 5, NULL ); break;
	  					case 'I' :
	  					case 'i' :	if ( (tmp[1]=='n')||(tmp[1]=='N') ) { tabModif[ nb_image ]->addOper( 11, NULL ); }
	  								else ff >> image; break;
	  					case 'S' :
	  					case 's' :  if ( (tmp[1]=='m')||(tmp[1]=='M') ) {  ff >> value0; tabModif[ nb_image ]->addOper( 9, value0 ); }
	  								else if ( (tmp[1]=='a')||(tmp[1]=='A') ) { ff >> value0; tabModif[ nb_image ]->addOper( 10, value0 ); }
	  								else { ff >> value0; tabModif[ nb_image ]->addOper( 12, value0 ); } break;
	  					default  :	ERROR( 134, ("undefined symbol '%s' in file lst_img", tmp))
	  				}
	  			} while (tmp[0]!=0);
	  			
	  			if (image[0] == 0) ERROR( 135, ("no image defined in line '%s' of file lst_img", tmp))
	  			else
	  			{
	      			if (nb_image) tab_img[nb_image] = tab_img[nb_image-1] + strlen(tab_img[nb_image-1]) + 1;
	      			strcpy(tab_img[nb_image], CURRENT_DIR);
	      		
     				newimage = image;
      				while ((newimage = strpbrk(newimage, "/")) != NULL)
						*(newimage++) = SEPARATEUR;
      				strcat(tab_img[nb_image], image);
	      			nb_image++;
						   
	      			if (nb_image > MAX_IMAGE)	ERROR(201,("maximum of images overflow"));
				}
	    	}
	  	} while (eof1!=EOF);
		
		//for (int index=0; index< nb_image; index++)
		//  printf("%s\n", tab_img[index]);
		//exit(-1);
      }

	void reserv()
	  {
		read_all_images(DIRECTORY);		
    	ptrImg[0] = new GlobalImgTreat(tab_img[0], 0);
    	tabModif[0]->applyOper( ptrImg[0] );
	  }
	  
 	virtual GlobalImgTreat **revert()
 	{
 		currentLearn = tabLearn[countImage];
 		if (countImage+imgAtOnce < nb_image+1)
 			{
 				for (int i=0; i<imgAtOnce; i++)
 				{
 					ptrImg[i]->revertImg(tab_img[countImage+i]);
 					tabModif[countImage+i]->applyOper(ptrImg[i]);
 				}
 				countImage += step;
				return ptrImg;
			}
		return NULL;
	}
	
    void reset( GlobalImgTreat *im)
	  { 
	  	countImage = 0;
	  	step	   = 1;
	  	nb_image   = 1;
	  	ptrImg[0]  = im;
	  	tabModif[0]->applyOper( ptrImg[0] );
	  }
	  		
	virtual void init( int imgAtOnce0)
	{
		// initialisation			
    	imgAtOnce = imgAtOnce0;
    	for (int i=1; i<imgAtOnce; i++)
    	{
    		ptrImg[i] = new GlobalImgTreat(tab_img[0], 0);
 			tabModif[0]->applyOper(ptrImg[i]);
 		}
	}
};

#endif
