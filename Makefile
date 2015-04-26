pasc: y.tab.o lex.yy.o tree.o scope.o node.o arglist.o
	gcc -g -o pasc tree.o scope.o node.o arglist.o y.tab.o lex.yy.o -lfl
tree.o: tree.c
	gcc -g -c tree.c
scope.o: scope.c
	gcc -g -c scope.c
node.o: node.c
	gcc -g -c node.c
arglist.o: arglist.c
	gcc -g -c arglist.c
y.tab.o: y.tab.c
	gcc -g -c y.tab.c
y.tab.c: pc.y
	yacc -dv pc.y
lex.yy.o: lex.yy.c
	gcc -g -c lex.yy.c
lex.yy.c: pc.l
	lex -l pc.l
clean:
	rm -f lex.yy.* y.tab.* *.o mypc
