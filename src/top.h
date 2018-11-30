/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: top.h 456 2018-09-20 14:50:36Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/top.h $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/
#ifndef FILE_TOP_H
#define FILE_TOP_H
/*======================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*======================================================================*/
/*= Some top macros and type.                                          =*/
#define LE_DEBUG 1  // 0:no debug, 1:debug enabled

#define LE_VarNb     16 // the maximum number of parameters for a function
#define LE_VarNameNb 26 // the number of parameter name
#define LE_PtsSize   (1<<LE_VarNb) // the point number in point set

#define Inline static inline
typedef       char* Tstr;
typedef const char* Cstr;

// Memory allocation handler (MAH)
// The main purpose of MAH is to track dynamic memory allocation (eg: malloc)
// for making  automatically the corresponding frees in emergency cases
// such as exception throw.
// MAH is either enabled or disabled so the standard use is:
//   // MAH is disabled
//   mah_malloc() ... mah_free() ... // standard malloc & free
//   try {
//      mah_open();
//      mah_malloc() ... mah_free() ...
//      mah_forget(); // forget pending frees
//      mah_close();
//   } catch( ... ) {
//      mah_close(); // frees all the pending frees
//   }
//   // MAH is disabled
//   mah_malloc() ... mah_free() ... // standard malloc & free
//
// mah_open:    It enables MAH. It assumes MAH is disabled.
// mah_close:   It frees all the pending frees and then disables MAH.
//              If assumes that MAH is enabled.
// mah_forget:  If MAH is enabled, it forgets all the pending frees,
//              leaving MAH enabled.
//              If MAH is disabled, it does nothing.
//
// mah_malloc:  If MAH is disabled, it is similar to malloc.
//              If MAH is enabled,  it performs "ptr=malloc(n)"
//              and then it stores ptr as pending free into MAH.
// mah_malloc0: Same as mah_malloc but set allocated memory to zero
// mah_realloc: If MAH is disabled, it is similar to realloc.
//              If MAH is enabled,  it performs "ptr=realloc(p,n)"
//              and then it updates the pending free set using ptr and p.
// mah_free:    Like free but if MAH is enabled, it suppresses p
//              of the MAH pending free set.
//
extern void  mah_open   ();
extern void  mah_close  ();
extern void  mah_forget ();

extern void* mah_malloc (int n);
extern void* mah_malloc0(int n);
extern void* mah_realloc(void* p, int n);
extern void  mah_free   (void* p);
extern char* mah_strdup (Cstr str);

// debug
#define BUG_ftl(...) do { bug_printf(0,__FILE__,__LINE__,__func__,__VA_ARGS__); } while (0)
#define BUG_err(...) do { bug_printf(1,__FILE__,__LINE__,__func__,__VA_ARGS__); } while (0)
#define BUG_wrn(...) do { bug_printf(2,__FILE__,__LINE__,__func__,__VA_ARGS__); } while (0)
#define BUG_inf(...) do { bug_printf(3,__FILE__,__LINE__,__func__,__VA_ARGS__); } while (0)
#define NYI_ftl()    do { bug_printf(0,__FILE__,__LINE__,__func__,"not yet implemented\n"); } while (0)
#define NYI_err()    do { bug_printf(1,__FILE__,__LINE__,__func__,"not yet implemented\n"); } while (0)
// level:  0=ftl=fatal 1=err=error 2=wrn=warning
extern void bug_printf(int level,Cstr file,int lineno,Cstr func,Cstr fmt,...);

/*======================================================================*/
/*= Type pre-definition.                                               =*/

// logic operators
typedef enum   _Tlope {
    LOPE_NONE = 0x00,
    LOPE_VAR  = 0x10, LOPE_TRUE  = 0x20, LOPE_FALSE = 0x30,
    LOPE_OR   = 0x41, LOPE_XOR   = 0x42, LOPE_AND   = 0x43,
    LOPE_NOR  = 0x44, LOPE_NXOR  = 0x45, LOPE_NAND  = 0x46,
    LOPE_NOT  = 0x87,
} Tlope;
Inline int lope_isBin(Tlope ope)         { switch (ope) { case LOPE_TRUE: case LOPE_FALSE: case LOPE_VAR:  case LOPE_NOT:  return 0; } return 1;}
Inline int lope_isAssociative(Tlope ope) { switch (ope) { case LOPE_XOR: case LOPE_OR: case LOPE_AND: return 1; } return 0;}
Inline int lope_priority(Tlope ope)      { return ope&0xf; }

// print options
typedef enum   _Tprtopt {
    POPT_LogExpr,          // print logic expression
    POPT_LogExprFP,        // print logic expression (Fully Parenthesized)
    POPT_LogExprPC,        // print logic expression simplified by constant
                           // propagation
    POPT_LogExprPCOA,      // print logic expression simplified by constant
                           // propagation + only OR/AND/NOT(V)
    POPT_LogExprDF,        // print logic expression under disjunctive form
                           //    (abc + !a!bc + ...)
    POPT_TTable,           // print truth table
    POPT_TTableCF,         // print truth table without entries f(pt)=1
    POPT_TTableDF,         // print truth table without entries f(pt)=0
} Tprtopt;

// set of function parameters
typedef struct _TvarSet  TvarSet;
typedef const   TvarSet  CvarSet;

// logic expression tree
typedef struct _TleTree  TleTree;
typedef const   TleTree  CleTree;
typedef struct _TletSet  TletSet;
typedef const   TletSet  CletSet;

// truth table of logic function
typedef struct _TtTable TtTable;
typedef const   TtTable CtTable;

// logic point  set
typedef struct _TpointSet TpointSet;
typedef const   TpointSet CpointSet;

// logic function (variable set, tree, truth table)
typedef struct _Tlfunc  Tlfunc;
typedef const   Tlfunc  Clfunc;

// logic data (the entities that are currently defined)
typedef struct _Tldt  Tldt;
typedef const   Tldt  Cldt;

/*======================================================================*/
/*= top functions.                                                     =*/

extern Cstr  gl_progname;  // base name of running program.
extern int   gl_msgLevel;  // 0:print only messages at error level (yywarning is quiet)
                           // 1:print only messages at error and warning level
extern int   gl_dbgLevel;  // 0:enable debug messages at level error & fatal
                           // 1:enable debug messages at level error & fatal & warning
                           // 2:enable debug messages at level error & fatal & warning & info

extern int   gl_compiler;  // 1:compiler 0:interpreter
extern void* gl_throwout;  // lonjmp data that branches back to yyparse caller
extern Tldt* gl_ldt;       // the current data of logic compiler/interpreter 

// lex/yacc standard function
//
// yyerror:
//   gds->throwout==0 --> exit(1)
//   gds->throwout!=0 --> throw to catch of caller
//
// yyparse:
//   gds->throwout==0 --> return 0 or 2 on success
//                        exit(1) on error
//   gds->throwout!=0 --> return 0 or 2 on success
//                        throw back to caller on error
//   with 0:end of input stream
//        2:found an "end" instruction
//
extern int  yyparse(); // return 0:EOF 2:quit 1:error
extern void yylex_setStreamToMem (Cstr  stream);
extern void yylex_setStreamToFile(FILE* stream);
extern int  yylex();
extern void yyerror  (const char*fmt,...);
extern void yywarning(const char*fmt,...);

/*======================================================================*/
/*= Generic set.                                                       =*/

#define SET_FIELD(t) \
    int   eleMemNb; /* element number of the memory block eles. */\
    int   eleNb;    /* element number of set (eleNb<=eleMemNb) */\
    t*    eles      /* the set elements */

typedef struct _Tset { SET_FIELD(char); } Tset;

extern void* set_new();
extern void* set_newClone(const void* src, int sze);
extern void  set_free  (void*s);
extern void  set_add   (void*s, void*e,         int sze);
extern void  set_addSet(void*s, const void*src, int sze);

/*======================================================================*/
/*= Generic pointer set.                                               =*/

Inline void* ptrset_new()                             { return set_new(); }
extern void  ptrset_free(void*s, void*free);
Inline void  ptrset_add (void*s, void*e)              { set_add(s,&e,sizeof(void*)); }

/*======================================================================*/
/*= char set.                                                          =*/

typedef struct _TcharSet { SET_FIELD(char); } TcharSet;
typedef const   TcharSet CcharSet;

Inline TcharSet* charset_new     ()                        { return set_new(); }
Inline TcharSet* charset_newClone(CcharSet*src)            { return set_newClone(src,1); }
Inline void      charset_free    (TcharSet*s)              { set_free(s); }
Inline void      charset_add     (TcharSet*s, char e)      { set_add(s,&e,1); }
Inline void      charset_addN    (TcharSet*s, char e, int n)   { while (n-->0) charset_add(s,e); }
Inline void      charset_addSet  (TcharSet*s, CcharSet* src)   { set_addSet(s,src,1); }
Inline void      charset_addSetN (TcharSet*s, CcharSet* src, int n) { while (n-->0) charset_addSet(s,src); }

/*======================================================================*/
/*= string set.                                                        =*/
/*=                                                                    =*/
/*= TstrSet is a set of memory allocated string.                       =*/
/*=  strset_free:  frees all the set strings and the set data.         =*/ 
/*=  strset_free2: frees only the set structures and returns an array  =*/
/*=                of the set strings.                                 =*/
/*=  strset_addClone: appends a clone of the string.                   =*/
/*=  strset_addEaten: appends the string, strset_free will free it.    =*/
/*=  strset_addNull:  appends the a null pointer to the set without    =*/
/*=                changing the set string number.                     =*/
/*=                It allows set->eles to be and strset_free2 to       =*/
/*=                a null terminated array of strings.                 =*/

typedef struct _TstrSet { SET_FIELD(char*); } TstrSet;
typedef const   TstrSet CstrSet;

Inline TstrSet* strset_new ()                          { return ptrset_new(); }
extern TstrSet* strset_newClone (CstrSet*src);
Inline void  strset_free     (TstrSet*s)               { ptrset_free(s,mah_free); }
Inline char**strset_free2    (TstrSet*s)               { char** strs=s->eles; mah_free(s); return strs; }
Inline void  strset_addEaten (TstrSet*s, Tstr e)       { ptrset_add(s,e); }
Inline void  strset_addClone (TstrSet*s, Cstr e)       { strset_addEaten(s,mah_strdup(e)); }
Inline void  strset_addNull  (TstrSet*s)               { Tstr e=NULL; strset_addEaten(s,e); s->eleNb-=1; }

/*======================================================================*/
#endif // FILE_TOP_H
/*======================================================================*/
