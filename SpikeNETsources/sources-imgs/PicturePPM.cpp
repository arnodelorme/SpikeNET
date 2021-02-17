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

// $Log: PicturePPM.cpp,v $

#include "PicturePPM.h"

#ifndef MODE
#define MODE 0
#endif

#ifndef ERROR
#define ERROR(X, Y)		{ printf("SpikeError %d: ", X); printf Y; printf("\n"); exit(-1); }
#endif

// write image on disk
void PicturePPM::write_ppm(	char *FileName, int step)
{
	//------------------ OLD C VERSION ----------------------
	char            TrueFileName[100];
	sprintf(TrueFileName, "%s.ppm", FileName);
	FILE            *fd = fopen(TrueFileName, "wb");			
	if (!fd) ERROR(5,("unable to open a new file for PPM %s", TrueFileName)); 
	// A PPM HEADER 
	fprintf(fd, "P5 %u %u %u\n", width, height, 255);
	//for (int i = 0; i < width * height; i+=step) putc(Im[i],fd);
	int realwidth = width*step;		
	for (int j=0; j< height; j++)
		for (int i = 0; i < realwidth; i+=step) 
			putc(Im[i + j*realwidth],fd);		
	fclose(fd);
} 

void PicturePPM::write_ppm_grab( int absc, int ordo, int widthG, int heightG, char *FileName)
{
	//------------------ OLD C VERSION ----------------------
	char            TrueFileName[100];
	sprintf(TrueFileName, "%s.ppm", FileName);
	FILE            *fd = fopen(TrueFileName, "wb");			
	if (!fd) ERROR(5,("unable to open a new file for PPM %s", TrueFileName)); 
	// A PPM HEADER 
	fprintf(fd, "P5 %u %u %u\n", widthG, heightG, 255);

	for (int j=ordo; j<heightG+ordo;j++)
		for (int i=absc; i<widthG+absc;i++)
			if ((j>=0) && (i>=0) && (j<height) && (i<width))
				putc(Im[ i + j*width ],fd);
			else putc(0,fd);		
	for (int i = 0; i < width * height; i++) putc(Im[i],fd);		
	fclose(fd);
} 

// remove comments and get the descrition of the image
int PicturePPM::getint(FILE *fp)
{
    int c, i, garbage;

    // note:  if it sees a '#' character, all characters from there to end of
    // line are appended to the comment string

    // skip forward to start of next number
    c = getc(fp);
    while (1) {
        // eat comments
        if (c=='#')
              while (((c=getc(fp))!='\n') && (c!=EOF)) {};
        if (c==EOF) return 0;
        if (c>='0' && c<='9') break;
        // see if we are getting garbage (non-whitespace)
        if (c!=' ' && c!='\t' && c!='\r' && c!='\n' && c!=',') garbage=1;
        c = getc(fp);
    }

    /*if (garbage)
      	{
	  fclose(fp);
	  ERROR(0,("Garbage characters in header")); 
	}*/

    // we're at the start of a number, continue until we hit a non-number
    i = 0;
    while (1) {
        i = (i*10) + (c - '0');
        c = getc(fp);
        if (c==EOF) return i;
        if (c<'0' || c>'9') break;
    }
    return i;
}

PicturePPM::PicturePPM(char *FileName) {
    FILE           	*fp;
    bytePerPixel  = 1;

    if (!(fp = fopen(FileName, "rb")))
    	ERROR(6,("read_ppm, Impossible to find the file %s", FileName));
	 
    char c  = getc(fp);  
    char c1 = getc(fp);
    if (c!='P' || c1!='5')
    	ERROR(7,("unknown PGM format: NOT A P5 PPM FILE %s", FileName)); 

    width  = getint(fp);
    height = getint(fp);
    int GrayLevelNb = getint(fp);
		
    //Im = new (unsigned char *)malloc(sizeof(char)*size);
    Im = new unsigned char[ height*width ];
  	if (Im == NULL) ERROR(1,("out of memory"));
    long numgot = fread(Im, 1, height*width, fp);   
	    	
    if (numgot != height*width)
	ERROR(8,("truncated PPM File %s", FileName)); 

   fclose(fp);

}
