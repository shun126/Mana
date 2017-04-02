# mana (compiler)
# Copyright (c) 2003 Shun Moriya <shun@mnu.sakura.ne.jp>
#

TARGET	= mana
LIBRARY	= ../library

CC	= gcc
CFLAGS	= --std=c99 --input-charset=cp932 -c -O3 -Wall -D NDEBUG -I$(LIBRARY)
#CC	= clang
#CFLAGS	= --std=c99 -c -O3 -Wall -D NDEBUG -I$(LIBRARY)

LK	= $(CC)
LFLAGS	= -ldl -lm 

YACC	= bison
YFLAGS	= -d -k

LEX	= flex
LXFLAGS	= -8 -F

CP      = cp -f
RM	= rm -f

SOURCES	= code.c data.c datalink_generator.c generator.c jump.c linker.c main.c node.c pool.c post_resolver.c pre_resolver.c register.c resolver.c symbol.c type.c parser.y lexer.l
OBJECTS	= code.o data.o datalink_generator.o generator.o jump.o linker.o main.o node.o pool.o post_resolver.o pre_resolver.o register.o resolver.o symbol.o type.o parser.o lexer.o
SECOUND	= parser.c parser.h lexer.c

.SUFFIXES: .y .l .c .o

.SECOUNDLY: $(SECOUND)

$(TARGET): $(SOURCES) $(OBJECTS) $(LIBRARY)/libmana.a
	$(LK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBRARY)/libmana.a

$(LIBRARY)/libmana.a:
	make --directory=$(LIBRARY)

.l.c:
	$(LEX) $(LXFLAGS) -o$@ $<

.y.c:
	$(YACC) $(YFLAGS) -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -o $@ $<

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

