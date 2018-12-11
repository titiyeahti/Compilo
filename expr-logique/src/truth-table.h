/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: truth-table.h 456 2018-09-20 14:50:36Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/truth-table.h $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/
#ifndef FILE_TRUTH_TABLE_H
#define FILE_TRUTH_TABLE_H
/*======================================================================*/
/*= Handler for truth tables.                                          =*/
/*======================================================================*/

#include "top.h"

/*======================================================================*/
/*= Data structure.                                                    =*/
/*= Its a simple char set, element values being 0 or 1.                =*/

struct _TtTable { SET_FIELD(char); };

/*= Constructors/destructor                                            =*/
Inline TtTable* tt_new()                   { return set_new(); }
Inline TtTable* tt_newClone(CtTable* src)  { return set_newClone(src,sizeof(char)); }
Inline void     tt_free(TtTable*tt)        { set_free(tt); }

/*======================================================================*/
/*= Features.                                                          =*/

/**
 * The tt_varSet function builds the varset {(a,a),(b,b),(c,c)...},
 * the number of variables being tt_isSizeTwoPower(tt).
 * Caution: if tt->nbEle is not a 2 power such as 3, then it returns
 * an empty variable set.
 *
 * Returns:
 *  - tt_isSizeTwoPower returns n such as n>0 and n<=LE_VarNb
 *  - and 2**n=tt->eleNb otherwise it returns 0.
 *  - tt_varSet returns a varset. The caller must free it after use.
**/
extern int      tt_isSizeTwoPower(CtTable* tt);
extern TvarSet* tt_varSet        (CtTable* tt);
Inline void     tt_addBool       (TtTable* tt, int b)        { set_add(tt,&b,1); }
Inline void     tt_addBoolN      (TtTable* tt, int b, int n) { int i; for (i=0;i<n;i+=1) tt_addBool(tt,b); }
Inline void     tt_addTTable     (TtTable* tt, CtTable* src)        { set_addSet(tt,src,1); }
Inline void     tt_addTTableN    (TtTable* tt, CtTable* src, int n) { int i; for (i=0;i<n;i+=1) tt_addTTable(tt,src); }

/*======================================================================*/
#endif // FILE_TRUTH_TABLE_H
/*======================================================================*/
