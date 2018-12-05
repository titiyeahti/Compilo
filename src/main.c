/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: main-eleve.c 456 2018-09-20 14:50:36Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/expr-logique/main-eleve.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/

/*======================================================================*/
/*= Template of logic main routines.                                   =*/
/*======================================================================*/

#include <unistd.h>
#include <libgen.h>
#include <setjmp.h>

#include "top.h"
#include "data.h"

// For enabling completion in logic interpreter, set it to 1.
#define WithCompletion 0

/*======================================================================*/
/*= Global variables                                                   =*/

const char* gl_progname;   // base name of running program.
int         gl_msgLevel;   // 0:do not print warnings
int         gl_dbgLevel=1; // 0:ftl+err, 1:ftl+err+wrn, 2::ftl+err+wrn+inf

int   gl_compiler;  // 1:compiler 0:interpreter
void* gl_throwout;  // lonjmp data that branches back to yyparse caller
Tldt* gl_ldt;       // the current data of logic compiler/interpreter 

/*======================================================================*/
/*= program argument                                                   =*/

// returns:
//   1: interactive mode
//   0: compiler    mode
static int main_getParam(int argc, char*argv[])
{
    int i,interactive=0;
    char *p = strdup(argv[0]);
    gl_progname = strdup(basename(p));
    free(p);

    // search for the help option
    for (i=1 ; i<argc ; i++) {
        if ( strcmp("-h",argv[i])==0 || strcmp("--help",argv[i])==0 ) {
            printf("usage: %s [options] asm-path\n\n",argv[0]);
            printf("   %-20s: this help\n"                                     , "-h --help");
            printf("   %-20s: force interactive mode (default auto-detected)\n", "-i");
            printf("   %-20s: print warnings \n"                               , "+v");
            printf("   %-20s: set debug level to 0 (default=1=error+warning)\n", "-d");
            printf("   %-20s: set debug level to 2 (default=1=error+warning)\n", "+d");
            exit(0);
        }
    }

    // search for other options
    for (i=1 ; i<argc ; i++) {
        char* opt= argv[i];
        if ( strcmp(opt,"-i")==0 ) {
            interactive = 1;
        } else if ( strcmp(opt,"+v")==0 ) {
            gl_msgLevel = 1;
        } else if ( strcmp(opt,"-d")==0 ) {
            gl_dbgLevel = 0;
        } else if ( strcmp(opt,"+d")==0 ) {
            gl_dbgLevel = 2;
        } else {
            fprintf(stderr,"%s: unrecognized option \"%s\".\n",argv[0],opt);
            fprintf(stderr,"%s: try --help option.\n",argv[0]);
            exit(1);
        }
    }

    // post treatment
    return interactive ? 1 : ttyname(0)!=0;
}

/*======================================================================*/
/*= main program                                                       =*/

static void  interpreter_init();
static char* interpreter_getLine();

int main(int argc, char*argv[])
{
    int interactive = main_getParam(argc,argv);

    // gl_ldt = ldt_new(); // inutile pour livrable 1
    if ( interactive==0 ) {
        gl_compiler = 1;
        yylex_setStreamToFile(stdin);
        NYI_ftl();
    } else {
        gl_throwout = malloc( sizeof(jmp_buf) );
        interpreter_init();
        char*line;
        while ( (line=interpreter_getLine()) ) {
            if (*line==0) continue; // empty line
            yylex_setStreamToMem(line);
            if ( setjmp(gl_throwout)==0 ) {
                int status;
                // ...
                status=yyparse();
				switch (status) {
					case 0 :
						return 0;
				}
				

                free(line);
BUG_inf("status=%d TRY\n",status);
                if ( status==2 ) break; // end instruction
            } else {
                // catch: yyparse failed by calling yyerror
                NYI_ftl();
                // ...
                free(line);
BUG_inf("status=%d CATCH\n",1);
            }
        }
        free( gl_throwout );
    }
    // ldt_delete(gl_ldt) // inutile pour livrable 1;
    free((char*)gl_progname);
    return 0;
}

/*======================================================================*/
/*= interpreter routines                                               =*/

#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

static char **character_name_completion(const char *, int, int);
static char history_file[2048];

static void  interpreter_init()
{
    int fd;
    if ( getenv("HOME")!=0 )
        sprintf(history_file,"%s/.logic.history",getenv("HOME"));
    else
        sprintf(history_file,"logic.history");
    if ( (fd=open(history_file,O_CREAT|O_WRONLY,0666))>=0 )
        close(fd);
    read_history(history_file);
#if WithCompletion
    rl_attempted_completion_function = character_name_completion;
#endif
}

static char* interpreter_getLine()
{
    char* line=0;
    char* more;
    line = readline ("logic> ");

    if (line==0 || *line==0) {
        if (line==0) printf("\n");
        return line;
    }

    int len = strlen(line);
    while ( line[len-1]=='\\' && (more=readline("     > "))!=0 ) {
        int len2 = strlen(more);
        line = realloc(line,len+len2);
        strcpy(line+len-1,more);
        len = len+len2-1;
    }
    
    add_history (line);
    append_history(1,history_file);
    history_truncate_file(history_file,100);

    line = realloc(line,len+2);
    line[len]    = '\n';
    line[len+1]  = 0;
    return line;
}

#if WithCompletion
/*======================================================================*/
/*= interpreter routines for completion                                =*/

// see comment in completion.l
#include "completionlex.c"

static char ** character_name_completion(const char *text, int start, int end)
{
//fprintf(stderr,"\n%s:E: text=%s start=%d end=%d\n",__func__,text,start,end);
//fprintf(stderr,  "%s:E: text=%s start=%d end=%d\n",__func__,rl_line_buffer,start,end);
    int i, expected, len = strlen(text);

  /* get the "expected" flags using the completion lex. */
    char* completion_line0;
    completion_line0 = malloc(strlen(rl_line_buffer)+10);
    strncpy(completion_line0,rl_line_buffer,end);
    completion_line0[end]='@';
    strcpy(completion_line0+end+1,rl_line_buffer+end);
//fprintf(stderr,  "%s:E: line0=<%s> start=%d end=%d\n",__func__,completion_line0,start,end);
    if ( *completion_line0==0 ) {
        expected = COMPLEX_COMMAND;
    } else {
        char *p;
        completion_line=completion_line0;
        // be sure that '@' is in the first LE command.
        for (p=completion_line ; *p!='@' ; p+=1 ) {
            if ( *p==';' ) // command without '@', we skip it.
                completion_line=p+1;
        }
        YY_FLUSH_BUFFER;
        expected = completionlex();
    }
    free(completion_line0);

  /* fill in the strs string set using the expected flags. */
    TstrSet* strs = strset_new();
    if ( (expected&COMPLEX_COMMAND)!=0 ) {
        static char *table[] = {
            "function", "fct", "def-set", "pts",
            "print", "out", "graph", "ls", NULL };
        for ( i=0 ; table[i]!=0 ; i+=1) {
            if (strncmp(table[i], text, len) == 0)
                strset_addClone(strs,table[i]);
        }
    }
    if ( (expected&COMPLEX_FCT_NAME)!=0 ) {
        TstrSet* names = strset_new(); ldt_addFuncNames(gl_ldt,names);
        for ( i=0 ; i<names->eleNb ; i+=1) {
            if (strncmp(names->eles[i], text, len) == 0) {
                strset_addClone(strs,names->eles[i]);
            }
        }
        strset_free(names);
    }
    if ( (expected&COMPLEX_PTS_NAME)!=0 ) {
        TstrSet* names = strset_new(); ldt_addPtsNames(gl_ldt,names);
        for ( i=0 ; i<names->eleNb ; i+=1) {
            if (strncmp(names->eles[i], text, len) == 0) {
                strset_addClone(strs,names->eles[i]);
            }
        }
        strset_free(names);
    }
    if ( (expected&COMPLEX_PRT_OPT)!=0 ) {
        static char *table[] = {
            "/le", "/fp", "/pc", "/oa", "/df",
            "/pc/le", "/oa/le", "/df/le",
            "/tt", "/df/tt", NULL };
        for ( i=0 ; table[i]!=0 ; i+=1) {
            if (strncmp(table[i], text, len) == 0)
                strset_addClone(strs,table[i]);
        }
    }
    if ( (expected&COMPLEX_GRA_OPT)!=0 ) {
        static char *table[] = {
            "/le", "/pc", "/oa", "/df",
            "/pc/le", "/oa/le", "/df/le",
            NULL };
        for ( i=0 ; table[i]!=0 ; i+=1) {
            if (strncmp(table[i], text, len) == 0)
                strset_addClone(strs,table[i]);
        }
    }

  /**
   * generate the return (see https://robots.thoughtbot.com/tab-completion-in-gnu-readline)
   *  - If there are no possible completions, we should return NULL.
   *  - If there is one possible completion, we should return an array
   *    containing that completion, followed by a NULL value.
   *  - If there are two or more possibilities, we should return an array
   *    containing the longest common prefix of all the options, followed
   *    by each of the possible completions, followed by a NULL value.
  **/
    char ** ret;
    if ( strs->eleNb==0 ) {
        strset_free(strs);
        ret = NULL;
    } else if ( strs->eleNb==1 ) {
        strset_addNull(strs);
        ret = strset_free2(strs);
    } else { // strs->eleNb>1
        strset_addClone(strs,strs->eles[0]);
        strset_addNull(strs);
        char* common_prefix = strs->eles[0];
        int   common_prefix_len = strlen(common_prefix);
        while ( common_prefix_len>0 ) {
            for ( i=1 ; i<strs->eleNb ; i++ ) {
                if ( strncmp(strs->eles[i],common_prefix,common_prefix_len)!=0 )
                    break;
            }
            if ( i==strs->eleNb )
                // common_prefix_len matches
                break;
            else
                common_prefix_len -= 1;
        }
        common_prefix[common_prefix_len] = 0;
        ret = strset_free2(strs);
    }
    rl_attempted_completion_over = 1; // imperative result when NULL is returned, otherwise 
                                      // readline falls back to its default path completion.
    return ret;
}
#endif // WithCompletion
/*======================================================================*/
