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

// $Log: ScreenSpikeold.h,v $

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

class ScreenBase : public BaseImg
{
    GC ct2;
    Drawable win1;
    Window root;
    Pixmap swp;
    Display *dpy; 
  
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
    
    ScreenBase( int posX, int posY, int maxX, int maxY, int saveAble) : BaseImg()
        {
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
            
            while (!winOk)
                {
                    XNextEvent(dpy,&ev);     
                    if(ev.type==EnterNotify)
                        {
                            //XFillRectangle(dpy,fond,ctfond,0,0,XMAX,YMAX); 
                            XSetBackground(dpy,ct2, 128 + (128 << 8) + (128 << 16));
                            XSetForeground(dpy,ct2, 128 + (128 << 8) + (128 << 16) );
                            XFillRectangle(dpy,win1,ct2,0,0,maxX,maxY); 
                            winOk =1;
                        }
                }
                
        }

    void refresh()
        { }   //XCopyArea(dpy,swp,win1,ct2,0,0,XMAX,YMAX,0,0);  }

};
#endif


#if SYSTEME == MAC
class ScreenBase : public GlobalImg
{
    Ptr           ScrnBase;
    short         mBarHeight;

public:
      
    ScreenBase( int posX, int posY, int maxX0, int maxY0, int saveAble)
    {
    	bytePerPixel = EnvVar::BYTES_PER_PIXEL;
		
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
    
    void put_pixel( int x, int y, int color )
    {
		unsigned char *ptre = (unsigned char *)ScrnBase + x*bytePerPixel + (y + mBarHeight)*width* bytePerPixel;
		for (int k=0; k<bytePerPixel; k++)
			*ptre++ = (unsigned char)color;
	}

    void put_RorVorB_pixel( int x, int y, int color )
    { *((unsigned char *)ScrnBase + x*bytePerPixel + (y + mBarHeight)*width* bytePerPixel) = (unsigned char)color; }

    void put_RVB_pixel( int x, int y, int color )
    { *(int *)((unsigned char *)ScrnBase + x*bytePerPixel + (y + mBarHeight)*width* bytePerPixel) = color; }


};
#endif


#if SYSTEME == PC
class ScreenBase : public BaseImg
{
public:
    ScreenBase( int posX, int posY, int maxX, int maxY, int saveAble) : BaseImg()
    { //ERROR(207,("display not yet available on this platform"));
    	width  = maxX;
    	height = maxY;
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
	int pos_x;
	int pos_y;
	int maxX;
	int maxY;
	
public:
	void save( char *fileName)
	{
		PicturePPM pict( Im, maxX, maxY);
		pict.write_ppm( fileName);
	}	

    ScreenSpike( int posX, int posY, int maxX0, int maxY0, int saveAble) : ScreenBase (posX, posY, maxX0, maxY0, saveAble)
    {
    	pos_x = posX;
    	pos_y = posY;
    	maxX  = maxX0;
    	maxY  = maxY0;

		if (saveAble != 0)	Im = new unsigned char[ maxX * maxY ];
		else 				Im = NULL;
	}

    void put_pixel( int x, int y, int color )
    {
    	ScreenBase::put_pixel( x, y, color);
    	if (Im != NULL) *(Im + x +y*maxX) = color;
	}
	
	void clearArea( int xinit, int yinit, int xend, int yend, int color)
	{
		for (int pos_y=yinit; pos_y < yend; pos_y++)
			for (int pos_x=xinit; pos_x < xend; pos_x++)
			{
				put_RVB_pixel( pos_x, pos_y, color);
    			if (Im != NULL) *(Im + pos_x + pos_y*maxX) = color;
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
			
	void putBaseImg( BaseImg *img, int coordx, int coordy, int zoom)
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
						put_pixel( i + coordx, j + coordy, tmpval);
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
							if (img->getBytePerPixel() == 1)
								put_pixel( i*factZoom + g + coordx, j*factZoom + h + coordy, *(img->Im + i + j*img->getWidth()));
							else
								put_RVB_pixel( i*factZoom + g + coordx, j*factZoom + h + coordy, *((int *)(img->Im + (i + j*img->getWidth())*img->getBytePerPixel())) & mask);
		}
	}
};

#endif
