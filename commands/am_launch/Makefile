# Makefile for am_launch command

TARGET = am_launch_exec

all: $(TARGET)

$(TARGET): am_launch.tab.c lex.yy.c
	gcc -o $(TARGET) am_launch.tab.c lex.yy.c -lfl

am_launch.tab.c am_launch.tab.h: am_launch.y
	bison -d am_launch.y

lex.yy.c: am_launch.l
	flex am_launch.l

clean:
	rm -f $(TARGET) am_launch.tab.c am_launch.tab.h lex.yy.c