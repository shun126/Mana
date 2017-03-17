# makefile for mana library
# Copyright (c) 2003 Shun Moriya <shun@mnu.sakura.ne.jp>
#

TARGET	= libmana.a
CC	= gcc
CFLAGS	= --std=c11 --input-charset=cp932 -c -O3 -Wall -D NDEBUG
#CC	= clang
#CFLAGS	= --std=c11 -c -O3 -Wall -D NDEBUG
AR	= ar
ARFLAGS	= ru $(TARGET)
SOURCES	= libmana.c mana.c mana_actor.c mana_array.c mana_buffer.c mana_datalink.c mana_define.c mana_frame.c mana_function.c mana_hash.c mana_info.c mana_lzss.c mana_malloc.c mana_plugin.c mana_stack.c mana_stream.c
OBJECTS	= libmana.o mana.o mana_actor.o mana_array.o mana_buffer.o mana_datalink.o mana_define.o mana_frame.o mana_function.o mana_hash.o mana_info.o mana_lzss.o mana_malloc.o mana_plugin.o mana_stack.o mana_stream.o

.SUFFIXES: .c .o

$(TARGET): $(SOURCES) $(OBJECTS)
	$(AR) $(ARFLAGS) $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(TARGET) $(OBJECTS)

depend:
	makedepend -Y -- $(CFLAGS) -- $(SOURCES)

# DO NOT DELETE

libmana.o: libmana.h mana_define.h mana_info.h mana.h mana_actor.h
libmana.o: mana_frame.h mana_stream.h mana_hash.h mana_stack.h mana_array.h
libmana.o: mana_datalink.h mana_buffer.h mana_function.h mana_lzss.h
libmana.o: mana_malloc.h mana_plugin.h
mana.o: libmana.h mana_define.h mana_info.h mana.h mana_actor.h mana_frame.h
mana.o: mana_stream.h mana_hash.h mana_stack.h mana_array.h mana_datalink.h
mana.o: mana_buffer.h mana_function.h mana_lzss.h mana_malloc.h mana_plugin.h
mana_actor.o: mana_define.h mana_info.h mana.h mana_actor.h mana_frame.h
mana_actor.o: mana_stream.h mana_hash.h mana_stack.h mana_array.h
mana_actor.o: mana_datalink.h mana_malloc.h
mana_array.o: mana_array.h mana_malloc.h
mana_buffer.o: mana_buffer.h mana_malloc.h
mana_datalink.o: mana_datalink.h mana_malloc.h
mana_define.o: mana_define.h mana_info.h
mana_frame.o: mana_frame.h mana_stream.h mana_malloc.h
mana_function.o: libmana.h mana_define.h mana_info.h mana.h mana_actor.h
mana_function.o: mana_frame.h mana_stream.h mana_hash.h mana_stack.h
mana_function.o: mana_array.h mana_datalink.h mana_buffer.h mana_function.h
mana_function.o: mana_lzss.h mana_malloc.h mana_plugin.h
mana_hash.o: mana_hash.h mana_malloc.h
mana_info.o: mana_malloc.h mana_info.h
mana_lzss.o: mana_lzss.h mana_info.h
mana_malloc.o: mana_malloc.h
mana_plugin.o: mana_define.h mana_info.h mana_malloc.h mana_plugin.h
mana_stack.o: mana_malloc.h mana_stack.h
mana_stream.o: mana_malloc.h mana_stream.h
