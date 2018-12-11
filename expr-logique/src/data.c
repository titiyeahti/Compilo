/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: data.c 456 2018-09-20 14:50:36Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/data.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/

#define _GNU_SOURCE
#include <search.h>

#include "data.h"
#include "logic-func.h"
#include "point-set.h"

/*======================================================================*/

// un symbol
typedef struct _Tsymb {
    char* name;
    void* data;
} Tsymb;
typedef const Tsymb Csymb;

static void sym_free_lfunc(Tsymb*s)
{
    mah_free   (s->name);
    lf_free    (s->data);
    mah_free   (s);
}

static void sym_free_pts(Tsymb*s)
{
    mah_free   (s->name);
    pts_free   (s->data);
    mah_free   (s);
}

static int symb_cmp(const void*l,const void*r)
{ return strcmp(((Tsymb*)l)->name,((Tsymb*)r)->name); }

static Tsymb* symbs_search(void** s, const char*n)
{
    Tsymb   key = { (char*)n, NULL };
    Tsymb** ret = (Tsymb**)tfind(&key,s,symb_cmp);
    return ret==0 ? 0 : *ret;
}

static void  symbs_add(void** s, Cstr n, void* ptr, void (*free_ptr)(void*))
{
    Tsymb* symb=symbs_search(s,n);
    if (symb!=0) {
        if (symb->data!=0 && free_ptr!=0 ) free_ptr(symb->data);
        symb->data = ptr;
        return;
    }
    symb = mah_malloc(sizeof(*symb));
    symb->name = mah_strdup(n);
    symb->data = ptr;
    tsearch(symb,s,symb_cmp);
}

/*======================================================================*/

extern Tldt* ldt_new()
{
    Tldt* ldt = mah_malloc0(sizeof(*ldt));
    return ldt;
}

extern void ldt_delete(Tldt* ldt)
{
    tdestroy(ldt->lfuncSet,(void (*)(void*))sym_free_lfunc);
    tdestroy(ldt->ptsSet  ,(void (*)(void*))sym_free_pts);
    mah_free(ldt);
}

/*======================================================================*/

extern Tlfunc*    ldt_getFuncByName(Tldt*ldt, Cstr n)
{
    Tsymb* symb = symbs_search(&ldt->lfuncSet,n);
    return symb==0 ? NULL : symb->data;
}
extern void       ldt_addChgFunc   (Tldt*ldt, Cstr n, Tlfunc* fct)
{ symbs_add(&ldt->lfuncSet,n,fct,(void(*)(void*))lf_free); }

extern TpointSet* ldt_getPtsByName (Tldt*ldt, Cstr n)
{
    Tsymb* symb = symbs_search(&ldt->ptsSet,n);
    return symb==0 ? NULL : symb->data;
}
extern void       ldt_addChgPts    (Tldt*ldt, Cstr n, TpointSet* pts)
{ symbs_add(&ldt->ptsSet,n,pts,(void(*)(void*))pts_free); }

/*======================================================================*/

static TstrSet* addnames_set;
static void addnames_action(const void *node, const VISIT which, const int depth)
{
    if ( which==leaf || which==postorder ) {
        Csymb*  symb  = *(void**)node;
        strset_addClone(addnames_set,symb->name);
    }
}
extern void       ldt_addFuncNames(Cldt*ldt,TstrSet* strs) { addnames_set=strs; twalk(ldt->lfuncSet,addnames_action); }
extern void       ldt_addPtsNames (Cldt*ldt,TstrSet* strs) { addnames_set=strs; twalk(ldt->ptsSet  ,addnames_action); }

/*======================================================================*/
