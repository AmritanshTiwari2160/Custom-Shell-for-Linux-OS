vscode_open_exec: vscode_open.tab.c lex.yy.c
	gcc -o vscode_open_exec vscode_open.tab.c lex.yy.c -lfl

vscode_open.tab.c vscode_open.tab.h: vscode_open.y
	bison -d vscode_open.y

lex.yy.c: vscode_open.l
	flex vscode_open.l

clean:
	rm -f vscode_open_exec vscode_open.tab.c vscode_open.tab.h lex.yy.c
