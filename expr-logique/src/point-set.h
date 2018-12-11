/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: point-set.h 456 2018-09-20 14:50:36Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/point-set.h $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/
#ifndef FILE_POINT_SET_H
#define FILE_POINT_SET_H
/*======================================================================*/
/*= Handler for definition sets.                                       =*/
/*======================================================================*/

#include "top.h"

/*======================================================================*/
/*= Data structure of point.                                           =*/
/*=                                                                    =*/
/*= A point is a vector of LE_VarNb boolean: ( b1, b2,... b(LE_VarNb ) =*/
/*=  b1 is the value of the first  variable (variable ident=0)         =*/
/*=  b2 is the value of the second variable (variable ident=1)         =*/
/*=  b(LE_VarNb) is the value of the last variable (LE_VarNb-1)        =*/
/*= A point is coded by an integer. Be bit(i) the i th bit of an       =*/
/*= integer point, we use the next conventions:                        =*/
/*=  - i>=LE_VarNb : bit(i) is zero                                    =*/
/*=  - i<LE_VarNb and bit(i) is zero : variable i is false.            =*/
/*=  - i<LE_VarNb and bit(i) is 1    : variable i is true.             =*/
/*= So the point vector is ( (pt&1)!=0, (pt&2)!=0, (pt&4)!=0, ...)     =*/

/*======================================================================*/
/*= Data structure of definition set.                                  =*/
/*=                                                                    =*/
/*= A point set is a collection of LE_PtsSize points.                  =*/

struct _TpointSet {
    char eles[LE_PtsSize]; // eles[pt]==0: pt is not in the set
                           // eles[pt]==1: pt is in the set
};

extern void pts_dbg(CpointSet*pts, Cstr label);  // label: true point

/*======================================================================*/
/*= feature                                                            =*/
/*=                                                                    =*/
/*= All the pts_add... function adds the elements if and only if they  =*/
/*= does not already exist.                                            =*/
/*=                                                                    =*/
/*= The pts_addByEBT function adds to the pts set the points that are  =*/
/*= defined by the Extented Boolean Table t[0],t[1,...t[nt-1].         =*/
/*=  - every t[i] is either '0' or '1' or '*'.                         =*/
/*=  - '*' means both 0 and 1, for instance, we have with nt=2:        =*/
/*=      t = (0,0) gives the point  (0,0, 0...)                        =*/
/*=      t = (1,0) gives the point  (1,0, 0...)                        =*/
/*=      t = (1,*) gives the points (1,0, 0...) , (1,1, 0...)          =*/
/*=      t = (*,*) gives the points (0,0, 0...) , (0,1, 0...) ,        =*/
/*=                                 (1,0, 0...) , (1,1, 0...)          =*/
/*= Ensure that nt is less or equal to LE_VarNb.                       =*/

Inline TpointSet* pts_new     ()                       { return mah_malloc0(sizeof(TpointSet)); }
Inline TpointSet* pts_newClone(CpointSet*src)          { TpointSet*pts=pts_new(); *pts=*src; return pts; }
Inline void       pts_free    (TpointSet*pts)          { mah_free(pts); }
Inline void       pts_add     (TpointSet*pts, int pt)  { pts->eles[pt]=1; }
extern void       pts_addSet  (TpointSet*pts, CpointSet*src); // pts = pts union src
extern void       pts_addByEBT(TpointSet*pts, char*t, int nt);

/*======================================================================*/
#endif // FILE_POINT_SET_H
/*======================================================================*/

