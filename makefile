turing: lex.yy.c y.tab.c tree.c tree.h
	gcc -g lex.yy.c y.tab.c tree.c main.c check.c -o turing

lex.yy.c: y.tab.c p2.l
	flex p2.l

y.tab.c: p2.y tree.h
	yacc -d p2.y

clean: 
	rm -f lex.yy.x y.tab.c y.tab.h turing
