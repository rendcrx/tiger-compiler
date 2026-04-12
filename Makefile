chapter2: main2.c AST.c AST.h Lex.h Lex.c Makefile
	cc -g -o chapter2 main2.c AST.c Lex.c

chapter1: main.c AST.c AST.h Makefile
	cc -g -o chapter1 main.c AST.c

clean:
	rm -f chapter1 chapter2
