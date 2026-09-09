# mana (driver)
# Copyright (c) 2003 Shun Moriya
#
# ../compiler のコンパイラを制御するコマンドラインツールです。
#

TARGET	= mana

COMPILER_DIR	= ../compiler
LIBRARY	= $(COMPILER_DIR)/libmana.a

#CC	= g++
CC	= clang++
CFLAGS	= --std=c++17 -O3 -Wall -D NDEBUG -c

LK	= $(CC)
LFLAGS	= --std=c++17 -O3 -Wall

COVERAGE=

CP = cp -f
RM = rm -f

SOURCES	= \
	Main.cpp

OBJECTS	= $(SOURCES:%.cpp=%.o)

.SUFFIXES: .cpp .o

$(TARGET): $(LIBRARY) $(SOURCES) $(OBJECTS)
	$(LK) $(LFLAGS) $(COVERAGE) -o $(TARGET) $(OBJECTS) $(LIBRARY) -ldl -lm

# コンパイラ本体は compiler ディレクトリで組み立てます
$(LIBRARY):
	cd $(COMPILER_DIR) && $(MAKE)

.cpp.o:
	$(CC) $(CFLAGS) $(COVERAGE) -o $@ $<

all:
	$(TARGET)

clean:
	$(RM) $(TARGET) $(OBJECTS)
	cd ../test/ && make clean

depend:
	makedepend -Y -- $(CFLAGS) -- $(SOURCES)
	cd ../test/ && make depend

cppcheck:
	cppcheck -D __unix__ --enable=all $(SOURCES)

test:
	./$(TARGET) --debug ../sample/Sample.mn -I ../sample/Function.mh
	./$(TARGET) ../test/ProgramImageTest.mn -o ../test/ProgramImageTest.mx
	cd ../test/ && $(MAKE)
	cd ../test/ && ./ProgramImageTest ProgramImageTest.mx
	cd ../test/ && ./EmbeddingTest
	cd ../test/ && python3 test.py ../driver/$(TARGET)

# DO NOT DELETE

Main.o: ../compiler/Compiler.h ../runner/common/Setup.h
Main.o: ../runner/common/Platform.h ../runner/common/Type.h
Main.o: ../runner/common/Assert.h ../runner/common/String.h
Main.o: ../runner/common/FileFormat.h ../runner/common/Math.h
Main.o: ../runner/common/Noncopyable.h ../runner/common/Memory.h
Main.o: ../runner/common/OutputStream.h ../runner/common/OutputStream.inl
Main.o: ../runner/common/Stopwatch.h ../compiler/CodeBuffer.h ../runner/common/Platform.h
Main.o: ../runner/common/FileFormat.h ../runner/common/Memory.h
Main.o: ../runner/common/Noncopyable.h ../compiler/DataBuffer.h ../compiler/GlobalAddressResolver.h
Main.o: ../compiler/GlobalSemanticAnalyzer.h ../compiler/SemanticAnalyzer.h ../compiler/Symbol.h ../compiler/TypeDescriptor.h
Main.o: ../compiler/SymbolFactory.h ../compiler/StringPool.h ../compiler/TypeDescriptorFactory.h ../compiler/SyntaxNode.h
Main.o: ../compiler/LocalAddressResolver.h ../compiler/LocalSemanticAnalyzer.h ../compiler/ErrorHandler.h ../compiler/Lexer.h
Main.o: ../compiler/Path.h ../compiler/ParsingDriver.h ../compiler/Version.h ../runner/Mana.h
Main.o: ../runner/common/Setup.h ../runner/Actor.h ../runner/Buffer.h
Main.o: ../runner/Buffer.inl ../runner/Event.h ../runner/Stack.h
Main.o: ../runner/common/Platform.h ../runner/common/Noncopyable.h
Main.o: ../runner/common/OutputStream.h ../runner/Stack.inl
Main.o: ../runner/Actor.inl ../runner/VM.h ../runner/VM.inl
Main.o: ../runner/Plugin.h ../runner/Plugin.inl ../runner/common/FileFormat.h
Main.o: ../runner/Function.h ../runner/ProgramImage.h
Main.o: ../runner/ProgramImage.inl
