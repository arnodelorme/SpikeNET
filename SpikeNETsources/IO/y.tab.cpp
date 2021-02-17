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

// $Log: y.tab.cpp,v $

#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#define YYPREFIX "yy"
    
#define COPY_CST 0
#define PLUS_CST 1
#define MINUS_CST 2
#define TIMES_CST 3
#define DIVIDE_CST 4
#define ADD_FOR_CHIF 10
#define CONVO_CST 20
#define POS_CST 21

#include "y.tab.h"
#include "GlobalImg.h"
#include "SpikeLisp.h"
    
    int yyparse();
    char *tmpString;
    int numberConvo = 0;
    int numberRes   = -1;
    int imageMax = 0;
    SpikeLisp sp ;
    
    
#define CONVO 257
#define IMAGE 258
#define CHIF 259
#define POS 260
#define PLUS 261
#define MINUS 262
#define DIVIDE 263
#define TIMES 264
#define ID 265
#define BEG 266
#define END 267
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    2,    2,    2,    2,    2,    3,    3,
};
short yylen[] = {                                         2,
    1,    6,    6,    6,    6,    6,    6,    6,    6,    3,
    6,    6,    1,    1,    1,    1,    1,    0,    1,
};
short yydefred[] = {                                      0,
    0,    0,    1,    0,    0,   17,   13,   14,   16,   15,
    0,    0,   10,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   19,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   12,   11,    9,
    8,    7,    6,    4,    5,    3,    2,
};
short yydgoto[] = {                                       2,
    3,   11,   28,
};
short yysindex[] = {                                   -248,
 -232,    0,    0, -238, -228,    0,    0,    0,    0,    0,
 -249, -243,    0, -247, -242, -245, -239, -239, -239, -239,
 -239, -239, -239, -239, -239, -239,    0, -227, -226, -225,
 -224, -223, -222, -221, -220, -219, -218,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,
};
short yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -217, -217, -217, -217,
 -217, -217, -217, -217, -217, -217,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,
};
short yygindex[] = {                                      0,
   22,    0,  -18,
};
#define YYTABLESIZE 50
short yytable[] = {                                      29,
   30,   31,   32,   33,   34,   35,   36,   37,   14,   15,
   19,   20,   24,   25,   17,   22,    1,    1,    1,   27,
    1,    0,    1,    1,    4,    5,   12,    6,    7,    8,
    9,   10,   16,   18,    0,   21,   23,   26,   13,   38,
   39,   40,   41,   42,   43,   44,   45,   46,   47,   18,
};
short yycheck[] = {                                      18,
   19,   20,   21,   22,   23,   24,   25,   26,  258,  259,
  258,  259,  258,  259,  258,  258,  266,  266,  266,  259,
  266,   -1,  266,  266,  257,  258,  265,  260,  261,  262,
  263,  264,   11,   12,   -1,   14,   15,   16,  267,  267,
  267,  267,  267,  267,  267,  267,  267,  267,  267,  267,
};
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 267
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"CONVO","IMAGE","CHIF","POS",
"PLUS","MINUS","DIVIDE","TIMES","ID","BEG","END",
};
char *yyrule[] = {
"$accept : princ",
"princ : expr",
"expr : BEG oper expr expr thres END",
"expr : BEG oper expr CHIF thres END",
"expr : BEG oper CHIF expr thres END",
"expr : BEG oper expr IMAGE thres END",
"expr : BEG oper CHIF IMAGE thres END",
"expr : BEG oper IMAGE expr thres END",
"expr : BEG oper IMAGE CHIF thres END",
"expr : BEG oper IMAGE IMAGE thres END",
"expr : BEG IMAGE END",
"expr : BEG CONVO ID expr thres END",
"expr : BEG CONVO ID IMAGE thres END",
"oper : PLUS",
"oper : MINUS",
"oper : TIMES",
"oper : DIVIDE",
"oper : POS",
"thres :",
"thres : CHIF",
};
#endif
#ifndef YYSTYPE
typedef long YYSTYPE;
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH 500
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE

#include "lex.yy.c"
            
void yyerror(const char *c)
	ERROR( 128, ("opening stream socket error"))

void y_tab::yyrun( FILE *f, SpikeLispSpike *spL)
{
	sp.reset();
    numberConvo = 0;
    numberRes   = -1;
	imageMax    = 0;
	yyrestart(f);
	yyparse();
	spL->copy(sp);
}

int y_tab::imageMaxi()
{ return (imageMax+1); }

/*int main()
    {    
        GlobalImg *listImg[MAX_IMG_LIST];
        
	FILE *f = fopen("command", "r");
	yyrestart(f);
        yyparse();
	SpikeLispSpike sp1(0.3, 100, 50000);
	sp1.copy(sp);
	sp.reset();
	
	fclose(f);
	
	f = fopen("command", "r");
	yyrestart(f);
        yyparse();
	SpikeLispSpike sp2(0.3, 100, 50000);
	sp2.copy(sp);
    fclose(f);

    listImg[0] = new GlobalImg("2001Space001.PPM", 0);
    listImg[1] = new GlobalImg("2001Space002.PPM", 0);
    listImg[2] = new GlobalImg("2001Space003.PPM", 0);
        
    sp1.reserv( -2 , listImg[0]->width, listImg[0]->height);
    sp2.reserv( 1 , listImg[0]->width, listImg[0]->height);
        
    (sp1.run( listImg ))->save("totores1");
    (sp2.run( listImg ))->save("totores2");
    //sp.makeSpikeList();
    //sp.showSpikeList();
}
*/































#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse()
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;
    extern char *getenv();

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 1:
{ /*printf("ok compilation\n"); */ }
break;
case 2:
{
  /* regular expression*/
  /*printf("processing expression : %d %d %d %d\n", $2, $3, $4, $5);*/
  sp.addOper( yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1]);
  yyval = numberRes--;
}
break;
case 3:
{
  /* regular expression*/
  /*printf("processing expression : %d %d %d %d\n", $2, $3, $4, $5);*/
  sp.addOper( yyvsp[-4]+ADD_FOR_CHIF, yyvsp[-3], yyvsp[-2], yyvsp[-1]);
  yyval = numberRes--;
}
break;
case 4:
{
  /* regular expression*/
  /*printf("processing expression : %d %d %d %d\n", $2, $3, $4, $5);*/
  sp.addOper( yyvsp[-4]+ADD_FOR_CHIF, yyvsp[-2], yyvsp[-3], yyvsp[-1]);
  yyval = numberRes--;
}
break;
case 5:
{
  /* regular expression*/
  /*printf("processing expression : %d %d %d %d\n", $2, $3, $4, $5);*/
  sp.addOper( yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1]);
  if (yyvsp[-2] > imageMax) imageMax = yyvsp[-2];
  yyval = numberRes--;
}
break;
case 6:
{
  /* regular expressionÓ*/
  /*printf("processing expression : %d %d %d %d\n", $2, $3, $4, $5);*/
  sp.addOper( yyvsp[-4]+ADD_FOR_CHIF, yyvsp[-2], yyvsp[-3], yyvsp[-1]);
  if (yyvsp[-2] > imageMax) imageMax = yyvsp[-2];
  yyval = numberRes--;
}
break;
case 7:
{
  /* regular expression*/
  /*printf("processing expression : %d %d %d %d\n", $2, $3, $4, $5);*/
  sp.addOper( yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1]);
  if (yyvsp[-3] > imageMax) imageMax = yyvsp[-3];
  yyval = numberRes--;
}
break;
case 8:
{
  /* regular expression*/
  /*printf("processing expression : %d %d %d %d\n", $2, $3, $4, $5);*/
  sp.addOper( yyvsp[-4]+ADD_FOR_CHIF, yyvsp[-3], yyvsp[-2], yyvsp[-1]);
  if (yyvsp[-3] > imageMax) imageMax = yyvsp[-3];
  yyval = numberRes--;
}
break;
case 9:
{
  /* regular expression*/
  /*printf("processing expression : %d %d %d %d\n", $2, $3, $4, $5);*/
  sp.addOper( yyvsp[-4], yyvsp[-3], yyvsp[-2], yyvsp[-1]);
  if (yyvsp[-3] > imageMax) imageMax = yyvsp[-3];
  if (yyvsp[-2] > imageMax) imageMax = yyvsp[-2];
  yyval = numberRes--;
}
break;
case 10:
{
  /*copy the image*/
  /*printf("processing image : %d \n", $2);*/
  sp.addOper( COPY_CST, yyvsp[-1], 0);
  if (yyvsp[-1] > imageMax) imageMax = yyvsp[-1];
  yyval = numberRes--;
}
break;
case 11:
{
  /*convolue an Image*/
  /*printf("processing convo : %s \n", $3);*/
  sp.addConvo( (char *)yyvsp[-3], numberConvo);
  sp.addOper(CONVO_CST, yyvsp[-2], numberConvo++, yyvsp[-1]);
  yyval = numberRes--;
}
break;
case 12:
{
  /*convolue an Image*/
  /*printf("processing convo : %s %d %d\n", $3, $4, $5);*/
  sp.addConvo( (char *)yyvsp[-3], numberConvo);
  sp.addOper(CONVO_CST, yyvsp[-2], numberConvo++, yyvsp[-1]);
  if (yyvsp[-2] > imageMax) imageMax = yyvsp[-2];
  yyval = numberRes--;
}
break;
case 13:
{ yyval = PLUS_CST;  }
break;
case 14:
{ yyval = MINUS_CST; }
break;
case 15:
{ yyval = TIMES_CST; }
break;
case 16:
{ yyval = DIVIDE_CST;}
break;
case 17:
{ yyval = POS_CST;   }
break;
case 18:
{ yyval = DEFAULT_THRESHOLD; }
break;
case 19:
{ yyval = yyvsp[0]; }
break;
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
