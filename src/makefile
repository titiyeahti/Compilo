all :
	bison -d parser.y && flex parser.l
	gcc -o  ../executable parser.tab.c lex.yy.c main.c top.c -lreadline 


