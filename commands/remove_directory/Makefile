all:
	bison -d remove_directory.y
	flex remove_directory.l
	gcc -o remove_directory_exec remove_directory.tab.c lex.yy.c -lfl

clean:
	rm -f remove_directory_exec remove_directory.tab.c remove_directory.tab.h lex.yy.c