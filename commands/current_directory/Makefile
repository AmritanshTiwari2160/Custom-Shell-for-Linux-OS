# Build current_directory_exec using Flex + Bison, linking libfl

current_directory_exec: current_directory.tab.c lex.yy.c
	gcc -o current_directory_exec current_directory.tab.c lex.yy.c -lfl

current_directory.tab.c current_directory.tab.h: current_directory.y
	bison -d current_directory.y

lex.yy.c: current_directory.l
	flex current_directory.l

clean:
	rm -f current_directory_exec current_directory.tab.c current_directory.tab.h lex.yy.c
