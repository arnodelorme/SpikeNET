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

// $Log: GlobalImg.cpp,v $

#include "GlobalImg.h"
#include <stdio.h>

void GlobalImg::setSize(int w, int h)
{ 
	width = w;
	height = h;
	
	if (ImInt != NULL)	delete ImInt; 
	if (Im    != NULL)	delete Im; 
  
  	ImInt = new FORMAT_POIDS[width * height];
  	Im    = NULL;
}

GlobalImg::GlobalImg(unsigned char *Im0, int w0, int h0)  : BaseImg( Im0, w0, h0 )
{
	ImInt = new FORMAT_POIDS[width * height];
	if (Im0 != NULL)
   		for (int i=0; i< width*height; i++) ImInt[i] = (FORMAT_POIDS) Im[i];
}

GlobalImg::GlobalImg(int w0, int h0)  : BaseImg(w0, h0)
{
    ImInt = new FORMAT_POIDS[width * height];
}

GlobalImg::GlobalImg(char *fileName, int type) : BaseImg()
{
  //type 0 = image
  //type 1 = convolution
    
  ImInt = NULL;
  Im = NULL;
  bytePerPixel = 1;
  if (type == 0)
	revertImg(fileName);
  else
    {                              
      FILESTREAM f(fileName);
      if (!f) ERROR(2,("unable to open file %s", fileName));
      
      //lecture de la convolution pour la retine
      char s[20];
      f >> s; width = atoi(s);
      f >> s; height = atoi(s);
      f >> s;
      
      //lecture du fichier : attention poids entiers
      ImInt = new FORMAT_POIDS[width*height];
  	  if (ImInt == NULL)
  		ERROR(1,("out of memory"));
      for (int j=0;j<height;j++)
		for (int i=0;i<width;i++)
	  {
	    f >> s; 
	    ImInt[i+j*width] = (FORMAT_POIDS) atof(s);
	  }
    }
  //Im = (unsigned char *) ImInt;
}

void GlobalImg::revertImg(char *fileName)
{
   
   BaseImg *pict;
   if (strstr(fileName, "TGA") != NULL) pict = new PictureTARGA(basestream::conform( fileName ) );
   else pict = new PicturePPM( basestream::conform( fileName ) );
   
   bytePerPixel = pict->getBytePerPixel();
   if ( (width  != pict->getWidth()) || (height != pict->getHeight()))
   {
  	 	width  = pict->getWidth();
   		height = pict->getHeight();
   		if (Im != NULL) delete Im;
   		if (ImInt != NULL) delete ImInt;
   		Im = NULL; ImInt = NULL;
   }
   if (Im == NULL)	Im 	  = new unsigned char[ width*height*bytePerPixel ];
   if (ImInt==NULL) ImInt = new FORMAT_POIDS[ width*height*bytePerPixel ];
   if (ImInt == NULL) ERROR(1,("out of memory"));
   for (int i=0; i< width*height*bytePerPixel; i++) { ImInt[i] = (FORMAT_POIDS) pict->Im[i]; Im[i] = pict->Im[i]; }
   delete pict;
   /*BaseImg *pict = new PicturePPM(fileName);
   if ( (width  != pict->getWidth()) || (height != pict->getHeight()))
   {
  	 	width  = pict->getWidth();
   		height = pict->getHeight();
   		if (Im != NULL) delete Im;
   		if (ImInt != NULL) delete ImInt;
   		Im = NULL; ImInt = NULL;
   }
   if (Im == NULL)	Im 	  = new unsigned char[ width*height ];
   if (ImInt==NULL) ImInt = new FORMAT_POIDS[ width*height ];
   if (ImInt == NULL) ERROR(1,("out of memory"));
   for (int i=0; i< width*height; i++) { ImInt[i] = (FORMAT_POIDS) pict->Im[i]; Im[i] = pict->Im[i]; }
   delete pict;*/
}

void GlobalImg::renormalize( FORMAT_POIDS renorm )
{
	FORMAT_POIDS sum = 0;
	
	for (int j=0; j<width * height; j++)
		sum += ImInt[ j ];

	for (int i=0; i<width * height; i++)
		ImInt[i] = (FORMAT_POIDS)((ImInt[i] * renorm) / sum);
}

void GlobalImg::renormalizeMax( FORMAT_POIDS renorm )
{
	minmax();
		
	for (int i=0; i<width * height; i++)
		ImInt[i] = (FORMAT_POIDS)((ImInt[i] * renorm) / max);
}

void GlobalImg::clear(int val)
{
  for (int i=0; i< width*height*bytePerPixel; i++)
	ImInt[i] = val;
}	

void GlobalImg::minmax()
{
  max = -10000000;
  min =  10000000;
  for (int i=0; i< width*height*bytePerPixel; i++)
    {
      if ((ImInt[i]>max) && (ImInt[i]!=INACTIVE)) max = ImInt[i];
      if ((ImInt[i]<min) && (ImInt[i]!=INACTIVE)) min = ImInt[i];
    }
}

void GlobalImg::saveText(char *fileName) {
	char tmp[200]; 
	FILE *fo = fopen( fileName, "w" );

	for (int j=0;j< height;j++) {
		for (int i=0;i< width;i++) {
			if (ImInt[i+j*width] != INACTIVE)
				fprintf(fo, "%f\t", ImInt[i+j*width] );
			else 	fprintf(fo, "0\t" );
		fprintf(fo, "\n");
		}
	}
	fclose(fo);
 }

void GlobalImg::convert2char(int renorm) {
	if (Im == NULL)	Im = new unsigned char[ width*height*bytePerPixel ];
	if (renorm) {
  		minmax();
  
  		if (max == min) max *= 256;
  		if ((-min) > max) max = -min; else min = -max;
  
  		for (int i=0; i< width*height*bytePerPixel; i++)
  			if (ImInt[i] != INACTIVE) Im[i] = (unsigned char)( ((ImInt[i]+max) * 255)/(2*max) );
    			else Im[i]=0;
   	}
   	else
   		for (int i=0; i< width*height*bytePerPixel; i++) 
   			if (ImInt[i] != INACTIVE) Im[i] = (unsigned char)( ImInt[i] );
   			else Im[i] = 0;
  		
}

void GlobalImg::updateInt()
{
	if (ImInt != NULL) delete [] ImInt;
  	ImInt = new FORMAT_POIDS[ width*height*bytePerPixel ];
  	for (int i=0; i< width*height*bytePerPixel; i++)
    	ImInt[ i ] = (FORMAT_POIDS)Im[ i ];
}
