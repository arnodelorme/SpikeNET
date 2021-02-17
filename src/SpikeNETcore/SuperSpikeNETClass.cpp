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

// $Log: SuperSpikeNETClass.cpp,v $

// ******************************
// *					    	*
// *		SpikeNet            *
// *						    *
// ******************************

// include global
#include "SuperSpikeNETClass.h"
#include "y.tab.h"
#include "PictureTARGA.h"
//#include "WatchArray.h"
//#include "WatchAct.h"
#include "Chrono.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

//#define PROFILER
#ifdef PROFILER
#include <Profiler.h>
#endif

// il faut virer ces references externes
extern void makenewSpikeNET();
GlobalImg *originalImg;
ScreenSpikeAuto *screenLearned = NULL;
ScreenSpikeAuto *screen2 = NULL;

// for supervised learning, put rectangles at definite positions
//extern void writeWatchArray( char *fileName);
//#include "MapResult.h"
//MapResult mapRes(":networkSupervManyLearn:megapict_allpos", ":Images:megapict1.ppm", 600, 340);
//MapResult mapRes(":networkSupervManyLearn:megapict_allpos", ":Images:megapict1.ppm", 0, 0);

// **********************************************************************
//							*	             __	   _	   *	 __  ___
// 	  		|\/|	 --		|	|  |   		|  |  | |	   |	|	  |
// 	  		|  |	|--|	|	|\ | 		|  |  |-\ 	   |    |--	  |
//	  		|  |	|  |	|	| \|    	 --	  |__|  \_/ 	|__	  |
//
// **********************************************************************

#if SYSTEME == MAC
#include <windows.h>
#include <SIOUX.h>
void SuperSpikeClass::initTextWindow()
{
	// pour la petite fenetre
	SIOUXSettings.toppixel = 710;
	SIOUXSettings.leftpixel = 500;
	SIOUXSettings.columns = 80;
	SIOUXSettings.rows = 5;
	SIOUXSettings.asktosaveonclose = 0;
}
#else
void SuperSpikeClass::initTextWindow() { }
#endif


void SuperSpikeClass::resetResBrime()
{
 	for (int i=0; i< nbMaps; i++) {
 		int num = tab_carte[i]->get_number();
 		numberBadIn[ num ] = 0;
 		numberBadOut[num ] = 0;
 		numberGoodIn[ num]=0;
 		numberGoodOut[ num]=0;
 	}
}


// global variable
void SuperSpikeClass::testResBrime(int temps)
{
 	int 		coordx, coordy;
	static		int tested =0;
	if (temps == 0) tested = temps;
	char *mapLearn;
	int posx0, posy0;
	
	if (!tested) {
		img_list->firstMapLearn( &mapLearn, &posx0, &posy0);
 		for (int i=0; i< nbMaps; i++)
  			if ( tab_carte[i]->get_couche() ==  EnvVar::LAYER_CONVERGE)
				if (tab_carte[i]->ordre_decharge) {
  					int posx = tab_carte[i]->spike_list.getNext()->array[0].x, posxx;
  					int posy = tab_carte[i]->spike_list.getNext()->array[0].y, posyy;
  					int zoom = tab_carte[i]->get_zoom();
					if (zoom >0)	{	posxx = posx0 >> zoom; posyy = posy0 >> zoom; }
					else			{	posxx = posx0 << -zoom; posyy = posy0 << -zoom; }

 					//double distance = sqrt(sqr(posx0 - posx) + sqr(posy0 - posy));
  					if  (!strcmp(tab_carte[i]->get_name(), mapLearn)) {
 						if ((abs(posxx - posx) < EnvVar::SIZE_RESP_ZONE) && (abs(posyy - posy) < EnvVar::SIZE_RESP_ZONE)) {
							numberGoodIn[ tab_carte[i]->get_number()]++;
							if (!EnvVar::NO_SHOW)  printf("++++IN++++ %s(%d-%d) -%s(%d-%d)- %d\n", tab_carte[i]->get_name(), posx, posy, mapLearn, posxx, posyy, numberGoodIn[ tab_carte[i]->get_number()]);
						}
						else {
 							numberGoodOut[ tab_carte[i]->get_number()]++;						
							if (!EnvVar::NO_SHOW) printf("++++OUT++++ %s(%d-%d) -%s(%d-%d)- %d\n", tab_carte[i]->get_name(), posx, posy, mapLearn, posxx, posyy, numberGoodOut[ tab_carte[i]->get_number()]);
						}
					}
					else 
 						if ((abs(posxx - posx) < EnvVar::SIZE_RESP_ZONE) && (abs(posyy - posy) < EnvVar::SIZE_RESP_ZONE)) {
							numberBadIn[ tab_carte[i]->get_number()]++;
							if (!EnvVar::NO_SHOW)  printf("-----IN----- %s(%d-%d) -%s(%d-%d)- %d\n", tab_carte[i]->get_name(), posx, posy, mapLearn, posxx, posyy, numberBadIn[ tab_carte[i]->get_number()]);
						}
						else {
 							numberBadOut[ tab_carte[i]->get_number()]++;						
							if (!EnvVar::NO_SHOW) printf("-----OUT---- %s(%d-%d) -%s(%d-%d)- %d\n", tab_carte[i]->get_name(), posx, posy, mapLearn, posxx, posyy, numberBadOut[ tab_carte[i]->get_number()]);
						}
					tested = 1;
				}
 	}
}			
			
void SuperSpikeClass::setMapLearn()
{
	if (EnvVar::LEARN_SUPERV) {
		int 		coordx, coordy;
		carte_base 	*found;
		char *mapLearn;
	
		while(img_list->getMapLearn( &mapLearn, &coordx, &coordy))
		{
			if ((mapLearn[0] >= '0') && (mapLearn[0] <= '9') && (mapLearn[1] == '_'))
			{
				int spikeLearn = int(mapLearn[0]) - int('0');
				mapLearn[0] = 'x';
 				found = SpikeNet[spikeLearn]->findInside(mapLearn);
 				if (found == NULL) {
					mapLearn[0] = 'X';
 					found = SpikeNet[spikeLearn]->findInside(mapLearn);
 					if (found == NULL) ERROR( 120,("map %s not found for learning", mapLearn));
 				}
 			}
 			else {
				if (((mapLearn[0] >= 'x') || (mapLearn[0] <= '9')) && (mapLearn[1] == '_'))
 					ERROR( 121,("cannot learn on multiscaled maps"))
 		
 				// search in the multiscaled map
 				found = SpikeNet[numberSpikeNET-1]->findInside(mapLearn);	
 			}
 			if (found != NULL)
 		 		 found->setLearnCoord( coordx, coordy); 					
			else ERROR( 132,("learning map %s not found for supervised learning", mapLearn))
		}
	}	
}

SuperSpikeClass::SuperSpikeClass(int argc, char *argv[])
{
	initTextWindow();
	increaseT = 0;

	EnvVar::init( NET_ENV );
	
	//if (EnvVar::NEW_IMAGE_LIST == NO)
	//  img_list = new ImageListLearn(EnvVar::STEP);
	//else // Note that ImageListRich is a childree class of ImageListLearn
	img_list = new ImageListRich(EnvVar::STEP);
		
	img_list->reserv();
	img_list->init( 1 );
	img_list->reset();
	init( *(img_list->revert()) );
}

void SuperSpikeClass::init( BaseImg *baseImg)
{
	// Sockets
	server = new Server();
    procManager = new ProcManager( server->getHostName() );

	//output initialisation
  	if (EnvVar::DISPLAY  == OUI)	screen0 = new ScreenSpikeAuto( 0, 0, 1024, 768, YES, YES); else screen0 = NULL;	
	if ((EnvVar::FOND) && (EnvVar::DISPLAY == OUI)) {
		PictureTARGA *pict = new PictureTARGA(":Images:SpikeNET5.tga");
		screen0->clearArea( 0, 0, 1024,768, 0xFF808080);
		screen0->putBaseImg( (BaseImg *)pict, 100, 100, 0);
	}
	
	if (EnvVar::LEARN) {
		screen2 = new ScreenSpikeAuto( 20, 200, 600, 800, YES, YES,16);
		screenLearned = new ScreenSpikeAuto( 20, 400, 660, 600, YES, YES, 24);
	}
	
	// create network
	networkType = determineMultiscale();
	SpikeNet[0] = new MapList( baseImg->getWidth(), baseImg->getHeight(), 0, 0 /* useless */, NULL, ONESCALE, this);
	if (networkType & MULTISCALE) {
		for (int i=1; i<EnvVar::NB_SPIKENET+1; i++)
	  		SpikeNet[i] = new MapList( baseImg->getWidth(), baseImg->getHeight(), i-1, 0 /* useless */, NULL, MULTISCALE, this);
		numberSpikeNET = EnvVar::NB_SPIKENET+1;
	}
	else numberSpikeNET = 1;

	// add all the map to mapList
	for (int ki=1; ki<numberSpikeNET; ki++)
		SpikeNet[0]->addMapList( SpikeNet[ki] );	  	
	
	// initialise network
	read_maps( baseImg );
	read_projs();
	for (int kk=0; kk<numberSpikeNET; kk++)
		SpikeNet[kk]->SpikeClassInit ( );
	
	// scanage des differentes images
	// ------------------------------	
	if ((EnvVar::FOND) && (EnvVar::DISPLAY == OUI))
		screen0->clearArea(  315, 170, 709, 450, 0xFF808080);
}


void SuperSpikeClass::compute()
{
  Chrono C;

#ifdef PROFILER
	if (!ProfilerInit(collectDetailed, bestTimeBase, 100, 5)) {
		ProfilerSetStatus(TRUE);
#endif

	//{ FILE *f =fopen(":save_rebuild:SpikeList_ON", "w");
	//for (int i=0; i< 2100; i++) fprintf( f, "0\t0\t0\n");
	//fclose(f); }
	//{ FILE *f =fopen(":save_rebuild:SpikeList_OFF", "w");
	//for (int i=0; i< 2100; i++) fprintf( f, "0\t0\t0\n");
	//fclose(f); }

	// watch activity
	if ( EnvVar::WATCH_ACTIVITY )
	{
/*		watchAct = new WatchAct();
		for (int ll=0; ll<nbMaps; ll++)
		  	if ( !tab_carte[ll]->get_FIXED() )
	    	 	watchAct->addWatch( tab_carte[ll]->get_name());*/
	}	

	if (img_list == NULL)
		ERROR( 308 ,("null pointer for the image list"));
	
	GlobalImgTreat **listImg;
	img_list->init( y_tab::imageMaxi() );
	img_list->reset();

	// renormalizes convolutions
	/*for (int kkk=0; kkk<nbMaps; kkk++) {
		tab_carte[kkk]->renormConvos(10000);
	  	tab_carte[kkk]->seuil_mod();
	}*/

	//getchar();
	while((listImg = img_list->revert()) != NULL)
	{
    	  originalImg = listImg[0];

		  SpikeNet[0]->init_neuron(listImg, 0 );
 
#if SYSTEME ==MAC
		  if (EnvVar::CHRONO) Timer.start();
#else 
		  if (EnvVar::CHRONO) C.Start();
		  //if (!EnvVar::NO_SHOW) printf("Starting chrono\n");
#endif
  		
  		  if (screen0 != NULL) screen0->affiche_image(listImg[0], EnvVar::SIZEX, EnvVar::SIZEY ,0);

		  //set learn
		  setMapLearn();
		  	  
		  // COMPUTATION  /  LEARNING
		  // --------
		  if (EnvVar::LEARN == NO)
		  {
          		//tab_carte[0]->write_spikelists( (img_list->getCount()-1)* 5);
                //tab_carte[1]->write_spikelists( (img_list->getCount()-1)* 5);
		  		for (int temps=0;temps< EnvVar::TIME_STOP;temps++) {
					SpikeNet[0]->resetComputeSync();
                 	SpikeNet[0]->compute_all();   
           			//if (EnvVar::TESTRES) testResults();
 					//testResBrime( temps);
             	}
            
            	// save current screen
 				static int indicator=0;
  				//{ char fileName[100]; sprintf(fileName, "%s%s%csaveruf_%.5d", CURRENT_DIR, EnvVar::DIR_SAVE_REBUILD, SEPARATEUR, indicator/4);
  				//screen0->save( fileName ); }
  				indicator ++;
            
          		if (EnvVar::LEARN_SUPERV == YES)
			  		if	((img_list->getCount() == EnvVar::RESCUE + 1) || !((img_list->getCount()-1) % 1))
		  				for (int kk=0; kk<nbMaps; kk++)
	    					if (tab_carte[kk]->saveAllConvos( (img_list->getCount() == EnvVar::RESCUE + 1) ))
	    						tab_carte[kk]->reconstructConvo( (img_list->getCount() == EnvVar::RESCUE + 1) );
		  }
		  else
		  {		
		  		for (int temps=0;temps< EnvVar::TIME_STOP;temps++) {
	 				SpikeNet[0]->resetComputeSync();
                 	SpikeNet[0]->compute_all();   
		       	}

				SpikeNet[0]->init_neuron(listImg, 1 );
		    	
		  		for (int temps2=0;temps2< EnvVar::TIME_STOP;temps2++) {
	 				SpikeNet[0]->resetComputeSyncLearn();
                 	SpikeNet[0]->learn_all();   
                }

		  		//ditotomic threhsold & renormalizes convolutions
	  			for (int kkk=0; kkk<nbMaps; kkk++) {
	  				tab_carte[kkk]->renormConvos(10000);
	  				//tab_carte[kkk]->seuil_mod();
	  				//tab_carte[kkk]->seuil_mod_dicot( 0.001);
	  			}

		  		if	((img_list->getCount() == EnvVar::RESCUE + 1) || !(img_list->getCount() % 20))
		  		{		  				
		  			for (int kk=0; kk<nbMaps; kk++) {
	    					if (tab_carte[kk]->saveAllConvos( (img_list->getCount() == EnvVar::RESCUE + 1) ))
	    						tab_carte[kk]->reconstructConvo( (img_list->getCount() == EnvVar::RESCUE + 1) );
	    				}
		  	 		//writeWatchArray(":allconvos");
		  			saveOnDisk( img_list->getCurrentImg(), img_list->getCount());
		  	 	}
		  		for (int ll=0; ll<nbMaps; ll++)
		  		{
					//tab_carte[ll]->seuil_mod();
		  	 		//tab_carte[ll]->writeStats();
		  	 		//tab_carte[ll]->writeRebuild();
		  		}
		  }

		  // watching neuronal maximum activity
		  if ( EnvVar::WATCH_ACTIVITY )
		  {
/*		      watchAct->addImg( img_list->getCurrentImg() );
		  	  for (int kk=2; kk<nbMaps; kk++)
		  	  	//if ( !tab_carte[kk]->get_FIXED() )
		  	  	//{
	    	  	  	watchAct->addAct( kk-2, tab_carte[kk]->getMaxAct() );
	    	  	  	//printf("%d\n",  tab_carte[kk]->getMaxAct() );*/
	    	  	//}
		  }
		  
		  printf("%d img. treated;\tspikes", img_list->getCount());	
		  for (int nn=0; nn<numberSpikeNET; nn++)
		      SpikeNet[nn]->affiche();
		  	
		  if (EnvVar::WAIT_ENTER) { printf("Press a key to continue...\n"); getchar(); }
		  
		  // affichage du temps
#if SYSTEME ==MAC
		  if (EnvVar::CHRONO) printf("Propagation time in seconds %d.\n", Timer.end());
#else 
		  C.Stop();
		  if (EnvVar::CHRONO) printf("Propagation time in milliseconds %d.\n", C.Read());
#endif
	}


	// write at last
	for (int kk=0; kk<nbMaps; kk++)
	    if (tab_carte[kk]->saveAllConvos( 0 ))
	    		tab_carte[kk]->reconstructConvo( 0 );
	    		
	if ( EnvVar::WATCH_ACTIVITY )
		;//watchAct->writeAll( "allAct");

	printf("Press a key to continue...\n"); getchar();	
	//mapRes.save(":resultat"); 
	//screen0->saveTARGA_grab_abs( ":test", 0, 0, 320, 360); 
	// fin (SNIF)
#ifdef PROFILER
	}
	ProfilerDump("\pSpikeNET.prof");
	ProfilerTerm();
#endif
}

void SuperSpikeClass::saveOnDisk(char *ImageName, int count_image)
{
	if (img_list->getCount() <= 10)
	{
		screenLearned->resize();
		screen2->resize();	
	}	
	screen2->update();	
	screenLearned->update();	

    	//count_image -= 1;
  	char fileName[50];
  	sprintf(fileName, "%s%s%crecog_%.5d", CURRENT_DIR, EnvVar::DIR_SAVE_REBUILD, SEPARATEUR, count_image);	screenLearned->save( fileName );
 	sprintf(fileName, "%s%s%crebuild_%.5d", CURRENT_DIR, EnvVar::DIR_SAVE_REBUILD, SEPARATEUR, count_image); screen2->save( fileName );
 	//if (!(count_image % 2600)&& (count_image != 0)) { screen2->shift( 200, -14*25);  }
 	//if (!(count_image % 200) && (count_image != 0) && (count_image < 2600)) { screen2->shift( 0, 25); screen2->clear(); }
 	
 	if (!(count_image % 100)) {
   		sprintf(fileName, "%s%s%crecog_%.5d", CURRENT_DIR, EnvVar::DIR_SAVE_REBUILD, SEPARATEUR, count_image);	screenLearned->save( fileName );
 		sprintf(fileName, "%s%s%crebuild_%.5d", CURRENT_DIR, EnvVar::DIR_SAVE_REBUILD, SEPARATEUR, count_image); screen2->save( fileName );
 		//screen2->clear(); 
	}
 	//sprintf(fileName, "savescreen_%.5d", count_image);	
  	//screen1->save( fileName );
}

void SuperSpikeClass::run()
{
	if (EnvVar::CONVERGE)
		for (increaseT = 0; increaseT< EnvVar::CONVERGE ; increaseT++) {
			resetResBrime();

			// modify thresholds
			if (!EnvVar::NO_SHOW) { printf("\ncycling %d\n", increaseT); }
			fprintf(SpikeNet[0]->fileOutput, "\n", increaseT);
			compute();
			actualiseSession( increaseT );
		}
	else compute();
}

void SuperSpikeClass::actualiseSession( int modulator)
{
    // adjust threshold : supervised
	double newT;
	// MODIFIED Nov 7 2002 do not write in output file
	//char outputName[30]; sprintf( outputName, "%stmp%coutput", CURRENT_DIR, SEPARATEUR);
	//FILE *fout = fopen(outputName, "w");
	//if (fout == NULL) ERROR( 12123,("can not create file tmp/%s (check that directory exist)", "output"));
	
	if (!EnvVar::NO_SHOW) printf( "\nUpdate\tName\tThresh\tSpikes\n");
	for (int i=0; i< nbMaps; i++)
	  if ( tab_carte[i]->get_couche() ==  EnvVar::LAYER_CONVERGE) {
		int num = tab_carte[i]->get_number();
		
		// MODIFIED Nov 7 2002 to encode the probability of discharge of neurons
		//int total_distract = img_list->getNbImages() - number2discharge[ num];
	  	
		int modulT = tab_carte[i]->modulateThreshold(0); // optimal 10 discharge per map
		int res = number2discharge[num] - modulT;
		//printf("%d: modulator, %d: modulT, %d: total_distract, %d:num\n", res, modulT, total_distract, num);
	
		if (modulator && res) {
		  double modifier = 0.02 / (double) modulator * (1 + (double)abs(res)/10);
		  if (modifier > 0.1) modifier = 0.1;
		  if (res < 0)	newT = tab_carte[i]->seuil_mod( modifier);
		  else			newT = tab_carte[i]->seuil_mod( -modifier);			
		  // change parameter in file
		  //changeInFile( tab_carte[i]->get_name(), newT);
		  //FILESTREAM f("tmp/net_tmp");
		  //FILE *fo = fopen("net_test", "w");
		  //char c; while ((c = f.get()) != EOF) fprintf( fo, "%c", c);
		  //fclose( fo );
		  if (!EnvVar::NO_SHOW) printf( "yes\t"); 
		}
		else if (!EnvVar::NO_SHOW) printf( "no\t"); 
		//else 	newT = tab_carte[i]->seuil_mod( 0 );
    			
		if (!EnvVar::NO_SHOW) {
		  printf( "%s\t", tab_carte[i]->get_name());
		  printf( "%1.4f\t%d\n", newT, modulT);
		  //printf( "%2.1f\t%d\t%d\t", 100 * (double)modulT / img_list->getNbImages(), modulT, img_list->getNbImages());
		  //printf( "%2.1f\t%d\t%d\t%d\t", 100 * (double)numberGoodIn[ num] / number2discharge[num], numberGoodIn[ num], numberGoodOut[ num], number2discharge[ num]);
		  //printf( "%2.1f\t%d\t%d\t%d\n", 100 * (double)numberBadIn[ num] / total_distract, numberBadIn[ num], numberBadOut[ num], total_distract);
		  //exit(-1);
		}
		//number2discharge[ tab_carte[i]->get_number()], 
		//fprintf(fout, "%s\t", tab_carte[i]->get_name());
		//fprintf( fout, "%2.1f\t%d\t%d\t", 100 * (double)modulT / img_list->getNbImages(), modulT, img_list->getNbImages());
		//fprintf( fout, "%2.1f\t%d\t%d\t%d\t", 100 * (double)numberGoodIn[ num] / number2discharge[num], numberGoodIn[ num], numberGoodOut[ num], number2discharge[ num]);
		//fprintf( fout, "%2.1f\t%d\t%d\t%d\n", 100 * (double)numberBadIn[ num] / total_distract, numberBadIn[ num], numberBadOut[ num], total_distract);
	  }
	//getchar();
	//fclose( fout );
}

void SuperSpikeClass::changeInFile( char *strName, double newT )
{
	FILESTREAM f("net_test");
	char outputName[30]; sprintf( outputName, "%stmp%cnet_tmp", CURRENT_DIR, SEPARATEUR);
	FILE *fo = fopen(outputName, "w");
	char tmp[500];

	while (f.eof() != EOF) {
		f.getline( tmp, 500, '\n');
		if (tmp[0] != 0) {
			if (strstr(tmp, "target") != NULL)
				if (strstr( tmp, strName) != NULL) {
					char *strPos = strstr( tmp, "0.") - 1;
					*strPos = 0;
					fprintf( fo, "%s\t%f\n", tmp, newT);
				}
				else fprintf( fo, "%s\n", tmp);
			else fprintf( fo, "%s\n", tmp);
		}
	}
	fclose( fo);
}

void SuperSpikeClass::testResults( )
{
/*  static int drawRectPrev = 0;
  if (drawRectPrev) screen0->flushSave( 0, 0, 320, 360);
  drawRectPrev = 0;
  
  for (int i=0; i< nbMaps; i++)
  	if ( tab_carte[i]->get_couche() ==  EnvVar::LAYER_CONVERGE)
  	{
  		int res;
  		for (int j=0; j< tab_carte[i]->spike_list.getSpikeList( 0 )->size; j++)
  		{
  			int posx = tab_carte[i]->spike_list.getSpikeList( 0 )->array[j].x;
  			int posy = tab_carte[i]->spike_list.getSpikeList( 0 )->array[j].y;
  			res = mapRes.testForRes( posx, posy, tab_carte[i]->get_name());
			if (res == 2) screen0->drawRect( posx-10, posy-13, posx+10, posy+13, 0xFF00FF00); //tab_carte[i]->getColor());
			if (res == 1) screen0->drawRect( posx-10, posy-13, posx+10, posy+13, 0xFFFF0000);
			screen0->resetWriteInImg();
			if (res == 2) screen0->putBaseImg( tab_carte[i]->getImageAssociated(), posx-12, posy-15, 2, 0xFF00FF00); //tab_carte[i]->getColor());
			if (res == 1) screen0->putBaseImg( tab_carte[i]->getImageAssociated(), posx-12, posy-15, 2, 0xFFFF0000);
			screen0->setWriteInImg();
			if ((res ==1) || (res == 2)) drawRectPrev = 1;
		}
	}*/
}
