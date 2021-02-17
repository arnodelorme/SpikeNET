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

// $Log: TabWeight.h,v $

#ifndef _TABWEIGHT
#define _TABWEIGHT

#include "mytimer.h"

/*class TabWeight
{
class PtrWeight
{
private:
		int 			afferent;
		int				offsetX;
		int				offsetY;
		FORMAT_POIDS 	value;

public:
	PtrWeight( int afferent0, int absc, int ord, FORMAT_POIDS value0)
	{
		afferent  = afferent0;
		offsetX	  = absc;
		offsetY	  = ord;
		value	  = value0;
	}

	int				getAfferent()	{ return afferent; }	
	int				getY() 			{ return offsetY; }	
	int				getX() 			{ return offsetX; }	
	FORMAT_POIDS 	getValue() 		{ return value; }
};

	PtrWeight 	 *tab_poids	[10000];
	int			 nbItems;

public : 

	TabWeight () { nbItems = 0; }
	int 		 getNbItems()  { return nbItems; }
	PtrWeight 	 *getItem( int pos )  { return tab_poids[pos]; }
	void addPoids( int afferent0, int absc, int ord, FORMAT_POIDS value0)
	{
		tab_poids [ nbItems ] = new PtrWeight( afferent0, absc, ord, value0 );
		nbItems++;
	}
	
	void sort()
	{
		for (int i=0; i< nbItems; i++)
			for (int j=i; j< nbItems; j++)
				if ( tab_poids [i]->getValue() < tab_poids [j]->getValue())
					{	PtrWeight 	 *tmp = tab_poids [j]; tab_poids [j] = tab_poids [i]; tab_poids [i] = tmp; }
	}
};*/

// this class deals with computing order of synaptic weight within a group (regroup many convolutions
// --------------------------------------------------------------------------------------------------

class TabWeight
{

//pointer to a synaptic weight
class PtrWeight
{
private:
		int 			afferent;		// reference to the convolution
		int				offsetX;		// X offset of the synaptic weight
		int				offsetY;		// Y offset of the synaptic weight
		FORMAT_POIDS 	*value;			// value of the synaptic weight

public:
	PtrWeight( int afferent0, int absc, int ord, FORMAT_POIDS *value0)
	{
		afferent  = afferent0;
		offsetX	  = absc;
		offsetY	  = ord;
		value	  = value0;
	}

	int				getAfferent()	{ return afferent; }	
	int				getY() 			{ return offsetY; }	
	int				getX() 			{ return offsetX; }	
	FORMAT_POIDS 	getValue() 		{ return *value; }
};

	typedef		PtrWeight	*PtrPtrWeight;
	typedef		int			*ptrInt;

	PtrWeight 	 **tab_poids;				// array of synaptic weights
	int			 nbItems;					// number of synaptic weights
	int			 maxItems;					// number maximum of ninaptic weight (the creation of synaptic weight is incremental and the maximum number of synaptic weight might depend on the convolution treated)
	static		 int	 	 **tabGlobal;	// global variable for fusion sorting (common to all maps)
	static		 int		 sizetabtmp;	// global variable for fusion sorting (common to all maps)

public : 

	TabWeight ( int maxItems0 ) 
	{ 
		nbItems = 0;
		// 2 fold because of zoomed convolutions
		maxItems = 10 * maxItems0;
		if (sizetabtmp < maxItems)
		{
			sizetabtmp = maxItems;
			if (tabGlobal != NULL) delete tabGlobal;
			tabGlobal = new ptrInt[ maxItems ];
		}
		tab_poids = new PtrPtrWeight[ maxItems ];
	}
	int 		 getNbItems()  { return nbItems; }
	PtrWeight 	 *getItem( int pos )  { return tab_poids[pos]; }
	void addPoids( int afferent0, int absc, int ord, FORMAT_POIDS *value0)
	{
		tab_poids [ nbItems ] = new PtrWeight( afferent0, absc, ord, value0 );
		nbItems++;
		if (nbItems > maxItems) ERROR( 309, ("too much weights for threshold computation"))
	}
	
	PtrPtrWeight *fusion(PtrPtrWeight *tab1, PtrPtrWeight *tab2, int size1, int size2)
	{
    	int i=0;
    	int j=0;
    	PtrPtrWeight *tabtmp = (PtrPtrWeight *)tabGlobal;
    	while ((i<size1) + (j<size2))
        {
            if (j==size2) { tabtmp[i+j] = *(tab1+i); i++; }
            else 
                if (i==size1) { tabtmp[i+j] = *(tab2+j); j++; }
                else 
                    if ( tab1[i]->getValue() < tab2[i]->getValue() ) { tabtmp[i+j] = *(tab2+j); j++; }
                    else { tabtmp[i+j] = *(tab1+i); i++; }
        }
    
    	for (i=0; i<(size1+size2); i++)
        	tab1[i] = tabtmp[i];
    
    	return tab1;
	}

	PtrPtrWeight *sortFusion(PtrPtrWeight *tab, int size)
	{
    	PtrPtrWeight tmp;
    	if (size>3)
        {
	  		int size1,size2;
	  		PtrPtrWeight *tab1,*tab2;
            size1 = size >> 1;
            size2 = size - size1;
            tab1 = sortFusion(tab, size1);
            tab2 = sortFusion(tab + size1, size2);

            /*if (tab1[size1-1]->getValue() > tab2[size2-1]->getValue())
            	return fusion2(tab1, tab2, size1, size2);
            else*/
            	return fusion(tab1, tab2, size1, size2);            	
        }
    	else
        {
        	if (size == 3) 
        		if (tab[0]->getValue() < tab[1]->getValue())
        			if (tab[0]->getValue() < tab[2]->getValue())
        				if (tab[1]->getValue() < tab[2]->getValue())
                			 { tmp = *tab; *tab = *(tab+2); *(tab+2) = tmp; } 						//CBA
                		else { tmp = *tab; *tab = *(tab+1); *(tab+1) = *(tab+2); *(tab+2) = tmp; } 	//BCA
                	else { tmp = *tab; *tab = *(tab+1); *(tab+1) = tmp; } 							//BAC
                else
        			if (tab[0]->getValue() < tab[2]->getValue())
                		{ tmp = *tab; *tab = *(tab+2); *(tab+2) = *(tab+1); *(tab+1) = tmp; } 		//CAB
                	else
        				if (tab[1]->getValue() < tab[2]->getValue())
                			 { tmp = *(tab+1); *(tab+1) = *(tab+2); *(tab+2) = tmp; } 				//ACB
 																									//ABC
            else if (tab[0]->getValue() < tab[1]->getValue())
                	{ tmp = *tab; *tab = *(tab+1); *(tab+1) = tmp; }
            return tab;
        }
	}
	
	/*PtrPtrWeight *fusion2(PtrPtrWeight *tab1, PtrPtrWeight *tab2, int size1, int size2)
	{
    	int stacked = 0;
    	PtrPtrWeight *tabtmp  = tab1;
    	PtrPtrWeight *tabinit = tab1;
    	while (tabtmp != tab2)
        {
            if ( tabtmp[0]->getValue() < tab2[0]->getValue() ) 
            { 
            	if (stacked) { PtrPtrWeight ptrTmp = *tab1; *tab1 = *tab2; tabtmp[stacked++] = ptrTmp; tab2++; tab1++; }
            	else		 { PtrPtrWeight ptrTmp = *tab1; *tab1 = *tab2; *tab2 = ptrTmp; tabtmp = tab2; tab2++; tab1++; } 
           	}
            else
            { 
            	if (stacked) { PtrPtrWeight ptrTmp = *tab1; *tab1++ = *tabtmp; *tabtmp = ptrTmp; }
            	else		 { tabtmp++; tab1++; } 
            }
        }
        // la moyenne de stacked par rapport a size1 peut donner une idée du gain de temps        
    	return tabinit;
	}*/

	double getMean()
	{
		double mean = 0;
		for (int i=0; i< nbItems; i++)
			mean += tab_poids [i]->getValue();
		return mean / nbItems;
	}

	double getVar()
	{
		double mean = getMean();
		double var  = 0;
		double var2 = 0;
		for (int i=0; i< nbItems; i++)
		{
			//var  += (tab_poids [i]->getValue() - mean) * (tab_poids [i]->getValue() - mean);
			var2 += tab_poids [i]->getValue() * tab_poids [i]->getValue();
		}
		//var  /= nbItems;
		var2 = (var2 - mean*mean*nbItems)/nbItems;
		//printf("%f %f\n", var, var2);
		return var2;
	}

	void sort()
	{
		//sortFusion( tab_poids, nbItems);
		for (int i=0; i< nbItems; i++)
			for (int j=i; j< nbItems; j++)
				if ( tab_poids [i]->getValue() < tab_poids [j]->getValue())
					{	PtrWeight 	 *tmp = tab_poids [j]; tab_poids [j] = tab_poids [i]; tab_poids [i] = tmp; }
		
		/*int inv = 1;
		while (inv != 0)
		{
			inv = 0;
			for (int i=0; i< nbItems-1; i++)
				if ( tab_poids [i]->getValue() < tab_poids [i+1]->getValue())
					{	PtrWeight *tmp = tab_poids [i]; tab_poids [i] = tab_poids [i+1]; tab_poids [i+1] = tmp; inv++; }
		}*/
	}
};
#endif