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
    /*printf*/;
    for (i=0 ; i<strs->eleNb ; i+=1) /*printf*/;
    /*printf*/;
    strset_free(strs);

    strs = ldt_getPtsNames(ldt);
    /*printf*/;
    for (i=0 ; i<strs->eleNb ; i+=1) /*printf*/;
    /*printf*/;
    strset_free(strs);
}
#endif

%}
%token SEP 
%token I16
%token I216
%token ZERO
%token UN
%token MULT
%token ADD
%token NOT
%token XOR
%token FCT
%token PTS 
%token PRINT
%token GRAPHE
%token QUIT
%token LS
%token IDENT
%token VAR 
%token TEOF
%token LEFT_BRACE
%token RIGHT_BRACE
%token LEFT_BRACKET
%token RIGHT_BRACKET
%token LEFT_ACC
%token RIGHT_ACC
%token VIR
%token EQ
%token DEQ
%token REP
%token PRINT_OPT
%token GRAPHE_OPT
%token PRINT_OPT_P

%start start
%%
/*======================================================================*/
/*= main rules                                                         =*/
start :
	  start instr {if (!gl_compiler ) return 0;}
	  |
	  ;
/* boolEENs */

bool :
	 ZERO
	 |UN
	 ;

/* EXPRESSIONS LOGIQUES */

log_exp :
		log_exp ADD log_exp
		|log_exp NOT ADD log_exp
		|log_exp MULT log_exp
		|log_exp NOT MULT log_exp
		|log_exp XOR log_exp
		|log_exp NOT XOR log_exp
		|NOT log_exp
		|LEFT_BRACE log_exp RIGHT_BRACE
		|IDENT
		|IDENT LEFT_BRACE log_exp_list RIGHT_BRACE
		|VAR
		|bool
		;

log_exp_list :
			 log_exp
			 |log_exp_list VIR log_exp
			 ;

/* TABLES DE VERITE */

/* I216 */

i216 :
	 i16
	 |I216
	 ;

i16 :
	UN
	|I16
	;

rbool :
	  bool
	  |bool REP i216
	  |IDENT
	  |IDENT REP i216
	  |LEFT_BRACE rbool_list RIGHT_BRACE REP i216
	  ;

rbool_list :
		   rbool
		   |rbool_list rbool
		   ;

rbool_list_vir :
			   rbool
			   |rbool_list_vir VIR rbool
			   ;

truth_table :
			LEFT_BRACKET rbool_list_vir RIGHT_BRACKET
			;

/* ENSEMBLES DE DEFINITIONS */

e_bool :
	   MULT
	   |bool
	   ;

re_bool :
		e_bool
		|e_bool REP i16
		|LEFT_BRACE re_bool_list RIGHT_BRACE REP i16
		|IDENT
		|IDENT DEQ bool
		;

re_bool_list :
			 re_bool
			 |re_bool_list re_bool
			 ;

re_bool_list_vir :
				 re_bool
				 |re_bool_list_vir VIR re_bool
				 ;

set_def :
		LEFT_ACC re_bool_list_vir RIGHT_ACC
		;


/* INSTRUCTIONS */

instr :
	  SEP {/*return 0*/;}
	  |QUIT {return 2;}
	  |TEOF {return 0;} 
	  |LS SEP {/*printf*/;/*return 0*/;}
	  |fct_def {/*printf*/;/*return 0*/;}
	  |PTS IDENT EQ set_def SEP {/*printf*/;} 
	  |PTS IDENT EQ truth_table SEP {/*printf*/;/*return 0*/;}
	  |PRINT IDENT print_opt_list SEP {/*printf*/;/*return 0*/;}
	  |GRAPHE IDENT graphe_opt_list SEP {/*printf*/;/*return 0*/;}
	  |PRINT IDENT DEQ IDENT SEP {/*return 0*/;}
	  |PRINT IDENT DEQ IDENT IDENT SEP {/*return 0*/;}
	  ;

fct_def :
		FCT IDENT LEFT_BRACE var_list_vir RIGHT_BRACE EQ log_exp SEP {/*printf*/;} 
		|FCT IDENT EQ log_exp SEP {/*printf*/;}
		|FCT IDENT LEFT_BRACE var_list_vir RIGHT_BRACE EQ truth_table SEP {/*printf*/;}
		|FCT IDENT EQ truth_table SEP {/*printf*/;}
		;

var_list_vir :
			 VAR
			 |var_list_vir VIR VAR
			 ;

print_opt_list :
			   |p_opt
			   |GRAPHE_OPT
			   |p_opt print_opt_list
			   |GRAPHE_OPT print_opt_list
			   ;

graphe_opt_list :
				|GRAPHE_OPT
				|GRAPHE_OPT graphe_opt_list
				;

p_opt :
	  PRINT_OPT
	  |PRINT_OPT_P IDENT
	  ;
	  

%%
