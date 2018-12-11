/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: truth-table.c 456 2018-09-20 14:50:36Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/truth-table.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/

#include "truth-table.h"
#include "var-set.h"

/*======================================================================*/

extern int      tt_isSizeTwoPower(CtTable* tt)
{
    int i;
    for (i=1 ; i<=LE_VarNb ; i+=1) {
        if ( (1<<i)==tt->eleNb ) {
            return i;
        }
    }
    return 0;
}

extern TvarSet* tt_varSet        (CtTable* tt)
{
    int i, n=tt_isSizeTwoPower(tt);
    TvarSet* vs = vs_new();
    for (i=0 ; i<n ; i+=1) {
        int name = i + 'a';
        vs_add(vs,name,name);
    }
    return vs;
}

/*======================================================================*/

