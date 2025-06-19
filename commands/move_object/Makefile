# Makefile for move_object command

TARGET = move_object_exec

all: $(TARGET)

$(TARGET): move_object.tab.c lex.yy.c
	gcc -o $(TARGET) move_object.tab.c lex.yy.c -lfl

move_object.tab.c move_object.tab.h: move_object.y
	bison -d move_object.y

lex.yy.c: move_object.l
	flex move_object.l

clean:
	rm -f $(TARGET) move_object.tab.c move_object.tab.h lex.yy.c
