TARGET = calculate_expression_exec

all: $(TARGET)

$(TARGET): calculate_expression.tab.c lex.yy.c
	gcc -o $(TARGET) calculate_expression.tab.c lex.yy.c -lfl

calculate_expression.tab.c calculate_expression.tab.h: calculate_expression.y
	bison -d calculate_expression.y

lex.yy.c: calculate_expression.l
	flex calculate_expression.l

clean:
	rm -f $(TARGET) calculate_expression.tab.c calculate_expression.tab.h lex.yy.c
