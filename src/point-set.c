/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: point-set.c 456 2018-09-20 14:50:36Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/point-set.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/

#include "point-set.h"

/*======================================================================*/

extern void pts_dbg(CpointSet*pts, Cstr label)
{
    int pt,nb;
    printf("%10s:",label ? label : "");
    for ( nb=0,pt=0 ; nb<16 && pt<sizeof(pts->eles) ; pt++) {
        if ( pts->eles[pt] ) { printf(" %d",pt); nb+=1; }
    }
    printf("\n");
}

/*======================================================================*/

extern void       pts_addSet  (TpointSet*pts, CpointSet*src)
{
    int i;
    long* d = (long*)pts->eles;
    long* s = (long*)src->eles;
    for ( i=0 ; i<(sizeof(pts->eles)/sizeof(*d)) ; i++)
        *d |= *s;
}

static void       pts_addByEBT_0(TpointSet*pts, char*t, int nt, char*pt, int npt)
{
    if ( npt==nt ) {
        int i,pti=0;
        for (i=0 ; i<npt ; i+=1) if ( pt[i]=='1' ) pti += (1<<i);
        pts_add(pts,pti);
        return;
    } else {
        int i=npt;
        switch ( t[i] ) {
            case '0': pt[npt]='0'; pts_addByEBT_0(pts,t,nt,pt,npt+1); return;
            case '1': pt[npt]='1'; pts_addByEBT_0(pts,t,nt,pt,npt+1); return;
            case '*': pt[npt]='0'; pts_addByEBT_0(pts,t,nt,pt,npt+1);
                      pt[npt]='1'; pts_addByEBT_0(pts,t,nt,pt,npt+1); return;
            default:
              BUG_ftl("0x%02x (%c) char is an invalid Extented Boolean.\n",t[i],t[i]);
        }
    }
}

extern void       pts_addByEBT(TpointSet*pts, char*t, int nt)
{
#if defined(LE_DEBUG) && LE_DEBUG 
    if (nt>LE_VarNb ) BUG_ftl("EBT array is to large (found:%d, expected<=%d\n",nt,LE_VarNb);
#endif
    char tmp[LE_VarNb];
    pts_addByEBT_0(pts,t,nt,tmp,0);
}

/*======================================================================*/

