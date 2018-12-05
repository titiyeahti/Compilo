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

#define YY_INPUT(buf,result,max_size) result = getdata(buf,max_size)
%}

%option noyywrap

%%
#.*				;
;				{printf("yytext: %s : token SEP\n", yytext); return SEP;}
echo\ .* 		{printf("%s\n", yytext+5);}
quit\  			{printf("yytext: %s : token QUIT\n", yytext); return QUIT;}
<<EOF>> 		{printf("yytext: %s : token TEOF\n", yytext); return TEOF;}
[2-9]|1[0-6]	{printf("yytext: %s : token I16\n", yytext); return I16;}
1[7-9]|[2-9][0-9]+|1[0-9][0-9]+	{printf("yytext: %s : token I216\n", yytext); return I216;}
0|1				{printf("yytext: %s : token BOOL\n", yytext); return BOOL;}
\*				{printf("yytext: %s : token BE\n", yytext); return BE;}
function|fct	{printf("yytext: %s : token FCT\n", yytext); return FCT;}
def\-set|pts	{printf("yytext: %s : token PTS\n", yytext); return PTS;}
print|out		{printf("yytext: %s : token PRINT\n", yytext); return PRINT;}
graph			{printf("yytext: %s : token GRAPHE\n", yytext); return GRAPHE;}
ls				{printf("yytext: %s : token BE\n", yytext); return LS;}
[a-zA-Z][0-9a-zA-Z]+	{printf("yytext: %s : token IDENT\n", yytext); return IDENT;}
[a-z]			{printf("yytext: %s : token VAR\n", yytext); return VAR;}
[ \t\n] ;
.		
%%

extern void yywarning(const char* fmt, ...)
{
    char buf[10000];
    va_list ap;
    va_start(ap,fmt);
    vsprintf(buf,fmt,ap);
    va_end(ap);
    if ( gl_compiler ) {
        NYI_ftl();
    } else {
        NYI_ftl();
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
        NYI_ftl();
    } else {
        NYI_ftl();
    }
    if ( gl_throwout )
        NYI_ftl();
    else
        NYI_ftl();
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
