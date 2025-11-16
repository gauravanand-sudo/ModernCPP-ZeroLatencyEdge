LEX := flex
YACC := bison
CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic

TARGET := calc
OBJS := parser.tab.c lex.yy.c

all: $(TARGET)

parser.tab.c parser.tab.h: parser.y
	$(YACC) -d -o parser.tab.c parser.y

lex.yy.c: lexer.l parser.tab.h
	$(LEX) -o lex.yy.c lexer.l

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(TARGET) parser.tab.c parser.tab.h lex.yy.c

.PHONY: all clean
