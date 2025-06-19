# Makefile for make_directory command

# Target executable
make_directory_exec: make_directory.tab.c lex.yy.c
	gcc -o make_directory_exec make_directory.tab.c lex.yy.c -lfl

# Generate parser source and header
make_directory.tab.c make_directory.tab.h: make_directory.y
	bison -d make_directory.y

# Generate lexer source
lex.yy.c: make_directory.l
	flex make_directory.l

# Clean up generated files
clean:
	rm -f make_directory_exec make_directory.tab.c make_directory.tab.h lex.yy.c