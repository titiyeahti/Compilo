/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: print-option.h 456 2018-09-20 14:50:36Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/print-option.h $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/
#ifndef FILE_PRINT_OPTION_H
#define FILE_PRINT_OPTION_H
/*======================================================================*/
/*= Handler for print/graph options.                                   =*/
/*======================================================================*/

#include "top.h"

/*======================================================================*/
/*= Data structure.                                                    =*/

struct _TprtoptEle {
    Tprtopt option;
    char*   name;
};
typedef struct _TprtoptEle TprtoptEle;
typedef const   TprtoptEle CprtoptEle;

struct _TprtoptSet { SET_FIELD(TprtoptEle); };
typedef struct _TprtoptSet TprtoptSet;
typedef const   TprtoptSet CprtoptSet;

/*======================================================================*/
/*= feature                                                            =*/

Inline TprtoptSet* prtoptset_new       ()                                { return set_new(sizeof(TprtoptSet)); }
extern TprtoptSet* prtoptset_newClone  (CprtoptSet*src);
extern void        prtoptset_free      (TprtoptSet*s);
Inline void        prtoptset_add       (TprtoptSet*s, TprtoptEle*e)      { set_add(s,e,sizeof(*e)); }
Inline void        prtoptset_addONeaten(TprtoptSet*s, Tprtopt o, char*n) { TprtoptEle e={o,n}; prtoptset_add(s,&e); }
Inline void        prtoptset_addONclone(TprtoptSet*s, Tprtopt o, Cstr n) { prtoptset_addONeaten(s,o,mah_strdup(n)); }
extern void        prtoptset_addSet    (TprtoptSet*s, CprtoptSet*src);

/*======================================================================*/
#endif // FILE_PRINT_OPTION_H
/*======================================================================*/
