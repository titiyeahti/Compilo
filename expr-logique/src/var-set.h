/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: var-set.h 456 2018-09-20 14:50:36Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/var-set.h $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/
#ifndef FILE_VAR_SET_H
#define FILE_VAR_SET_H
/*======================================================================*/
/*= Handler for function parameters.                                   =*/
/*======================================================================*/

#include "top.h"

/*======================================================================*/
/*= Data structure of parameters of logic function.                    =*/

typedef struct _TvarEle {
    char idx;    // index used in TleTree for variable nodes.
    char name;   // name is the letter of the parameter
} TvarEle;
typedef const TvarEle CvarEle;

struct _TvarSet {
    TvarEle eles[LE_VarNb];
    int     eleNb;  // number of element in eles
    char    sorted; // 0:no sorted 1:sorted by idx & eles[i].idx==i
};

// dbg:topmess: nb=N (i0,n0) ...
extern void vs_dbg   (CvarSet* vs, Cstr topmess);

/*= Constructors/destructor                                            =*/

extern TvarSet* vs_new   ();
extern TvarSet* vs_clone (CvarSet* vs);
extern void     vs_free  (TvarSet* vs);

/*= feature                                                            =*/

/**
 * The vs_getNameOfIdx function searches for an element (idx,*) of the
 * vs variable and returns its name part.
 * The vs_getIdxOfName function searches for an element (*,name) of the
 * vs variable and returns its idx part.
 * Caution: if the vs set contains several (idx,*) or (*,name) element,
 * the result of the vs_getNameOfIdx and vs_getIdxOfName functions
 * is unpredictable.
 *
 * The vs_add function appends to the vs variable set the (idx,name)
 * elements. It leaves the set unsorted.
 * The vs_addUniq function searches for a (idx,*) elements in the vs
 * variable set.
 *  - If a such element is found, it does nothing.
 *  - If the search fails, it adds (idx,name) to the set
 *    leaving the set unsorted.
 * CAUTION: when adding a variable ensure that the set is not full.
 *
 * The vs_setIdx function sets the idx of the i th element to i and
 * marks the set sorted.
 *
 * Returns:
 *  vs_getNameOfIdx returns the name (positive value) on success otherwise it returns -1.
 *  vs_getIdxOfName returns the index (positive value) on success otherwise it returns -1.
 *  vs_addUniq returns 1 if the variable is added to the set otherwise it returns 0.
**/
extern int      vs_getNameOfIdx(CvarSet*vs, int idx);
extern int      vs_getIdxOfName(CvarSet*vs, int name);
extern void     vs_add         (TvarSet*vs, int idx, int name);
Inline int      vs_addUniq     (TvarSet*vs, int idx, int name) { int ret=vs_getNameOfIdx(vs,idx)<0; if (ret) vs_add(vs,idx,name); return ret; }
extern void     vs_setIdx      (TvarSet*vs);
                                
extern TvarSet* vs_sortByName  (TvarSet*vs);

/**
 * The vs_print function writes in the buf buffer the string
 * "(v0,v1, ...)", vi being the name of the ith variable of the vs set.
 * The vs_print function always returns the buf pointer.
**/
extern char* vs_print(char* buf, TvarSet*vs);
/*======================================================================*/
#endif // FILE_VAR_SET_H
/*======================================================================*/

