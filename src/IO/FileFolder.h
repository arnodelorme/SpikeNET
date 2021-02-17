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

// $Log: FileFolder.h,v $

#ifndef _FILEFOLDER
#define _FILEFOLDER

#define		MAXIMG		70
#define		HEADER_SIZE	50
#define 		MAXFILE		255
#define		MAXSIZE		5000000
#define		MAXREAD		150000

// structure 
// --------
// %d total file
// (Pos HEADER_SIZE)*nb files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class FileFolder
{
private:	
	// file info
	int     nbFile;				// number of files in the directory
	char  *fileName[MAXFILE];		// names of the files
	int	 fileEndPos[MAXFILE];		// offset of the files

	// data
	char 	*data;				// all datas
	char  *currentPtr;			// current pointer on the data

private:
	int strcasesame( char *a, char *b) {
		if (strlen(a) != strlen(b)) return 0;
		for (int i=0; i< strlen(a); i++)
			if (((a[i] >= 'A') && (a[i] <= 'Z')) && ((b[i] >= 'a') && (b[i] <= 'z'))) 
				if ((a[i] + 'a' - 'A' ) != b[i]) return 0; else;
			else   if (((b[i] >= 'A') && (b[i] <= 'Z')) && ((a[i] >= 'a') && (a[i] <= 'z'))) 
					if ((b[i] + 'a' - 'A' ) != a[i]) return 0; else;
				else if( a[i] != b[i]) return 0;
		return 1;
	}				

	int getIndex( char *fileName0) {
		for (int i=0; i<nbFile; i++)
			if (strcasesame( fileName[i], fileName0)) return i;
		return -1;
	}
	
	void extract( FILE *fo, int index) {
		if (!index) fwrite( data, 1, fileEndPos[ index ], fo);
		else		fwrite( data + fileEndPos[ index-1 ], 1, fileEndPos[ index ] -  fileEndPos[ index-1 ], fo);
	}	
	
	void extract( char *fileNameOut, int index) {
		FILE *fw = fopen(fileNameOut, "wb"); if (!fw) { printf("unable to open for writing %s\n", fileNameOut); exit(-1); }
		extract( fw, index);
		fclose( fw);
	}

	void encript( char *buffer, int size, int code) {
		for (int i=0; i < size; i++)	buffer[ i ] += code;
//		int *bufInt = (int *) buffer;
//		for (int i=0; i < (size >> 2); i++)	bufInt[ i ] += code;
	}
	
	void decript( char *buffer, int size, int code) {
		for (int i=0; i < size; i++)	buffer[ i ] -= code;
		//int *bufInt = (int *) buffer;
		//for (int i=0; i < (size >> 2); i++)	bufInt[ i ] -= code;
	}
	
public:
	
	FileFolder() {
		data = new char[MAXSIZE];
		currentPtr = data;
		nbFile = 0;
	}
	
	FileFolder( char *fileName0, int code = 0 ) {
		// getting data
		data = new char[MAXSIZE];
		FILE *fr = fopen(fileName0, "rb");
		if (!fr) { 
			currentPtr = data;
			nbFile = 0;
		}
		else {
			long fileSize = fread( data, 1, MAXSIZE, fr);
			if (fileSize == MAXSIZE)  { printf("Error, buffer exeeded\n"); exit(-1); }
			fclose( fr);
		
			// decript data
			FILE *ftmp = fopen("tmpfileName", "wb");
			if (code) decript( data, fileSize, code);
			fwrite( data, 1, fileSize, ftmp);
			fclose (ftmp);
			ftmp = fopen("tmpfileName", "rb");
		
			// read data
			char tmp[100];
			fscanf(ftmp, "%s", tmp);
			//printf( "nb of file is %s\n", tmp); 
			nbFile = atoi( tmp );
			for (int i=0; i<nbFile; i++) {
				fileName[ i ] = new char[255];
				fscanf(ftmp, "%s", tmp);
				fileEndPos[ i ] = atoi( tmp );
				fscanf(ftmp, "%s", fileName[ i ]);
			}
			getc( ftmp);
			long numgot = fread( data, 1, MAXSIZE, ftmp);
			currentPtr = data + numgot;
			fclose( ftmp );
			remove( "tmpfileName");
		}
	}
		
	int addFile( char *fileName0, char *fileNameToStore = NULL) {
		if (getIndex( fileName0 ) != -1) return -1;
		if (fileNameToStore == NULL) fileNameToStore = fileName0;
		// copy data
		FILE *fr = fopen(fileName0, "rb"); if (!fr) return -1;
		long numgot = fread(currentPtr, 1, MAXSIZE - (int)(currentPtr - data), fr);
		if (numgot == (int)(MAXSIZE - (int)(currentPtr - data)))  { printf("Error, buffer exeeded\n"); exit(-1); }
		currentPtr += numgot;
		fclose(fr);
		fileEndPos[nbFile] = (int)(currentPtr - data);
		// copy name
		fileName[nbFile] = new char[ strlen( fileNameToStore) +1 ];
		strcpy( fileName[nbFile++], fileNameToStore);
		return 0;
	}
	
	int addData( char *fileName0, char *buffer, int bufSize) {
		if (getIndex( fileName0 ) != -1) return -1;
		memcpy( currentPtr, buffer, bufSize);
		currentPtr += bufSize;
		// copy name
		fileName[nbFile] = new char[ strlen( fileName0) +1 ];
		strcpy( fileName[nbFile++], fileName0);
		return 0;
	}
	
	void flush( char *name, int code = 0) {
		// flush in buffer
		char *tmpBuf = new char[ 255 * nbFile ]; 
		char *ptrTmp = tmpBuf;
		sprintf( ptrTmp, "%d\n", nbFile); ptrTmp += strlen( ptrTmp);
		for (int i=0; i<nbFile; i++)
			{ sprintf( ptrTmp, "%d\t%s\n", fileEndPos[ i ], fileName[ i ]); ptrTmp += strlen( ptrTmp); }
		
		// concatenate buffers
		int textSize = (int)(ptrTmp - tmpBuf);
		for (int j=(int)(currentPtr - data)-1; j>=0; j--)
			data[ j + textSize ] = data[ j ];
		for (int k=0; k< textSize; k++) data[ k ] = tmpBuf[ k ];
		currentPtr +=  textSize;
		
		if (code) {
			encript( data      , (int)(currentPtr - data), code);
			//decript( data      , (int)(currentPtr - data), code);
		}
		
		// write file
		FILE *fw = fopen(name, "wb"); if (!fw) { printf("unable to open for writing %s\n", name); exit(-1); }
		fwrite(data, 1, (int)(currentPtr - data), fw);
		fclose( fw);
		delete [] tmpBuf;
		
		// restore buffer
		currentPtr -=  textSize;
		for (int l=0; l< (int)(currentPtr - data); l++) data[ l ] = data[ l + textSize ];		
	}
	
	int extract( char *fileName0, char *fileNameOut = NULL) {
		int index = getIndex( fileName0 );
		if (index == -1)  return -1;
		if (fileNameOut == NULL) fileNameOut = fileName0;
		extract( fileNameOut, index);
		return index;
	}
	
	FILE *extractTmp( char *fileName0) {
		int index = getIndex( fileName0 );
		if (index == -1) return NULL;
		char *tmpName = tmpnam("tmpfilename");
		FILE *f = fopen(tmpName, "w");
		extract( f, index);
		fclose(f);
		return fopen(tmpName, "r"); 
		/*FILE *f = tmpfile();
		extract( f, index);
		fseek( f, 0, 0);
		return f;*/
	}
	
	int extractAll() {
		for (int i=0; i<nbFile; i++)
			extract( fileName[i], i);
		return nbFile;
	}
};

#endif