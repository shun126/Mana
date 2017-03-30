# mana (compiler)
# Copyright (c) 2003 Shun Moriya <shun@mnu.sakura.ne.jp>
#

TARGET	= mana
LIBRARY	= ../library

CC	= gcc
CFLAGS	= --std=c11 --input-charset=cp932 -c -O3 -Wall -D NDEBUG -I$(LIBRARY)
#CC	= clang
#CFLAGS	= --std=c11 -c -O3 -Wall -D NDEBUG -I$(LIBRARY)

LK	= $(CC)
LFLAGS	= 
#LK	= ld
#LFLAGS	= --library=c --library=dl --library=m

YACC	= bison
YFLAGS	= -d -k

LEX	= flex
LXFLAGS	= -8 -F

CP      = cp -f
RM	= rm -f

SOURCES	= mana_code.c mana_data.c mana_datalink_generator.c mana_generator.c mana_jump.c mana_linker.c mana_main.c mana_node.c mana_pool.c mana_post_resolver.c mana_pre_resolver.c mana_register.c mana_resolver.c mana_symbol.c mana_type.c mana_parser.y mana_lexer.l
OBJECTS	= mana_code.o mana_data.o mana_datalink_generator.o mana_generator.o mana_jump.o mana_linker.o mana_main.o mana_node.o mana_pool.o mana_post_resolver.o mana_pre_resolver.o mana_register.o mana_resolver.o mana_symbol.o mana_type.o mana_parser.o mana_lexer.o
SECOUND	= mana_parser.c mana_parser.h mana_lexer.c

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

clean:
	$(RM) $(TARGET) $(OBJECTS) $(SECOUND)
	make --directory=$(LIBRARY) clean

depend:
	makedepend -Y -- $(CFLAGS) -- $(SOURCES)
	make --directory=$(LIBRARY) depend

test:
	./$(TARGET) --debug ../sample/sample.mn

# DO NOT DELETE

mana_code.o: mana_code.h ../library/libmana.h ../library/mana_define.h
mana_code.o: ../library/mana_info.h ../library/mana.h ../library/mana_actor.h
mana_code.o: ../library/mana_frame.h ../library/mana_stream.h
mana_code.o: ../library/mana_hash.h ../library/mana_stack.h
mana_code.o: ../library/mana_array.h ../library/mana_datalink.h
mana_code.o: ../library/mana_buffer.h ../library/mana_function.h
mana_code.o: ../library/mana_lzss.h ../library/mana_malloc.h
mana_code.o: ../library/mana_plugin.h mana_main.h mana_node.h mana_symbol.h
mana_code.o: mana_type.h mana_pool.h
mana_data.o: ../library/mana_info.h ../library/mana_malloc.h mana_data.h
mana_data.o: ../library/libmana.h ../library/mana_define.h
mana_data.o: ../library/mana_info.h ../library/mana.h ../library/mana_actor.h
mana_data.o: ../library/mana_frame.h ../library/mana_stream.h
mana_data.o: ../library/mana_hash.h ../library/mana_stack.h
mana_data.o: ../library/mana_array.h ../library/mana_datalink.h
mana_data.o: ../library/mana_buffer.h ../library/mana_function.h
mana_data.o: ../library/mana_lzss.h ../library/mana_malloc.h
mana_data.o: ../library/mana_plugin.h
mana_datalink_generator.o: mana_main.h mana_node.h mana_symbol.h
mana_datalink_generator.o: ../library/libmana.h ../library/mana_define.h
mana_datalink_generator.o: ../library/mana_info.h ../library/mana.h
mana_datalink_generator.o: ../library/mana_actor.h ../library/mana_frame.h
mana_datalink_generator.o: ../library/mana_stream.h ../library/mana_hash.h
mana_datalink_generator.o: ../library/mana_stack.h ../library/mana_array.h
mana_datalink_generator.o: ../library/mana_datalink.h
mana_datalink_generator.o: ../library/mana_buffer.h
mana_datalink_generator.o: ../library/mana_function.h ../library/mana_lzss.h
mana_datalink_generator.o: ../library/mana_malloc.h ../library/mana_plugin.h
mana_datalink_generator.o: mana_type.h mana_pool.h mana_datalink_generator.h
mana_generator.o: mana_generator.h mana_symbol.h ../library/libmana.h
mana_generator.o: ../library/mana_define.h ../library/mana_info.h
mana_generator.o: ../library/mana.h ../library/mana_actor.h
mana_generator.o: ../library/mana_frame.h ../library/mana_stream.h
mana_generator.o: ../library/mana_hash.h ../library/mana_stack.h
mana_generator.o: ../library/mana_array.h ../library/mana_datalink.h
mana_generator.o: ../library/mana_buffer.h ../library/mana_function.h
mana_generator.o: ../library/mana_lzss.h ../library/mana_malloc.h
mana_generator.o: ../library/mana_plugin.h mana_code.h mana_data.h
mana_generator.o: mana_jump.h mana_node.h mana_type.h mana_linker.h
mana_generator.o: mana_main.h mana_pool.h mana_register.h mana_resolver.h
mana_jump.o: mana_main.h mana_node.h mana_symbol.h ../library/libmana.h
mana_jump.o: ../library/mana_define.h ../library/mana_info.h
mana_jump.o: ../library/mana.h ../library/mana_actor.h
mana_jump.o: ../library/mana_frame.h ../library/mana_stream.h
mana_jump.o: ../library/mana_hash.h ../library/mana_stack.h
mana_jump.o: ../library/mana_array.h ../library/mana_datalink.h
mana_jump.o: ../library/mana_buffer.h ../library/mana_function.h
mana_jump.o: ../library/mana_lzss.h ../library/mana_malloc.h
mana_jump.o: ../library/mana_plugin.h mana_type.h mana_pool.h mana_code.h
mana_jump.o: mana_jump.h mana_linker.h
mana_linker.o: mana_linker.h mana_code.h ../library/libmana.h
mana_linker.o: ../library/mana_define.h ../library/mana_info.h
mana_linker.o: ../library/mana.h ../library/mana_actor.h
mana_linker.o: ../library/mana_frame.h ../library/mana_stream.h
mana_linker.o: ../library/mana_hash.h ../library/mana_stack.h
mana_linker.o: ../library/mana_array.h ../library/mana_datalink.h
mana_linker.o: ../library/mana_buffer.h ../library/mana_function.h
mana_linker.o: ../library/mana_lzss.h ../library/mana_malloc.h
mana_linker.o: ../library/mana_plugin.h
mana_main.o: mana_main.h mana_node.h mana_symbol.h ../library/libmana.h
mana_main.o: ../library/mana_define.h ../library/mana_info.h
mana_main.o: ../library/mana.h ../library/mana_actor.h
mana_main.o: ../library/mana_frame.h ../library/mana_stream.h
mana_main.o: ../library/mana_hash.h ../library/mana_stack.h
mana_main.o: ../library/mana_array.h ../library/mana_datalink.h
mana_main.o: ../library/mana_buffer.h ../library/mana_function.h
mana_main.o: ../library/mana_lzss.h ../library/mana_malloc.h
mana_main.o: ../library/mana_plugin.h mana_type.h mana_pool.h mana_code.h
mana_main.o: mana_generator.h mana_data.h mana_datalink_generator.h
mana_main.o: mana_pre_resolver.h mana_jump.h mana_register.h mana_version.h
mana_node.o: mana_data.h ../library/libmana.h ../library/mana_define.h
mana_node.o: ../library/mana_info.h ../library/mana.h ../library/mana_actor.h
mana_node.o: ../library/mana_frame.h ../library/mana_stream.h
mana_node.o: ../library/mana_hash.h ../library/mana_stack.h
mana_node.o: ../library/mana_array.h ../library/mana_datalink.h
mana_node.o: ../library/mana_buffer.h ../library/mana_function.h
mana_node.o: ../library/mana_lzss.h ../library/mana_malloc.h
mana_node.o: ../library/mana_plugin.h mana_datalink_generator.h mana_main.h
mana_node.o: mana_node.h mana_symbol.h mana_type.h mana_pool.h
mana_pool.o: ../library/mana_malloc.h mana_pool.h
mana_post_resolver.o: mana_post_resolver.h mana_symbol.h ../library/libmana.h
mana_post_resolver.o: ../library/mana_define.h ../library/mana_info.h
mana_post_resolver.o: ../library/mana.h ../library/mana_actor.h
mana_post_resolver.o: ../library/mana_frame.h ../library/mana_stream.h
mana_post_resolver.o: ../library/mana_hash.h ../library/mana_stack.h
mana_post_resolver.o: ../library/mana_array.h ../library/mana_datalink.h
mana_post_resolver.o: ../library/mana_buffer.h ../library/mana_function.h
mana_post_resolver.o: ../library/mana_lzss.h ../library/mana_malloc.h
mana_post_resolver.o: ../library/mana_plugin.h mana_resolver.h mana_main.h
mana_post_resolver.o: mana_node.h mana_type.h mana_pool.h
mana_pre_resolver.o: mana_pre_resolver.h mana_resolver.h mana_main.h
mana_pre_resolver.o: mana_node.h mana_symbol.h ../library/libmana.h
mana_pre_resolver.o: ../library/mana_define.h ../library/mana_info.h
mana_pre_resolver.o: ../library/mana.h ../library/mana_actor.h
mana_pre_resolver.o: ../library/mana_frame.h ../library/mana_stream.h
mana_pre_resolver.o: ../library/mana_hash.h ../library/mana_stack.h
mana_pre_resolver.o: ../library/mana_array.h ../library/mana_datalink.h
mana_pre_resolver.o: ../library/mana_buffer.h ../library/mana_function.h
mana_pre_resolver.o: ../library/mana_lzss.h ../library/mana_malloc.h
mana_pre_resolver.o: ../library/mana_plugin.h mana_type.h mana_pool.h
mana_register.o: ../library/mana_info.h mana_register.h
mana_resolver.o: mana_resolver.h
mana_symbol.o: mana_generator.h mana_symbol.h ../library/libmana.h
mana_symbol.o: ../library/mana_define.h ../library/mana_info.h
mana_symbol.o: ../library/mana.h ../library/mana_actor.h
mana_symbol.o: ../library/mana_frame.h ../library/mana_stream.h
mana_symbol.o: ../library/mana_hash.h ../library/mana_stack.h
mana_symbol.o: ../library/mana_array.h ../library/mana_datalink.h
mana_symbol.o: ../library/mana_buffer.h ../library/mana_function.h
mana_symbol.o: ../library/mana_lzss.h ../library/mana_malloc.h
mana_symbol.o: ../library/mana_plugin.h mana_code.h mana_data.h mana_linker.h
mana_symbol.o: mana_main.h mana_node.h mana_type.h mana_pool.h
mana_symbol.o: mana_register.h
mana_type.o: mana_main.h mana_node.h mana_symbol.h ../library/libmana.h
mana_type.o: ../library/mana_define.h ../library/mana_info.h
mana_type.o: ../library/mana.h ../library/mana_actor.h
mana_type.o: ../library/mana_frame.h ../library/mana_stream.h
mana_type.o: ../library/mana_hash.h ../library/mana_stack.h
mana_type.o: ../library/mana_array.h ../library/mana_datalink.h
mana_type.o: ../library/mana_buffer.h ../library/mana_function.h
mana_type.o: ../library/mana_lzss.h ../library/mana_malloc.h
mana_type.o: ../library/mana_plugin.h mana_type.h mana_pool.h
mana_parser.o: mana_generator.h mana_symbol.h ../library/libmana.h
mana_parser.o: ../library/mana_define.h ../library/mana_info.h
mana_parser.o: ../library/mana.h ../library/mana_actor.h
mana_parser.o: ../library/mana_frame.h ../library/mana_stream.h
mana_parser.o: ../library/mana_hash.h ../library/mana_stack.h
mana_parser.o: ../library/mana_array.h ../library/mana_datalink.h
mana_parser.o: ../library/mana_buffer.h ../library/mana_function.h
mana_parser.o: ../library/mana_lzss.h ../library/mana_malloc.h
mana_parser.o: ../library/mana_plugin.h mana_linker.h mana_main.h mana_node.h
mana_parser.o: mana_type.h mana_pool.h mana_post_resolver.h
mana_parser.o: mana_pre_resolver.h
mana_lexer.o: mana_main.h mana_node.h mana_symbol.h ../library/libmana.h
mana_lexer.o: ../library/mana_define.h ../library/mana_info.h
mana_lexer.o: ../library/mana.h ../library/mana_actor.h
mana_lexer.o: ../library/mana_frame.h ../library/mana_stream.h
mana_lexer.o: ../library/mana_hash.h ../library/mana_stack.h
mana_lexer.o: ../library/mana_array.h ../library/mana_datalink.h
mana_lexer.o: ../library/mana_buffer.h ../library/mana_function.h
mana_lexer.o: ../library/mana_lzss.h ../library/mana_malloc.h
mana_lexer.o: ../library/mana_plugin.h mana_type.h mana_pool.h mana_parser.h
