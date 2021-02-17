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

// $Log: RunLearn.cc,v $

// ******************************
// *					    	*
// *		SpikeNet            *
// *						    *
// ******************************

// include global
#include "SuperSpikeNETClass.h"

    int 	EnvVar::NB_COLOR      		= DEFAULT_NB_COLOR;
    int 	EnvVar::MAX_TEMPS     		= DEFAULT_MAX_TEMPS;
    int 	EnvVar::MAX_NORM      		= DEFAULT_MAX_NORM;
    int	 	EnvVar::SAVE_ON_DISK  		= DEFAULT_SAVE;
    int	 	EnvVar::DISPLAY  			= DISPLAY;
    int 	EnvVar::NO_SHOW       		= DEFAULT_SHOW;
    int		EnvVar::MOD_LINEAIRE  		= DEFAULT_MOD;
    int		EnvVar::NB_SPIKENET   		= 1;
    int		EnvVar::WAIT_ENTER    		= 0;
    int		EnvVar::CONVERGENCE_CONVO 	= DEFAULT_CONV_CONVO;
    int		EnvVar::NET_NAME_VERSION	= DEFAULT_NAME_VERSION;
	int		EnvVar::STEP			  	= 1;
    double	EnvVar::MOD_CUT       		= atof(DEFAULT_MOD_CUT);
    double	EnvVar::RAISE_THRESHOLD 	= atof(DEFAULT_RAISE_THRE);
    double	EnvVar::LOWER_THRESHOLD 	= atof(DEFAULT_LOWER_THRE);
    char 	EnvVar::RESULTS [40] 		= DEFAULT_RESULTS;
    char 	EnvVar::MAIN_DIRECTORY[100] = ".";
 	int	    EnvVar::MOD_TYPE			= DEFAULT_MODABS;
  	int	    EnvVar::ZOOM_CONVO			= DEFAULT_ZOOM_CONVO;
  	char    EnvVar::DIR_SAVE_CONVOS[40]	= DEFAULT_DIR_SAVE_CONVOS;
  	char    EnvVar::DIR_SAVE_LEARN[40]	= DEFAULT_DIR_SAVE_LEARN;
  	char    EnvVar::DIR_SAVE_REBUILD[40]= DEFAULT_DIR_SAVE_REBUILD;
  	char    EnvVar::DIR_SAVE_STATS[40]	= DEFAULT_DIR_SAVE_STATS;
 	int	    EnvVar::PURE_ORDER			= DEFAULT_PURE_ORDER;
  	int	    EnvVar::BYTES_PER_PIXEL		= DEFAULT_BYTES_PER_PIXEL;
  	int	    EnvVar::BYTES_FILE			= 1;
  	int	    EnvVar::SIZEX				= DEFAULT_SIZEX;
    int	    EnvVar::SIZEY				= DEFAULT_SIZEY;
    int	    EnvVar::REPLACE0_1			= DEFAULT_REPLACE0_1;
    int	    EnvVar::LEARN   			= DEFAULT_LEARN;
    int	    EnvVar::FOND    			= 1;
    int	    EnvVar::RESCUE   			= 0;
    double  EnvVar::TIME_BASE  			= atof(DEFAULT_TIME_BASE);
    double  EnvVar::SPONTANEOUS  		= 0.0;
    int 	EnvVar::BRIME_LIB     		= 0;
    int 	EnvVar::THRESHOLD_TYPE     	= 0;
    int		EnvVar::WATCH_ACTIVITY     	= 0;
    int     EnvVar::LEARN_SUPERV		= NO;
    int     EnvVar::RECONSTRUCT_MOD		= NO;
    int		EnvVar::CONVERGE 			= NO;
    int		EnvVar::NEW_IMAGE_LIST		= NO;
    int		EnvVar::LAYER_CONVERGE		= 2;
   int		EnvVar::CONVERGE_VALUE		= 8;    
    int		EnvVar::TIME_STOP			= -1;    
    int		EnvVar::TESTRES				= 0; 
    int		EnvVar::SIZE_RESP_ZONE		= 2;

    int		TabWeight::sizetabtmp		= 0;
    int		**TabWeight::tabGlobal		= NULL;
    FileFolder	*ifilestream::testing	= new FileFolder(ARCHIVE_NAME);

/////////////////////////////////

static SuperSpikeClass *mainObject = NULL;

#include "SpikeNETlib.h"
#include "allDefs.h"
#include "ImageListRich.h"
#if SYSTEME != UNIX
#include <stat.h>
#endif
#include <unistd.h>
    
#define  BRIME_MODE	1		// 1=LEARN 0=RUN
#define  DEFAULT_FILE		"net_test"
#define  DATA				"Data"
#define  TMP				"tmp"
#define READ_OR_WRITE 		 0x0 /* fake a UNIX mode */ 

// SPLIT       LEARN
//************
char baseNet[100];
char baseSaveDir[100];
char *sessionName = NULL;
int    optimize = 0;

// scaling lines
// scaling lines
char  *scalingMap( char *init, char *mapName = NULL)
{
	// return map name in the second argument if != null
	STRSTREAM ff(init);
	char tmp[200];
	char dest[200];
	double scale = 1.0;
	int	 newAbs, newOrd;
	dest[0] = 0;
	if (mapName != NULL) mapName[0] = 0;
	
	while( ff.eof() != EOF) {
		ff >> tmp;
		if (!strcmp("Scale", tmp)) { ff >> tmp; scale = atof( tmp ); strcat(dest, "\tScale\t"); strcat( dest, tmp);}
		else if (!strcmp("Learn", tmp)) {
			strcat( dest, "\tLearn\t");
			ff >> tmp;	strcat( dest, tmp);
			if (mapName != NULL) strcpy( mapName, tmp);
			ff >> tmp;	newAbs = rint( atoi( tmp ) * scale );
			ff >> tmp;	newOrd = rint( atoi( tmp ) * scale );
			strcpy( tmp, dest);
			sprintf( dest, "%s\t%d\t%d", tmp, newAbs, newOrd);
		}
		else { if (dest[0] !=0) strcat(dest, "\t");  strcat( dest, tmp); }
	}
	strcpy( init, dest);
	return init;
}

// Optimize
void Optimize( int accuracy)
{
	//copy net_env
	char c;
	char netEnvOut[100]; sprintf( netEnvOut, "%s%s%cnet_env", CURRENT_DIR, TMP, SEPARATEUR);
	FILE *fo = fopen(netEnvOut, "w");
	char netEnvIn[100]; sprintf( netEnvIn, "%s%s%c%s%cnet_env", CURRENT_DIR, DATA, SEPARATEUR, baseNet, SEPARATEUR);
	FILESTREAM f3(netEnvIn);
	while ((c = f3.get()) !=EOF)
		fprintf(fo, "%c",c);
		
	if (accuracy > 0)	fprintf(fo, "CONVERGE\t%d\n", accuracy);		
	if (accuracy > 0)	fprintf(fo, "LAYER_CONVERGE\t2\n");		
	fclose( fo);
}

void Split( char *file, int learn1run0)
{
	FILESTREAM f(file);
	char tmp[500];
	int absc[50];
	int ordo[50];
	int color[50];
	int scale[50];
	char inhibition[500]; inhibition[0] = 0;
	
	// default coordinates for target maps
	for (int iii=0; iii<50; iii++) {
		absc[ iii ] = (iii/6)*123 + 530;
		ordo[ iii ] = (iii %6)*120;
		color[ iii ] = 0;
		scale[ iii ] = 0;
	}

	// networktype
	baseNet[0] = 0;
	int settings =1;
	while (settings && (f.eof() != EOF)) {
		f >> tmp;
		if ( !strcmp( "SessionName", tmp)) { f >> tmp; sessionName = baseSaveDir; sprintf(  baseSaveDir, "%s%c%s", DATA, SEPARATEUR, tmp ); }
		else if ( !strcmp( "Inhibition", tmp)) { f >> inhibition;}
		else if ( !strcmp( "Networktype", tmp)) { f >> baseNet; }
		else if ( !strcmp( "Optimize", tmp)) { f >> tmp; optimize = atoi( tmp ); }
		else settings = 0;
	}
	if (baseNet[ 0 ] == 0) ERROR(0, ("A network have to be defined in file %s", file))
	
	// Reading all Maps to learn
	char *learnMap[100]; learnMap[0] = new char[10000];
	int nbLearn[100];
	int sizex[100];
	int sizey[100];
	double threshold[100]; threshold[0] = 0;
	int nbLoop = 0;
	int lastStr = 0;
	
	while (!strcmp("target", tmp)) {
		lastStr = 0;
		f >> tmp;
		strcpy( learnMap[ nbLoop ], tmp);
		learnMap[nbLoop+1] = learnMap[nbLoop] + strlen(tmp)+1;
		f >> tmp;

		// determining map position if necessary
		char *tmppos;
		if ((tmppos = strpbrk( tmp, "x")) != NULL) {
			*tmppos = 0;
			absc[ nbLoop ] = atoi( tmp );
			ordo[ nbLoop ] = atoi( tmppos+1 );
			f >> tmp; color[ nbLoop ] = strtoul(tmp, (char **) NULL, 0);
			f >> tmp; scale[ nbLoop ] = atoi( tmp );
			f >> tmp; sizex[ nbLoop ] = atoi( tmp );
		}
		else sizex[ nbLoop ] = atoi( tmp );
		f >> tmp; sizey[ nbLoop ] = atoi( tmp );
		f >> tmp;
		if (strcmp("target", tmp)) { threshold[ nbLoop] = atof(tmp); f >> tmp; lastStr = 1; }
		nbLearn[nbLoop]=0;
		nbLoop++;
	}
	
	//setting all thresholds if not
	if (threshold[0] == 0.0)
		for (int ii = 0; ii<nbLoop; ii++)
			threshold[ii] = 0.2;
	
	//create directories
	if (sessionName == NULL) {
		char newFolder[100];
		/*for (int ii = 0; ii<nbLoop; ii++) {
		sprintf(newFolder,"%s%s%c%s%c%s_%dx%d", CURRENT_DIR, DATA, SEPARATEUR, baseNet, SEPARATEUR, learnMap[ ii ], sizex[ ii ], sizey[ ii ]);
		mkdir(newFolder, READ_OR_WRITE );
		}*/
		// find the last dir
		int folderExisted = 0;
		int found = 0;
		while ( !found ) {
			sprintf(newFolder,"%s%s%cNetwork%.4d", CURRENT_DIR, DATA, SEPARATEUR, folderExisted);
			if( mkdir(newFolder, 0xFFFFFFFF ) == -1 ) { 
				//printf("Failed to Create folder%s\n", newFolder); 
				folderExisted++; 
			} else found =1;
		}
		// delete new dir if runing
		if (!learn1run0) { rmdir(newFolder); folderExisted--; }
		sprintf(baseSaveDir,"%s%cNetwork%.4d", DATA, SEPARATEUR, folderExisted);
	}
	
	// flusing lst_img
	char listeImages[100], c;
	char maptmp[50];
	sprintf( listeImages, "%s%s%clst_img", CURRENT_DIR, TMP, SEPARATEUR);
	FILE *fo = fopen(listeImages, "w");
	if (!lastStr)	fprintf( fo, "image\t%s\t", scalingMap(tmp));
	else			fprintf( fo, "%s\t", scalingMap(tmp));
	while (f.eof() !=EOF) {
		f.getline( tmp, 500, '\n'); 
		if (tmp[0] != 0) {
			scalingMap(tmp, maptmp);
			for (int ii=0; ii< nbLoop; ii++)
				if (!strcmp( maptmp, learnMap[ii])) {
					//printf("compare %s\t%s\n", maptmp, learnMap[ii]);
					nbLearn[ii]++;
				}
			fprintf(fo, "%s\n",tmp);
		}
		//if (!learn1run0) *strstr( tmp, "Learn") =0;	
	}
	fclose( fo);
	
	// Converting net_names
	char netNamesOut[100]; sprintf( netNamesOut, "%s%s%cnet_names", CURRENT_DIR, TMP, SEPARATEUR);
	fo = fopen(netNamesOut, "w");
	char netNamesIn[100]; sprintf( netNamesIn, "%s%s%c%s%cnet_names", CURRENT_DIR, DATA, SEPARATEUR, baseNet, SEPARATEUR);
	FILESTREAM f1(netNamesIn);
	while (f1.eof() != EOF) {
		f1.getline( tmp, 500, '\n');
		if ( strstr( tmp, "target") == NULL) fprintf(fo, "%s\n", tmp);
		else {
			char *tmp2 = strstr( tmp, "target");
			*tmp2 = 0;
			for (int i=0; i<nbLoop; i++)
				if (learn1run0)	fprintf( fo, "%s%s\tThreshold\t0.5\tFreq\t%d\tState\tSuperv\t%s\tColor\t0x%x\tPos\t%d\t%d\t%d\n", tmp, learnMap[ i ], nbLearn[ i ], tmp2+6, color[i], absc[i], ordo[i], scale[i]);
				else			fprintf( fo, "%s%s\tThreshold\t%f\tFreq\t%d\t%s\tColor\t0x%x\tPos\t%d\t%d\t%d\n", tmp, learnMap[ i ], threshold[i], nbLearn[ i ], tmp2+6, color[i], absc[i], ordo[i], scale[i] );
	 		//break; //TO REMOVE
	 	}
	 }
	 fclose( fo);

	// Converting net_projs
	char netProjsOut[100]; sprintf( netProjsOut, "%s%s%cnet_projs", CURRENT_DIR, TMP,SEPARATEUR);
	fo = fopen(netProjsOut, "w");
	char netProjsIn[100]; sprintf( netProjsIn, "%s%s%c%s%cnet_projs", CURRENT_DIR, DATA, SEPARATEUR, baseNet, SEPARATEUR);
	FILESTREAM f2(netProjsIn);
	while (f2.eof() != EOF) {
		f2.getline( tmp, 500, '\n');
		if ( strstr( tmp, "target") == NULL) fprintf(fo, "%s\n", tmp);
		else {
			char *tmp2 = strstr( tmp, "target");
			*(tmp2-1) = 0;
			for (int i=0; i<nbLoop; i++)
				if (learn1run0)	fprintf( fo, "%s\t%s\tFILE\tUNLOCK\t%s\t%dx%d\n", tmp, learnMap[ i ], tmp2+6, sizex[ i ], sizey[ i ] );
				else			fprintf( fo, "%s\t%s\tFILE\tLOCK\t%s\t%s%c%s_%s\n", tmp, learnMap[ i ], tmp2+6, baseSaveDir, SEPARATEUR, learnMap[ i ], tmp );
	 	}
	 }
	 // adding inhibition if necessary
	 if (!learn1run0)
	 	if ( ( inhibition[0] != 0 )  && (!optimize) ) {
	 		fprintf( fo, "\n");
			for (int i=0; i<nbLoop; i++) {
				for (int j=0; j<nbLoop; j++)
					if (i != j) fprintf( fo, "%s\t%s\tFILE\tLOCK\tGROUP\tINHIB\t%s\n", learnMap[ i ], learnMap[ j ], inhibition );
			}
	 	}	 
	 fclose( fo);

	 Optimize( learn1run0 ? 0 : optimize );	 
}

void copy( char *fileName1, char *fileName2)
{
	// updating the log file
	FILESTREAM f(fileName2);
	FILE *fo = fopen(fileName1, "w");
	if (!fo) ERROR( 0, ("unable to open file for writting %s", fileName2))
	char c; while ((c = f.get()) != EOF) fprintf( fo, "%c", c);
	fclose( fo );
}

// SUPERSPIKECLASS METHODS
//*********************
//time facilities
#include <time.h>
void gettime( char *res)
{
       time_t systime;
        systime = time(NULL);
        strcpy( res, ctime(&systime));
}

#define LEARNING 1

int main(int argc, char *argv[])
{
	//char test[200] = "Image	Images/grats35x35/grats35x35_000.PPM	Scale	0.5	Random	0.5_1	Learn	Car	34	56";
	
	// choosing directory
	char cons[100];
	sprintf( cons, "%sconsole", CURRENT_DIR);
	FILE *f2 = fopen( cons, "w");
	fprintf( f2, "%s\n", TMP);
	fclose(f2);

	// spliting the declaration file
	 Split( DEFAULT_FILE,  LEARNING); //learn 1 run 0 then optimize
	mainObject = new SuperSpikeClass( argc, argv);
	strcpy( EnvVar::DIR_SAVE_CONVOS,  baseSaveDir);	
	mainObject->run();
	
	// backup file
	copy( "logfile_backup", "logfile");
	
	// update file
	char line[200], netType[10], tmp[100], whattime[100];
	gettime( whattime ); whattime[ strlen(whattime) -1] = 0;
	if (LEARNING) sprintf( netType, "Learn");
	else if (optimize) sprintf( netType, "Optimize");
	        else sprintf( netType, "Run");
	sprintf(line, "%s\t%s\t%s\t%s",  whattime, netType, baseNet, strchr( baseSaveDir+1, SEPARATEUR)+1);

	FILE *fo = fopen("logfile", "a");
	if (optimize && !LEARNING) {
		FILESTREAM ff("tmp/output");
		while (ff.eof() != EOF) {
			ff.getline( tmp, 100, '\n');
			if (tmp[0] != 0) fprintf( fo, "%s\t%s\n", line, tmp);
		}
	}
	else fprintf( fo, "%s\n", line);
}
