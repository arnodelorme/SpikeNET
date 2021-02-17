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

// $Log: makenet.cc,v $

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "allDefs.h"
#include "RandomRank.h"

//functions
void makeFaceLearn();		// build list_img file for learn with contrast or not
void statRank();			// rank statistic on random distribution of weights
void splitLstImg();			// split lst_img in two : odd and even images
void treatLstImg();			// treat a file : concatenate every 32 lines
void makeNet();				// build net_names and net_env
void makeNetLearn64();		// build net_names and net_env
void makeNetLearnMulti16();
void treatLearn();			// treat outputfile : concatenate every 440 lines
void makeFaceTest();
void makeFaceMultiContrast();
void makeFaceLearnMultiSize();
void choose();
void treatTest();

class Interpretor
{
	char *test;
public:
	void treatLineStr( char *lineur )
	{
		istrngstream	f(lineur);
		char tmp[100];
		
		while (f.eof() !=  EOF)
		{
			f.getline(tmp, 100, '\n');
			printf("%s\n", tmp);
		}
	};
	void salut() { printf("salut"); }
};

int main( int argc, char *argv[])
{
	printf("%d %d %d %d\n", 'a', 'z', 'A', 'Z');
	//Interpretor *interp = new Interpretor();
	
	//interp->treatLineStr("Salut comment ca va");
	//interp->salut();
	//makeNetLearnMulti16();
	//treatTest();
	//treatLearn();
	//makeFaceMultiContrast();
	//choose();
}

char *tabLearn[] = {
"Images/orl_faces/s1-10.pgm s1 56 71",
"Images/orl_faces/s1-3.pgm s1 44 67",
"Images/orl_faces/s1-4.pgm s1 34 63",
"Images/orl_faces/s1-5.pgm s1 58 63",
"Images/orl_faces/s1-6.pgm s1 31 65",
"Images/orl_faces/s1-7.pgm s1 43 62",
"Images/orl_faces/s1-8.pgm s1 47 64",
"Images/orl_faces/s1-9.pgm s1 50 53",
"Images/orl_faces/s10-10.pgm s10 46 69",
"Images/orl_faces/s10-3.pgm s10 52 80",
"Images/orl_faces/s10-4.pgm s10 54 79",
"Images/orl_faces/s10-5.pgm s10 43 78",
"Images/orl_faces/s10-6.pgm s10 49 80",
"Images/orl_faces/s10-7.pgm s10 54 82",
"Images/orl_faces/s10-8.pgm s10 44 82",
"Images/orl_faces/s10-9.pgm s10 29 79",
"Images/orl_faces/s11-10.pgm s11 44 66",
"Images/orl_faces/s11-3.pgm s11 41 66",
"Images/orl_faces/s11-4.pgm s11 44 62",
"Images/orl_faces/s11-5.pgm s11 47 59",
"Images/orl_faces/s11-6.pgm s11 41 67",
"Images/orl_faces/s11-7.pgm s11 46 65",
"Images/orl_faces/s11-8.pgm s11 50 64",
"Images/orl_faces/s11-9.pgm s11 45 64",
"Images/orl_faces/s12-10.pgm s12 46 72",
"Images/orl_faces/s12-3.pgm s12 39 72",
"Images/orl_faces/s12-4.pgm s12 44 73",
"Images/orl_faces/s12-5.pgm s12 46 75",
"Images/orl_faces/s12-6.pgm s12 45 70",
"Images/orl_faces/s12-7.pgm s12 47 72",
"Images/orl_faces/s12-8.pgm s12 46 72",
"Images/orl_faces/s12-9.pgm s12 47 73",
"Images/orl_faces/s13-10.pgm s13 40 69",
"Images/orl_faces/s13-3.pgm s13 35 66",
"Images/orl_faces/s13-4.pgm s13 36 68",
"Images/orl_faces/s13-5.pgm s13 42 69",
"Images/orl_faces/s13-6.pgm s13 56 70",
"Images/orl_faces/s13-7.pgm s13 32 70",
"Images/orl_faces/s13-8.pgm s13 35 70",
"Images/orl_faces/s13-9.pgm s13 48 70",
"Images/orl_faces/s14-10.pgm s14 48 57",
"Images/orl_faces/s14-3.pgm s14 50 55",
"Images/orl_faces/s14-4.pgm s14 41 55",
"Images/orl_faces/s14-5.pgm s14 40 56",
"Images/orl_faces/s14-6.pgm s14 50 60",
"Images/orl_faces/s14-7.pgm s14 40 61",
"Images/orl_faces/s14-8.pgm s14 39 60",
"Images/orl_faces/s14-9.pgm s14 50 61",
"Images/orl_faces/s15-10.pgm s15 43 68",
"Images/orl_faces/s15-3.pgm s15 47 61",
"Images/orl_faces/s15-4.pgm s15 48 62",
"Images/orl_faces/s15-5.pgm s15 41 67",
"Images/orl_faces/s15-6.pgm s15 46 60",
"Images/orl_faces/s15-7.pgm s15 43 67",
"Images/orl_faces/s15-8.pgm s15 45 66",
"Images/orl_faces/s15-9.pgm s15 43 65",
"Images/orl_faces/s16-10.pgm s16 43 72",
"Images/orl_faces/s16-3.pgm s16 43 70",
"Images/orl_faces/s16-4.pgm s16 46 62",
"Images/orl_faces/s16-5.pgm s16 47 62",
"Images/orl_faces/s16-6.pgm s16 48 63",
"Images/orl_faces/s16-7.pgm s16 55 69",
"Images/orl_faces/s16-8.pgm s16 31 65",
"Images/orl_faces/s16-9.pgm s16 43 59",
"Images/orl_faces/s17-10.pgm s17 30 75",
"Images/orl_faces/s17-3.pgm s17 47 73",
"Images/orl_faces/s17-4.pgm s17 46 73",
"Images/orl_faces/s17-5.pgm s17 49 70",
"Images/orl_faces/s17-6.pgm s17 36 75",
"Images/orl_faces/s17-7.pgm s17 34 74",
"Images/orl_faces/s17-8.pgm s17 44 76",
"Images/orl_faces/s17-9.pgm s17 38 74",
"Images/orl_faces/s18-10.pgm s18 40 71",
"Images/orl_faces/s18-3.pgm s18 36 68",
"Images/orl_faces/s18-4.pgm s18 51 67",
"Images/orl_faces/s18-5.pgm s18 45 69",
"Images/orl_faces/s18-6.pgm s18 42 68",
"Images/orl_faces/s18-7.pgm s18 49 67",
"Images/orl_faces/s18-8.pgm s18 41 68",
"Images/orl_faces/s18-9.pgm s18 49 68",
"Images/orl_faces/s19-10.pgm s19 40 63",
"Images/orl_faces/s19-3.pgm s19 41 62",
"Images/orl_faces/s19-4.pgm s19 46 60",
"Images/orl_faces/s19-5.pgm s19 47 61",
"Images/orl_faces/s19-6.pgm s19 41 60",
"Images/orl_faces/s19-7.pgm s19 43 59",
"Images/orl_faces/s19-8.pgm s19 46 58",
"Images/orl_faces/s19-9.pgm s19 48 63",
"Images/orl_faces/s2-10.pgm s2 44 68",
"Images/orl_faces/s2-3.pgm s2 45 67",
"Images/orl_faces/s2-4.pgm s2 49 65",
"Images/orl_faces/s2-5.pgm s2 46 65",
"Images/orl_faces/s2-6.pgm s2 49 68",
"Images/orl_faces/s2-7.pgm s2 45 68",
"Images/orl_faces/s2-8.pgm s2 47 68",
"Images/orl_faces/s2-9.pgm s2 49 65",
"Images/orl_faces/s20-10.pgm s20 52 69",
"Images/orl_faces/s20-3.pgm s20 48 69",
"Images/orl_faces/s20-4.pgm s20 49 64",
"Images/orl_faces/s20-5.pgm s20 48 67",
"Images/orl_faces/s20-6.pgm s20 49 63",
"Images/orl_faces/s20-7.pgm s20 47 68",
"Images/orl_faces/s20-8.pgm s20 46 71",
"Images/orl_faces/s20-9.pgm s20 47 71",
"Images/orl_faces/s21-10.pgm s21 40 71",
"Images/orl_faces/s21-3.pgm s21 42 72",
"Images/orl_faces/s21-4.pgm s21 47 72",
"Images/orl_faces/s21-5.pgm s21 51 73",
"Images/orl_faces/s21-6.pgm s21 55 71",
"Images/orl_faces/s21-7.pgm s21 39 74",
"Images/orl_faces/s21-8.pgm s21 40 71",
"Images/orl_faces/s21-9.pgm s21 50 71",
"Images/orl_faces/s22-10.pgm s22 41 64",
"Images/orl_faces/s22-3.pgm s22 47 68",
"Images/orl_faces/s22-4.pgm s22 46 59",
"Images/orl_faces/s22-5.pgm s22 45 66",
"Images/orl_faces/s22-6.pgm s22 38 67",
"Images/orl_faces/s22-7.pgm s22 50 67",
"Images/orl_faces/s22-8.pgm s22 47 67",
"Images/orl_faces/s22-9.pgm s22 45 65",
"Images/orl_faces/s23-10.pgm s23 38 72",
"Images/orl_faces/s23-3.pgm s23 49 75",
"Images/orl_faces/s23-4.pgm s23 31 74",
"Images/orl_faces/s23-5.pgm s23 53 75",
"Images/orl_faces/s23-6.pgm s23 32 73",
"Images/orl_faces/s23-7.pgm s23 40 74",
"Images/orl_faces/s23-8.pgm s23 57 73",
"Images/orl_faces/s23-9.pgm s23 30 73",
"Images/orl_faces/s24-10.pgm s24 44 66",
"Images/orl_faces/s24-3.pgm s24 35 68",
"Images/orl_faces/s24-4.pgm s24 44 62",
"Images/orl_faces/s24-5.pgm s24 47 63",
"Images/orl_faces/s24-6.pgm s24 42 65",
"Images/orl_faces/s24-7.pgm s24 42 62",
"Images/orl_faces/s24-8.pgm s24 42 71",
"Images/orl_faces/s24-9.pgm s24 46 68",
"Images/orl_faces/s25-10.pgm s25 50 72",
"Images/orl_faces/s25-3.pgm s25 46 72",
"Images/orl_faces/s25-4.pgm s25 32 70",
"Images/orl_faces/s25-5.pgm s25 25 70",
"Images/orl_faces/s25-6.pgm s25 46 71",
"Images/orl_faces/s25-7.pgm s25 41 71",
"Images/orl_faces/s25-8.pgm s25 28 71",
"Images/orl_faces/s25-9.pgm s25 40 71",
"Images/orl_faces/s26-10.pgm s26 63 72",
"Images/orl_faces/s26-3.pgm s26 45 74",
"Images/orl_faces/s26-4.pgm s26 62 70",
"Images/orl_faces/s26-5.pgm s26 65 68",
"Images/orl_faces/s26-6.pgm s26 43 71",
"Images/orl_faces/s26-7.pgm s26 40 62",
"Images/orl_faces/s26-8.pgm s26 60 70",
"Images/orl_faces/s26-9.pgm s26 47 71",
"Images/orl_faces/s27-10.pgm s27 41 67",
"Images/orl_faces/s27-3.pgm s27 44 67",
"Images/orl_faces/s27-4.pgm s27 44 68",
"Images/orl_faces/s27-5.pgm s27 44 68",
"Images/orl_faces/s27-6.pgm s27 45 72",
"Images/orl_faces/s27-7.pgm s27 41 71",
"Images/orl_faces/s27-8.pgm s27 43 70",
"Images/orl_faces/s27-9.pgm s27 43 70",
"Images/orl_faces/s28-10.pgm s28 29 62",
"Images/orl_faces/s28-3.pgm s28 36 63",
"Images/orl_faces/s28-4.pgm s28 37 60",
"Images/orl_faces/s28-5.pgm s28 47 62",
"Images/orl_faces/s28-6.pgm s28 43 65",
"Images/orl_faces/s28-7.pgm s28 47 65",
"Images/orl_faces/s28-8.pgm s28 58 65",
"Images/orl_faces/s28-9.pgm s28 54 68",
"Images/orl_faces/s29-10.pgm s29 42 67",
"Images/orl_faces/s29-3.pgm s29 39 72",
"Images/orl_faces/s29-4.pgm s29 45 71",
"Images/orl_faces/s29-5.pgm s29 38 67",
"Images/orl_faces/s29-6.pgm s29 40 67",
"Images/orl_faces/s29-7.pgm s29 42 70",
"Images/orl_faces/s29-8.pgm s29 32 71",
"Images/orl_faces/s29-9.pgm s29 26 73",
"Images/orl_faces/s3-10.pgm s3 52 69",
"Images/orl_faces/s3-3.pgm s3 55 71",
"Images/orl_faces/s3-4.pgm s3 32 71",
"Images/orl_faces/s3-5.pgm s3 30 69",
"Images/orl_faces/s3-6.pgm s3 41 72",
"Images/orl_faces/s3-7.pgm s3 36 71",
"Images/orl_faces/s3-8.pgm s3 37 72",
"Images/orl_faces/s3-9.pgm s3 48 71",
"Images/orl_faces/s30-10.pgm s30 45 71",
"Images/orl_faces/s30-3.pgm s30 50 73",
"Images/orl_faces/s30-4.pgm s30 36 74",
"Images/orl_faces/s30-5.pgm s30 31 72",
"Images/orl_faces/s30-6.pgm s30 45 71",
"Images/orl_faces/s30-7.pgm s30 42 74",
"Images/orl_faces/s30-8.pgm s30 42 72",
"Images/orl_faces/s30-9.pgm s30 46 72",
"Images/orl_faces/s31-10.pgm s31 49 64",
"Images/orl_faces/s31-3.pgm s31 46 63",
"Images/orl_faces/s31-4.pgm s31 46 64",
"Images/orl_faces/s31-5.pgm s31 47 64",
"Images/orl_faces/s31-6.pgm s31 40 70",
"Images/orl_faces/s31-7.pgm s31 44 73",
"Images/orl_faces/s31-8.pgm s31 42 71",
"Images/orl_faces/s31-9.pgm s31 44 74",
"Images/orl_faces/s32-10.pgm s32 44 63",
"Images/orl_faces/s32-3.pgm s32 44 63",
"Images/orl_faces/s32-4.pgm s32 43 57",
"Images/orl_faces/s32-5.pgm s32 50 64",
"Images/orl_faces/s32-6.pgm s32 48 63",
"Images/orl_faces/s32-7.pgm s32 60 67",
"Images/orl_faces/s32-8.pgm s32 40 63",
"Images/orl_faces/s32-9.pgm s32 46 66",
"Images/orl_faces/s33-10.pgm s33 44 71",
"Images/orl_faces/s33-3.pgm s33 46 71",
"Images/orl_faces/s33-4.pgm s33 44 72",
"Images/orl_faces/s33-5.pgm s33 50 67",
"Images/orl_faces/s33-6.pgm s33 43 66",
"Images/orl_faces/s33-7.pgm s33 46 69",
"Images/orl_faces/s33-8.pgm s33 44 70",
"Images/orl_faces/s33-9.pgm s33 46 67",
"Images/orl_faces/s34-10.pgm s34 46 63",
"Images/orl_faces/s34-3.pgm s34 47 61",
"Images/orl_faces/s34-4.pgm s34 39 64",
"Images/orl_faces/s34-5.pgm s34 40 63",
"Images/orl_faces/s34-6.pgm s34 45 62",
"Images/orl_faces/s34-7.pgm s34 36 65",
"Images/orl_faces/s34-8.pgm s34 45 64",
"Images/orl_faces/s34-9.pgm s34 47 64",
"Images/orl_faces/s35-10.pgm s35 37 69",
"Images/orl_faces/s35-3.pgm s35 36 67",
"Images/orl_faces/s35-4.pgm s35 43 70",
"Images/orl_faces/s35-5.pgm s35 34 67",
"Images/orl_faces/s35-6.pgm s35 47 71",
"Images/orl_faces/s35-7.pgm s35 48 68",
"Images/orl_faces/s35-8.pgm s35 46 72",
"Images/orl_faces/s35-9.pgm s35 34 63",
"Images/orl_faces/s36-10.pgm s36 43 62",
"Images/orl_faces/s36-3.pgm s36 43 67",
"Images/orl_faces/s36-4.pgm s36 47 67",
"Images/orl_faces/s36-5.pgm s36 45 67",
"Images/orl_faces/s36-6.pgm s36 36 63",
"Images/orl_faces/s36-7.pgm s36 35 64",
"Images/orl_faces/s36-8.pgm s36 44 66",
"Images/orl_faces/s36-9.pgm s36 46 67",
"Images/orl_faces/s37-10.pgm s37 45 63",
"Images/orl_faces/s37-3.pgm s37 46 64",
"Images/orl_faces/s37-4.pgm s37 47 61",
"Images/orl_faces/s37-5.pgm s37 48 60",
"Images/orl_faces/s37-6.pgm s37 49 61",
"Images/orl_faces/s37-7.pgm s37 47 62",
"Images/orl_faces/s37-8.pgm s37 49 65",
"Images/orl_faces/s37-9.pgm s37 46 65",
"Images/orl_faces/s38-10.pgm s38 43 73",
"Images/orl_faces/s38-3.pgm s38 40 72",
"Images/orl_faces/s38-4.pgm s38 45 70",
"Images/orl_faces/s38-5.pgm s38 43 72",
"Images/orl_faces/s38-6.pgm s38 36 73",
"Images/orl_faces/s38-7.pgm s38 45 71",
"Images/orl_faces/s38-8.pgm s38 49 74",
"Images/orl_faces/s38-9.pgm s38 32 70",
"Images/orl_faces/s39-10.pgm s39 36 70",
"Images/orl_faces/s39-3.pgm s39 30 62",
"Images/orl_faces/s39-4.pgm s39 36 62",
"Images/orl_faces/s39-5.pgm s39 43 69",
"Images/orl_faces/s39-6.pgm s39 29 70",
"Images/orl_faces/s39-7.pgm s39 54 71",
"Images/orl_faces/s39-8.pgm s39 55 69",
"Images/orl_faces/s39-9.pgm s39 43 69",
"Images/orl_faces/s4-10.pgm s4 64 70",
"Images/orl_faces/s4-3.pgm s4 35 69",
"Images/orl_faces/s4-4.pgm s4 49 70",
"Images/orl_faces/s4-5.pgm s4 56 70",
"Images/orl_faces/s4-6.pgm s4 30 70",
"Images/orl_faces/s4-7.pgm s4 45 70",
"Images/orl_faces/s4-8.pgm s4 38 69",
"Images/orl_faces/s4-9.pgm s4 49 72",
"Images/orl_faces/s40-10.pgm s40 32 73",
"Images/orl_faces/s40-3.pgm s40 53 71",
"Images/orl_faces/s40-4.pgm s40 28 71",
"Images/orl_faces/s40-5.pgm s40 40 69",
"Images/orl_faces/s40-6.pgm s40 47 75",
"Images/orl_faces/s40-7.pgm s40 29 67",
"Images/orl_faces/s40-8.pgm s40 51 73",
"Images/orl_faces/s40-9.pgm s40 31 68",
"Images/orl_faces/s5-10.pgm s5 39 68",
"Images/orl_faces/s5-3.pgm s5 49 71",
"Images/orl_faces/s5-4.pgm s5 31 70",
"Images/orl_faces/s5-5.pgm s5 30 72",
"Images/orl_faces/s5-6.pgm s5 32 72",
"Images/orl_faces/s5-7.pgm s5 28 70",
"Images/orl_faces/s5-8.pgm s5 39 75",
"Images/orl_faces/s5-9.pgm s5 40 74",
"Images/orl_faces/s6-10.pgm s6 42 75",
"Images/orl_faces/s6-3.pgm s6 47 71",
"Images/orl_faces/s6-4.pgm s6 43 77",
"Images/orl_faces/s6-5.pgm s6 39 73",
"Images/orl_faces/s6-6.pgm s6 45 73",
"Images/orl_faces/s6-7.pgm s6 36 74",
"Images/orl_faces/s6-8.pgm s6 53 75",
"Images/orl_faces/s6-9.pgm s6 28 76",
"Images/orl_faces/s7-10.pgm s7 46 65",
"Images/orl_faces/s7-3.pgm s7 46 67",
"Images/orl_faces/s7-4.pgm s7 47 68",
"Images/orl_faces/s7-5.pgm s7 48 66",
"Images/orl_faces/s7-6.pgm s7 45 65",
"Images/orl_faces/s7-7.pgm s7 46 68",
"Images/orl_faces/s7-8.pgm s7 45 69",
"Images/orl_faces/s7-9.pgm s7 46 65",
"Images/orl_faces/s8-10.pgm s8 52 68",
"Images/orl_faces/s8-3.pgm s8 45 68",
"Images/orl_faces/s8-4.pgm s8 53 66",
"Images/orl_faces/s8-5.pgm s8 44 66",
"Images/orl_faces/s8-6.pgm s8 45 66",
"Images/orl_faces/s8-7.pgm s8 46 68",
"Images/orl_faces/s8-8.pgm s8 44 67",
"Images/orl_faces/s8-9.pgm s8 37 66",
"Images/orl_faces/s9-10.pgm s9 57 72",
"Images/orl_faces/s9-3.pgm s9 52 75",
"Images/orl_faces/s9-4.pgm s9 45 75",
"Images/orl_faces/s9-5.pgm s9 52 75",
"Images/orl_faces/s9-6.pgm s9 34 75",
"Images/orl_faces/s9-7.pgm s9 31 74",
"Images/orl_faces/s9-8.pgm s9 42 73",
"Images/orl_faces/s9-9.pgm s9 46 76" };

// **********************************************************************
// **********************************************************************

/*void choose()
{
	FILE *fo1 = fopen(":choose", "w");
	for (int ii = 0; ii<40; ii++)
	{
		int choosed = (int)((double)rand()/(RAND_MAX)*3) + ii * 8;
		fprintf( fo1, "Image %s\n", tabLearn[ choosed ]);
		choosed = (int)((double)rand()/(RAND_MAX)*3) + ii * 8 + 3;
		fprintf( fo1, "Image %s\n", tabLearn[ choosed ]);
		choosed = (int)((double)rand()/(RAND_MAX)*2) + ii * 8 + 6;
		fprintf( fo1, "Image %s\n", tabLearn[ choosed ]);
	}
	fclose( fo1 );
}*/

void makeFaceLearn()
{

	FILE *fo1 = fopen(":Learn1", "w");
	FILE *fo2 = fopen(":Learn2", "w");
	for (int ii = 0; ii<320; ii++)
	{
		int res1 = rand()/(RAND_MAX/4);
		int res2;
		while( (res2 = rand()/(RAND_MAX/4)) == res1);
		int res3 = -1, res4;
		
		for (int u=0; u<4; u++)
			if ((u != res1) && (u != res2))
				if ((res3 != -1) && (u != res3)) res4 = u;
				else res3 = u;
		
		switch( res1 )
		{
			case 0 : fprintf( fo1, "Image %s\n", tabLearn[ ii ]); break;
			case 1 : fprintf( fo1, "Image %s Contrast 0.5\n", tabLearn[ ii ]); break;
			case 2 : fprintf( fo1, "Image %s Contrast 0.5 Luminance 64\n", tabLearn[ ii ]); break;
			case 3 : fprintf( fo1, "Image %s Contrast 0.5 Luminance -64\n", tabLearn[ ii ]); break;
			default : { printf("error\n"); exit(-1); }
		}
		switch( res2 )
		{
			case 0 : fprintf( fo1, "Image %s\n", tabLearn[ ii ]); break;
			case 1 : fprintf( fo1, "Image %s Contrast 0.5\n", tabLearn[ ii ]); break;
			case 2 : fprintf( fo1, "Image %s Contrast 0.5 Luminance 64\n", tabLearn[ ii ]); break;
			case 3 : fprintf( fo1, "Image %s Contrast 0.5 Luminance -64\n", tabLearn[ ii ]); break;
			default : { printf("error\n"); exit(-1); }
		}
		switch( res3 )
		{
			case 0 : fprintf( fo2, "Image %s\n", tabLearn[ ii ]); break;
			case 1 : fprintf( fo2, "Image %s Contrast 0.5\n", tabLearn[ ii ]); break;
			case 2 : fprintf( fo2, "Image %s Contrast 0.5 Luminance 64\n", tabLearn[ ii ]); break;
			case 3 : fprintf( fo2, "Image %s Contrast 0.5 Luminance -64\n", tabLearn[ ii ]); break;
			default : { printf("error\n"); exit(-1); }
		}
		switch( res4 )
		{
			case 0 : fprintf( fo2, "Image %s\n", tabLearn[ ii ]); break;
			case 1 : fprintf( fo2, "Image %s Contrast 0.5\n", tabLearn[ ii ]); break;
			case 2 : fprintf( fo2, "Image %s Contrast 0.5 Luminance 64\n", tabLearn[ ii ]); break;
			case 3 : fprintf( fo2, "Image %s Contrast 0.5 Luminance -64\n", tabLearn[ ii ]); break;
			default : { printf("error\n"); exit(-1); }
		}
	}
	fclose(fo1);	
	fclose(fo2);
}

void makeFaceLearnMultiSize()
{
	FILE *fo1 = fopen(":Learn1", "w");
	int maxSizex = (125 * 92)/100;
	int maxSizey = (125 * 112)/100;
	
	for (int ii = 0; ii<320; ii++)
	{
		/*int res1 = rand()/(RAND_MAX/4);
		int res2;
		while( (res2 = rand()/(RAND_MAX/4)) == res1);
		int res3 = -1, res4;
		
		for (int u=0; u<4; u++)
			if ((u != res1) && (u != res2))
				if ((res3 != -1) && (u != res3)) res4 = u;
				else res3 = u;*/
		char *tmp2 = strpbrk( tabLearn[ ii ], " "); *tmp2 =0; tmp2++;
		char *tmp3 = strpbrk( tmp2, " ");*tmp3 =0; tmp3++;
		char *tmp4 = strpbrk( tmp3, " ");*tmp4 =0; tmp4++;
		
		for (int tt=75; tt<130; tt+=5)
		{
			int newSizex = (tt * 92)/100;
			int newSizey = (tt * 112)/100;
			int newCoordx = rint((double)(atoi(tmp3) * tt)/100.0);
			int newCoordy = rint((double)(atoi(tmp4) * tt)/100.0);
			
			fprintf( fo1, "Image %s\tResize\t%dx%d\t\t\tPut\t%dx%d-0x0\tLearn\t%s\t%d\t%d\n", tabLearn[ ii ], newSizex, newSizey, maxSizex, maxSizey, tmp2, newCoordx, newCoordy);		
			//fprintf( fo1, "Image %s\tResize\t%d\t%d\tContrast\t0.25\tPut\t%d\t%d\tLearn\t%s\t%d\t%d\n", tabLearn[ ii ], newSizex, newSizey, maxSizex, maxSizey, tmp2,  newCoordx, newCoordy);		
			//fprintf( fo1, "Image %s\tResize\t%d\t%d\tContrast\t0.10\tPut\t%d\t%d\tLearn\t%s\t%d\t%d\n", tabLearn[ ii ], newSizex, newSizey, maxSizex, maxSizey, tmp2,  newCoordx, newCoordy);			
		}
	}
	fclose(fo1);
}

void makeFaceMultiContrast()
{
	FILE *fo1 = fopen(":Learn1", "w");
	int ii;
	//double contrast[] = { 0.5, 0.4, 0.3, 0.1, 0.05, 0.04, 0.03, 0.025, 0.022, 0.02, 0.019, 0.018, 0.017, 0.016, 0.015, 0.014, 0.013, 0.012, 0.011, 0.01, 0.009, 0.008, 0.007, 0.006, 0.005 };
	//double noise[] = {  0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1, 1.05, 1.1, 1.15, 1.2, 1.25, 1.3 };
	double noise[] = { 0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1 };
	
	//for (int y = 0; y<25; y++)
		for ( ii = 0; ii<320; ii++)
		{
			 *(strpbrk( tabLearn[ ii ], " ")) = 0;
			//fprintf( fo1, "Image\t%s\tContrast\t%1.3f\n", tabLearn[ ii ], contrast[ y ]);		
		}
		
	for (int yy = 0; yy<21; yy++)
		for ( ii = 0; ii<320; ii++)
			fprintf( fo1, "Image\t%s\tRandom\t%1.3f\n", tabLearn[ ii ], noise[ yy ]);		

	fclose(fo1);
}

// **********************************************************************
// **********************************************************************

void statRank()
{
	FORMAT_POIDS weight[100];
	
	for (int i=0; i< 100; i++)
		weight[i] = (double)rand()/RAND_MAX;
	
		FILE *f = fopen(":tmp", "w");
		RandomRank rnd( 100 );
		for (int j=0; j<10; j++)
		{
			 int posM = 0, posW = 0;
			 double sum1 =0, sum2 = 0;
			 rnd.reInit();
			 while( (posM = rnd.getRank()) != -1)
			 {
			 	//sum1 += weight[ posW++ ] * exp( posM++ * log( 0.9747 ) );
			 	sum2 += weight[ posM++ ] * (1000 - sum2) / 100;
			 }
			 	
			 printf( "%f\n", sum2);
			 fprintf( f, "%.0f\n", 100000 * sum2);
		}
		fclose( f);
}

// **********************************************************************
// **********************************************************************

void splitLstImg()
{
	char fileName[100], tmp[100];
	char *tabStr[100];
	int eof;
	
	FILE *outp1 = fopen( ":allres1", "w");
	FILE *outp2 = fopen( ":allres2", "w");
	FILESTREAM f(":lst_img");
	for (int j=0; j< 400; j++)
		{
			f.getline(tmp, 100, '\n');
			if (j % 2) fprintf(outp1, "%s\n", tmp);
			else       fprintf(outp2, "%s\n", tmp);
		}
	
	fclose( outp1);
	fclose( outp2);
}

void treatLstImg()
{
	char fileName[100], tmp[100];
	char *tabStr[100];
	int eof;
	
	for (int i=1; i<32; i++)
	{
		sprintf(fileName, ":lst_img%d", i);
		FILESTREAM f(fileName);
		for (int j=0; j< 100; j++)
			{
				f >> tmp;
				if (i == 1) 
				{
					tabStr[j] = new char[10000];
					strcpy( tabStr[j], tmp);
				}
				else		strcat( tabStr[j], tmp);
				strcat( tabStr[j], "\t");
				f.getline( tmp, 100, '\n');
			}
	}
	
	FILE *outp = fopen( ":allres", "w");
	for (int j=0; j< 100; j++)
		fprintf( outp, "%s\n", tabStr[j]);
	fclose( outp);
}

// **********************************************************************
// **********************************************************************

void makeNetLearn64()
{
	FILE *f1 = fopen("net_names", "w");
	FILE *f2 = fopen("net_projs", "w");
	int nbOfMaps = 64;
	
	fprintf( f1, "Name	ON	Layer	0	Threshold	0.5	Zoom	0	Estim	1	Pos	10	100	0	Input	(Convo	Convolutions/OnCenter	Image1	-10000)\nName	OFF	Layer	0	Threshold	0.5	Zoom	0	Estim	1	Pos	10	200	0	Input	(Convo	Convolutions/OffCenter	Image1	-10000)\n\n");

	for (int i=0; i<nbOfMaps; i++)
	{
		fprintf( f1, "Name ALEARN%d Layer 1 Zoom	0	Threshold	0.18	MOD \"networkUnsuperv/modReal\"	Conv	100	Estim	0\n", i+1);
		fprintf( f2, "ON  ALEARN%d	FILE	UNLOCK	GROUP RET RAND	0.5	RENORM	1000	Convosnew/newGauss1\n", i+1);
		fprintf( f2, "OFF ALEARN%d	FILE	UNLOCK	GROUP RET RAND	0.5	RENORM	1000	Convosnew/newGauss1\n", i+1);
	}
	fprintf( f2 , "\n");
	fclose(f1);
	
	for (int j=0; j<nbOfMaps; j++)
	{
		for (int k=0; k<nbOfMaps; k++)
			fprintf( f2, "ALEARN%d	ALEARN%d	FILE	LOCK	GROUP	INHIB MOD 1 MULT 2	Convosnew/gauss11inhib\n", j+1, k+1);
		fprintf( f2 , "\n");
	}
	fclose(f2);
}

// **********************************************************************
// **********************************************************************

void makeNetLearnMulti16()
{
	FILE *f1 = fopen("net_names", "w");
	FILE *f2 = fopen("net_projs", "w");
	int nbOfMaps = 16;
	int nbOfNets   = 5;
	int thresh[] = { 1, 10, 100, 200, 1000 };
	fprintf( f1, "Name	ON	Layer	0	Threshold	0.5	Zoom	0	Estim	1	Pos	10	100	0	Input	(Convo	Convolutions/OnCenter	Image1	-10000)\nName	OFF	Layer	0	Threshold	0.5	Zoom	0	Estim	1	Pos	10	200	0	Input	(Convo	Convolutions/OffCenter	Image1	-10000)\n\n");

	for (int j=0; j<nbOfNets; j++) {
		for (int i=0; i<nbOfMaps; i++) {
			fprintf( f1, "Name %cLEARN%d Layer 1 Zoom	0	Threshold	0.18	MOD \"networkUnsuperv/modReal\"	Conv	1000	Estim	0\n", j+65,  i+1);
			fprintf( f2, "ON  %cLEARN%d	FILE	UNLOCK	GROUP RET RAND	0.5	RENORM	1000	Convosnew/newGauss1\n",j+65,  i+1);
			fprintf( f2, "OFF %cLEARN%d	FILE	UNLOCK	GROUP RET RAND	0.5	RENORM	1000	Convosnew/newGauss1\n",j+65,  i+1);
		}
		fprintf( f1 , "\n");
		fprintf( f2 , "\n");
	}
	fclose(f1);
	
	
	for (int l=0; l<nbOfNets; l++) {
		for (int i=0; i<nbOfMaps; i++) {
			for (int k=0; k<nbOfMaps; k++)
				if (i != k) fprintf( f2, "%cLEARN%d	%cLEARN%d	FILE	LOCK	GROUP	INHIB MOD 1 MULT %d Convosnew/gauss11inhib\n", l+65, i+1, l+65, k+1, thresh[l]);
			fprintf( f2 , "\n");
		}
		fprintf( f2 , "\n");
	}
	fclose(f2);
}

// **********************************************************************
// **********************************************************************

char *tabFace[] = {
"s1",
"s10",
"s11",
"s12",
"s13",
"s14",
"s15",
"s16",
"s17",
"s18",
"s19",
"s2",
"s20",
"s21",
"s22",
"s23",
"s24",
"s25",
"s26",
"s27",
"s28",
"s29",
"s3",
"s30",
"s31",
"s32",
"s33",
"s34",
"s35",
"s36",
"s37",
"s38",
"s39",
"s4",
"s40",
"s5",
"s6",
"s7",
"s8",
"s9"};

void treatTest()
{
	//printf( "----%.4d-----\n", 5);

// make columns with the result file
	//double value[] = { 0.5, 0.4, 0.3, 0.1, 0.05, 0.04, 0.03, 0.025, 0.022, 0.02, 0.019, 0.018, 0.017, 0.016, 0.015, 0.014, 0.013, 0.012, 0.011, 0.01, 0.009, 0.008, 0.007, 0.006, 0.005, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1, 1.05, 1.1, 1.15, 1.2, 1.25, 1.3 };
	double value[] = { 0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1 };

	FILE *fi = fopen(":outputfile1", "r");
	FILE *fo = fopen(":outputfile2", "w");

	const int nbImages = 320;
	const int nbMaps   = 40;
	const int nbConditions = 21;

	char *tabStr[ nbImages ], tmp[ 1000 ];
	
	for (int k=0; k< nbImages; k++)
	{
		tabStr[ k ] = new char[1000];
		strcpy( tabStr[k], "");
	}
	

	for (int m=0; m<nbConditions; m++)
	{
		for (int j=0; j<nbImages; j++)
		{
			fscanf( fi, "%s", tmp);
			fscanf( fi, "%s", tmp);
			//tmp[ strlen(tmp) - 1] = 0;
			if ( !strcmp( tmp, tabFace[ j / (nbImages/nbMaps) ]))
				strcat( tabStr[ j ], "1\t");
			else
				strcat( tabStr[ j ], "0\t");
		}
	}		

	for (int l=0; l<nbImages; l++)
		fprintf(fo, "%s\n", tabStr[ l ]);
	
	for (int j=0; j<nbConditions; j++)
		fprintf( fo, "%1.3f\t", value[ j ]);
	fprintf( fo, "\n");

	fclose( fi );
	fclose( fo );
}

void treatLearn()
{
	//printf( "----%.4d-----\n", 5);

// make columns with the result file
	FILE *fi = fopen(":outputfile1", "r");
	FILE *fo = fopen(":outputfile2", "w");

	const int nbImages = 960;
	const int nbMaps   = 40;
	const int nbPresentations = 50;

	char *tabStr[nbImages + nbMaps], tmp[ nbMaps ];
	
	for (int k=0; k< nbImages + nbMaps; k++)
	{
		tabStr[ k ] = new char[1000];
		strcpy( tabStr[k], "");
	}

	for (int n=0; n<nbImages + nbMaps; n++)
	{
		fscanf( fi, "%s", tmp);
		strcat( tabStr[ n ], tmp);
		strcat( tabStr[ n ], "\t");
		
		fscanf( fi, "%s", tmp);
		//tmp[ strlen(tmp) - 1] = 0;
	}

	for (int m=0; m<nbPresentations; m++)
	{
		for (int j=0; j<nbImages; j++)
		{
			fscanf( fi, "%s", tmp);
			fscanf( fi, "%s", tmp);
			//tmp[ strlen(tmp) - 1] = 0;
			if ( !strcmp( tmp, tabFace[ j / (nbImages/nbMaps) ]))
				strcat( tabStr[ j ], "1\t");
			else
				strcat( tabStr[ j ], "0\t");
		}
		/*for (int j=0; j<640; j++)
		{
			fscanf( fi, "%s", tmp);
			fscanf( fi, "%s", tmp);
			//tmp[ strlen(tmp) - 1] = 0;
			//if ( !strcmp( tmp, tabFace[ j / (nbImages/nbMaps) ]))
			if ( !strcmp( tmp, tabFace[ j / 16 ]))
				strcat( tabStr[ j ], "1\t");
			else
				strcat( tabStr[ j ], "0\t");
		}
		for (int jj=0; jj<640; jj++)
		{
			fscanf( fi, "%s", tmp);
			fscanf( fi, "%s", tmp);
			//tmp[ strlen(tmp) - 1] = 0;
			if ( !strcmp( tmp, tabFace[ jj / 16 ]))
				strcat( tabStr[ jj + 640 ], "1\t");
			else
				strcat( tabStr[ jj + 640 ], "0\t");
		}
		for (int jjj=0; jjj<320; jjj++)
		{
			fscanf( fi, "%s", tmp);
			fscanf( fi, "%s", tmp);
			//tmp[ strlen(tmp) - 1] = 0;
			if ( !strcmp( tmp, tabFace[ jjj / 8 ]))
				strcat( tabStr[ jjj + 1280 ], "1\t");
			else
				strcat( tabStr[ jjj + 1280 ], "0\t");
		}*/
		
		for (int o=0; o<nbMaps; o++)
		{
			fscanf( fi, "%s", tmp);
			fscanf( fi, "%s", tmp);
			if (*(strpbrk( tmp, "." )) != NULL)
				*(strpbrk( tmp, "." )) = ',';
			if (strcmp( tmp, "untouched"))
				strcat( tabStr[ o+nbImages ], tmp);
			strcat( tabStr[ o+nbImages ], "\t");
		}
	}
	
	for (int l=0; l<nbImages + nbMaps; l++)
		fprintf(fo, "%s\n", tabStr[ l ]);
	
	fclose( fi );
	fclose( fo );
}

char *tabTest[] = {
"Images/orl_faces/s1-1.pgm",
"Images/orl_faces/s1-2.pgm",
"Images/orl_faces/s10-1.pgm",
"Images/orl_faces/s10-2.pgm",
"Images/orl_faces/s11-1.pgm",
"Images/orl_faces/s11-2.pgm",
"Images/orl_faces/s12-1.pgm",
"Images/orl_faces/s12-2.pgm",
"Images/orl_faces/s13-1.pgm",
"Images/orl_faces/s13-2.pgm",
"Images/orl_faces/s14-1.pgm",
"Images/orl_faces/s14-2.pgm",
"Images/orl_faces/s15-1.pgm",
"Images/orl_faces/s15-2.pgm",
"Images/orl_faces/s16-1.pgm",
"Images/orl_faces/s16-2.pgm",
"Images/orl_faces/s17-1.pgm",
"Images/orl_faces/s17-2.pgm",
"Images/orl_faces/s18-1.pgm",
"Images/orl_faces/s18-2.pgm",
"Images/orl_faces/s19-1.pgm",
"Images/orl_faces/s19-2.pgm",
"Images/orl_faces/s2-1.pgm",
"Images/orl_faces/s2-2.pgm",
"Images/orl_faces/s20-1.pgm",
"Images/orl_faces/s20-2.pgm",
"Images/orl_faces/s21-1.pgm",
"Images/orl_faces/s21-2.pgm",
"Images/orl_faces/s22-1.pgm",
"Images/orl_faces/s22-2.pgm",
"Images/orl_faces/s23-1.pgm",
"Images/orl_faces/s23-2.pgm",
"Images/orl_faces/s24-1.pgm",
"Images/orl_faces/s24-2.pgm",
"Images/orl_faces/s25-1.pgm",
"Images/orl_faces/s25-2.pgm",
"Images/orl_faces/s26-1.pgm",
"Images/orl_faces/s26-2.pgm",
"Images/orl_faces/s27-1.pgm",
"Images/orl_faces/s27-2.pgm",
"Images/orl_faces/s28-1.pgm",
"Images/orl_faces/s28-2.pgm",
"Images/orl_faces/s29-1.pgm",
"Images/orl_faces/s29-2.pgm",
"Images/orl_faces/s3-1.pgm",
"Images/orl_faces/s3-2.pgm",
"Images/orl_faces/s30-1.pgm",
"Images/orl_faces/s30-2.pgm",
"Images/orl_faces/s31-1.pgm",
"Images/orl_faces/s31-2.pgm",
"Images/orl_faces/s32-1.pgm",
"Images/orl_faces/s32-2.pgm",
"Images/orl_faces/s33-1.pgm",
"Images/orl_faces/s33-2.pgm",
"Images/orl_faces/s34-1.pgm",
"Images/orl_faces/s34-2.pgm",
"Images/orl_faces/s35-1.pgm",
"Images/orl_faces/s35-2.pgm",
"Images/orl_faces/s36-1.pgm",
"Images/orl_faces/s36-2.pgm",
"Images/orl_faces/s37-1.pgm",
"Images/orl_faces/s37-2.pgm",
"Images/orl_faces/s38-1.pgm",
"Images/orl_faces/s38-2.pgm",
"Images/orl_faces/s39-1.pgm",
"Images/orl_faces/s39-2.pgm",
"Images/orl_faces/s4-1.pgm",
"Images/orl_faces/s4-2.pgm",
"Images/orl_faces/s40-1.pgm",
"Images/orl_faces/s40-2.pgm",
"Images/orl_faces/s5-1.pgm",
"Images/orl_faces/s5-2.pgm",
"Images/orl_faces/s6-1.pgm",
"Images/orl_faces/s6-2.pgm",
"Images/orl_faces/s7-1.pgm",
"Images/orl_faces/s7-2.pgm",
"Images/orl_faces/s8-1.pgm",
"Images/orl_faces/s8-2.pgm",
"Images/orl_faces/s9-1.pgm",
"Images/orl_faces/s9-2.pgm"};

void makeFaceTest()
{
	FILE *fo1 = fopen(":Learn3", "w");
	for (int ii = 0; ii<80; ii++)
	{
		fprintf( fo1, "Image %s\n", tabTest[ ii ]);
		fprintf( fo1, "Image %s Contrast 0.5\n", tabTest[ ii ]);
		fprintf( fo1, "Image %s Contrast 0.5 Luminance 64\n", tabTest[ ii ]);
		fprintf( fo1, "Image %s Contrast 0.5 Luminance -64\n", tabTest[ ii ]);
	}
	fclose(fo1);	
}

