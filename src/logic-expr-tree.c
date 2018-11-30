/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: logic-expr-tree.c 457 2018-10-21 15:12:50Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/logic-expr-tree.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/

#include "logic-expr-tree.h"
#include "var-set.h"

/*======================================================================*/

#define IsBinNode(let) lope_isBin((let)->le_kd)
Inline int IsAssNode(CleTree*let)
{
    if ( let==0 ) return 0;
    return lope_isAssociative((let)->le_kd);
}
Inline int PriNode(CleTree*let)
{
    if ( let==0 ) return 0;
    return lope_priority(let->le_kd);
}
Inline int IsTrue (CleTree*let) { return let->le_kd==LOPE_TRUE; }
Inline int IsFalse(CleTree*let) { return let->le_kd==LOPE_FALSE; }

/*======================================================================*/

static TleTree* let_new(Tlope kd)
{
    TleTree* let = mah_malloc0(sizeof(*let));
    let->le_kd = kd;
    return let;
}

extern TleTree* let_newCst   (int cst)        { return let_new(cst ? LOPE_TRUE : LOPE_FALSE); }
extern TleTree* let_newVar   (int var)        { TleTree*let=let_new(LOPE_VAR); let->le_var=var; return let; }
extern TleTree* let_newNot   (TleTree* child) { NYI_ftl();  }
extern TleTree* let_newBinOp (Tlope kd, TleTree* lc, TleTree* rc) { NYI_ftl(); }

extern TleTree* let_newMinTerm(int pt, int nvars)
{
    int i;
    TleTree* ret = 0;
    for (i=0 ; i<nvars ; i++) {
        TleTree* currvar = let_newVar(i);
        if ( (pt&(1<<i))==0 )
            currvar = let_newNot(currvar);
        if ( ret==0 )
            ret = currvar;
        else
            ret = let_newBinOp(LOPE_AND,currvar,ret);
    }
    return ret;
}

extern TleTree* let_newClone (CleTree* let)
{
    if ( let==0 ) return 0;
    TleTree* clone = let_new(LOPE_NONE);
    *clone = *let;
    clone->le_lc = let_newClone( clone->le_lc);
    clone->le_rc = let_newClone( clone->le_rc);
}

extern void     let_free     (TleTree* let)
{
    if ( let==0 ) return;
    let_free(let->le_lc); 
    let_free(let->le_rc); 
    mah_free(let);
}

/*======================================================================*/

static void     let_varSet_0(CleTree* let,TvarSet*vs)
{
    if ( let==0 ) return;
    if ( let->le_kd==LOPE_VAR ) {
        vs_addUniq(vs,let->le_var,let->le_var);
    } else {
        let_varSet_0(let->le_lc,vs);
        let_varSet_0(let->le_rc,vs);
    }
}
extern TvarSet* let_varSet(CleTree* let)
{
    TvarSet* vs = vs_new();
    let_varSet_0(let,vs);
    return vs;
}

extern void     let_chgVar(TleTree* let, int idx, int newidx)
{
    if ( let==0 ) return;
    if ( let->le_kd==LOPE_VAR ) {
        if ( let->le_var==idx )
            let->le_var=newidx;
    } else {
        let_chgVar(let->le_lc,idx,newidx);
        let_chgVar(let->le_rc,idx,newidx);
    }
}

extern void     let_chgVarsIdxToName(TleTree* let, CvarSet* vs)
{
    int i;
    if ( let==0 ) return;
    if ( let->le_kd==LOPE_VAR ) {
        for (i=0 ; i<vs->eleNb ; i++) {
          if ( let->le_var==vs->eles[i].idx ) {
            let->le_var=vs->eles[i].name;
            break;
          }
        }
    } else {
        let_chgVarsIdxToName(let->le_lc,vs);
        let_chgVarsIdxToName(let->le_rc,vs);
    }
}

extern void     let_chgVarsNameToIdx(TleTree* let, CvarSet* vs)
{
    int i;
    if ( let==0 ) return;
    if ( let->le_kd==LOPE_VAR ) {
        for (i=0 ; i<vs->eleNb ; i++) {
          if ( let->le_var==vs->eles[i].name ) {
            let->le_var=vs->eles[i].idx;
            break;
          }
        }
    } else {
        let_chgVarsNameToIdx(let->le_lc,vs);
        let_chgVarsNameToIdx(let->le_rc,vs);
    }
}
extern void     let_chgVarsIdxToTree(TleTree**let, CletSet* lets)
{
    if ( let==0 || *let==0 ) return;
    if ( let[0]->le_kd==LOPE_VAR ) {
        TleTree* var = *let;
        *let = let_newClone( lets->eles[var->le_var] );
        let_free(var);
    } else {
        let_chgVarsIdxToTree(&let[0]->le_lc,lets);
        let_chgVarsIdxToTree(&let[0]->le_rc,lets);
    }
}

/*======================================================================*/

extern int      let_eval  (CleTree* let, int pt)
{
    switch (let->le_kd) {
        case LOPE_TRUE  : return 1;
        case LOPE_FALSE : return 0;
        case LOPE_VAR   : return (pt&(1<<let->le_var))!=0;
        case LOPE_NOT   : NYI_ftl();
        case LOPE_XOR   : NYI_ftl();
        case LOPE_OR    : NYI_ftl();
        case LOPE_AND   : NYI_ftl();
        case LOPE_NXOR  : NYI_ftl();
        case LOPE_NOR   : NYI_ftl();
        case LOPE_NAND  : NYI_ftl();
        default:
            BUG_ftl("%d is an invalid kind.\n",let->le_kd);
    }
}

extern TleTree* let_optimize1(CleTree* let)
{
    TleTree *child,*lc,*rc,*ret;
    switch (let->le_kd) {
        case LOPE_TRUE  : return let_newClone(let);
        case LOPE_FALSE : return let_newClone(let);
        case LOPE_VAR   : return let_newClone(let);
        case LOPE_NOT   :
            child = let_optimize1(let->le_lc);
            switch (child->le_kd) {
                case LOPE_TRUE  : let_free(child); return let_newCst(0);
                case LOPE_FALSE : let_free(child); return let_newCst(1);
                case LOPE_VAR   : return let_newClone(let);
                case LOPE_NOT   : ret=child->le_lc; mah_free(child); return ret;
                case LOPE_XOR   : return child;
                case LOPE_OR    : return child;
                case LOPE_AND   : return child;
                case LOPE_NXOR  : child->le_kd=LOPE_XOR; return child;
                case LOPE_NOR   : child->le_kd=LOPE_OR;  return child;
                case LOPE_NAND  : child->le_kd=LOPE_AND; return child;
                default:
                  BUG_ftl("%d is an invalid kind.\n",child->le_kd);
            }
        case LOPE_OR    :
            lc = let_optimize1(let->le_lc);
            rc = let_optimize1(let->le_rc);
            if ( IsTrue(lc) || IsTrue(rc) ) { let_free(lc); let_free(rc); return let_newCst(1);
            } else if ( IsFalse(lc) )       { let_free(lc); return rc;
            } else if ( IsFalse(rc) )       { let_free(rc); return lc;
            } else                            return let_newBinOp(let->le_kd,lc,rc);
        case LOPE_XOR    :
            // x xor 0 = x !0 + !x 0 = x
            // x xor 1 = x !1 + !x 1 = !x
            lc = let_optimize1(let->le_lc);
            rc = let_optimize1(let->le_rc);
            if        ( IsTrue(lc)  && IsTrue(rc)  ) { let_free(lc); let_free(rc); return let_newCst(0); 
            } else if ( IsFalse(lc) && IsFalse(rc) ) { let_free(lc); let_free(rc); return let_newCst(0);
            } else if ( IsFalse(lc) && IsTrue(rc)  ) { let_free(lc); let_free(rc); return let_newCst(1);
            } else if ( IsTrue(lc)  && IsFalse(rc) ) { let_free(lc); let_free(rc); return let_newCst(1);
            } else if ( IsFalse(lc) ) { let_free(lc); return rc;
            } else if ( IsFalse(rc) ) { let_free(rc); return lc;
            } else if ( IsTrue(lc)  ) { let_free(lc); return let_newNot(rc);
            } else if ( IsTrue(rc)  ) { let_free(rc); return let_newNot(lc);
            } else                                    return let_newBinOp(let->le_kd,lc,rc);
        case LOPE_AND   :
            lc = let_optimize1(let->le_lc);
            rc = let_optimize1(let->le_rc);
            if        ( IsFalse(lc) || IsFalse(rc) ) { let_free(lc); let_free(rc); return let_newCst(0);
            } else if ( IsTrue(lc) )                 { let_free(lc); return rc;
            } else if ( IsTrue(rc) )                 { let_free(rc); return lc;
            } else                                   return let_newBinOp(let->le_kd,lc,rc);
        case LOPE_NOR   :
            lc = let_optimize1(let->le_lc);
            rc = let_optimize1(let->le_rc);
            if        ( IsTrue(lc)  || IsTrue(rc) ) { let_free(lc); let_free(rc); return let_newCst(0);
            } else if ( IsFalse(lc) && IsFalse(rc)) { let_free(lc); let_free(rc); return let_newCst(1);
            } else if ( IsFalse(lc) )               { let_free(lc); return let_newNot(rc);
            } else if ( IsFalse(rc) )               { let_free(rc); return let_newNot(lc);
            } else                                    return let_newBinOp(let->le_kd,lc,rc);
        case LOPE_NXOR  :
            // x nxor 0 = x 0 + !x !0 = !x
            // x nxor 1 = x 1 + !x !1 = x
            lc = let_optimize1(let->le_lc);
            rc = let_optimize1(let->le_rc);
            if        ( IsTrue(lc)  && IsTrue(rc)  ) { let_free(lc); let_free(rc); return let_newCst(1); 
            } else if ( IsFalse(lc) && IsFalse(rc) ) { let_free(lc); let_free(rc); return let_newCst(1);
            } else if ( IsFalse(lc) && IsTrue(rc)  ) { let_free(lc); let_free(rc); return let_newCst(0);
            } else if ( IsTrue(lc)  && IsFalse(rc) ) { let_free(lc); let_free(rc); return let_newCst(0);
            } else if ( IsTrue(lc)  ) { let_free(lc); return rc;
            } else if ( IsTrue(rc)  ) { let_free(rc); return lc;
            } else if ( IsFalse(lc) ) { let_free(lc); return let_newNot(rc);
            } else if ( IsFalse(rc) ) { let_free(rc); return let_newNot(lc);
            } else                                    return let_newBinOp(let->le_kd,lc,rc);
        case LOPE_NAND   :
            lc = let_optimize1(let->le_lc);
            rc = let_optimize1(let->le_rc);
            if        ( IsTrue(lc)  && IsTrue(rc) ) { let_free(lc); let_free(rc); return let_newCst(0);
            } else if ( IsFalse(lc) || IsFalse(rc)) { let_free(lc); let_free(rc); return let_newCst(1);
            } else if ( IsTrue(lc) )                { let_free(lc); return let_newNot(rc);
            } else if ( IsTrue(rc) )                { let_free(rc); return let_newNot(lc);
            } else                                     return let_newBinOp(let->le_kd,lc,rc);
        default:
            BUG_ftl("%d is an invalid kind.\n",let->le_kd);
    }
}

extern TleTree* let_oaln     (CleTree* let)
{
    TleTree *child,*lc,*rc,*ret;
    switch (let->le_kd) {
        case LOPE_TRUE  : return let_newClone(let);
        case LOPE_FALSE : return let_newClone(let);
        case LOPE_VAR   : return let_newClone(let);
        case LOPE_NOT   :
            child = let->le_lc;
            switch (child->le_kd) {
                case LOPE_TRUE  : return let_newCst(0);
                case LOPE_FALSE : return let_newCst(1);
                case LOPE_VAR   : return let_newClone(let);
                case LOPE_NOT   : return let_oaln(child->le_lc);
                case LOPE_XOR   : let=child; goto do_nxor;
                case LOPE_OR    : let=child; goto do_nor;
                case LOPE_AND   : let=child; goto do_nand;
                case LOPE_NXOR  : let=child; goto do_xor;
                case LOPE_NOR   : let=child; goto do_or;
                case LOPE_NAND  : let=child; goto do_and;
                default:
                  BUG_ftl("%d is an invalid kind.\n",child->le_kd);
            }
do_or: case LOPE_OR    :
            lc = let->le_lc; rc = let->le_rc;
            return let_newBinOp(LOPE_OR,let_oaln(lc), let_oaln(rc));
do_xor: case LOPE_XOR    : // !(l^r) = l*!r + !l*r
            lc = let->le_lc; rc = let->le_rc;
            return let_newBinOp(LOPE_OR,
                     let_newBinOp(LOPE_AND,let_oaln(lc),
                                           let_oaln(let_newNot(rc))),
                     let_newBinOp(LOPE_AND,let_oaln(let_newNot(lc)),
                                           let_oaln(rc)));
do_and: case LOPE_AND   :
            lc = let->le_lc; rc = let->le_rc;
            return let_newBinOp(LOPE_AND,let_oaln(lc), let_oaln(rc));
do_nor: case LOPE_NOR   : // !(l+r) = !l * !r
            lc = let->le_lc; rc = let->le_rc;
            return let_newBinOp(LOPE_AND,let_oaln(let_newNot(lc)),
                                         let_oaln(let_newNot(rc)));
do_nxor: case LOPE_NXOR  : // !(l^r) = l*r + !l*!r
            lc = let->le_lc; rc = let->le_rc;
            return let_newBinOp(LOPE_OR,
                     let_newBinOp(LOPE_AND,let_oaln(lc),let_oaln(rc)),
                     let_newBinOp(LOPE_AND,let_oaln(let_newNot(lc)),
                                           let_oaln(let_newNot(rc))));
do_nand: case LOPE_NAND  : // !(l*r) = !l + !r
            lc = let->le_lc; rc = let->le_rc;
            return let_newBinOp(LOPE_OR,let_oaln(let_newNot(lc)),
                                        let_oaln(let_newNot(rc)));
        default:
            BUG_ftl("%d is an invalid kind.\n",let->le_kd);
    }
}

static void     let_printFP0 (CleTree*let, CvarSet* vs, int root)
{
    Cstr opestr;
    switch (let->le_kd) {
        case LOPE_TRUE  : printf("1"); return;
        case LOPE_FALSE : printf("0"); return;
        case LOPE_VAR   : printf("%c",vs->eles[let->le_var].name); return;
        case LOPE_NOT   :
            printf("!");
            if ( IsBinNode(let->le_lc) ) {
                printf("(");
                let_printFP0(let->le_lc,vs,0);
                printf(")");
            } else
                let_printFP0(let->le_lc,vs,0);
            return;
        case LOPE_XOR   : opestr="^";  goto do_binope;
        case LOPE_OR    : opestr="+";  goto do_binope;
        case LOPE_AND   : opestr="*";  goto do_binope;
        case LOPE_NXOR  : opestr="!^"; goto do_binope;
        case LOPE_NOR   : opestr="!+"; goto do_binope;
        case LOPE_NAND  : opestr="!*"; goto do_binope;
        default:
            BUG_ftl("%d is an invalid kind.\n",let->le_kd);
    }
    return;
do_binope:
    if (!root) printf("(");
    // ...
    NYI_ftl();
    // ...
    if (!root) printf(")");
}
extern void     let_printFP  (CleTree*let, CvarSet* vs) { let_printFP0(let,vs,1); }

static void let_printMP0(CleTree*lt, CleTree*let, CleTree*rt,CvarSet* vs)
{
    Cstr opestr;
    switch (let->le_kd) {
        case LOPE_TRUE  : printf("1"); return;
        case LOPE_FALSE : printf("0"); return;
        case LOPE_VAR   : printf("%c",vs->eles[let->le_var].name); return;
        case LOPE_NOT   :
            printf("!");
            if ( IsBinNode(let->le_lc) ) {
                printf("(");
                let_printMP0(NULL,let->le_lc,NULL,vs);
                printf(")");
            } else
                let_printMP0(NULL,let->le_lc,NULL,vs);
            return;
        case LOPE_XOR   : opestr="^"; goto do_binope;
        case LOPE_OR    : opestr="+"; goto do_binope;
        case LOPE_AND   : opestr="*"; goto do_binope;
        case LOPE_NXOR  : opestr="!^"; goto do_binope;
        case LOPE_NOR   : opestr="!+"; goto do_binope;
        case LOPE_NAND  : opestr="!*"; goto do_binope;
        default:
            BUG_ftl("%d is an invalid kind.\n",let->le_kd);
    }
    return;
do_binope:
    if ( PriNode(lt)>PriNode(let) || PriNode(rt)>PriNode(let) ||
                  ( PriNode(rt)==PriNode(let) && ! IsAssNode(rt) ) ) {
        printf("(");
        let_printMP0(NULL,let->le_lc,let,vs);
        printf("%s",opestr);
        let_printMP0(let,let->le_rc,NULL,vs);
        printf(")");
    } else {
        let_printMP0(lt,let->le_lc,let,vs);
        printf("%s",opestr);
        let_printMP0(let,let->le_rc,rt,vs);
    }
}
extern void     let_printMP(CleTree*let, CvarSet* vs) { let_printMP0(NULL,let,NULL,vs); }

// *node: - as input  it is the dot node identifier of the let node
//          and id > *node are free dot node identifier.
//        - as output id >= *node are free dot node identifier.
static void let_printDot0(CleTree*let, CvarSet* vs, int* node, FILE*stream)
{
#define printnode(id,lab)       fprintf(stream,"  %d [label=\"%s\"]\n",id,lab)
#define printarrow(idp,idc,lab) fprintf(stream,"  %d -> %d [label=\"%s\"]\n",idp,idc,lab)
    int letid = *node; *node += 1;
    Cstr opestr; char buf[2];
    switch (let->le_kd) {
        case LOPE_TRUE  : printnode(letid,"1"); return;
        case LOPE_FALSE : printnode(letid,"0"); return;
        case LOPE_VAR   : buf[0]=vs->eles[let->le_var].name; buf[1]=0;
                          printnode(letid,buf); return;
        case LOPE_NOT   : printnode(letid,"!");
                          printarrow(letid,*node,"");
                          let_printDot0(let->le_lc,vs,node,stream);
                          return;
        case LOPE_XOR   : opestr="^"; goto do_binope;
        case LOPE_OR    : opestr="+"; goto do_binope;
        case LOPE_AND   : opestr="*"; goto do_binope;
        case LOPE_NXOR  : opestr="!^"; goto do_binope;
        case LOPE_NOR   : opestr="!+"; goto do_binope;
        case LOPE_NAND  : opestr="!*"; goto do_binope;
do_binope:
                          printnode(letid,opestr);
                          printarrow(letid,*node,"l");
                          let_printDot0(let->le_lc,vs,node,stream);
                          printarrow(letid,*node,"r");
                          let_printDot0(let->le_rc,vs,node,stream);
                          return;
        default:
            BUG_ftl("%d is an invalid kind.\n",let->le_kd);
    }
}
extern void     let_printDot(CleTree*let, CvarSet* vs, FILE*stream)
{
    int node=1;
    fprintf(stream,"digraph logic {\n");
    let_printDot0(let,vs,&node,stream);
    fprintf(stream,"}\n");
}

/*======================================================================*/
