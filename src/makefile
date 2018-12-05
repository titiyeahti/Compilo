all :
	bison -d parser.y && flex parser.l
	gcc -o  ../logic parser.tab.c lex.yy.c main.c top.c -lreadline 


