# mana (compiler)
# Copyright (c) 2003 Shun Moriya <shun@mnu.sakura.ne.jp>
#
# The zlib/libpng License
#
# This software is provided 'as-is', without any express or implied warranty.
# In no event will the authors be held liable for any damages arising from
# the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it freely,
# subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not claim that
#    you wrote the original software. If you use this software in a product,
#    an acknowledgment in the product documentation would be appreciated but is not required.
#
# 2. Altered source versions must be plainly marked as such,
#    and must not be misrepresented as being the original software.
#
# 3. This notice may not be removed or altered from any source distribution.
#

TARGET	= mana
CC	= gcc
#CFLAGS	= -m32 -c -O3 -Wall -D NDEBUG --input-charset=utf-8
CFLAGS	= -m32 -c -O3 -Wall -D NDEBUG
LK	= gcc
LFLAGS	= -m32
YACC	= bison
YFLAGS	= -d -k
LEX	= flex
LXFLAGS	= -8 -F

CP      = cp -f
RM	= rm -f

HEADERS	= mana_code.h mana_data.h mana_datalink_generator.h mana_jump.h mana_linker.h mana_main.h mana_node.h mana_pool.h mana_register.h mana_symbol.h mana_type.h ../library/mana_datalink.h ../library/mana_hash.h ../library/mana_malloc.h ../library/mana_info.h
SOURCES	= mana_code.c mana_data.c mana_datalink_generator.c mana_jump.c mana_linker.c mana_main.c mana_node.c mana_pool.c mana_register.c mana_symbol.c mana_type.c ../library/mana_datalink.c ../library/mana_hash.c ../library/mana_malloc.c ../library/mana_info.c mana_parser.y mana_lexer.l
OBJECTS	= mana_code.o mana_data.o mana_datalink_generator.o mana_jump.o mana_linker.o mana_main.o mana_node.o mana_pool.o mana_register.o mana_symbol.o mana_type.o ../library/mana_datalink.o ../library/mana_hash.o ../library/mana_malloc.o ../library/mana_info.o mana_parser.o mana_lexer.o

SECOUND	= mana_parser.c mana_parser.h mana_lexer.c

.SUFFIXES: .y .l .c .o

.SECOUNDLY: $(SECOUND)

$(TARGET): $(HEADERS) $(SOURCES) $(OBJECTS)
	$(LK) $(LFLAGS) -o $(TARGET) $(OBJECTS)

.l.c:
	$(LEX) $(LXFLAGS) -o$@ $<

.y.c:
	$(YACC) $(YFLAGS) -o $@ $<

.c.o: $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $<

run:
	./$(TARGET) --debug ../script/test.mn

clean:
	$(RM) $(TARGET) $(OBJECTS) $(SECOUND)

depend:
	makedepend -Y -- $(CFLAGS) -- $(SOURCES)

# DO NOT DELETE

mana_code.o: mana_code.h mana_main.h ../library/mana_datalink.h
mana_code.o: ../library/mana_hash.h ../library/mana_info.h
mana_code.o: ../library/mana_malloc.h mana_pool.h
mana_data.o: ../library/mana_info.h ../library/mana_malloc.h mana_data.h
mana_datalink_generator.o: mana_main.h ../library/mana_datalink.h
mana_datalink_generator.o: ../library/mana_hash.h ../library/mana_info.h
mana_datalink_generator.o: ../library/mana_malloc.h mana_pool.h
mana_datalink_generator.o: mana_datalink_generator.h
mana_jump.o: mana_main.h ../library/mana_datalink.h ../library/mana_hash.h
mana_jump.o: ../library/mana_info.h ../library/mana_malloc.h mana_pool.h
mana_jump.o: mana_code.h mana_jump.h mana_node.h mana_symbol.h mana_type.h
mana_jump.o: mana_linker.h
mana_linker.o: mana_code.h mana_data.h mana_linker.h mana_node.h
mana_linker.o: mana_symbol.h mana_type.h mana_main.h
mana_linker.o: ../library/mana_datalink.h ../library/mana_hash.h
mana_linker.o: ../library/mana_info.h ../library/mana_malloc.h mana_pool.h
mana_main.o: mana_main.h ../library/mana_datalink.h ../library/mana_hash.h
mana_main.o: ../library/mana_info.h ../library/mana_malloc.h mana_pool.h
mana_main.o: mana_code.h mana_data.h mana_datalink_generator.h mana_jump.h
mana_main.o: mana_node.h mana_symbol.h mana_type.h mana_version.h
mana_node.o: mana_data.h mana_datalink_generator.h mana_main.h
mana_node.o: ../library/mana_datalink.h ../library/mana_hash.h
mana_node.o: ../library/mana_info.h ../library/mana_malloc.h mana_pool.h
mana_node.o: mana_node.h mana_symbol.h mana_type.h
mana_pool.o: ../library/mana_malloc.h mana_pool.h
mana_symbol.o: mana_code.h mana_data.h mana_linker.h mana_node.h
mana_symbol.o: mana_symbol.h mana_type.h mana_main.h
mana_symbol.o: ../library/mana_datalink.h ../library/mana_hash.h
mana_symbol.o: ../library/mana_info.h ../library/mana_malloc.h mana_pool.h
mana_type.o: mana_main.h ../library/mana_datalink.h ../library/mana_hash.h
mana_type.o: ../library/mana_info.h ../library/mana_malloc.h mana_pool.h
mana_type.o: mana_symbol.h mana_type.h
../library/mana_datalink.o: ../library/mana_datalink.h
../library/mana_datalink.o: ../library/mana_malloc.h
../library/mana_hash.o: ../library/mana_hash.h ../library/mana_malloc.h
../library/mana_malloc.o: ../library/mana_malloc.h
../library/mana_info.o: ../library/mana_malloc.h ../library/mana_info.h
mana_parser.o: mana_code.h mana_data.h mana_jump.h mana_node.h mana_symbol.h
mana_parser.o: mana_type.h mana_linker.h mana_main.h
mana_parser.o: ../library/mana_datalink.h ../library/mana_hash.h
mana_parser.o: ../library/mana_info.h ../library/mana_malloc.h mana_pool.h
mana_lexer.o: mana_main.h ../library/mana_datalink.h ../library/mana_hash.h
mana_lexer.o: ../library/mana_info.h ../library/mana_malloc.h mana_pool.h
mana_lexer.o: mana_node.h mana_symbol.h mana_type.h mana_parser.h
