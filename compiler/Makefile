# mana (compiler)
# Copyright (c) 2003 Shun Moriya
#

TARGET	= mana
LIBRARY	= ../library

CC	= gcc
#CC	= clang
#CFLAGS	= --std=c99 -O3 -c -Wall -D NDEBUG -I$(LIBRARY)
CFLAGS	= --std=c99 -g -O0 -c -Wall -D _DEBUG -I$(LIBRARY)

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
	ruby -C ../test/mana test.rb

version:
	ruby version.rb

# DO NOT DELETE

code.o: code.h ../library/libmana.h ../library/mana_define.h
code.o: ../library/mana_info.h ../library/mana.h ../library/mana_actor.h
code.o: ../library/mana_frame.h ../library/mana_stream.h
code.o: ../library/mana_hash.h ../library/mana_stack.h
code.o: ../library/mana_array.h ../library/mana_datalink.h
code.o: ../library/mana_buffer.h ../library/mana_function.h
code.o: ../library/mana_lzss.h ../library/mana_malloc.h
code.o: ../library/mana_plugin.h main.h node.h symbol.h type.h pool.h
data.o: ../library/mana_info.h ../library/mana_malloc.h data.h
data.o: ../library/libmana.h ../library/mana_define.h ../library/mana_info.h
data.o: ../library/mana.h ../library/mana_actor.h ../library/mana_frame.h
data.o: ../library/mana_stream.h ../library/mana_hash.h
data.o: ../library/mana_stack.h ../library/mana_array.h
data.o: ../library/mana_datalink.h ../library/mana_buffer.h
data.o: ../library/mana_function.h ../library/mana_lzss.h
data.o: ../library/mana_malloc.h ../library/mana_plugin.h
datalink_generator.o: datalink_generator.h ../library/libmana.h
datalink_generator.o: ../library/mana_define.h ../library/mana_info.h
datalink_generator.o: ../library/mana.h ../library/mana_actor.h
datalink_generator.o: ../library/mana_frame.h ../library/mana_stream.h
datalink_generator.o: ../library/mana_hash.h ../library/mana_stack.h
datalink_generator.o: ../library/mana_array.h ../library/mana_datalink.h
datalink_generator.o: ../library/mana_buffer.h ../library/mana_function.h
datalink_generator.o: ../library/mana_lzss.h ../library/mana_malloc.h
datalink_generator.o: ../library/mana_plugin.h error.h
error.o: error.h lexer.h main.h node.h symbol.h ../library/libmana.h
error.o: ../library/mana_define.h ../library/mana_info.h ../library/mana.h
error.o: ../library/mana_actor.h ../library/mana_frame.h
error.o: ../library/mana_stream.h ../library/mana_hash.h
error.o: ../library/mana_stack.h ../library/mana_array.h
error.o: ../library/mana_datalink.h ../library/mana_buffer.h
error.o: ../library/mana_function.h ../library/mana_lzss.h
error.o: ../library/mana_malloc.h ../library/mana_plugin.h type.h pool.h
generator.o: generator.h symbol.h ../library/libmana.h
generator.o: ../library/mana_define.h ../library/mana_info.h
generator.o: ../library/mana.h ../library/mana_actor.h
generator.o: ../library/mana_frame.h ../library/mana_stream.h
generator.o: ../library/mana_hash.h ../library/mana_stack.h
generator.o: ../library/mana_array.h ../library/mana_datalink.h
generator.o: ../library/mana_buffer.h ../library/mana_function.h
generator.o: ../library/mana_lzss.h ../library/mana_malloc.h
generator.o: ../library/mana_plugin.h code.h data.h error.h jump.h node.h
generator.o: type.h linker.h main.h pool.h register.h resolver.h
generator.o: pre_resolver.h post_resolver.h
jump.o: main.h node.h symbol.h ../library/libmana.h ../library/mana_define.h
jump.o: ../library/mana_info.h ../library/mana.h ../library/mana_actor.h
jump.o: ../library/mana_frame.h ../library/mana_stream.h
jump.o: ../library/mana_hash.h ../library/mana_stack.h
jump.o: ../library/mana_array.h ../library/mana_datalink.h
jump.o: ../library/mana_buffer.h ../library/mana_function.h
jump.o: ../library/mana_lzss.h ../library/mana_malloc.h
jump.o: ../library/mana_plugin.h type.h pool.h code.h error.h generator.h
jump.o: jump.h linker.h
linker.o: linker.h code.h ../library/libmana.h ../library/mana_define.h
linker.o: ../library/mana_info.h ../library/mana.h ../library/mana_actor.h
linker.o: ../library/mana_frame.h ../library/mana_stream.h
linker.o: ../library/mana_hash.h ../library/mana_stack.h
linker.o: ../library/mana_array.h ../library/mana_datalink.h
linker.o: ../library/mana_buffer.h ../library/mana_function.h
linker.o: ../library/mana_lzss.h ../library/mana_malloc.h
linker.o: ../library/mana_plugin.h
main.o: main.h node.h symbol.h ../library/libmana.h ../library/mana_define.h
main.o: ../library/mana_info.h ../library/mana.h ../library/mana_actor.h
main.o: ../library/mana_frame.h ../library/mana_stream.h
main.o: ../library/mana_hash.h ../library/mana_stack.h
main.o: ../library/mana_array.h ../library/mana_datalink.h
main.o: ../library/mana_buffer.h ../library/mana_function.h
main.o: ../library/mana_lzss.h ../library/mana_malloc.h
main.o: ../library/mana_plugin.h type.h pool.h code.h generator.h data.h
main.o: datalink_generator.h error.h pre_resolver.h jump.h lexer.h linker.h
main.o: register.h version.h
node.o: node.h symbol.h ../library/libmana.h ../library/mana_define.h
node.o: ../library/mana_info.h ../library/mana.h ../library/mana_actor.h
node.o: ../library/mana_frame.h ../library/mana_stream.h
node.o: ../library/mana_hash.h ../library/mana_stack.h
node.o: ../library/mana_array.h ../library/mana_datalink.h
node.o: ../library/mana_buffer.h ../library/mana_function.h
node.o: ../library/mana_lzss.h ../library/mana_malloc.h
node.o: ../library/mana_plugin.h type.h data.h lexer.h
pool.o: ../library/mana_malloc.h pool.h
post_resolver.o: post_resolver.h datalink_generator.h ../library/libmana.h
post_resolver.o: ../library/mana_define.h ../library/mana_info.h
post_resolver.o: ../library/mana.h ../library/mana_actor.h
post_resolver.o: ../library/mana_frame.h ../library/mana_stream.h
post_resolver.o: ../library/mana_hash.h ../library/mana_stack.h
post_resolver.o: ../library/mana_array.h ../library/mana_datalink.h
post_resolver.o: ../library/mana_buffer.h ../library/mana_function.h
post_resolver.o: ../library/mana_lzss.h ../library/mana_malloc.h
post_resolver.o: ../library/mana_plugin.h error.h main.h node.h symbol.h
post_resolver.o: type.h pool.h resolver.h
pre_resolver.o: pre_resolver.h error.h resolver.h main.h node.h symbol.h
pre_resolver.o: ../library/libmana.h ../library/mana_define.h
pre_resolver.o: ../library/mana_info.h ../library/mana.h
pre_resolver.o: ../library/mana_actor.h ../library/mana_frame.h
pre_resolver.o: ../library/mana_stream.h ../library/mana_hash.h
pre_resolver.o: ../library/mana_stack.h ../library/mana_array.h
pre_resolver.o: ../library/mana_datalink.h ../library/mana_buffer.h
pre_resolver.o: ../library/mana_function.h ../library/mana_lzss.h
pre_resolver.o: ../library/mana_malloc.h ../library/mana_plugin.h type.h
pre_resolver.o: pool.h
register.o: ../library/mana_info.h register.h
resolver.o: resolver.h error.h lexer.h type.h
symbol.o: generator.h symbol.h ../library/libmana.h ../library/mana_define.h
symbol.o: ../library/mana_info.h ../library/mana.h ../library/mana_actor.h
symbol.o: ../library/mana_frame.h ../library/mana_stream.h
symbol.o: ../library/mana_hash.h ../library/mana_stack.h
symbol.o: ../library/mana_array.h ../library/mana_datalink.h
symbol.o: ../library/mana_buffer.h ../library/mana_function.h
symbol.o: ../library/mana_lzss.h ../library/mana_malloc.h
symbol.o: ../library/mana_plugin.h code.h data.h error.h linker.h main.h
symbol.o: node.h type.h pool.h register.h
type.o: main.h node.h symbol.h ../library/libmana.h ../library/mana_define.h
type.o: ../library/mana_info.h ../library/mana.h ../library/mana_actor.h
type.o: ../library/mana_frame.h ../library/mana_stream.h
type.o: ../library/mana_hash.h ../library/mana_stack.h
type.o: ../library/mana_array.h ../library/mana_datalink.h
type.o: ../library/mana_buffer.h ../library/mana_function.h
type.o: ../library/mana_lzss.h ../library/mana_malloc.h
type.o: ../library/mana_plugin.h type.h pool.h error.h
parser.o: error.h generator.h symbol.h ../library/libmana.h
parser.o: ../library/mana_define.h ../library/mana_info.h ../library/mana.h
parser.o: ../library/mana_actor.h ../library/mana_frame.h
parser.o: ../library/mana_stream.h ../library/mana_hash.h
parser.o: ../library/mana_stack.h ../library/mana_array.h
parser.o: ../library/mana_datalink.h ../library/mana_buffer.h
parser.o: ../library/mana_function.h ../library/mana_lzss.h
parser.o: ../library/mana_malloc.h ../library/mana_plugin.h lexer.h linker.h
parser.o: main.h node.h type.h pool.h pre_resolver.h
lexer.o: lexer.h error.h main.h node.h symbol.h ../library/libmana.h
lexer.o: ../library/mana_define.h ../library/mana_info.h ../library/mana.h
lexer.o: ../library/mana_actor.h ../library/mana_frame.h
lexer.o: ../library/mana_stream.h ../library/mana_hash.h
lexer.o: ../library/mana_stack.h ../library/mana_array.h
lexer.o: ../library/mana_datalink.h ../library/mana_buffer.h
lexer.o: ../library/mana_function.h ../library/mana_lzss.h
lexer.o: ../library/mana_malloc.h ../library/mana_plugin.h type.h pool.h
