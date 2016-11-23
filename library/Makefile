# makefile for mana library
# Copyright (c) 2003 Shun Moriya <shun126@users.sourceforge.jp>
#
# The MIT License
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
# THE USE OR OTHER DEALINGS IN THE SOFTWARE.

TARGET	= libmana.a
CC	= gcc
CFLAGS	= -c -O3 -Wall -D NDEBUG --input-charset=utf-8
AR	= ar
ARFLAGS	= ru $(TARGET)
HEADERS	= libmana.h mana.h mana_actor.h mana_buffer.h mana_datalink.h mana_define.h mana_frame.h mana_function.h mana_hash.h mana_info.h mana_lzss.h  mana_malloc.h mana_plugin.h mana_stack.h mana_stream.h
SOURCES	= libmana.c mana.c mana_actor.c mana_buffer.c mana_datalink.c mana_define.c mana_frame.c mana_function.c mana_hash.c mana_info.c mana_lzss.c mana_malloc.c mana_plugin.c mana_stack.c mana_stream.c
OBJECTS	= libmana.o mana.o mana_actor.o mana_buffer.o mana_datalink.o mana_define.o mana_frame.o mana_function.o mana_hash.o mana_info.o mana_lzss.o mana_malloc.o mana_plugin.o mana_stack.o mana_stream.o

.SUFFIXES: .c .o

$(TARGET): $(SOURCES) $(OBJECTS)
	$(AR) $(ARFLAGS) $(OBJECTS)

.c.o: $(HEADERS)
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
mana_buffer.o: mana_buffer.h mana_malloc.h
mana_datalink.o: mana_datalink.h mana_malloc.h
mana_define.o: mana_define.h mana_info.h
mana_frame.o: mana_frame.h mana_stream.h mana_malloc.h
mana_function.o: libmana.h mana_define.h mana_info.h mana.h mana_actor.h
mana_function.o: mana_frame.h mana_stream.h mana_hash.h mana_stack.h
mana_function.o: mana_array.h mana_datalink.h mana_buffer.h mana_function.h
mana_function.o: mana_lzss.h mana_malloc.h mana_plugin.h
mana_hash.o: mana_hash.h mana_malloc.h
mana_info.o: ../library/mana_malloc.h mana_info.h
mana_lzss.o: mana_lzss.h
mana_malloc.o: mana_malloc.h
mana_plugin.o: mana_define.h mana_info.h mana_malloc.h mana_plugin.h
mana_stack.o: mana_malloc.h mana_stack.h
mana_stream.o: mana_malloc.h mana_stream.h
