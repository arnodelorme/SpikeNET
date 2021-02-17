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

// $Log: PictureTARGA.cpp,v $

#include "PictureTARGA.h"

#ifndef ERROR
#define ERROR(X, Y)		{ printf("SpikeError %d: ", X); printf Y; printf("\n"); exit(-1); }
#endif

PictureTARGA::PictureTARGA (char  *fileName) : BaseImg()
	{
   		FILE           	*fp;

    		if (!(fp = fopen(fileName, "rb")))
    			ERROR(0,("read_targa, Impossible to find the file %s", fileName));
		
			// the first int must be 0
    		char c  = getc(fp);  
    		char c1 = getc(fp);
    		char c2 = getc(fp);  
    		char c3 = getc(fp);
		
			if (((c | c1) != 0) || (c2 != 2))
				if ((c | c1) != 0)	ERROR(0, ("not a TGA file "))
				else				ERROR(0, ("bat TGA file format"));

			c3 = getc(fp);	c3 = getc(fp);
			c3 = getc(fp);	c3 = getc(fp);
			c3 = getc(fp);	c3 = getc(fp);
			c3 = getc(fp);	c3 = getc(fp);
				        		
        	width   = (((int)(getc(fp)+256)) & 0xFF);
        	width   += ((((int)(getc(fp)+256)) & 0xFF) << 8);
        	height  = (((int)(getc(fp)+256)) & 0xFF);
        	height  += ((((int)(getc(fp)+256)) & 0xFF) << 8);
        		
			c3 = getc(fp);	c3 = getc(fp);
				
			bytePerPixel = 3;   
			Im = new unsigned char[ width * height * bytePerPixel];
			long numgot = fread(Im, 1, height*width*bytePerPixel, fp);
	    	
    		if (numgot != height*width*bytePerPixel)
				ERROR(0,("truncated TARGA File %s %d instead of ", fileName, numgot, height*width*3))

   			fclose(fp);
   			
   			//reverse the image
 			for (int j=0; j < height/2; j++)
				for (int i=0; i < width*bytePerPixel; i++)
					{	
						unsigned char tmp = *(Im + i + j * width*bytePerPixel);
						*(Im + i + j * width*bytePerPixel) = *(Im + i + (height-1-j) * width*bytePerPixel);
						*(Im + i + (height-1-j) * width*bytePerPixel) = tmp;
					}

		//conversion en entier automatiquement
		//convert2int();
	}			
	
void PictureTARGA::writeTARGA(char *fileName)
	{
		/*byte[]  cropIm  = new byte[ wSelect * hSelect * 3];
		int		posCrop = 0;

		for (int j=yInit; j < (yInit + hSelect); j++)
			for (int i=xInit; i < (xInit + wSelect); i++)
				if (((i>=0) && (j>=0)) && ((i < originalwidth) && (j < originalheight))) { cropIm[posCrop++] = Im[(i + j * originalwidth)*3]; cropIm[posCrop++] = Im[(i+1 + j * originalwidth)*3]; cropIm[posCrop++] = Im[(i+2 + j * originalwidth)*3]; }
				else						     				 						 { cropIm[posCrop++] = 0; cropIm[posCrop++] = 0; cropIm[posCrop++] = 0; }
		*/ 
		FILE            *fd;
		char            TrueFileName[50];
		sprintf(TrueFileName, "%s.tga", fileName);
			
		if (!(fd = fopen(TrueFileName, "wb")))
	   		ERROR(0,("unable to open a new file for TARGA %s", TrueFileName)); 

		// A TARGA HEADER 
		putc(0, fd); putc(0, fd);
		putc(2, fd);
		putc(0, fd); 
		putc(0, fd); putc(0, fd);
		putc(0, fd); putc(0, fd);
		putc(0, fd); putc(0, fd);
		putc(0, fd); putc(0, fd);
		putc((width % 256), fd); putc((width / 256), fd);
		putc((height % 256), fd); putc((height / 256), fd);
		putc(24, fd); putc(0, fd);		
			
		int size = (unsigned long) width * (unsigned long) height * 3;
		for (int j = 0; j < height; j++)
			for (int i = 0; i < 3*width; i++) 
					putc(Im[i + (height-1-j)*width*3],fd);
		
		fclose(fd);

	}			

void PictureTARGA::writeTARGA_grab(int absc, int ordo, int widthG, int heightG, char *fileName)
	{
		/*byte[]  cropIm  = new byte[ wSelect * hSelect * 3];
		int		posCrop = 0;

		for (int j=yInit; j < (yInit + hSelect); j++)
			for (int i=xInit; i < (xInit + wSelect); i++)
				if (((i>=0) && (j>=0)) && ((i < originalwidth) && (j < originalheight))) { cropIm[posCrop++] = Im[(i + j * originalwidth)*3]; cropIm[posCrop++] = Im[(i+1 + j * originalwidth)*3]; cropIm[posCrop++] = Im[(i+2 + j * originalwidth)*3]; }
				else						     				 						 { cropIm[posCrop++] = 0; cropIm[posCrop++] = 0; cropIm[posCrop++] = 0; }
		*/ 
		FILE            *fd;
		char            TrueFileName[50];
		sprintf(TrueFileName, "%s.tga", fileName);
			
		if (!(fd = fopen(TrueFileName, "wb")))
	   		ERROR(0,("unable to open a new file for TARGA %s", TrueFileName)); 

		// A TARGA HEADER 
		putc(0, fd); putc(0, fd);
		putc(2, fd);
		putc(0, fd); 
		putc(0, fd); putc(0, fd);
		putc(0, fd); putc(0, fd);
		putc(0, fd); putc(0, fd);
		putc(0, fd); putc(0, fd);
		putc((widthG % 256), fd); putc((widthG / 256), fd);
		putc((heightG % 256), fd); putc((heightG / 256), fd);
		putc(24, fd); putc(0, fd);		
			
		int size = (unsigned long) width * (unsigned long) height * 3;
		for (int j=ordo; j<heightG+ordo;j++)
			for (int i=absc*3; i<3*(widthG+absc);i++)
				if ((j>=0) && (i>=0) && (j<height) && (i<3*width))
					putc(Im[ i + (heightG+ordo-1-j)*width*3 ],fd);
			else putc(0,fd);
		
		fclose(fd);

	}			
