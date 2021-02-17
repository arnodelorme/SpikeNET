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

// $Log: ScreenSpike.h,v $

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "allDefs.h"
#include "PicturePPM.h"

#ifndef _XDISPLAY
#define _XDISPLAY

//#define XMAX 500
//#define YMAX 500

#if SYSTEME == UNIX
#include<X11/Xlib.h>
#include <X11/Xutil.h>

// this class deals with handling display
// --------------------------------------

class ScreenBase : public BaseImg
{
    GC ct2;
    Drawable win1;
    Window root;
    Pixmap swp;
    Display *dpy; 
public:
	int 		maxX;
	int 		maxY;
  
  public:
    
    void put_RVB_pixel( int x, int y, int color)
        {
            GC ct1 = ct2;
            XSetForeground(dpy,ct1,color);
            XDrawPoint( dpy, win1, ct1, x, y);
        }

    void put_RorVorB_pixel( int x, int y, int color ) { }
    void put_pixel( int x, int y, int color)
        {
            int realColor = color + (color << 8) + (color << 16);
            put_RVB_pixel( x, y, realColor);
        }
    
    ScreenBase( int posX, int posY, int maxX0, int maxY0, int saveAble) : BaseImg()
        {
        	maxX = maxX0;
        	maxY = maxY0;
            XEvent ev;
            dpy=XOpenDisplay(0);
            
            int ecran=DefaultScreen(dpy);

            unsigned long bpx=BlackPixel(dpy,ecran);
            unsigned long wpx=WhitePixel(dpy,ecran);           
            root=DefaultRootWindow(dpy);
            
            
            win1=XCreateSimpleWindow(dpy,root,0,0,maxX+2,maxY+2,24,bpx,wpx); /* Zone d'affichage */   
            swp= XCreatePixmap(dpy,win1,maxX,maxY,24); /* Zone de travail */
            
            unsigned long gcmask=GCFunction | GCForeground | GCBackground;
            
            XGCValues gcv;
            gcv.foreground=0;
            gcv.background=0;
            gcv.function=GXcopy;
            
            ct2=XCreateGC(dpy,swp,gcmask,&gcv);            
            XMapWindow(dpy,win1);
            
            XSelectInput(dpy,win1,KeyPressMask | EnterWindowMask | PointerMotionMask);              
 
            int winOk = 0;
            
            //while (!winOk)
            //   {
            //      XNextEvent(dpy,&ev);     
            //      if(ev.type==EnterNotify)
            //          {
                            XSetBackground(dpy,ct2, 128 + (128 << 8) + (128 << 16));
                            XSetForeground(dpy,ct2, 128 + (128 << 8) + (128 << 16) );
                            XFillRectangle(dpy,win1,ct2,0,0,maxX,maxY); 
                            winOk =1;
            XFlushGC(dpy,ct2);
            XFlush(dpy);
            //        }
            // }
			// clear the screen
            for (int i=0; i< maxX; i++)
            	for (int j=0; j< maxY; j++)
            		put_RVB_pixel(i,j, 128 + (128 << 8) + (128 << 16));
            for (int i=0; i< maxX; i++)
            	for (int j=0; j< maxY; j++)
            		put_RVB_pixel(i,j, 128 + (128 << 8) + (128 << 16));
            XFlushGC(dpy,ct2);
            XFlush(dpy);
                
        }

    void refresh()
        { }   //XCopyArea(dpy,swp,win1,ct2,0,0,XMAX,YMAX,0,0);  }

};
#endif


#if SYSTEME == MAC
class ScreenBase
{
private:
	int			width;
	int			height;
	int			bytePerPixel;
protected:
	int 		pos_x;
	int 		pos_y;
	int 		maxX;
	int 		maxY;

public:
    Ptr           ScrnBase;
    short         mBarHeight;

public:
      
    ScreenBase( int posX, int posY, int maxX0, int maxY0, int saveAble)
    {
    	bytePerPixel = EnvVar::BYTES_PER_PIXEL;
    	pos_x = posX;
    	pos_y = posY;
    	maxX  = maxX0;
    	maxY  = maxY0;
		
		if (saveAble)
		{
			InitGraf(&qd.thePort);
			InitFonts();
			InitWindows();
			InitMenus();
			TEInit();
			InitDialogs(NULL);

			FlushEvents(everyEvent, 0);
			//ScrnBase = LMGetScrnBase(); //+ qd.screenBits.rowBytes * 8 * 2000;
		}
		
		width  = qd.screenBits.rowBytes * 8; //8doit etre fixe
		height = 768;		

		ScrnBase = LMGetScrnBase() + width * bytePerPixel * posY + posX * bytePerPixel; //+ qd.screenBits.rowBytes * 8 * 768 - 800 * 2200;
		//LMSetMBarEnable(0);
		
		mBarHeight = GetMBarHeight();
    }

	void shift( int shiftx, int shifty)
	{
		pos_x += shiftx;
		pos_y += shifty;
		ScrnBase = LMGetScrnBase() + width * bytePerPixel * pos_y + pos_x * bytePerPixel; //+ qd.screenBits.rowBytes * 8 * 768 - 800 * 2200;
    }
    
    void put_pixel( int x, int y, int color )
    {
		unsigned char *ptre = (unsigned char *)ScrnBase + x*bytePerPixel + (y + mBarHeight)*width* bytePerPixel;
		for (int k=0; k<bytePerPixel; k++)
			*ptre++ = (unsigned char)color;
	}

    void put_RorVorB_pixel( int x, int y, int color )
    { *((unsigned char *)ScrnBase + x*bytePerPixel + (y + mBarHeight)*width* bytePerPixel) = (unsigned char)color; }

    void put_RVB_pixel( int x, int y, int color )
    { *(int *)((unsigned char *)ScrnBase + x*bytePerPixel + (y + mBarHeight)*width* bytePerPixel) = color;    }
};
#endif


#if SYSTEME == PC
class ScreenBase : public BaseImg
{
public:
	int 		maxX;
	int 		maxY;

public:
    ScreenBase( int posX, int posY, int maxX0, int maxY0, int saveAble) : BaseImg()
    { //ERROR(207,("display not yet available on this platform"));
    	width  = maxX0;
    	height = maxY0;
    }

   void put_pixel( int x, int y, int color ) { }
   void putBaseImg( BaseImg *img, int coordx, int coordy, int zoom) { }	
   void clearArea( int xinit, int yinit, int xend, int yend, int color) { }
   void put_RorVorB_pixel( int x, int y, int color ) {  }
   void put_RVB_pixel( int x, int y, int color ) { }
};
#endif

class ScreenSpike : public ScreenBase
{
protected:
    BaseImg		  *imgAssoc;
    int			  writeInImg;
	
public:
    ScreenSpike( int posX, int posY, int maxX0, int maxY0, int saveAble) : ScreenBase (posX, posY, maxX0, maxY0, saveAble)
    {
		if (saveAble) imgAssoc = new BaseImg( maxX0 - posX, maxY0 - posY, 3);
		else 		  imgAssoc = NULL;
		writeInImg = 1;
	}
	
	void setWriteInImg() { writeInImg = 1; }
	void resetWriteInImg() { writeInImg = 0; }

	void save( char *fileName)
	{
		PicturePPM pict( imgAssoc->Im, imgAssoc->getWidth(), imgAssoc->getHeight());
		pict.write_ppm( fileName, 3);
	}	

	void saveTARGA_grab_abs( char *fileName, int xinit, int yinit, int xend, int yend)
	{
		PictureTARGA pict( imgAssoc->Im, imgAssoc->getWidth(), imgAssoc->getHeight());
		//pict.writeTARGA_grab(  0, 0, sizeX, sizeY, fileName);
		pict.writeTARGA_grab( xinit, yinit, xend, yend, fileName);
	}	

	void flushSave( int xinit, int yinit, int xend, int yend)
	{
		for (int pos_y=yinit; pos_y < yend; pos_y++)
			for (int pos_x=xinit; pos_x < xend; pos_x++)
				put_RVB_pixel( pos_x, pos_y, imgAssoc->getRVBPix( pos_x, pos_y));					
	}
	
	void saveTARGA( char *fileName)
	{
		PictureTARGA pict( imgAssoc->Im, imgAssoc->getWidth(), imgAssoc->getHeight());
		pict.writeTARGA( fileName);
	}	

    void put_pixel( int x, int y, int color )
    {
    	ScreenBase::put_pixel( x, y, color);
    	if ((imgAssoc != NULL) && writeInImg) imgAssoc->put_pixel( x, y, color);
	}
	
    void put_RVB_pixel( int x, int y, int color )
    {
    	ScreenBase::put_RVB_pixel( x, y, color);
    	if ((imgAssoc != NULL) && writeInImg) imgAssoc->put_RVB_pixel( x, y, color);
	}
	
	void clearArea( int xinit, int yinit, int xend, int yend, int color)
	{
		for (int pos_y=yinit; pos_y < yend; pos_y++)
			for (int pos_x=xinit; pos_x < xend; pos_x++)
				put_RVB_pixel( pos_x, pos_y, color);					
	}

	void drawRect( int xinit, int yinit, int xend, int yend, int color)
	{
		//BaseImg::drawRect( xinit, yinit, xend, yend, color);
		
		if (xend > maxX) xend = maxX;
		if (yend > maxY) yend = maxY;
		for (int i=xinit; i< xend; i++)
		{
				put_RVB_pixel( i, yinit, color);
				put_RVB_pixel( i, yend-1, color);
		}
		for (int j=yinit; j< yend; j++)
		{
			put_RVB_pixel( xinit, j, color);
			put_RVB_pixel( xend-1, j, color);
		}
	}
    	
	void 	affiche_image(GlobalImg *img, int coordx, int coordy, int zoom)
	{
		unsigned char *ptr_e;
		int	          factZoom = 1 << zoom;

		for(int j=0;j<img->getHeight() / factZoom;j++)
		    for(int i=0;i<img->getWidth() / factZoom;i++)
        	    {
            		int tmpval = 0;
            		for (int g=0; g<factZoom; g++)
            			for (int h=0; h<factZoom; h++)
								tmpval+= (int)*(img->ImInt + i*factZoom+h + (j*factZoom+g)*img->getWidth());
					tmpval = tmpval / (factZoom * factZoom);
				
					put_pixel( i + coordx, j + coordy, tmpval);
				}
	}
			
	void putBaseImg( BaseImg *img, int coordx, int coordy, int zoom, int filter = 0xFFFFFFFF)
	{
		// bit mask
		int mask = 0;
		for(int i=0;i<img->getBytePerPixel();i++)
			mask = (mask << 8) | 0xFF;
		
		if (zoom >0)
		{
			int	factZoom = 1 << zoom;
			//if (img->getBytePerPixel() == 1)
			for(int j=0;j<img->getHeight()/ factZoom;j++)
	    		for(int i=0;i<img->getWidth()*img->getBytePerPixel()/ factZoom;i++)
				{
            		int tmpval = 0;
            		for (int g=0; g<factZoom; g++)
            			for (int h=0; h<factZoom; h++)
							tmpval+= (int)*(img->Im + i*factZoom+h + (j*factZoom+g)*img->getWidth()*img->getBytePerPixel());
					tmpval = tmpval / (factZoom * factZoom);
					if (img->getBytePerPixel() == 1)
					{
						tmpval = 0xFF000000 + (tmpval << 16) + (tmpval << 8) + tmpval;
						put_RVB_pixel( i + coordx, j + coordy, tmpval & filter);
					}
					//	put_pixel( i + coordx, j + coordy, tmpval);
					else
						put_RorVorB_pixel( i + coordx, j + coordy, tmpval);
				}
		}
		else
		{
			int factZoom = 1 << -zoom;
			for(int j=0;j<img->getHeight();j++)
	    		for(int i=0;i<img->getWidth();i++)
            		for (int g=0; g<factZoom; g++)
            			for (int h=0; h<factZoom; h++)
								put_RVB_pixel( i*factZoom + g + coordx, j*factZoom + h + coordy, img->getRVBPix( i, j) & filter);
//					if (img->getBytePerPixel() == 1)
//								put_pixel( i*factZoom + g + coordx, j*factZoom + h + coordy, *(img->Im + i + j*img->getWidth()));
//							else
//								put_RVB_pixel( i*factZoom + g + coordx, j*factZoom + h + coordy, *((int *)(img->Im + (i + j*img->getWidth())*img->getBytePerPixel())) & mask);
		}
	}
};

class ScreenSpikeAuto : public ScreenSpike
{
	class BaseImgAffich 
	{	
		public:
		BaseImg *baseImg;
		int		posx;
		int		posy;
		int		zoom;
		BaseImgAffich( BaseImg *baseImg0, int posx0, int posy0, int zoom0) { baseImg = baseImg0; posx = posx0; posy = posy0; zoom = zoom0; }
	};

	int maxXcurrent;
	int maxYcurrent;
	int currentX;
	int currentY;
	BaseImgAffich *tabBaseImg[MAX_IMG_AFFICH];
	int nbImg;
	int affich;
	int lineJump;
	int lineJump2;
	
public:
/*	void save( char *fileName)
	{
		PicturePPM pict( Im + maxX*(tabBaseImg[0]->baseImg->getHeight()+INTER_HAUT), maxX, maxY+tabBaseImg[0]->baseImg->getHeight()+INTER_HAUT);
		pict.write_ppm( fileName);
	}*/	

    ScreenSpikeAuto( int posX, int posY, int maxX0, int maxY0, int saveAble, int displayAble, int lineJump0 = 1000, int lineJump1 = 1000) : ScreenSpike (posX, posY, maxX0, maxY0, saveAble)
    {
		maxXcurrent = 0;
		maxYcurrent = 0;
		currentX = 0;
		currentY = 0;
		affich   = displayAble;
		lineJump = lineJump0;
		lineJump2 = lineJump1;
		
		nbImg = 0;
		for (int i=0; i< MAX_IMG_AFFICH; i++) tabBaseImg[i] = NULL;
	}

	void addBaseImg( BaseImg *baseImg0, int posx0, int posy0, int zoom0)
	{	
		double	factZoom;
		if (zoom0 > 0)	factZoom = 1.0/(1 << zoom0);
		else			factZoom = 1 << -zoom0;		
		tabBaseImg[ nbImg++ ] = new BaseImgAffich( baseImg0, posx0, posy0, zoom0);
		if (nbImg > MAX_IMG_AFFICH) ERROR( 216, ("maximun map for display overflow"));
		if ((posx0 + (int)(baseImg0->getWidth() * factZoom)) > maxXcurrent) maxXcurrent = posx0 + (int)(baseImg0->getWidth() * factZoom);
		if ((posy0 + (int)(baseImg0->getHeight()* factZoom)) > maxYcurrent) maxYcurrent = posy0 + (int)(baseImg0->getHeight() * factZoom);
	}
	
	void addBaseImg( BaseImg *baseImg0, int zoom0 = 0)
	{
		if ((currentX + baseImg0->getWidth()) > maxX) { currentX = 0; currentY = maxYcurrent ; }
		if (nbImg) if (!(nbImg % lineJump)) jumpLine();
		if (nbImg) if (!(nbImg % lineJump2)) jumpLine();
		addBaseImg( baseImg0, currentX, currentY, zoom0);
		currentX += baseImg0->getWidth() + INTER_LARG;
	}
	
	void jumpLine()
	{ 	currentY = maxYcurrent + INTER_HAUT; currentX = 0;}

	void resize()
	{
		maxX = maxXcurrent;
		maxY = maxYcurrent;
		if (imgAssoc != NULL)	{ delete imgAssoc; imgAssoc = new BaseImg( maxXcurrent, maxYcurrent, 3);	}
		clearArea( 0, 0, maxX, maxY, 0 );
	}

	virtual void clear(int val=0)
	{
		clearArea( 0, 0, maxX, maxY, 0 );
		if (affich != 0)
			for (int i=0; i < nbImg; i++)
				tabBaseImg[ i ]->baseImg->clear();
	}

	virtual int update()
	{
		for (int i=0; i< nbImg; i++)
			tabBaseImg[ i ]->baseImg->update();
			
		if (affich != 0)
			for (int i=0; i < nbImg; i++)
				putBaseImg( tabBaseImg[ i ]->baseImg, tabBaseImg[ i ]->posx, tabBaseImg[ i ]->posy, tabBaseImg[ i ]->zoom);
		return 0;
	} 
};

#endif
