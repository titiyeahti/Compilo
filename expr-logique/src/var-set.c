/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: var-set.c 456 2018-09-20 14:50:36Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/var-set.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/

#include "var-set.h"

/*======================================================================*/

extern void vs_dbg   (CvarSet* vs, Cstr topmess)
{
    printf("dbg:%s: ",topmess ? topmess : "vs");
    if ( vs==0) {
        printf("NULL pointer\n");
        return;
    }
    printf("sorted=%d NB=%d",vs->sorted,vs->eleNb);
    int i;
    for (i=0;i<vs->eleNb;i+=1)
        printf(" (%d,%d:%c)",vs->eles[i].idx,vs->eles[i].name,vs->eles[i].name);
    printf("\n");
}

/*======================================================================*/

extern TvarSet* vs_new   ()             { return mah_malloc0(sizeof(TvarSet)); }
extern TvarSet* vs_clone (CvarSet* vs)  { TvarSet* clone = vs_new(); *clone=*vs; return clone;; }
extern void     vs_free  (TvarSet* vs)  { mah_free(vs); }

/*======================================================================*/

extern int      vs_getNameOfIdx(CvarSet*vs, int idx)
{
    if ( vs->sorted )
        return idx<vs->eleNb ? vs->eles[idx].name : -1;
    else {
        int i;
        for (i=0; i<vs->eleNb ; i+=1) {
            if ( vs->eles[i].idx==idx )
                return vs->eles[i].name;
        }
    }
    return -1;
}

extern int      vs_getIdxOfName(CvarSet*vs, int name)
{
    int i;
    for (i=0; i<vs->eleNb ; i+=1) {
        if ( vs->eles[i].name==name )
            return vs->eles[i].idx;
    }
    return -1;
}

extern void vs_add         (TvarSet*vs, int idx, int name)
{
#if defined(LE_DEBUG) && LE_DEBUG
    if ( vs->eleNb==LE_VarNb )
        BUG_ftl("already %d variables in set.\n",vs->eleNb);
#endif
    vs->eles[vs->eleNb].idx  = idx;
    vs->eles[vs->eleNb].name = name;
    vs->eleNb               += 1;
    vs->sorted = 0;
}

extern void     vs_setIdx      (TvarSet*vs)
{
    int i;
    for (i=0; i<vs->eleNb ; i+=1) {
        vs->eles[i].idx=i;
    }
    vs->sorted = 1;
}
                                
static int cmp_byName(const void *l, const void* r)
{ CvarEle* lv = l; CvarEle* rv = r; return lv->name-rv->name; }

extern TvarSet* vs_sortByName  (TvarSet*vs)
{
    vs->sorted = 0;
    qsort(vs->eles,vs->eleNb,sizeof(vs->eles[0]),cmp_byName);
}

extern char* vs_print(char* buf, TvarSet*vs)
{
    int i;
    int pos=0;
    buf[pos++]='(';
    for (i=0; i<vs->eleNb ; i+=1) {
        if (i>0 ) buf[pos++]=',';
        buf[pos++]=vs->eles[i].name;
    }
    buf[pos++]=')';
    buf[pos]=0;
    return buf;
}
/*======================================================================*/
