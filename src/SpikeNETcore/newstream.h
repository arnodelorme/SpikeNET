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

// $Log: newstream.h,v $


#ifndef _NEWSTREAM
#define _NEWSTREAM
//#include "allDefs.h"
//#include "FileFolder.h"
#include "operatingsystem.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ERROR
#define ERROR(X, Y)		{ printf("SpikeError %d: ", X); printf Y; printf("\n"); exit(-1); }
#endif

#define STRSTREAM	istrngstream
#define FILESTREAM 	ifilestream

#if SYSTEME == MAC
#define SEPARATEUR     ':'
#define SEPARATEURSTR  ":"
#define CURRENT_DIR    ":"
#endif
#if SYSTEME == UNIX
#define SEPARATEUR     '/'
#define SEPARATEURSTR  "/"
#define CURRENT_DIR    "./"
#endif
#if SYSTEME == PC
#define SEPARATEUR     '\\'
#define SEPARATEURSTR  "\\"
#define CURRENT_DIR    ".\\"
#define rint(X)		   floor(X+0.5) 
// strictly equivalent to unix rint 1; 1000000 run on rand pos and neg nb => same result
#endif

class basestream
{
public:
	virtual void operator>>(char *dest) = 0;
	virtual int operator!() = 0;
	virtual int get() = 0;
	virtual int eof() = 0;
	virtual int getline(char *buffer, int length, char delimiter) = 0;

    static char *conform(char *fileName)
    {
		char *tmp = new char[ strlen( fileName ) + 10];
		if (fileName[0] != SEPARATEUR)
		{
        	strcpy(tmp, CURRENT_DIR);
			strcat(tmp, fileName);
			char *ptrtmp = tmp;
                
        	// change separator depending on computer type
         	while ((ptrtmp = strpbrk(ptrtmp, "/")) != NULL)
             	*(ptrtmp++) = SEPARATEUR;
		}
		else strcpy(tmp, fileName);
    	return tmp;
    }
};

class ifilestream : public basestream
{
//	static FileFolder *testing;
	FILE *f;
	char c;

public:

	ifilestream(char *fileName)
	{		
		char tmp[100], *ptrtmp;
		if (fileName[0] != SEPARATEUR)
		{
        	strcpy(tmp, CURRENT_DIR);
			strcat(tmp, fileName);
			ptrtmp = tmp;
                
        	// change separator depending on computer type
         	while ((ptrtmp = strpbrk(ptrtmp, "/")) != NULL)
             	*(ptrtmp++) = SEPARATEUR;
		}
		else strcpy(tmp, fileName);
		//testing->addFile(  fileName);
		//testing->flush("toto");
		//if (testing != NULL) if ((f = testing->extractTmp( fileName)) != NULL) printf("loading file %s\n", tmp);
		//else {	f = fopen(tmp,"r"); } 
		//else 
		{	f = fopen(tmp,"r"); } 
		
		
		//testing->addFile(  fileName); printf("adding file %s\n", tmp); testing->flush(ARCHIVE_NAME); }
		if (!f) ERROR(2, ("unable to open %s", tmp));
	};
	
	//void operator>>(const filestream &fs, char *dest)
	void operator>>(char *dest)
	{
		c = (char)fscanf(f, "%s", dest);
	};
	
	int operator!()
	{
		return (f == NULL);
	};
	
	int get()
	{
		return c = (char)fgetc(f);
	};
	
	int eof()
	{ return c; }

	int getline(char *buffer, int length, char delimiter)
	{
		int cmpt=0;
		do
		{
			c=(char)fgetc(f);
			
			if ((cmpt < (length-1)) && (c!=delimiter) && (c!=EOF)) buffer[cmpt++] = c;
		}
		while ((cmpt < (length-1)) && (c!=delimiter) && (c!=EOF));
		//if (c == delimiter) buffer[cmpt-1] = 0;
		buffer[cmpt] = 0;
		return c;
	}
	~ifilestream()
	{
		fclose(f);
	};
};

class istrngstream : public basestream
{
	char *ptr;
	char *ptrcourant;
	int  len;
	
public:
	istrngstream(char *charstring)
	{
		ptr        = charstring;
		ptrcourant = charstring;
		len = (int)strlen(charstring);
	};
	
	//void operator>>(const filestream &fs, char *dest)
	void operator>>(char *dest)
	{
		int cmpt = 0;
		while ((*ptrcourant == ' ') || (*ptrcourant == '\t') || (*ptrcourant =='\n')) ptrcourant++;
		if ((ptrcourant-ptr) < len)
				while ((*ptrcourant != ' ') && (*ptrcourant != '\t') && (*ptrcourant !='\n'))
					if ((ptrcourant-ptr) <= len) dest[cmpt++] = *ptrcourant++; else break;	
		dest[cmpt] = 0;
		//c = sscanf("%s", 
	};

	int operator!()
	{
		return (ptr == NULL);
	};
	
	int get()
	{
		if ((ptrcourant-ptr) >= len) return EOF;
		else return *ptrcourant++;
	};
	
	int eof()
	{
		if ((ptrcourant-ptr) >= len) return EOF; else return -EOF;
	}
	
	int getline(char *buffer, int length, char delimiter)
	{
		int cmpt=0;
		while ((ptrcourant[cmpt] != delimiter) && ((ptrcourant+cmpt-ptr) < len)) buffer[cmpt++] = ptrcourant[cmpt];
		buffer[cmpt] = 0;
		ptrcourant += cmpt;
		if ((ptrcourant-ptr) >= length) return EOF;
		else return *ptrcourant++;
	}
	
	~istrngstream()
	{
	};
};


#endif	






