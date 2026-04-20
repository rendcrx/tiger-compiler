chapter5: main3.c TigerAST.c TigerAST.h Lex.h Symbol.c Symbol.h Table.h Table.c Makefile
	cc -g -o chapter5 main3.c TigerAST.c Lex.c Symbol.c Table.c

chapter3: main3.c TigerAST.c TigerAST.h Lex.h Makefile
	cc -g -o chapter3 main3.c TigerAST.c Lex.c

chapter2: main2.c AST.c AST.h Lex.h Lex.c Makefile
	cc -g -o chapter2 main2.c AST.c Lex.c

chapter1: main.c AST.c AST.h Makefile
	cc -g -o chapter1 main.c AST.c

clean:
	rm -rf chapter*
