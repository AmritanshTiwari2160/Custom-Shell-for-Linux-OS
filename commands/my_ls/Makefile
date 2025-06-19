my_ls_exec: my_ls.tab.c lex.yy.c
	gcc -o my_ls_exec my_ls.tab.c lex.yy.c -lfl

my_ls.tab.c my_ls.tab.h: my_ls.y
	bison -d my_ls.y

lex.yy.c: my_ls.l
	flex my_ls.l

clean:
	rm -f my_ls_exec my_ls.tab.* lex.yy.c