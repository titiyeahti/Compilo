all : top.c top.h bison flex main.c 
	gcc -o  ../logic parser.tab.c lex.yy.c main.c top.c -lreadline 

bison : parser.y
	bison -dtg parser.y  
	
flex : parser.l 
	flex parser.l

test : ../logic
	(cd .. ; bash test/bad-syntax.sh ; cd src/)

