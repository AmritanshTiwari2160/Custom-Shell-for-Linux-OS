# Makefile for my_editor command

my_editor_exec: my_editor.tab.c lex.yy.c
	gcc -o my_editor_exec my_editor.tab.c lex.yy.c -lfl

my_editor.tab.c my_editor.tab.h: my_editor.y
	bison -d my_editor.y

lex.yy.c: my_editor.l
	flex my_editor.l

clean:
	rm -f my_editor_exec my_editor.tab.c my_editor.tab.h lex.yy.c
