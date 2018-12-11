/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: parser-eleve.l 456 2018-09-20 14:50:36Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/parser-eleve.l $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/
%{
/*======================================================================*/
/*= Template of logic lexical analyser.                                =*/
/*======================================================================*/

#include <stdarg.h>

#include "top.h"

#include "print-option.h"
#include "parser.tab.h"

static int getdata(char* buf, int max_size);
int line_count = 1;

#define YY_INPUT(buf,result,max_size) result = getdata(buf,max_size)
%}

%option noyywrap

%%
#.*										;
echo\ .*								{printf("%s\n", yytext+5);}
";"										{return SEP;}
\n										{if (!gl_compiler) {return SEP;}else {line_count ++;}}
[0-9]*									{yylval=atof(yytext);
										if (yylval == 0){ return ZERO;}
										else {
												if (yylval == 1){ return UN;}
												else {
														if (yylval < 17) {return I16;} 
														else {
																if (yylval<65537){ return I216;} 
														}
												}
										}
										}
\(										{return LEFT_BRACE;}
\)										{return RIGHT_BRACE;}
"["										{return LEFT_BRACKET;}
"]"										{return RIGHT_BRACKET;}
"{"										{return LEFT_ACC;}
"}"										{return RIGHT_ACC;}
:										{return REP;}	
==										{return DEQ;}
=										{return EQ;}
\*										{return MULT;}
\+										{return ADD;}
!										{return NOT;}
\^										{return XOR;}
,										{return VIR;}
function|fct							{return FCT;}
"def-set"|pts							{return PTS;}
print|out								{return PRINT;}
graph									{return GRAPHE;}
quit|end								{return QUIT;}
ls										{return LS;}
<<EOF>>									{if (gl_compiler) {return TEOF;}}
"/le"|"/pc/le"|"/oa/le"|"/df/le"		{return GRAPHE_OPT;}
"/fp"									{return PRINT_OPT;}
("/tt"|"/df/tt")(\ [a-zA-Z][0-9a-zA-Z]+)?	{int len = strlen(yytext);
										if (len > 7 || len > 3) //la deuxième partie de la capture est non nulle
										{
												return PRINT_OPT_P;
										}
										else
										{
												return PRINT_OPT;
										}
										}
[a-z]									{return VAR;}
[a-zA-Z][0-9a-zA-Z]+					{return IDENT;}
[ \t]+									;
%%

extern void yywarning(const char* fmt, ...)
{
    char buf[10000];
    va_list ap;
    va_start(ap,fmt);
    vsprintf(buf,fmt,ap);
    va_end(ap);
    if ( gl_compiler ) {
		printf("line=%i:warning:%s: %s\n", line_count, "NYI", buf);
	} else {
		printf("warning:%s: %s\n", "NYI", buf);
	}
}

extern void yyerror(const char* fmt, ...)
{
    extern void longjmp(void*);
    char buf[10000];
    va_list ap;
    va_start(ap,fmt);
    vsprintf(buf,fmt,ap);
    va_end(ap); 
    if ( gl_compiler ) {
    	printf("line=%i:error:%s: %s\n", line_count, yytext, buf);
	} else {
		printf("error:%s: %s\n", yytext, buf);
    }
    if ( !gl_throwout )
		exit(1);//toto long jumpr gl throw out;
	else
		exit(1);
}

/*======================================================================*/

static int         getdata_from_buf;
static const char* getdata_buf;

extern void yylex_setStreamToMem (Cstr  stream)
{ getdata_from_buf=1; getdata_buf =stream; }

extern void yylex_setStreamToFile(FILE* stream)
{ getdata_from_buf=0; if (stream) yyin=stream; }

static int getdata(char* buf, int max_size)
{
    if ( getdata_from_buf==0 )
        return fread(buf,1,max_size,yyin);

    if ( getdata_buf==0 || *getdata_buf==0 )
        return 0;

    *buf = *getdata_buf++;
    return 1;
}
