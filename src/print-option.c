/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: print-option.c 456 2018-09-20 14:50:36Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/print-option.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/

#include "print-option.h"

/*======================================================================*/

extern TprtoptSet* prtoptset_newClone  (CprtoptSet*src)
{
    int i;
    TprtoptSet* s = set_newClone(src,sizeof(TprtoptEle));
    for ( i=0 ; i<s->eleNb ; i+=1 ) {
        if ( s->eles[i].name!=0 ) s->eles[i].name = mah_strdup( s->eles[i].name);
    }
}

extern void        prtoptset_free      (TprtoptSet*s)
{
    int i;
    for ( i=0 ; i<s->eleNb ; i+=1 ) {
        if ( s->eles[i].name!=0 ) mah_free( s->eles[i].name );
    }
    set_free(s);
}

extern void        prtoptset_addSet    (TprtoptSet*s, CprtoptSet*src)
{
    int i,start = s->eleNb;
    set_addSet(s,src,sizeof(TprtoptEle));
    for ( i=0 ; i<src->eleNb ; i+=1 ) {
        if ( s->eles[start+i].name==0 ) continue; 
        s->eles[start+i].name = mah_strdup( s->eles[start+i].name);
    }
}

/*======================================================================*/

