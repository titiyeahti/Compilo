/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: data.h 456 2018-09-20 14:50:36Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/data.h $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/
#ifndef FILE_DATA_H
#define FILE_DATA_H
/*======================================================================*/
/*= Current compiler/interpreter data.                                 =*/
/*=   - already defined logic functions.                               =*/
/*=   - already defined definition sets.                               =*/
/*======================================================================*/

#include "top.h"

/*======================================================================*/
/*= Data structure.                                                    =*/

struct _Tldt {
    void* lfuncSet;     // Defined logic functions (ABR by name)
    void* ptsSet;       // Defined definition setd (ABR by name)
};

/*======================================================================*/
/*= feature                                                            =*/

// constructors/destructor
extern Tldt* ldt_new();
extern void  ldt_delete(Tldt* dt);

/**
 * The ldt_getFuncByName function returns the "n" named function of 
 * the ldt data structure if it exists otherwise it returns the NULL
 * pointer.
 *
 * The ldt_addChgFunc function stores into the ldt data structure the
 * fct function under the "n" name.
 * If the "n" function already exists, it is overwritten. 
 * The "fct" function is eaten by the "ldt" data structure.
 *
 * The ldt_getPtsByName and ldt_addChgPts functions are similar to
 * the ldt_getFuncByName and ldt_addChgFunc functions except that they
 * work with point sets (TpointSet) instead of logic function (TlfuncSet).
**/
extern Tlfunc*    ldt_getFuncByName(Tldt*ldt, Cstr n);
extern void       ldt_addChgFunc   (Tldt*ldt, Cstr n, Tlfunc* fct);
extern TpointSet* ldt_getPtsByName (Tldt*ldt, Cstr n);
extern void       ldt_addChgPts    (Tldt*ldt, Cstr n, TpointSet* pts);

/**
 * The ldt_addFuncNames (resp: ldt_addPtsNames) function appends to
 * the strs string set, all the functions (resp: definition sets)
 * names that are defined in the ldt logic data.
**/
extern void     ldt_addFuncNames(Cldt*ldt,TstrSet* strs);
extern void     ldt_addPtsNames (Cldt*ldt,TstrSet* strs);
Inline TstrSet* ldt_getFuncNames(Cldt*ldt) { TstrSet* s=strset_new(); ldt_addFuncNames(ldt,s); return s; }
Inline TstrSet* ldt_getPtsNames (Cldt*ldt) { TstrSet* s=strset_new(); ldt_addPtsNames (ldt,s); return s; }

/*======================================================================*/
#endif // FILE_DATA_H
/*======================================================================*/
