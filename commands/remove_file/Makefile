# Makefile for remove_file command

remove_file_exec: remove_file.tab.c lex.yy.c
	gcc -o remove_file_exec remove_file.tab.c lex.yy.c -lfl

remove_file.tab.c remove_file.tab.h: remove_file.y
	bison -d remove_file.y

lex.yy.c: remove_file.l
	flex remove_file.l

clean:
	rm -f remove_file_exec remove_file.tab.c remove_file.tab.h lex.yy.c