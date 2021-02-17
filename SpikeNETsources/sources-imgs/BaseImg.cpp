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

// $Log: BaseImg.cpp,v $

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define rint(X)		   (int)(((X + 0.5) == (int)X) ? (int)X : (int)X+1)

#include "BaseImg.h"

	unsigned int BaseImg::getRVBPix( int posx, int posy)
	{
		unsigned int res = 0;
		switch (bytePerPixel)
		{
			case 1 : res = Im[ posx + posy*width ]; res = 0xFF000000 + (res << 16) + (res << 8) + res; break;
			case 3 : res = 0xFF000000 + (Im[ (posx+posy*width)*bytePerPixel + 2 ] << 16) + (Im[ (posx+posy*width)*bytePerPixel+1 ] << 8) +(Im[ (posx+posy*width)*bytePerPixel ]); break; 
//			case 3 : *((int *)(Im + (posx + posy*width)*bytePerPixel)) | 0xFF000000; break;
			case 4 : res = *((unsigned int *)Im +  posx + posy*width);
			
		}
		return res;
	}
	
	int BaseImg::setDepth( int newBytePerPixel) 
	{
		unsigned char 	*ImTmp = new unsigned char[ width * height * newBytePerPixel ];
		
		int tmpBytePix = bytePerPixel;
		if (newBytePerPixel < bytePerPixel) tmpBytePix = newBytePerPixel;
		
		for (int i=0; i< width * height; i++)
		{
			int j;
			
			// decrease depth
			if (newBytePerPixel <= bytePerPixel)
				for (j=0; j < newBytePerPixel; j++)
					*(ImTmp + i*newBytePerPixel+j) = *(Im + i*bytePerPixel+j);
			else
			{
				for (j=0; j < bytePerPixel; j++)
					*(ImTmp + i*newBytePerPixel+j) = *(Im + i*bytePerPixel+j);
				for (; j < newBytePerPixel; j++)		
					*(ImTmp + i*newBytePerPixel+j) = *(Im + i*bytePerPixel);
			}
		}
		
		bytePerPixel = newBytePerPixel;
		Im = ImTmp;
		delete ImTmp;
		return 1;
	}

	void BaseImg::clear( int color )
	{
		for(int j=0;j<height*width*bytePerPixel;j++) Im[j] = (unsigned char)color;
		/*for(int j=0;j<height;j++)
	    	for(int i=0;i<width;i++)
				put_pixel( i, j, color);*/
	}

	void BaseImg::put_pixel( int pos_x, int pos_y, int color)
	{
		unsigned char *ptre = Im + pos_x*bytePerPixel + pos_y * width*bytePerPixel;
		for (int k=0; k<bytePerPixel; k++)
			*ptre++ = (unsigned char)color;
	}
	
	void BaseImg::put_RVB_pixel( int pos_x, int pos_y, int color)
	{
		unsigned char *ptre = Im + pos_x*bytePerPixel + pos_y * width*bytePerPixel;
		for (int k=0; k<bytePerPixel; k++)
			*ptre++ = (unsigned char)((color >> 8*k) & 0xFF);
//			*ptre++ = (unsigned char)(color >> 8*(bytePerPixel - 1 - k)) & 0xFF;
	}

	void BaseImg::horzMiror()
	{
		for (int j=0; j< height; j++)
    		for (int i=0; i< width / 2; i++)
   				for (int k=0; k< bytePerPixel; k++)
    			{
     				unsigned char miror = Im[i+j*width*bytePerPixel + k];
    				Im[i+j*width*bytePerPixel + k] = Im[(width*bytePerPixel - i - bytePerPixel) + j*width*bytePerPixel + k];
    				Im[(width*bytePerPixel - i - bytePerPixel) + j*width*bytePerPixel + k] = miror;
				}
	}

	void BaseImg::vertMiror()
	{
		for (int j=0; j< height / 2; j++)
    		for (int i=0; i< width; i++)
   				for (int k=0; k< bytePerPixel; k++)
    			{
     				unsigned char miror = Im[i+j*width*bytePerPixel + k];
    				Im[i+j*width*bytePerPixel + k] = Im[i+ (height - j - 1)*width*bytePerPixel + k];
    				Im[i+ (height - j - 1)*width*bytePerPixel + k] = miror;
				}
	}

	double BaseImg::meanLum()
	{
		double meanLum = 0;
  		for (int j=0; j< height; j++)
    		for (int i=0; i< width; i++)
				meanLum += Im[i+j*width];
		return meanLum / (width*height);
	}
				
	void BaseImg::smooth(int nb )
	{
  		unsigned char *ImTmp = new unsigned char[width*height*bytePerPixel];
		unsigned char *ImR = Im;
		unsigned char *ImW = ImTmp;
		
		// only for one byte per pixel; not optimized
		for (int count=0; count < nb; count++)
		{
			int sizeSm = 1;
			int tabsmooth[] = { 33, 118, 33, 118, 420, 118, 33, 118, 33 };
  			for (int j=0; j< height; j++)
    			for (int i=0; i< width; i++) {
    				int res = 0;
    				for (int l=-sizeSm; l<sizeSm+1; l++)
     					for (int m=-sizeSm; m<sizeSm+1; m++) {
     						int realAbs = i + m;
     						if (realAbs < 0) realAbs = 0;
     						if (realAbs >= width) realAbs = width-1;
     						int realOrd = j + l;
     						if (realOrd < 0) realOrd = 0;
     						if (realOrd >= height) realOrd = height-1;
     						res += ImR[realAbs + realOrd*width] * tabsmooth[ m+1 + (l+1)*3 ];
     					}
     				ImW[i+j*width] = (unsigned char)(res >> 10);
				}
			unsigned char *tmp = ImR; ImR = ImW; ImW = tmp;
		}
		Im = ImR;
		delete [] ImW;
     }

	void BaseImg::random( double randFact, int factScale )
	{
	  /* Below is for multiscale NOISE, does not work, commented out 4 Nov 2002
		for (int j=factScale; j< height-factScale; j+=factScale)
    		for (int i=factScale; i< width-factScale; i+=factScale)
    			for (int l=-factScale; l<factScale+1; l++)
     				for (int m=-factScale; m<factScale+1; m++)
     				{
     					double randVal = floor((double)rand()/RAND_MAX * 256.0) * randFact;
   						for (int k=0; k< bytePerPixel; k++)
    					{
    						//int tmpval = (int)((double)Im[i+j*width*bytePerPixel+k] + 127.5 * (2*(double)rand()/RAND_MAX-1) * randFact);
    						//int tmpval = (int)(randVal + (double)Im[i+m+(j+l)*width*bytePerPixel+k]*(1.0-randFact));
    						int tmpval = rint(randVal + (double)Im[i+m+(j+l)*width*bytePerPixel+k]*(1.0-randFact));
    				
    						if (tmpval > 255) 	{ tmpval = 255; }
    						if (tmpval < 0) 	{ tmpval = 0;   }
    		
    						Im[i+m+(j+l)*width*bytePerPixel+k] = (unsigned char) tmpval;    	
						}
						}*/
	  for (int j=0; j< height; j++)
		for (int i=0; i< width; i++)
		  for (int k=0; k< bytePerPixel; k++)
			{
			  //int tmpval = (int)((double)Im[i+j*width*bytePerPixel+k] + 127.5 * (2*(double)rand()/RAND_MAX-1) * randFact);
			  int tmpval = (int)(floor((double)rand()/RAND_MAX * 256.0) * randFact + (double)Im[i+j*width*bytePerPixel+k]*(1.0-randFact));
			  
			  if (tmpval > 255) tmpval = 255;
			  if (tmpval < 0) tmpval = 0;
			  
			  Im[i+j*width*bytePerPixel+k] = (unsigned char) tmpval;    	
			}
	}

	void BaseImg::shiftLum( int offset)
	{
		if (offset > 0)
  			for (int j=0; j< height; j++)
    			for (int i=0; i< width; i++)
  					for (int k=0; k< bytePerPixel; k++)
    					if (Im[i+j*width*bytePerPixel+k] > (255-offset)) 	Im[i+j*width*bytePerPixel+k]  = 255;
						else 												Im[i+j*width*bytePerPixel+k] += offset;
		else
  			for (int j=0; j< height; j++)
    			for (int i=0; i< width; i++)
   					for (int k=0; k< bytePerPixel; k++)
   						if (Im[i+j*width*bytePerPixel+k] < (unsigned char)(-offset)) 	Im[i+j*width*bytePerPixel+k]  = 0;
						else 															Im[i+j*width*bytePerPixel+k] += (unsigned char) offset;
	}
	
	void BaseImg::recalculateContrast( double factor)
	{	
  		for (int j=0; j< height; j++)
    		for (int i=0; i< width; i++)
   				for (int k=0; k< bytePerPixel; k++)
    			{
    				int tmpval = (int)(((double)Im[i+j*width*bytePerPixel+k] - 128.0) * factor) + 128;
    				tmpval = (int)(((double)Im[i+j*width*bytePerPixel+k] - 128.0) * (1.0/factor)) + 128;
    				if (tmpval > 255) tmpval = 255;
    				if (tmpval < 0) tmpval = 0;
    		
    				Im[i+j*width*bytePerPixel+k] = (unsigned char) tmpval;
				}
	}
	
	void BaseImg::rotate90()
	{
  		unsigned char *ImTmp = new unsigned char[width*height*bytePerPixel];

	  	/*for (int j=0; j< height; j++)
    		for (int i=0; i< width; i++)
   				for (int k=0; k< bytePerPixel; k++)
					ImTmp[i+j*width*bytePerPixel+k] = Im[j + (width-1-i)*height + k];*/
	  	for (int j=0; j< height; j++)
    		for (int i=0; i< width; i++)
					ImTmp[ j + (width-1-i)*height ] = Im[ i + j*width ];

  		int w = width;
  		width  = height;
  		height = w;

		delete [] Im;
  		Im = ImTmp;
	}

	double BaseImg::addLine( int posAbs, double multiplier, double posFlt, double factor )
	{
		if (((posFlt - floor(posFlt)) + factor) < 1.00000001)
			return multiplier*factor*Im[ posAbs ];
		else
		{
			double tmpval = (1-(posFlt - floor(posFlt))) * Im[ posAbs ];
			double tmpfactor = factor - 1 + (posFlt - floor(posFlt));
			posAbs++;
			
			while (tmpfactor > 1.0)
			{
				tmpval += Im[ posAbs ];
				tmpfactor--;
				posAbs++;
			}
			
			tmpval += Im[ posAbs ] * tmpfactor;
			return tmpval * multiplier;
		}
	}

	void BaseImg::replaceInNewImage( int widthN, int heightN, int absN, int ordN)
	{
		unsigned char *tmpIm = new unsigned char[widthN * heightN];
		for (int ii=0; ii<widthN * heightN; ii++) tmpIm[ ii ] = 128;
		
		for (int j=0; j<heightN;j++)
			for (int i=0; i<widthN;i++)
			{
				if ((i>=width+absN) || (i<absN) || (j>=height+ordN) || (j<ordN))
				{
					// extrapolation procedure
					/*
					int absO = 0, ordO = 0, distX = absN-i, distY = ordO-j;
					
					if (i>=width+absN)  { absO = width-1; distX = i - (width+absN)+1; }
					else if (i>=absN)   { absO = i; distX = 0; }
					
					if (j>=height+ordN)  { ordO = height-1; distY = j - (height+ordN)+1; }
					else if (j>=ordN)    { ordO = j; distY = 0; }
					
					int realDist = distX*distX + distY*distY;
					
					tmpIm[ i+j*widthN ] = (unsigned char) ((((int)Im[ absO + ordO*width ] << 6)/ (64 + realDist)) );*/
					tmpIm[ i+j*widthN ] = 128;
				}
				else
					tmpIm[i+j*widthN] = Im[ i - absN + (j-ordN)*width ];
					/*{
						int newAbs = absN + i;
						int newOrd = ordN + j;
						if ((newOrd<heightN) && (newAbs<widthN))
							tmpIm[newAbs + newOrd*widthN] = Im[ i + j*width ];
					}*/
			}
		width  = widthN;
		height = heightN;
		delete [] Im;
		Im = tmpIm;
	}
	
	void BaseImg::scale( double factor)
	{	resize( rint(factor * width), rint(factor * height)); }
	
	void BaseImg::resize( int w0, int h0)
	{
		int w = w0; // zoom;
		int h = h0; // zoom;
		unsigned char *tmpIm = new unsigned char[w * h];
		
		double 		factorw = (double)width/(double)w;
		double 		factorh = (double)height/(double)h;
		double		posw = 0, posh =0;
		int 		counterw =0, counterh =0;
		
		for (int j=0; j<h; j++)
		{
			//System.out.println( "line : " + originalWidth + "x" + originalHeight +"     " +j + "," + posh + "   " +counterh);
			posw = 0;
			counterw =0;
			for (int i=0; i<w; i++)
			{
				counterh = (int) posh;
				double tmpval = 0;

				if (((posh - floor(posh)) + factorh) < 1.00000001)
				{	// only 1 column
					tmpval = addLine( counterw + counterh*width, factorh, posw, factorw);
					posw +=factorw;
					counterw = (int) posw;
				}
				else
				{	// more than 1 column								
					tmpval       = addLine( counterw + counterh*width, 1-(posh - floor(posh)), posw, factorw);
					double tmpfactorh = factorh - 1 + (posh - floor(posh));
					counterh++;
			
					while (tmpfactorh > 1.0)
					{
						tmpval += addLine( counterw + counterh*width, 1, posw, factorw);
						tmpfactorh--;
						counterh++;
					}
					
					tmpval += addLine( counterw + counterh*width, tmpfactorh, posw, factorw);
					posw +=factorw;
					counterw = (int) posw;
				}

				tmpval = tmpval/(factorw*factorh);
				if (tmpval > 255) tmpval = 255;				
				int grey  = ((int)tmpval) & 0xFF;
				tmpIm[ i + j*w] = (255 << 24) | (grey << 16) | (grey << 8) | grey;
			}
			posh += factorh;		
		}
		width  = w;
		height = h;
		delete [] Im;
		Im  = tmpIm;
	}

	void BaseImg::div2()
	{
    	int w 	= (int)ceil((double)width / 2);
    	int h 	= (int)ceil((double)height / 2);

	  	for (int j=0; j< h; j++)
    		for (int i=0; i< w; i++)
    		{
    			int value = 0;
	  			for (int l=0; l< 2; l++)
    				for (int k=0; k< 2; k++)
    					if ((i*2+k < width) && (j*2+l < height))
    						value += Im[ i*2 + k + (j*2+l)*width ];
    			Im[ i + j*w ] = (unsigned char) (value >> 2);
			}
  		width  = w;
  		height = h;
  	}
	
	void BaseImg::grab( int absc, int ordo, int widthG, int heightG)
	{
		int cmpt =0;

		for (int j=ordo; j<heightG+ordo;j++)
			for (int i=absc; i<widthG+absc;i++)
				if ((j>=0) && (i>=0) && (j<height) && (i<width))
					Im[cmpt++] = Im[ i + j*width ];
				else Im[cmpt++] = 0;		
		width  = widthG;
		height = heightG;
	}
	
	void BaseImg::drawRect( int xinit, int yinit, int xend, int yend, int color)
	{
		if (xend > width) xend = width;
		if (yend > height) yend = height;
		for (int i=xinit; i< xend; i++)
		{
			for (int k=0; k< bytePerPixel; k++)
			{
				Im[ i*bytePerPixel + k + yinit*width*bytePerPixel ] = (color >> 8*(bytePerPixel - 1 - k)) & 0xFF;
				Im[ i*bytePerPixel + k + (yend-1)*width*bytePerPixel  ] = (color >> 8*(bytePerPixel - 1 - k)) & 0xFF;
			}
		}
		for (int j=yinit; j< yend; j++)
		{
			for (int k=0; k< bytePerPixel; k++)
			{
				Im[ xinit*bytePerPixel    + k + j*width*bytePerPixel ] = (color >> 8*(bytePerPixel - 1 - k)) & 0xFF;
				Im[ (xend-1)*bytePerPixel + k + j*width*bytePerPixel  ] = (color >> 8*(bytePerPixel - 1 - k)) & 0xFF;
			}
		}
	}
	
	void BaseImg::invert()
	{
  		for (int j=0; j< height; j++)
    		for (int i=0; i< width; i++)
   				for (int k=0; k< bytePerPixel; k++)
    				Im[i+j*width*bytePerPixel+k] = 255-Im[i+j*width*bytePerPixel+k];
	}		
	
	void BaseImg::recopi_ecran( unsigned char *base_copy, int width_copy)
	{
		unsigned char *copy_tmp = base_copy;
		unsigned char *img_tmp = Im;
		for(int j=0;j<height;j++)
		{
	    	for(int i=0;i<width;i++)
 				*(copy_tmp++) = *(img_tmp++);
			copy_tmp += width_copy - width;
		}
	}
