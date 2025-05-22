# Build switch_to_exec

switch_to_exec: switch_to.tab.c lex.yy.c
	gcc -o switch_to_exec switch_to.tab.c lex.yy.c -lfl

switch_to.tab.c switch_to.tab.h: switch_to.y
	bison -d switch_to.y

lex.yy.c: switch_to.l
	flex switch_to.l

clean:
	rm -f switch_to_exec switch_to.tab.c switch_to.tab.h lex.yy.c