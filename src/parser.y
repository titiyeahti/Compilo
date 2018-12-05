%{
/*************************************************************************
 * Template of logic parser
*************************************************************************/

#include "var-set.h"
#include "point-set.h"
#include "logic-expr-tree.h"
#include "truth-table.h"
#include "logic-func.h"
#include "data.h"
#include "print-option.h"

#if 0
static void parser_check_funcparam(Cstr fn,Clfunc*lf,CletSet*lets)
{
    if ( lets->eleNb==lf->lf_varNb ) return;
    if ( fn==0 )
        yyerror("%d invalid number of parameters (expected %d).",
                lets->eleNb,lf->lf_varNb);
    else
        yyerror("%d invalid number of parameters for calling the %s function (expected %d).",
                lets->eleNb,fn,lf->lf_varNb);
}

static void parser_ls(Tldt*ldt)
{
    int i; TstrSet* strs;

    strs = ldt_getFuncNames(ldt);
    printf(" function: ");
    for (i=0 ; i<strs->eleNb ; i+=1) printf(" %8s",strs->eles[i]);
    printf("\n");
    strset_free(strs);

    strs = ldt_getPtsNames(ldt);
    printf(" point set:");
    for (i=0 ; i<strs->eleNb ; i+=1) printf(" %8s",strs->eles[i]);
    printf("\n");
    strset_free(strs);
}
#endif

%}
%token SEP
%token BOOL
%token BE
%token I16
%token I216
%token IDENT
%token VAR
%token FCT
%token PTS
%token PRINT
%token GRAPHE
%token TEOF
%token QUIT
%token LS

%start start
%%
/*======================================================================*/
/*= main rules                                                         =*/
start : 
	  {printf("coucou je ne fais rien");}|
	  bool_expr {printf("pute1 : %s\n", $$);return 0;}
;
bool_expr :
		    bool_expr '*' bool_expr {printf("%s", $$);}
		  | bool_expr '+' bool_expr {printf("%s", $$);}
		  | bool_expr '^' bool_expr {printf("%s", $$);}
		  | bool_expr '!' '*' bool_expr {printf("%s", $$);}
		  | bool_expr '!' '+' bool_expr {printf("%s", $$);}
		  | bool_expr '!' '^' bool_expr {printf("%s", $$);}
		  | '!' bool_expr {printf("%s", $$);}
		  | VAR {printf("%s", $$);}
		  | BOOL {printf("%s", $$);}
		  ;

%%
