# mana (test)
#
# ProgramImageTest : 生成物の読み出しを確かめます
# EmbeddingTest    : ライブラリとして組み込む経路を確かめます
#

TARGETS = ProgramImageTest EmbeddingTest

CONFIG ?= Release

CC = clang++
CFLAGS = --std=c++17 -Wall
ifeq ($(CONFIG),Debug)
CFLAGS += -O0 -g
else
CFLAGS += -O2 -D NDEBUG
endif

COMPILER_DIR = ../compiler
LIBRARY = $(COMPILER_DIR)/libmana.a

all: $(TARGETS)

ProgramImageTest: ProgramImageTest.o
	$(CC) $(CFLAGS) -o $@ $<

# 組み込み用インターフェースの試験はコンパイラ本体を必要とします
EmbeddingTest: EmbeddingTest.o $(LIBRARY)
	$(CC) $(CFLAGS) -o $@ EmbeddingTest.o $(LIBRARY) -ldl -lm

$(LIBRARY):
	cd $(COMPILER_DIR) && $(MAKE) CONFIG=$(CONFIG)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGETS) *.o
	rm -f *.h
	rm -f *.md

depend:
	makedepend -Y -- $(CFLAGS) -- ProgramImageTest.cpp EmbeddingTest.cpp

# DO NOT DELETE

ProgramImageTest.o: ../runner/ProgramImage.h ../runner/common/Setup.h
ProgramImageTest.o: ../runner/common/Platform.h ../runner/common/Type.h
ProgramImageTest.o: ../runner/common/Assert.h ../runner/common/Fault.h
ProgramImageTest.o: ../runner/common/String.h ../runner/common/FileFormat.h
ProgramImageTest.o: ../runner/common/Math.h ../runner/common/Noncopyable.h
ProgramImageTest.o: ../runner/common/Memory.h ../runner/common/OutputStream.h
ProgramImageTest.o: ../runner/common/OutputStream.inl
ProgramImageTest.o: ../runner/common/Stopwatch.h ../runner/ProgramImage.inl
EmbeddingTest.o: ../compiler/Compiler.h ../compiler/ErrorHandler.h
EmbeddingTest.o: ../compiler/SourceResolver.h ../runner/Mana.h
EmbeddingTest.o: ../runner/common/Setup.h ../runner/common/Platform.h
EmbeddingTest.o: ../runner/common/Type.h ../runner/common/Assert.h
EmbeddingTest.o: ../runner/common/Fault.h ../runner/common/String.h
EmbeddingTest.o: ../runner/common/FileFormat.h ../runner/common/Math.h
EmbeddingTest.o: ../runner/common/Noncopyable.h ../runner/common/Memory.h
EmbeddingTest.o: ../runner/common/OutputStream.h
EmbeddingTest.o: ../runner/common/OutputStream.inl
EmbeddingTest.o: ../runner/common/Stopwatch.h ../runner/Actor.h
EmbeddingTest.o: ../runner/Buffer.h ../runner/Buffer.inl ../runner/Event.h
EmbeddingTest.o: ../runner/Stack.h ../runner/Stack.inl ../runner/Actor.inl
EmbeddingTest.o: ../runner/VM.h ../runner/VM.inl ../runner/Plugin.h
EmbeddingTest.o: ../runner/Plugin.inl ../runner/Function.h
EmbeddingTest.o: ../runner/ProgramImage.h ../runner/ProgramImage.inl
