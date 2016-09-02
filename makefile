TARGET = epsi
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall -Wno-unused-function -Wno-return-type

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c)) lex.yy.c parser.c
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

lex.yy.c:	scanner.lex parser.tab.h
	flex scanner.lex

parser.c: parser.y
	bison -d parser.y

parser.tab.h: parser.y
	bison -d parser.y

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall -Wno-unused-function -Wno-return-type $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f lex.yy.c
	-rm -f parser.c
	-rm -f parser.tab.h
	-rm -f $(TARGET)
