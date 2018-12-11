/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: top.c 456 2018-09-20 14:50:36Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/top.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/

#include <stdarg.h>

#include "top.h"

/*======================================================================*/
/*= Memory allocation.                                                 =*/

/*= Data structure.                                                    =*/
typedef struct _TmahSet { SET_FIELD(void*); } TmahSet;

extern TmahSet* mahset_new  ()           { TmahSet*s=malloc(sizeof(*s)); memset(s,0,sizeof(*s)); return s;}
extern void     mahset_delete(TmahSet*s) { int i; if (s->eleNb) BUG_wrn("there are %d pending frees\n",s->eleNb);
                                           for (i=0 ; i<s->eleNb ; i+=1) free(s->eles[i]);
                                           free(s->eles); free(s); }
Inline void  mahset_clr  (TmahSet*s)     { s->eleNb=0; }
Inline void  mahset_add  (TmahSet*s,void*ptr)
{   if (ptr==0) return;
    if ( s->eleNb==s->eleMemNb ) {
        s->eleMemNb += s->eleMemNb>20 ? s->eleMemNb/10 : 2;
        s->eles = realloc(s->eles, s->eleMemNb*sizeof(void*));
    }
    s->eles[s->eleNb]  = ptr;
    s->eleNb += 1;
}
static void  mahset_sup  (TmahSet*s,void*ptr)
{
    int i;
    for ( i=0 ; i<s->eleNb ; i++) {
        if ( ptr==s->eles[i] ) {
            s->eles[i] = s->eles[s->eleNb-1];
            s->eleNb -=1;
            return;
        }
    }
}
static void  mahset_chg (TmahSet*s,void*oldptr, void*newptr)
{
    int i;
    for ( i=0 ; i<s->eleNb ; i++) {
        if ( s->eles[i]==oldptr ) {
            s->eles[i]=newptr;
            return;
        }
    }
}

/*= Interface.                                                         =*/
static TmahSet* mah;
extern void  mah_open   ()   { if (mah!=0) BUG_ftl("MAH is enabled\n"); mah = mahset_new(); }
extern void  mah_close  ()   { if (mah==0) BUG_ftl("MAH is disabled\n"); mahset_delete(mah); mah=0; }
extern void  mah_forget ()   { if (mah!=0) mahset_clr(mah); }

extern void* mah_malloc (int n)
{
    void *ret = malloc(n);
    if (mah!=0) mahset_add(mah,ret);
    return ret;
}

extern void* mah_malloc0(int n)
{
    void *ret = mah_malloc(n);
    memset(ret,0,n);
    return ret;
}

extern void* mah_realloc(void* p, int n)
{
    if ( p==0 ) return mah_malloc(n);
    void* newptr = realloc(p,n);
    if (mah!=0 && p!=newptr) mahset_chg(mah,p,newptr);
    return newptr;
}

extern void  mah_free   (void* p)
{
    if ( p==0 ) return;
    if ( mah!=0 ) mahset_sup(mah,p);
    free(p);
}

extern char* mah_strdup (Cstr str)
{
    if (str==0) return 0;
    char* ptr=strdup(str);
    if ( mah!=0 ) mahset_add(mah,ptr);
    return ptr;
}

/*======================================================================*/
/*= Debug.                                                             =*/

extern void bug_printf(int level,Cstr file,int lineno,Cstr func,Cstr fmt,...)
{
    if ( (level==2 && gl_dbgLevel<=0) ||
         (level==3 && gl_dbgLevel<=1) ) return;
    fprintf(stderr,"%s:%d:%s:%s: ", file,lineno,func,
            level==0 ? "fatal" : level==1 ? "error" : level==2 ? "warning" : "info");
    va_list va;
    va_start(va,fmt);
    vfprintf(stderr,fmt,va);
    va_end(va);
    if (level==0) exit(1);
}

/*======================================================================*/
/*= Generic set.                                                       =*/

extern void* set_new() { Tset* s=mah_malloc0(sizeof(*s)); return s; }

extern void* set_newClone(const void* _src, int sze)
{
    const Tset* src = _src;
    Tset* s=mah_malloc0(sizeof(*s));
    s->eleMemNb = src->eleNb;
    s->eleNb    = src->eleNb;
    s->eles     = mah_malloc( s->eleNb*sze );
    memcpy(s->eles,src->eles,s->eleNb*sze);
    return s;
}

extern void  set_free  (void*_s) { Tset*s=_s; if (s==0) return; mah_free(s->eles); mah_free(s); }

extern void  set_add   (void*_s, void*e,         int sze)
{
    Tset* s = _s;
    if ( s->eleNb==s->eleMemNb ) {
        s->eleMemNb += s->eleMemNb>20 ? s->eleMemNb/10 : 2;
        s->eles = mah_realloc(s->eles, s->eleMemNb*sze);
    }
    memcpy(s->eles+s->eleNb*sze,e,sze);
    s->eleNb += 1;
}

extern void  set_addSet(void* _s, const void* _src, int sze)
{
    Tset* s = _s;
    const Tset* src = _src;
    if ( (s->eleMemNb-s->eleNb)<=src->eleNb ) {
        s->eleMemNb += s->eleNb + src->eleNb;
        s->eles = mah_realloc(s->eles, s->eleMemNb*sze);
    }
    memcpy(s->eles+s->eleNb*sze,src->eles,sze*src->eleNb);
    s->eleNb += src->eleNb;
}

/*======================================================================*/
/*= Generic pointer set.                                               =*/

extern void  ptrset_free(void*_s, void*_free)
{
    Tset* s = _s;
    if (s==0) return;
    if ( _free!=0 && s->eleNb!=0 ) {
        int i;
        void (*free)(void*) = _free;
        void **eles = (void**)s->eles;
        for ( i=0 ; i<s->eleNb ; i+=1) free(eles[i]);
    }
    set_free(_s);
}

/*======================================================================*/
/*= char set.                                                          =*/

/*======================================================================*/

