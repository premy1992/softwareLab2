all: yacc lex merge 
yacc:
	bison -d yacc.y
lex:
	lex lex.l
merge:
	gcc yacc.tab.c lex.yy.c -o ly.out
