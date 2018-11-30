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
#.*\n ;
echo\ .* {printf("%s\n", yytext+5);}
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
