# mana (compiler)
# Copyright (c) 2003 Shun Moriya
#

TARGET	= mana
LIBRARY	= ../library

CC	= gcc
#CC	= clang
CFLAGS	= --std=c99 -O3 -c -Wall -D NDEBUG -I$(LIBRARY)

LK	= $(CC)
LFLAGS	= --std=c99 -O3

COVERAGE=

YACC	= bison
YFLAGS	= -d -k

LEX	= flex
LXFLAGS	= -8 -F

CP      = cp -f
RM	= rm -f

SOURCES	= code.c data.c datalink_generator.c error.c generator.c jump.c linker.c main.c node.c pool.c post_resolver.c pre_resolver.c register.c resolver.c symbol.c type.c parser.y lexer.l
OBJECTS	= code.o data.o datalink_generator.o error.o generator.o jump.o linker.o main.o node.o pool.o post_resolver.o pre_resolver.o register.o resolver.o symbol.o type.o parser.o lexer.o
SECOUND	= parser.c parser.h lexer.c

.SUFFIXES: .y .l .c .o

.SECOUNDLY: $(SECOUND)

$(TARGET): $(SOURCES) $(OBJECTS) $(LIBRARY)/libmana.a
	$(LK) $(LFLAGS) $(COVERAGE) -o $(TARGET) $(OBJECTS) $(LIBRARY)/libmana.a -ldl -lm

$(LIBRARY)/libmana.a:
	make --directory=$(LIBRARY) COVERAGE=$(COVERAGE)

.l.c:
	$(LEX) $(LXFLAGS) -o$@ $<

.y.c:
	$(YACC) $(YFLAGS) -o $@ $<

.c.o:
	$(CC) $(CFLAGS) $(COVERAGE) -o $@ $<

all: $(TARGET)

clean:
	$(RM) $(TARGET) $(OBJECTS) $(SECOUND)
	make --directory=$(LIBRARY) clean

depend:
	makedepend -Y -- $(CFLAGS) -- $(SOURCES)
	make --directory=$(LIBRARY) depend

test:
	./$(TARGET) --debug ../sample/sample.mn

version:
	ruby version.rb

# DO NOT DELETE

