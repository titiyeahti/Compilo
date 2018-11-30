/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: logic-expr-tree.h 457 2018-10-21 15:12:50Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/logic-expr-tree.h $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/
#ifndef FILE_LOGIC_EXPR_TREE_H
#define FILE_LOGIC_EXPR_TREE_H
/*======================================================================*/
/*= Handler for storing logic expression under a binary tree.          =*/
/*======================================================================*/

#include "top.h"

/*======================================================================*/
/*= Logic expression tree                                              =*/

/*= Data structure of a logic expression tree                          =*/

struct _TleTree {
    Tlope     le_kd;
    TleTree*  le_lc;   // left  child
    TleTree*  le_rc;   // right child
    int       le_var;  // - when buiding it is the variable name ('a', 'b'...)
                       // - once built,  it is the parameter number in the
                       //   parameter list of the function.
                       // ex: f(x,y) for a node y we have:
                       //     le_var='y' when building,
                       //     le_var=1 (2nd parameter) when built
};

/*= Constructors/destructor                                            =*/
/*=                                                                    =*/
/*= The let_newMinTerm function creates the minterm                    =*/
/*=   v0 * v1 * ... v(nvars-1)                                         =*/
/*= where vi is                the ith variable if (vars&(1<<i))!=0    =*/
/*= where vi is the the not of the ith variable if (vars&(1<<i))==0.   =*/
/*= The function returns the minterm tree.                             =*/

extern TleTree* let_newCst    (int cst);
extern TleTree* let_newVar    (int var);
extern TleTree* let_newNot    (TleTree* child);
extern TleTree* let_newBinOp  (Tlope kd, TleTree* lc, TleTree* rc);
extern TleTree* let_newMinTerm(int pt, int nvars);
extern TleTree* let_newClone  (CleTree* let);
extern void     let_free      (TleTree* let);

/*= features                                                           =*/

/**
 * The let_varSet function gets all the variables used in the "let"
 * logic expression tree and puts them into a variable set.
 *  - The elements of variable are set to (le_var,le_var).
 *  - All the elements of the variable set are different.
 *
 * The let_chgVar function sets to idxnew, all the idx variable
 * nodes of the "let" tree.
 *
 * The let_chgVarsIdxToName function performs let_chgVar(let,idx,name)
 * for all the (idx,name) elements of the vs set.
 * The let_chgVarsNameToIdx function performs let_chgVar(let,name,idx)
 * for all the (idx,name) elements of the vs set.
 *
 * The let_chgVarsIdxToTree function replaces every vn variable node
 * of the "let" tree by a clone of the vn->le_var th tree of the
 * "lets" tree set.
 *
 * Returns:
 *  let_varSet returns a varset. The caller must free it after use.
**/
extern TvarSet* let_varSet(CleTree* let);
extern void     let_chgVar(TleTree* let, int idx, int newidx);
extern void     let_chgVarsIdxToName(TleTree* let, CvarSet* vs);
extern void     let_chgVarsNameToIdx(TleTree* let, CvarSet* vs);
extern void     let_chgVarsIdxToTree(TleTree**let, CletSet* lets);

/**
 * The let_eval function evaluates the "let" logic expression tree
 * for the pt point ( "(pt&(1<<i))!=0" being the value of the ith variable.
 *
 * The let_optimize1 function returns a logic tree that implements the
 * same logic function as the let tree but
 *  - without NOT NOT, NOT NOR, NOT NXOR and NOT NAND sequences.
 *  - vithout boolean leaves except for the constant function.
 *
 * The let_oaln function returns a logic tree that implements the
 * same logic function as the let tree but vith only OR and AND operators,
 * and NOT operators of tree leaves.
 *
 * The let_printFP function prints into the stdout stream the logic expression
 * of the let tree. The vs sets is used for getting the name of the variables.
 * The expression is fully paranthesed except leaves (variable and boolean).
 *
 * The let_printMP function is similar to the let_printFP function
 * except that it minimizes the use of paranthesis paires.
 *
 * Returns:
 *  let_eval returns true (1) or false (0).
 *  let_optimize1 and let_oaln return a tree that must be freed after use.
**/
extern int      let_eval     (CleTree* let, int pt);
extern TleTree* let_optimize1(CleTree* let);
extern TleTree* let_oaln     (CleTree* let);
extern void     let_printFP  (CleTree*let, CvarSet* vs);
extern void     let_printMP  (CleTree*let, CvarSet* vs);
extern void     let_printDot (CleTree*let, CvarSet* vs, FILE*stream);

/*======================================================================*/
/*= Set of logic expression trees                                      =*/

/*= Data structure                                                     =*/

struct _TletSet { SET_FIELD(TleTree*); };

/*= features                                                           =*/
Inline TletSet* letset_new ()              { return ptrset_new(); }
Inline void     letset_free(TletSet* s)    { ptrset_free(s,let_free); }
Inline void     letset_addEaten(TletSet* s, TleTree* let) { ptrset_add(s,let); }
Inline void     letset_addClone(TletSet* s, CleTree* let) { letset_addEaten(s,let_newClone(let)); }

/*======================================================================*/
#endif // FILE_LOGIC_EXPR_TREE_H
/*======================================================================*/
