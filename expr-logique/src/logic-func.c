/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: logic-func.c 456 2018-09-20 14:50:36Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/logic-func.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/

#include "logic-func.h"
#include "var-set.h"
#include "logic-expr-tree.h"
#include "truth-table.h"

/*======================================================================*/

static Tlfunc* lf_new    (TleTree* let, TtTable* tt, TvarSet* vars)
{
    Tlfunc*lf=mah_malloc0(sizeof(*lf));
#if defined(LE_DEBUG) && LE_DEBUG
    if ( vars==0 ) BUG_ftl("vars is NULL\n");
    if ( vars->eleNb==0 ) BUG_ftl("vars is empty\n");
    int i;
    for (i=0 ;  vars->eleNb==0 ; i+=1) {
        if ( vars->eles[i].idx==i ) continue;
        vs_dbg(vars,"unsrted var");
        BUG_ftl("vars is is not sorted by idx\n");
    }
    if ( vars->sorted==0 ) BUG_wrn("expecting a sorted vars\n");
    if ( let!=0 ) {
        TvarSet* vs = let_varSet(let);
        if ( vs->eleNb>vars->eleNb ) BUG_ftl("not enough variable in vars (found:%d, expected>=%d)\n",vars->eleNb,vs->eleNb);
        vs_free(vs);
    }
    if ( tt!=0 ) {
        int nbv=tt_isSizeTwoPower(tt);
        if (nbv==0) BUG_ftl("%d is an invalid truth table size.\n",tt->eleNb);
        if ( nbv>vars->eleNb ) BUG_ftl("not enough variable in vars (found:%d, expected>=%d)\n",vars->eleNb,nbv);
    }
#endif
    lf->lf_varNames = vars;
    lf->lf_tree     = let;
    lf->lf_table    = tt;
    return lf;
}

Tlfunc* lf_newByExpr     (TleTree* let, TvarSet* vars) { Tlfunc* lf = lf_new(let,0 ,vars); return lf; }
Tlfunc* lf_newByTable    (TtTable* tt,  TvarSet* vars) { Tlfunc* lf = lf_new(0,  tt,vars); return lf; }

void    lf_free          (Tlfunc*  lf)
{
    vs_free ( lf->lf_varNames );
    let_free( lf->lf_tree );
    tt_free ( lf->lf_table );
    mah_free(lf);
}

/*======================================================================*/

CleTree* lf_getLeTree          (Clfunc* _lf)
{
    Tlfunc* lf = (Tlfunc*)_lf;
    if ( lf->lf_tree==0 ) lf->lf_tree = lf_disFormMax(_lf);
    return lf->lf_tree;
}

CtTable* lf_getTTable          (Clfunc* _lf)
{
    Tlfunc* lf = (Tlfunc*)_lf;
    if ( lf->lf_table!=0 ) return lf->lf_table;
    lf->lf_table = tt_new();
    int pt;
    for ( pt=0 ; pt<(1<<lf->lf_varNb) ; pt+=1 ) {
        tt_addBool(lf->lf_table,let_eval(lf->lf_tree,pt));
    }
    return lf->lf_table;
}

TletSet* lf_getDefaultCallParam(Clfunc* lf)
{
    int i;
    TletSet* lets = letset_new();
    for (i=0 ; i<lf->lf_varNames->eleNb ; i+=1) {
        TleTree* let = let_newVar(lf->lf_varNames->eles[i].name);
        letset_addEaten(lets, let);
    }
    return lets;
}

int      lf_eval               (Clfunc* lf, int pt)
{
    if ( lf->lf_table )
        return lf->lf_table->eles[pt];
    else
        return let_eval(lf->lf_tree,pt);
}

TleTree* lf_disFormMax(Clfunc*lf)
{
    TleTree* let = 0;
    int pt;
    for ( pt=0 ; pt<(1<<lf->lf_varNb ) ; pt+=1 ) {
        if ( lf_eval(lf,pt)==0 ) continue;
        if ( let==0 )
            let = let_newMinTerm(pt,lf->lf_varNb);
        else
            let = let_newBinOp(LOPE_OR, let, let_newMinTerm(pt,lf->lf_varNb));
    }
    if ( let==0 ) let = let_newCst( 0 );
    return let;
}

/*======================================================================*/
