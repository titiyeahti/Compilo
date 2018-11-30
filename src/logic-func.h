/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: logic-func.h 456 2018-09-20 14:50:36Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/logic-func.h $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/
#ifndef FILE_LOGIC_FUNC_H
#define FILE_LOGIC_FUNC_H
/*======================================================================*/
/*= Handler for logcic functions defined                               =*/
/*=   * by a logic expression                                          =*/
/*=   * or by a truth table.                                           =*/
/*======================================================================*/

#include "top.h"

/*======================================================================*/
/*= Data structure of a logic expression                               =*/

struct _Tlfunc {
    TvarSet*  lf_varNames; // names of variables (never null) sorted by idx:
                           //  - lf_varNames->eles[i].idx == i
                           //  - lf_varNames->eles[i].name is the name of i th variable
#define       lf_varNb lf_varNames->eleNb  // variable number
    TleTree*  lf_tree;     // the logic expression of the function
    TtTable*  lf_table;    // the truth table of the function
                           // lf_table->eleNb must be 2**lf_varNb
    /**
     * ENSURE:
     *  - lf_varNb!=0.
     *  - lf_tree and lf_table are not both NULL.
     *  - lf_table->eleNb must be 2**lf_varNb (if lf_table!=0).
    **/
};

/*= Constructors/destructor                                            =*/

Tlfunc* lf_newByExpr     (TleTree* let, TvarSet* vars);
Tlfunc* lf_newByTable    (TtTable* tt,  TvarSet* vars);
void    lf_free          (Tlfunc*  lf);

/*= feature                                                            =*/

/**
 * The lf_getLeTree function updates the lf->lf_tree tree from the
 * lf->lf_table truth table if lf->lf_tree is null and then returns it.
 * When lf->lf_tree is computed, it represents the conjunctive form.
 *
 * The lf_getTTable function updates the lf->lf_table truth table
 * from the lf->lf_tree tree if lf->lf_table is null and then
 * it returns it.
 * 
 * The lf_getDefaultCallParam function returns a s set of logic
 * expression trees where all the s[i] tree are variable nodes.
 * The s[i]->le_var is the name of the i th variable of the lf logic function.

 * The lf_eval function evaluates the "lf" logic function
 * for the variable values given by vars, "(vars&(1<<i))!=0" being
 * the value of the ith variable.
 *
 * Returns:
 *  lf_getDefaultCallParam returns a tree set that the caller must free
 *  after use.
 *  lf_eval returns true (1) or false (0).
**/
CleTree* lf_getLeTree          (Clfunc* lf);
CtTable* lf_getTTable          (Clfunc* lf);
TletSet* lf_getDefaultCallParam(Clfunc* lf);
int      lf_eval               (Clfunc* lf, int pt);

/**
 * The lf_disFormMax function returns a logic tree under disjunctive
 * form of the lf function.
 * Each minterm of the tree references all the function variables.
 * The returned tree must be freed after use.
**/
TleTree* lf_disFormMax(Clfunc*lf);

/*======================================================================*/
#endif // FILE_LOGIC_FUNC_H
/*======================================================================*/

