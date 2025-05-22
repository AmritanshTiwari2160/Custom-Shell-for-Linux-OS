all:
	bison -d remove_dir.y
	flex remove_dir.l
	gcc -o remove_dir_exec remove_dir.tab.c lex.yy.c -lfl

clean:
	rm -f remove_dir_exec remove_dir.tab.c remove_dir.tab.h lex.yy.c