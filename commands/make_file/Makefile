make_file_exec: make_file.y make_file.l
	bison -d make_file.y
	flex make_file.l
	gcc -o make_file_exec make_file.tab.c lex.yy.c -lfl