# Builds available_disk_space_exec

available_disk_space_exec: available_disk_space.tab.c lex.yy.c
	gcc -o available_disk_space_exec \
	    available_disk_space.tab.c lex.yy.c -lfl

available_disk_space.tab.c available_disk_space.tab.h: available_disk_space.y
	bison -d available_disk_space.y

lex.yy.c: available_disk_space.l
	flex available_disk_space.l

clean:
	rm -f available_disk_space_exec \
	       available_disk_space.tab.c available_disk_space.tab.h lex.yy.c