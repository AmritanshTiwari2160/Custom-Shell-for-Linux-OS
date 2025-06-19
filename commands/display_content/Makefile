all:
	bison -d display_content.y
	flex display_content.l
	gcc -o display_content_exec display_content.tab.c lex.yy.c -lfl
	
clean:
	rm -f display_content_exec display_content.tab.c display_content.tab.h lex.yy.c