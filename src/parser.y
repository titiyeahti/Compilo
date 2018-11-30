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


%start start
%%
/*======================================================================*/
/*= main rules                                                         =*/
start :  ;

%%
