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

// $Log: MapBuffer.h,v $

#if SYSTEME == UNIX
#include <sys/socket.h>
#include <unistd.h>
#endif

#ifndef _MAP_NET
#define _MAP_NET

#include "Map.h"
#include "Server.h"
#include "Sender.h"

// network maps as buffers
// -----------------------

class carte_virtual : public carte_base
{
	unsigned char buffer[BUFFER_NETWORK];			// buffer of all datas
	unsigned char *ptrBuffer;						// current pointer on the buffer
	coord_array	  *current_list_ret;				// 

	coord_array *spikeBuffer[ MAX_SPIKE_BUFFER ];	// array of pointer to spike lists
    int  begBuffer;									// begining offset to spike lists
	int  endBuffer;									// ending offset to spike lists
	proc *remoteProc;								// reference to the processor onto which the real map is located

public:
	carte_virtual(int numero0, OutputDisplay *disp0, char *name0, int largeur_norm0, int hauteur_norm0, int zoom0, proc *remoteProc0) :
		carte_base(numero0, disp0, name0, largeur_norm0, hauteur_norm0, zoom0)
		{ 
			virtMap = 1;
			remoteProc = remoteProc0;		
		
			//initialise buffer
			begBuffer = 0;
			endBuffer = 0;
			ptrBuffer = buffer;
			for (int i=0; i<MAX_SPIKE_BUFFER; i++)
				spikeBuffer[ i ] = NULL;
		}
    
/*   void receiveSpikeList(unsigned int sock,int bytes)
        {
            // read the spike list and incremente buffer pointer
            coord_array *ptrCoordArray = (coord_array *) ptrBuffer;
            
            if (bytes<=0) 
                {
                    // nothing to read
                    ptrCoordArray->size = 0;
                    ptrCoordArray->array = NULL;
                    ptrBuffer += sizeof(coord_array);              
                }
            else
                { // readData
                    int newdata=0, justRead;
                    while(newdata<sizeof(int))
                        {
                            justRead=read(sock,(char *) (ptrBuffer)+newdata,sizeof(int)-newdata);
                            if (justRead>0) newdata+=justRead;
                        }
                    
                    while(newdata<bytes)
                        {
                            justRead=read(sock,(char *) (ptrBuffer+sizeof(int *))+newdata,bytes-newdata);
                            if (justRead>0) newdata+=justRead;
                        }
                    ptrCoordArray->array = (coord *) (ptrBuffer + sizeof(int *) + sizeof(int));
                    ptrBuffer += bytes + sizeof(int *);              
                }
        }

	
	virtual void putSpikeInBuffer(int sock0, int bytes0)
	{

		int realSize = bytes0 + sizeof(int *);

		if ( ((endBuffer+1) % MAX_SPIKE_BUFFER) == begBuffer )
			ERROR(0,("number in spike list overflow in server"));
			
		// cycling ptrBuffer
		if ((int)((ptrBuffer - buffer) + realSize) > BUFFER_NETWORK) 
		 	ptrBuffer = buffer;

		// testing for buffer overflow
		if (( (int)ptrBuffer <= (int)spikeBuffer[ begBuffer ]) &&
				((int)(ptrBuffer + realSize) >= (int)spikeBuffer[ begBuffer ]))
					ERROR(0,("buffer spike list overflow in map mymap"));			
				
		spikeBuffer[ endBuffer ] = (coord_array *) ptrBuffer;
        printf("One more message successfully received %d -> size %d offset %d pointer %d/%d\n", bytes0,
               (int)spikeBuffer[ endBuffer ]- (int)spikeBuffer[ begBuffer],  (int)spikeBuffer[ endBuffer ]- (int)buffer, begBuffer, endBuffer);
		
		// receive the spikes
        receiveSpikeList( sock0, bytes0);        
		
        endBuffer = (endBuffer+1) % MAX_SPIKE_BUFFER;

 LOCAL*/
    
    //virtual
    void putSpikeInBuffer(int sock0, int bytes0)
    {
    
#if SYSTEME == UNIX   
       // read the spike list and incremente buffer pointer
        ///////////////////////////////////////////////////
        
		((coord_array *) ptrBuffer)->array = (coord *) (ptrBuffer + sizeof(coord_array));
		coord *ptrCoord = (coord *)(((coord_array *) ptrBuffer)->array);

        if (bytes0<=0)
            { ((coord_array *) ptrBuffer)->size = 0;  ptrCoord = (coord *) (ptrBuffer + sizeof(coord_array)); }
         else
            { // readData
                int newdata=0, justRead;
                while(newdata<sizeof(int))
                    {
                        justRead=read(sock0,(char *) (ptrBuffer)+newdata,sizeof(int)-newdata);
                        if (justRead>0) newdata+=justRead;
                    }
                
                while(newdata<bytes0)
                    {
                        justRead=read(sock0,(char *) (ptrBuffer + sizeof(int *))+newdata,bytes0-newdata);
                        if (justRead>0) newdata+=justRead;
                    }
                ptrCoord = (coord *) (ptrBuffer + sizeof(int *) + bytes0);
            }

#else
        coord_array *spike_list = (coord_array *) sock0;
		((coord_array *) ptrBuffer)->size = spike_list->size;
		((coord_array *) ptrBuffer)->array = (coord *) (ptrBuffer + sizeof(coord_array));
		
		coord *ptrCoord = (coord *)(((coord_array *) ptrBuffer)->array);
		for (int i=0; i<spike_list->size; i++)
		{
			ptrCoord->x = spike_list->array[i].x;
			ptrCoord->y = spike_list->array[i].y;
			ptrCoord++;
		}
#endif
        
        ///////////////////////////////////////////////////
		
		int size = (int) ((unsigned char *)ptrCoord - ptrBuffer);

		if ( ((endBuffer+1) % MAX_SPIKE_BUFFER) == begBuffer )
			ERROR( 205,("number in spike list overflow in server"));
			
		spikeBuffer[ endBuffer ] = (coord_array *) ptrBuffer;
		//(parent->getMap(mapNumber))->putSpikeInBuffer( (coord_array *) ptrBuffer,  spikeBuffer + endBuffer );
        //if (*begBuffer == endBuffer) current_list = (coord_array *)ptrBuffer;
        //if (current_list == NULL) current_list = (coord_array *)ptrBuffer;
		endBuffer = (endBuffer+1) % MAX_SPIKE_BUFFER;
        //printf(" map %s add:%x realSize : %d (%d-%d)\n", name, (int)this, ((coord_array *) ptrBuffer)->size, begBuffer, endBuffer);
		
		// incrementing ptrBuffer
		int inf = 0;
		if (ptrBuffer < (unsigned char *)spikeBuffer[ begBuffer ]) inf = 1;
		ptrBuffer += size;
		if ((ptrBuffer - buffer) > BUFFER_NETWORK-MAX_MSG_SIZE) ptrBuffer = buffer;
		if (inf && (ptrBuffer > (unsigned char *)spikeBuffer[ begBuffer ]))
			ERROR(206,("buffer spike list overflow in server"));
    }
	
	virtual void init_neuron(GlobalImgTreat **listImg, int supervParam) 
	{ 
		// init du buffer
		begBuffer = 0;
		endBuffer = 0; 
		ptrBuffer = buffer;
		for (int i=0; i<MAX_SPIKE_BUFFER; i++)
			spikeBuffer[ i ] = NULL;
	
		current_list_ret = NULL;
        temps = 0;
        ordre_decharge = 0;
        if (disp != NULL) disp->clear( 0 );
	}
	virtual int compute_all() //coord_array **liste_in, coord_array **liste_out)
        {
		if ((endBuffer == begBuffer) % MAX_SPIKE_BUFFER) current_list_ret = NULL;
		else
		{ 
			//if (spikeRef[ begBuffer ] != NULL) *(spikeRef[ begBuffer ]) = NULL;
			current_list_ret = spikeBuffer[ begBuffer ];
			begBuffer = (begBuffer+1) % MAX_SPIKE_BUFFER;
            ordre_decharge += current_list_ret->size;
		}
        if (current_list_ret == NULL) return 0;
        else { temps ++; return 1; }
        
	}
	virtual void resetComputeSync() 		{ current_list_ret = NULL; if (disp != NULL) disp->writeSpikes( spike_list.getSpikeList( 0 ) ); }
	virtual void resetComputeSyncLearn() 	{ current_list_ret = NULL; }
	virtual proc *get_virtual()		{ return remoteProc; }
	virtual void affiche()
	{
        printf("%10s %4d\t%3d *%3d  VIRTUAL on %s ", name, numero, border_l, border_h, remoteProc->name);
        if (virtMap == 1) printf(" SUPRESSED\n"); else printf(" CONSERVED\n");
        if (d_IP != NULL) d_IP->affiche(); else printf("\t\t\tno afferent\n");
    }
	
	//empty virtual functions
	virtual void detect_convolution(carte_base *une_carte_from, char *name_convolution, int convLock,
			    double randFact, double multFact, double decFact, char *modFact, char *nameGroup, int duplicator, char *latency, int noZoom, double renorm) {}	
    virtual void add_convolutions() {}
	virtual int  saveAllConvos(int force, ScreenSpike *Ecran) { return 0; }
 	virtual void modif_seuil(double raise_threshold, double lower_threshold) {}

};


#endif
