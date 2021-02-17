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

// $Log: Weights.cpp,v $

#include <stdlib.h>
#include <string.h>
#include "Map.h"
#include "Weights.h"
#include "PicturePPM.h"

#if MODE == INTERNAL
#include "convo.h"
#endif

#include "TabWeight.h"

#include "WatchArray.h"
//static WatchArray *watchArray = new WatchArray();
// cette fonction est utiliser pour pouvoir acceder a watchArray a partir du main et sauver les poids synaptiques
//void writeWatchArray( char *fileName)
//{	watchArray->writeAll( fileName); }

// detection de la taille d'une convolution
// ****************************************

int Weights::detect_convo( int *widthConv, int *heightConv, char *name, int convLock0, 
				    double randFact0, double multFact0, double decFact0, int duplicator0, char *latency, int noZoom0, double renorm0)
{
	watchAdded = 0;
    nbLatency  = 1;
    convLock   = convLock0;
    randFact   = randFact0;
    multFact   = multFact0;
    decFact    = decFact0;
    duplicator = duplicator0;
    renorm	   = renorm0;
    noZoom	   = noZoom0;
    
    char s[20];
    fileName  = new char[strlen(name)+1];
    strcpy(fileName, name);
        
#if MODE == EXTERNAL
    FILESTREAM f(name);
#else
    static cmpt =0;
    FILESTREAM f(tab_convo[cmpt++]);
#endif

    if (!f) ERROR(2,("unable to open file %s", fileName));
    
    f >> s; width =  atoi(s); *widthConv = width;
    f >> s; height =  atoi(s); *heightConv = height;
    
    // latency treatment
    int maxLatency = 1;
    if ((latency != NULL) && (latency[0] != 0))
    {
    	 latencyMap = new GlobalImg( latency, 1 );
    	 latencyMap->minmax();
    	 if (latencyMap->getMin() < 0)		 	 ERROR(124,("negative latencies not allowed in %s", latency));
    	 if (latencyMap->getWidth() != width) 	 ERROR(125,("latency map width different of its convolution map in %s", latency));
    	 if (latencyMap->getHeight() != height)  ERROR(126,("latency map height different of its convolution map in %s", latency));
		 maxLatency = latencyMap->getMax() + 1;
   	}
    else latencyMap = NULL;
    
    return maxLatency;
    //f.close();
};

FORMAT_POIDS Weights::totalPoids()
{
	/*FORMAT_POIDS sum = 0;
	
    for (int jj=0; jj<nbZooms; jj++)
	   for (int k=0; k < nbLines[0]; k++) {
	      	WeightLine *currentLine = (WeightLine *)((char *)zoomOffset[jj] + *(tabZoomOffset[jj]+k) );
	      	for (int l=0; l<currentLine->taille; l++)
	      		if (*(&(currentLine->firstPoids)+ l) < 0) *(&(currentLine->firstPoids)+ l) = 0;
				else sum += *(&(currentLine->firstPoids) + l);
		}
   //printf("sum %d\n", sum);
   return sum;*/

	FORMAT_POIDS somme_poids = 0;
	
    for (int j=0; j< totalLines; j++)
		for (int k=0; k< tabPoidsLine[j]->taille; k++)
	  	{
	    	FORMAT_POIDS val = tabPoidsLine[ j ]->tabPoids[ k ];
	    	if (val > 0) somme_poids += val;
	  	}
	return somme_poids;
}

/*
void Weights::duplicate(int width_tot, int height_tot, int widthCarte, int heightCarte)
{
  int nb = width_tot * height_tot;
  allTabPoidsLine = (WeightLine *) malloc(sizeTabPoidsLine * nb);
  if (allTabPoidsLine == NULL) ERROR(1,("out of memory"));

  // recopie les lignes
  for (int i=0; i < sizeTabPoidsLine; i++)
    {
      for (int j=0; j < nb; j++)
	*(((unsigned char *)allTabPoidsLine) + i + j*sizeTabPoidsLine) = *(((unsigned char *)tabPoidsLine[0]) + i);
      //for (int j=(nb>>2); j < nb; j++)
	//*(((unsigned char *)allTabPoidsLine) + i + j*sizeTabPoidsLine) = 0; //*(((unsigned char *)tabPoidsLine[0]) + i);
    }

  // verifie si les widths et les heights coincident
  //if ((width != widthCarte) || (height != heightCarte))
  //  { cerr << "erreur de taille de convo " << fileName << endl; exit(EXIT_FAILURE); }

  // recalcule tous les offsets
  int offset_center = (width_tot >> 1) + (height_tot >> 1)*width_tot;
  int border_l = (width_tot - widthCarte) >> 2;
  int border_h = (height_tot - heightCarte) >> 2;


  for (int j=0; j< width_tot; j++)
    for (int i=0; i< width_tot; i++)
      {
	int valeur_relative;
	if (((i >= border_l) && (i <= (widthCarte + border_l))) &&
	    ((j >= border_h) && (i <= (heightCarte + border_h))))
             valeur_relative = ImInt[i+j*widthCarte];
	else valeur_relative = 0;

	int       offset_relative = j * width_tot + i - offset_center;
	WeightLine      *tmp_poids = (WeightLine *)(allTabPoidsLine + (i+j*width_tot) * sizeTabPoidsLine);

	for (int k=0; k < nbLines; k++)
	  {
	    WeightLine *tmp_poids_line = (WeightLine *)((char *)tmp_poids + tabOffset[k-1]);
	    tmp_poids_line->offset    += offset_relative; //+ ou - a voir
	    FORMAT_POIDS *tmp_val    = &(tmp_poids_line->firstPoids);

	    for (int l=tmp_poids_line->taille; l>0; l--)
	      *tmp_val++ = valeur_relative;
	  }
      }

  // il faut ensuite soustraire cet offset a tous les offsets des lignes des convolutions
  // egalise tous les poids
  // probleme : on est plus dans le meme systeme de coordonnes et ca va etre dure
  // il va falloir bidouiller avec ce qui se passe a la fin pour ecrire les images

};
*/

void Weights::duplicate2(int width_tot, int height_tot)
{
    // generalisation a tous les neurones
    int nb = width_tot * height_tot;
    allTabPoidsLine = (WeightLine *) malloc(sizeTabPoidsLine * nb);
    if (allTabPoidsLine == NULL) ERROR(1,("out of memory"));

    for (int j=0; j< height_tot; j++)
      for (int i=0; i < width_tot; i++)
	  {
	    WeightLine *tmp_poids = (WeightLine *)((char*)allTabPoidsLine + (i + j * width_tot) * sizeTabPoidsLine);
	    for (int l=0; l < totalLines; l++)
		  {
		   	WeightLine *tmp_poids_line 		= (WeightLine *)((char *)tmp_poids       + tabOffset[l]);
		   	WeightLine *tmp_poids_line_init 	= tabPoidsLine[l];
		   
		   	tmp_poids_line->recopi(tmp_poids_line_init);
		   	tmp_poids_line->alea(randFact);
		  }
	  }
	
	// remise a jour des zooms
	for (int i=0; i< nbZooms; i++)
		zoomOffset[i] = (WeightLine *)((char *)allTabPoidsLine + (int)((char *)zoomOffset[i] - (char *)tabPoidsLine[0]));
}

// for negative zooms
void Weights::zoomNegIter()
{
	// convolution ALREADY TURNED
	FORMAT_POIDS *newTabW = new FORMAT_POIDS[(width+1)*(height+1)];
	
	for (int i=0; i<(width+1)*(height+1); i++) newTabW[i]=0;

	// apply 4 time the convolution at 4 different positions
	// care : weights per neuron have to be divided by 4 (total weight of the convolution)
	for (int j=0; j<height; j++)
		for (int i=0; i<width; i++)
		{
			newTabW[i   + j*(width+1)] 		+= ImInt[i+j*width];
			newTabW[i+1 + j*(width+1)] 		+= ImInt[i+j*width];
			newTabW[i   + (j+1)*(width+1)] 	+= ImInt[i+j*width];
			newTabW[i+1 + (j+1)*(width+1)] 	+= ImInt[i+j*width];
		}
		
	delete [] ImInt;
	ImInt = newTabW;
	width++;
	height++;
	
	// concerning the center of the convolution
	// even -> odd  (no problem)
	// odd	-> even (the center have to on the right left corner of the 4 centered points)
}

void Weights::zoomPos(int width_map)
{
	// algorithm : 	1 - create a big array initialised at 0
	//				2 - shifth and apply the convolution (initial shifth = shifth for the center to fix the right corner of a zoomed square)
	//				3 - mean the convolution (always the same

	// convolution ALREADY TURNED
	int centreX  = width >> 1;
	int centreY  = height >> 1;
	int factZoom = 1 << zoom;
	
	int tmpLargeur = width  + (centreX % factZoom) + factZoom-1;
	int tmpHauteur = height  + (centreY % factZoom) + factZoom-1;

	int meanLargeur = (tmpLargeur+factZoom-1) >> zoom;
	int meanHauteur = (tmpHauteur+factZoom-1) >> zoom;
	int meanCentreX = (centreX + (centreX % factZoom)) >> zoom;
	int meanCentreY = (centreY + (centreY % factZoom)) >> zoom;
	
	FORMAT_POIDS *tmpTabW 	= new FORMAT_POIDS[tmpLargeur  * tmpHauteur];
	FORMAT_POIDS *meanTabW  = new FORMAT_POIDS[meanLargeur * meanHauteur];

	for (int scanZoomY=0; scanZoomY<factZoom; scanZoomY++)
		for (int scanZoomX=0; scanZoomX<factZoom; scanZoomX++)
		{
			// initialize array
			for (int i=0; i<tmpLargeur*tmpHauteur; i++) tmpTabW[i] = 0;
			{for (int i=0; i<meanLargeur*meanHauteur; i++) meanTabW[i] = 0;}
		
			// copy the convolution at the right place (scanZoomX&Y : cursors)
			int offset = (centreX % factZoom) + scanZoomX + ((centreY % factZoom) + scanZoomY)*tmpLargeur;
			for (int j=0; j<height; j++)
				for (int i=0; i<width; i++)
					tmpTabW[ offset + i + j*tmpLargeur ] = ImInt[ i + j*width ];
			
			// mean the array
			{for (int j=0; j<tmpHauteur; j++)
				for (int i=0; i<tmpLargeur; i++)
					meanTabW[ (i >> zoom) + (j >> zoom)*meanLargeur ] += tmpTabW[ i + j*tmpLargeur ];}
			
			// add the weights to the convolution
			zoomOffset[scanZoomX + (scanZoomY << zoom)] 	= ptrArrayLine;
			tabZoomOffset[scanZoomX + (scanZoomY << zoom)] 	= tabOffset+totalLines;
			nbLines[scanZoomX + (scanZoomY << zoom)] 		= addToConvo( meanTabW, meanLargeur, meanHauteur, meanCentreX, meanCentreY, width_map);
						
		}
		
	delete [] tmpTabW;
	delete [] meanTabW;
}

// return the number of line added
int Weights::addToConvo(FORMAT_POIDS *tab, int tabLarg, int tabHaut, int tabCentX, int tabCentY, int width_map)
{
				
    int offsetNeuron;
    int offsetX, offsetY, itmp;

    int offsetLine = 0;

	// attention aux initialisations
	int tmp_numLine = 0;

    for (int j=0;j<tabHaut;j++)
    {
		int i = 0; // width
		while (i < tabLarg)
	  	{
	    	if (tab[i+j*tabLarg]==0) i++;
	    	else
	      	{
				itmp = i + 1;
				while (itmp < tabLarg) 
		  			if (tab[itmp+j*tabLarg] != 0) itmp++; else break;
		  			
				offsetX          = i - tabCentX;
				offsetY          = j - tabCentY;
				offsetNeuron     = offsetX + offsetY * width_map;
				
				// add the weights to the convolution
				tabPoidsLine[totalLines+tmp_numLine] = ptrArrayLine;
				tabOffset   [totalLines+tmp_numLine] = offsetLine;
				ptrArrayLine->constructeur(itmp-i, offsetNeuron, offsetX, offsetY, tab + i + j*tabLarg);
				offsetLine   += sizeof(WeightLine) + sizeof(FORMAT_POIDS) * (itmp - i -1);
				ptrArrayLine  = (WeightLine *)((char *)ptrArrayLine + sizeof(WeightLine) + sizeof(FORMAT_POIDS) * (itmp - i -1));
		
				i = itmp;
				tmp_numLine++;
    			// detection du maximum de lignes
    			if ((tmp_numLine+totalLines) > MAX_LINE_POIDS) ERROR(0,("max line per convolution exeeded in %s", fileName));
	      	}
	  	}
     }
     
     totalLines += tmp_numLine;
     return tmp_numLine;
}

void Weights::unZoom(FORMAT_POIDS *tab, int tabLarg, int tabHaut, int tabCentX, int tabCentY, int tmp_zoom)
{
	WeightLine       *tmp_poids 		= getZoomOffset( tmp_zoom, 0);																												
    unsigned short  *tmp_tab_offset = getTabZoomOffset( tmp_zoom );																												
                            																																												
    for (int k = getNbLines(tmp_zoom); k>0 ; k--)																													
    {																																											
          WeightLine       *tmp_poids_line = (WeightLine *)((char *)tmp_poids + tmp_tab_offset[k-1]);																								
          FORMAT_POIDS    *tmp_val = &(tmp_poids_line->firstPoids);
          int tmpOffsetX  = tmp_poids_line->offsetX + tabCentX;
          int tmpOffsetY  = tmp_poids_line->offsetY + tabCentY;																															

          for (int l=tmp_poids_line->taille; l>0; l--)
          {
            int 			  tmp_pos = tmpOffsetX +  tmpOffsetY * tabLarg;
            if ((tmpOffsetX >= tabLarg) || (tmpOffsetX < 0) || (tmpOffsetY >= tabHaut) || (tmpOffsetY < 0))
	      		ERROR(306, ("error saving convolution, index out of bound exception %d %d in 0-%dx0-%d" , tmpOffsetX, tmpOffsetY, tabLarg, tabHaut)) //{ printf("Error sving convo : index out of bound exception %d %d dans 0-%d 0-%d\n", coordX, coordY, size_x, size_y); exit(-1); }
            tab[ tmp_pos ] = *tmp_val;
          	tmp_val ++;
          	tmpOffsetX ++;
		  }
    }
}	

void Weights::init_convo( int width_map, int height_map, int zoomInit)
{
 
    long i, j; 
    char s[20];
    zoom = zoomInit;
    
    // pour une image de la convo
    Im = new unsigned char[width * height];

    // ouverture du fichier
#if MODE == EXTERNAL
    FILESTREAM f(fileName);
#else
    static cmpt =0;
    FILESTREAM f(tab_convo[cmpt++]);
#endif

    if (!f) ERROR(2,("unable to open file %s", fileName));
    
    f >> s; width = atoi(s);
    f >> s; height = atoi(s);
    f >> s;
    
    //lecture du fichier
    ImInt   = new FORMAT_POIDS[width*height];
    
    // Read all the weight and return the convolution
    nbPoids = 0;
    for (j=(height-1);j>=0;j--)
      for (i=(width-1);i>=0;i--)
	  {
	  	f >> s;
	  	if (atof(s) != 0)
	  	   		ImInt[i+j*width] = (FORMAT_POIDS) (atof(s) * multFact * (1 + (2*(double)rand()/RAND_MAX-1) * randFact));
	  	else 	ImInt[i+j*width] = 0;
	  	if (EnvVar::REPLACE0_1 == OUI)
	  		if (ImInt[i+j*width] == 0) ImInt[i+j*width] = 1;
	  	if (ImInt[i+j*width] != 0) nbPoids++;
	  	
	  	//ImInt[i+j*width] = (FORMAT_POIDS) (atof(s) * multFact * (1 + (double)rand()/RAND_MAX * randFact));
	  	//if (!convLock && (ImInt[i+j*width] > NORMALISE_LEARN)) ImInt[i+j*width] = NORMALISE_LEARN;
	  	//if (!convLock && (ImInt[i+j*width] < 0)) 				ImInt[i+j*width] = 0;
	}
	
	if (!convLock && (renorm == -1)) 
	{
		conv_order( NULL, 0.95 * 0.95 );
		renormalize( 10240 );
	}		
	if (renorm > 0) renormalizeMax( renorm );
	
    // apply the zoom factor	
    totalLines = 0;
    if (zoom > 0) 	nbZooms = (1 << zoom) * (1 << zoom);
    else		 	nbZooms = 1;
    
    // maximum memory reservation (as if all weight will take a line) - majoration very huge here, could be lowered
    sizeTabPoidsLine = sizeof(WeightLine) * nbPoids * nbZooms;
    if (zoom < 0) sizeTabPoidsLine += sizeof(WeightLine) * ((width+height) << -zoom) * 2;
    tabPoidsLine[0]  = (WeightLine *) malloc(sizeTabPoidsLine);
    ptrArrayLine	 = tabPoidsLine[0];
	totalLines = 0;

	// apply the zoom factor
    int zoom0 = zoom;

	if (noZoom) 
	{
		zoomOffset[0] 	 = ptrArrayLine;
		tabZoomOffset[0] = tabOffset;
		addToConvo( ImInt, width, height, width >> 1, height >> 1, width_map);
		if (zoom > 0)
			for (int i=1; i< (1 << zoom) * (1 << zoom); i++)
			{
				zoomOffset[ i ] 	 = ptrArrayLine;
				tabZoomOffset[ i ]   = tabOffset;
				nbLines[ i ]		 = nbLines[ 0 ];
			}
	}
	if (zoom > 0) zoomPos(width_map);
	else
	{
		while (zoom0 < 0) 
			{ zoomNegIter(); zoom0++; }
		// add the weights to the convolution
		zoomOffset[0] 	 = ptrArrayLine;
		tabZoomOffset[0] = tabOffset;
		nbLines[0] 		 = addToConvo( ImInt, width, height, width >> 1, height >> 1, width_map);
	}

	// in case we need to have duplicate convolution, adjust size reservation to optimal size
    sizeTabPoidsLine = sizeof(WeightLine) * totalLines + sizeof(FORMAT_POIDS) * (nbPoids - totalLines);
	
    //duplication de la convolution si necessaire
    if (duplicator) duplicate2(width_map, height_map);
    
    //affiche();
    //printf("\n");
}


/*
void Weights::init_convo( int width_map, int height_map, int zoom)
{
    long i, j, k, l; 
    char s[20];
    
    // ouverture du fichier
#if MODE == EXTERNAL
    FILESTREAM f(fileName);
#else
    static cmpt =0;
    FILESTREAM f(tab_convo[cmpt++]);
#endif

    if (!f) ERROR(2,("unable to open file %s", fileName));
    
    f >> s; width = atoi(s);
    f >> s; height = atoi(s);
    f >> s;
    offsetHauteur = -(height / 2);
    
    //lecture du fichier
    //FORMAT_COMPUTE *ImInt   = new FORMAT_COMPUTE[width*height];
    ImInt   = new FORMAT_POIDS[width*height];
    FORMAT_POIDS last_poids  = 0;

    // Read all the weight and return the convolution
    for (j=(height-1);j>=0;j--)
      for (i=(width-1);i>=0;i--)
	  {
	  	f >> s;
	  	if (atof(s) != 0)
	  	   		ImInt[i+j*width] = (FORMAT_POIDS) (atof(s) * multFact * (1 + (2*(double)rand()/RAND_MAX-1) * randFact));
	  	else 	ImInt[i+j*width] = 0;
	  	#if REPLACE0_1 == OUI
	  		if (ImInt[i+j*width] == 0) ImInt[i+j*width] = 1;
	  	#endif
	  	//ImInt[i+j*width] = (FORMAT_POIDS) (atof(s) * multFact * (1 + (double)rand()/RAND_MAX * randFact));
	  	if (!convlock && (ImInt[i+j*width] > NORMALISE_LEARN)) ImInt[i+j*width] = NORMALISE_LEARN;
	  	if (!convLock && (ImInt[i+j*width] < 0)) 				ImInt[i+j*width] = 0;
	}
	
    // apply the zoom factor	
    int zoom0 = zoom;
    while (zoom0)
    {
		int abs, ord, cmpt;
    	FORMAT_POIDS gauss[] = { 1, 3, 1, 3, 5, 3, 1, 3, 1 };
    	int width_filtre = 3;
    	int height_filtre = 3;
    	int larg_flt_div2 = width_filtre / 2;
    	int haut_flt_div2 = height_filtre / 2;
    	int centre_filtre = width_filtre * height_filtre / 2;

		int newheight = (height + 1) >> 1;
		int newwidth = (width + 1) >> 1;
		FORMAT_POIDS *ImInt2   = new FORMAT_POIDS[newwidth*newheight];
		for (j=0;j<newheight;j++)
	  		for (i=0;i<newwidth;i++)
	    		ImInt2[i+j*newwidth] = 0; 	
    
		for(j=0;j<height;j+=2)
	    	for(i=0;i<width;i+=2)
            	{
            		cmpt=0;
                	for (k=-larg_flt_div2;k<(larg_flt_div2+1);k++)
                    	for (l=-haut_flt_div2;l<(haut_flt_div2+1);l++)
                        	{
                            	abs = i+k;
                            	ord = j+l;
                            	if (((abs>=0) && (abs<width)) && ((ord>=0) && (ord<height)))
                                {
                                	FORMAT_POIDS val = gauss[k + centre_filtre + l*width_filtre];
                                    ImInt2[(i >> 1) + (j >> 1)*newwidth] += ImInt[abs+ord*width] 
                                    												* val;
                                    cmpt+=val;
                                }                                  
                        	}
                   	ImInt2[(i >> 1) + (j >> 1)*newwidth] /= cmpt;
             	}
		width = newwidth;
		height = newheight;
		delete ImInt;
		ImInt = ImInt2;
   		zoom0--;
   	}


   	// on applique pas de gaussienne pour l'instant
   		
   	// on construit les tableaux pour compter les poids
    nbPoids = 0;
    nbZooms = 0;
    totalLines = 0;
    int realZoom = 1 << zoom;
   	for (int jj=0; jj<realZoom; jj++)
   		for (int ii=0; ii<realZoom; ii++)
   			{
				int newheight = (int)ceil((double)height / realZoom);
				int newwidth = (int)ceil((double)width / realZoom);
				FORMAT_POIDS *ImInt2   = new FORMAT_POIDS[newwidth*newheight];

				// recopi les poids relevant
				for (j=0;j<newheight;j++)
	  				for (i=0;i<newwidth;i++)
	  				{
	  					int Abs = (i << zoom) + ii;
	  					int Ord = (j << zoom) + jj;
	  					
   						if ((Abs < height) && (Ord < width))
   							ImInt2[i + j*newwidth] = ImInt[Abs + Ord*width];
   						else
   							ImInt2[i + j*newwidth] = 0;
					}
				
	    		// compte le nb de lignes
    			for (j=0;j<newheight;j++)
      			{
        			for (i=0;i<newwidth;i++)
	  				{
	    				if (ImInt2[i+j*newwidth]!=0) nbPoids++;
	    				if ((ImInt2[i+j*newwidth]==0) && (last_poids !=0))   totalLines++;
	    				if ((ImInt2[i+j*newwidth]!=0) && (i == (newwidth-1))) totalLines++;
	    				last_poids = ImInt2[i+j*newwidth];
	  				}
					last_poids = 0;
					if (nbPoids == 0) offsetHauteur ++;
     			}
     			delete [] ImInt2;
     			nbZooms++;
    		}


    	// reserve la memoire necessaire
    	sizeTabPoidsLine = sizeof(WeightLine) * totalLines + sizeof(FORMAT_POIDS) * (nbPoids - totalLines);
    		
    	//reservation
    	tabPoidsLine[0]  = (WeightLine *) malloc(sizeTabPoidsLine);
    	WeightLine *ptrArrayLine = tabPoidsLine[0];

   		// on reconstruit les tableaux pour creer les convos
	  	int tmp_numLine = 0;
   		for (int jj=0; jj<realZoom; jj++)
   			for (int ii=0; ii<realZoom; ii++)
   			{
				int newheight = (int)ceil((double)height / realZoom);
				int newwidth = (int)ceil((double)width / realZoom);
				FORMAT_POIDS *ImInt2   = new FORMAT_POIDS[newwidth*newheight];

				// recopi les poids relevant
				for (j=0;j<newheight;j++)
	  				for (i=0;i<newwidth;i++)
	  				{
	  					int Abs = (i << zoom) + ii;
	  					int Ord = (j << zoom) + jj;
	  					
   						if ((Abs < height) && (Ord < width))
   							ImInt2[i + j*newwidth] = ImInt[Abs + Ord*width];
   						else
   							ImInt2[i + j*newwidth] = 0;
					}
				
    			int offsetNeuron;
    			int offsetX, offsetY, itmp;

	  			zoomOffset[ii + (jj << zoom)] = ptrArrayLine;
	  			tabZoomOffset[ii + (jj << zoom)] = tabOffset+tmp_numLine;
      			nbLines[ii + (jj << zoom)] = 0;
    			int offsetLine = 0;

    			for (j=0;j<newheight;j++)
      			{
					i = 0; // width
					while (i < newwidth)
	  				{
	    				if (ImInt2[i+j*newwidth]==0) i++;
	    				else
	      				{
							itmp = i + 1;
							while (itmp < newwidth) 
		  						if (ImInt2[itmp+j*newwidth] != 0) itmp++; else break;
							offsetX          = (newwidth/2-itmp+1);
							offsetY          = (newheight/2-j);
							offsetNeuron     = offsetX + (newheight/2-j)*width_map;

							tabPoidsLine[tmp_numLine] = ptrArrayLine;
							tabOffset   [tmp_numLine] = offsetLine;
							ptrArrayLine->constructeur(itmp-i, offsetNeuron, offsetX, offsetY, ImInt2 + i + j*newwidth);
							offsetLine   += sizeof(WeightLine) + sizeof(FORMAT_POIDS) * (itmp - i -1);
							ptrArrayLine  = (WeightLine *)((char *)ptrArrayLine + sizeof(WeightLine) + sizeof(FORMAT_POIDS) * (itmp - i -1));
		
							i = itmp;
							tmp_numLine++;
      						nbLines[ii + (jj << zoom)]++;
	      				}
	  				}
      			}

      			delete [] ImInt2;
  			}
  			
    //duplication de la convolution si necessaire
    if (duplicator) duplicate2(width_map, height_map);
}
*/
// renormalisation des poids par rapport ˆ la somme
void Weights::renormalizeLines( FORMAT_POIDS renorm )
{
	FORMAT_POIDS sum = 0;
	
    for (int jj=0; jj<nbZooms; jj++)
	   	for (int k=0; k < nbLines[0]; k++)
		{
	      	WeightLine *currentLine = (WeightLine *)((char *)zoomOffset[jj] + *(tabZoomOffset[jj]+k) );
	      	for (int l=0; l<currentLine->taille; l++)
	      		if (*(&(currentLine->firstPoids)+ l) < 0) *(&(currentLine->firstPoids)+ l) = 0;
				else sum += *(&(currentLine->firstPoids) + l);
		}

    {for (int jj=0; jj<nbZooms; jj++)
	   	for (int k=0; k < nbLines[0]; k++)
		{
	      	WeightLine *currentLine = (WeightLine *)((char *)zoomOffset[jj] + *(tabZoomOffset[jj]+k) );
	      	for (int l=0; l<currentLine->taille; l++)
				*(&(currentLine->firstPoids) + l) = (FORMAT_POIDS)((*(&(currentLine->firstPoids) + l) * renorm) / sum);
		}}
	totalPoids();	
}

// renormalisation des poids absolue
void Weights::renormalizeLinesAbsolute( FORMAT_POIDS renorm )
{
    for (int jj=0; jj<nbZooms; jj++)
	   	for (int k=0; k < nbLines[0]; k++)
		{
	      	WeightLine *currentLine = (WeightLine *)((char *)zoomOffset[jj] + *(tabZoomOffset[jj]+k) );
	      	for (int l=0; l<currentLine->taille; l++)
				*(&(currentLine->firstPoids) + l) = (FORMAT_POIDS)((*(&(currentLine->firstPoids) + l) * renorm) );
		}
	totalPoids();	
}
void Weights::saveConvos( char *name, char *dest, int force)
{	
	//renormalizeLines( 10240 );
	
    if (zoom > 0)
	{

		int centreX  = width >> 1;
		int centreY  = height >> 1;
		int factZoom = 1 << zoom;
	
		int tmpLargeur = width  + (centreX % factZoom) + factZoom-1;
		int tmpHauteur = height  + (centreY % factZoom) + factZoom-1;

		int meanLargeur = (tmpLargeur+factZoom-1) >> zoom;
		int meanHauteur = (tmpHauteur+factZoom-1) >> zoom;
		int meanCentreX = (centreX + (centreX % factZoom)) >> zoom;
		int meanCentreY = (centreY + (centreY % factZoom)) >> zoom;
	
		FORMAT_POIDS *tmpTabW 	= new FORMAT_POIDS[tmpLargeur  * tmpHauteur];
		FORMAT_POIDS *meanTabW  = new FORMAT_POIDS[meanLargeur * meanHauteur];
	
		for (int j=0; j<height; j++)
			for (int i=0; i<width; i++)
				ImInt[ i + j*width ] = 0;

		for (int scanZoomY=0; scanZoomY<factZoom; scanZoomY++)
			for (int scanZoomX=0; scanZoomX<factZoom; scanZoomX++)
			{
				//retrieve array
				unZoom( meanTabW, meanLargeur, meanHauteur, meanCentreX, meanCentreY, scanZoomX + (scanZoomY << zoom) );
		
				// copy array
				for (int j=0; j<tmpHauteur; j++)
					for (int i=0; i<tmpLargeur; i++)
						tmpTabW[ i + j*tmpLargeur ] = meanTabW[ (i >> zoom) + (j >> zoom)*meanLargeur ];
		
				// copy the convolution at the right place (scanZoomX&Y : cursors)
				int offset = (centreX % factZoom) + scanZoomX + ((centreY % factZoom) + scanZoomY)*tmpLargeur;			
				{for (int j=0; j<height; j++)
					for (int i=0; i<width; i++)
						ImInt[ i + j*width ] += tmpTabW[ offset + i + j*tmpLargeur ];}
			}
		
		delete [] tmpTabW;
		delete [] meanTabW;
	}
	else
	{
		int realZoom = (int) sqrt(nbZooms);
		int newheight = (int)ceil((double)height / realZoom);
		int newwidth = (int)ceil((double)width / realZoom);
		int centre_x = (newwidth/2) * realZoom;
		int centre_y = (newheight/2) * realZoom;
		int size_x = width;
		int size_y = height;
		int max = 0;
		
		for (int k=0; k < size_x*size_y; k++)
			 ImInt[k] = 0;

		//WeightLine *currentLine = (WeightLine *)((char *)ImIntPoidsLine[0];
	
		// ATTENTION - ici il faut prendre la racine
		//printf("%d \n", realZoom);
	   	for (int jj=0; jj<realZoom; jj++)
	   	for (int ii=0; ii<realZoom; ii++)
	  	{
   	  		int realZoomOffset = ii + jj * realZoom;
	   	 	for (int k=0; k < nbLines[realZoomOffset]; k++)
			{
	      		WeightLine *currentLine = (WeightLine *)((char *)zoomOffset[realZoomOffset] + *(tabZoomOffset[realZoomOffset]+k) );
	      		// rerenverse la convolution
	      		int coordX = centre_x - (currentLine->offsetX*realZoom) + ii;
	      		int coordY = centre_y - (currentLine->offsetY*realZoom) + jj;
	      		for (int l=0; l<currentLine->taille; l++)
	      		{
	      			if ((coordX >= size_x) || (coordX < 0) || (coordY >= size_y) || (coordY < 0))
	      				;//{ printf("Error sving convo : index out of bound exception %d %d dans 0-%d 0-%d\n", coordX, coordY, size_x, size_y); exit(-1); }
	      			else
	      			{
		  				ImInt[coordX + coordY * size_x] += *(&(currentLine->firstPoids) + l);
		  				//if (tab[coordX + coordY * size_x]) { printf("Error : not nul\n"); exit(-1); }
		  				if (ImInt[coordX + coordY * size_x] > max) max = ImInt[coordX + coordY * size_x];
		  			}
		  			coordX-=realZoom;
		  		}
			}
	  	}
	}
	
	// nom de la sauvegarde
	//int newlen = strlen(newname);
	//if (num>99) newname[newlen++] = (num % 100) + 48;
	//if (num>9)  newname[newlen++] = (num % 10) + 48;
	//newname[newlen] = num + 48;
	//newname[newlen+1] = 0;
	
	// calcul de l'image
	minmax();
	if (Im    == NULL) Im    = new unsigned char[ width * height ];
	for (int k=0; k < width * height; k++)
		Im[k] = (unsigned char)(int(ImInt[k] * 255) / max);


	// sauvegarde d'un fichier texte
	if (force)
	{
		sprintf(newname, "%s%s%c%s_%s", CURRENT_DIR, EnvVar::DIR_SAVE_CONVOS, SEPARATEUR, name, dest);
		FILE *f = fopen(newname, "w");
		if (!f) ERROR( 2, ("unable to open file %s", newname))
		fprintf(f, "%d %d 0\n", width, height);
		for (int j=0; j< height; j++)
			for (int i=0; i< width; i++)
				if (i != (width-1)) 	fprintf(f,"%f\t", ImInt[i+j*width]);
				else					fprintf(f,"%f\n", ImInt[i+j*width]);
		fclose(f);
	
		// sauvegarde de l'image
    	//write_ppm(newname);
	}
	
	if (!watchAdded)
	{
    		/*if (!convLock)
    		{
    		tabWeight = new TabWeight( nbPoids );
			for (int a =0; a < width; a++)
		  		for (int b=0; b < height; b++)
		  			if ( ImInt[ a + b*width ] != 0 )
						tabWeight->addPoids( 0, a, b, ImInt + a + b*width);
 			sprintf(newname, "%s_%s", name, dest);
   			watchArray->addWatch( newname, tabWeight);
   		}
   		watchAdded++;*/
   	}
}

void Weights::conv_order(char *name2, double fact_modulation)
{
    long i,j,nb_poids,k;
        
    //offset initialisation
    long *offset = new long[width*height];
    for (i=0;i<width * height;i++)
            offset[i] = i;
      
    // sorting weights (bublle)
    FORMAT_POIDS tmp_val;
    int   tmp_off;
    for (i=0;i<width * height;i++)
        for (j=1; j<width * height;j++)
            if (ImInt[j-1] < ImInt[j])
                {
                    tmp_val = ImInt[j-1];
                    ImInt[j-1] = ImInt[j];
                    ImInt[j] = tmp_val;
                    
                    tmp_off = offset[j-1];
                    offset[j-1] = offset[j];
                    offset[j] = tmp_off;
                }
    
    // new weight depending on modulation (with the RENORM parameter)
    float modulation = 1;
    int poids_consecutifs;
    for (i=0;i<width * height;i+=poids_consecutifs)
        {
            poids_consecutifs=1;
            if (ImInt[i] != 0)
                {
                	double consecutive_mod = modulation;
                    modulation *= fact_modulation; 
                    while(ImInt[i+poids_consecutifs] == ImInt[i]) 
                    	{ 
                    		consecutive_mod = (consecutive_mod * poids_consecutifs + modulation)/(poids_consecutifs+1);
                    		modulation *= fact_modulation; 
                    	  	poids_consecutifs++;
                    	}
                	FORMAT_POIDS intMod;
                	if ((((FORMAT_POIDS) 0.1) == (float)0.1) || (((FORMAT_POIDS) 0.1) == (double)0.1))
                		 intMod = consecutive_mod * (1 << RENORM);
                	else intMod = rint( consecutive_mod * (1 << RENORM));
                	
                    for (k=0; k<poids_consecutifs; k++)
                        ImInt[i+k] = intMod;
                    
                }
        }

    // actualizing weights
    FORMAT_POIDS *tab_poids = new FORMAT_POIDS [width*height];
    for (i=0;i<width * height;i++)
        tab_poids[offset[i]] = ImInt[i];


    /*if (name2 == NULL)
        {
            // displaying weights
            printf("%d %d %d\n",width, height, 10000);
            for (j=0; j<height;j++)
                {
                    for (i=0;i<width;i++)
                    //    printf("%1.4f\t", tab_poids[i + j*width]);
                        printf("%4d\t", tab_poids[i + j*width]);
                    printf("\n");
                }
        }
    else
        {
            FILE *f = fopen(name2,"w");
            fprintf(f,"%d %d %d\n",width, height, 10000);
            for (j=0; j<height;j++)
                {
                    for (i=0;i<width;i++)
                        fprintf(f,"%1.4f\t", tab_poids[i + j*width]);
                    fprintf(f,"\n");
                }
        }*/
    
    for (i=0;i<width * height;i++)
    	ImInt[i] = tab_poids[i];
    
    //exit(-1);
}


// ---------------------------------------------------------------------------------
void Weights::saveAllConvos(int widthTot, int heightTot, int borderL, int borderH)
{
/*  //cout << widthTot << " " << heightTot << " " << borderL<< " " << borderH << endl;
  int w = (widthTot - 2*borderL)*width;
  int h = (heightTot - 2*borderH)*height;
  unsigned char *bigImg = new unsigned char[w*h];
  if (bigImg == NULL)         
      ERROR(1,("out of memory"))

  PicturePPM *pict = new PicturePPM(bigImg, w, h);

  // recopi l'image
  
  for (int j=0; j < heightTot; j++)
    for (int i=0; i< widthTot; i++)
      if (((i<borderL) || (i>=(-borderL+widthTot))) ||
	((j<borderH) || (j>=(-borderH+heightTot)))) { }
      else
	{
	  WeightLine *tmp_poids = (WeightLine *)((char *)allTabPoidsLine + (i + j*widthTot)*sizeTabPoidsLine);
	  unsigned char *posImg = bigImg + (i-borderL) * width + (j-borderH) * height * w;

	  for (int k=0; k<nbLines; k++)
	    {
	      WeightLine *currentLine = (WeightLine *)((char *)tmp_poids + tabOffset[k]);
	      unsigned char *coordPos = posImg + currentLine->offsetX + width/2 + (currentLine->offsetY + height/2)*w;
	      for (int l=0; l<currentLine->taille; l++)
		{
		  double pd = *(&(currentLine->firstPoids) + l) * 255;
		  if (pd<0) pd = 0;
		  else if (pd > 255) pd = 255;
		  *coordPos = (unsigned char) pd;
		  coordPos++;
		}
	    }
	}

  char *name = new char[100];
  strcpy(name, "Results/");
  strcat(name, strpbrk(fileName,"/")+1);
  pict->write_ppm(name);
  delete name;
*/
}

// decremente tous les poids d'une valeur fixe
// -------------------------------------------
void Weights::decrementeAll( double bottom, int heightTot, int widthTot )
{
/*  noModif = 1;
  FORMAT_POIDS bottom0 = (FORMAT_POIDS)bottom;
  
  for (int j=0; j < heightTot; j++)
    for (int i=0; i< widthTot; i++)
      {
		WeightLine *tmp_poids = (WeightLine *)((char *)allTabPoidsLine + (i + j*widthTot)*sizeTabPoidsLine);
		for (int k=0; k<nbLines; k++)
	  	{
	   	 	WeightLine *currentLine = (WeightLine *)((char *)tmp_poids + tabOffset[k]);
	    	for (int l=0; l<currentLine->taille; l++)
	      		if (*(&(currentLine->firstPoids) + l)>bottom0)
					{ *(&(currentLine->firstPoids) + l) -= (FORMAT_POIDS) decFact; noModif = 0; }
	  	}
      }
*/
};

double Weights::computeMean()
{
	double sum = 0;
	int	   nbItems = 0;
    for (int jj=0; jj<nbZooms; jj++)
	   	for (int k=0; k < nbLines[jj]; k++)
		{
	      	WeightLine *currentLine = (WeightLine *)((char *)zoomOffset[jj] + *(tabZoomOffset[jj]+k) );
	      	for (int l=0; l<currentLine->taille; l++)
	      		{ sum += *(&(currentLine->firstPoids) + l); nbItems ++; }
		}
		
	return sum / nbItems;
}
	
double Weights::computeVar()
{
/*	double mean = 0;
	double var  = 0;
	for (int i=0; i<width*height; i++)
	{
		mean += ImInt[i];
		var  += ImInt[i] * ImInt[i];
	}
	var = sqrt(mean*mean - var)	/ ((double)(width * height));
	mean /= ((double)(width * height));
	return var;*/ return 0;
}

