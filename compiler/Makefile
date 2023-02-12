# mana (compiler)
# Copyright (c) 2003 Shun Moriya
#

TARGET	= mana

#CC	= g++
CC	= clang++
CFLAGS	= --std=c++17 -O3 -Wall -D NDEBUG -c 

LK	= $(CC)
LFLAGS	= --std=c++17 -O3 -Wall

COVERAGE=

YACC = bison
YFLAGS	= -d

LEX	= flex
LXFLAGS	= -8 -F

CP = cp -f
RM = rm -f

SOURCES	= \
	CodeBuffer.cpp \
	CodeGenerator.cpp \
	DataBuffer.cpp \
	ErrorHandler.cpp \
	GlobalAddressResolver.cpp \
	GlobalSemanticAnalyzer.cpp \
	IntermediateLanguage.cpp \
	LocalAddressResolver.cpp \
	LocalSemanticAnalyzer.cpp \
	Main.cpp \
	ParsingDriver.cpp \
	Path.cpp \
	Runner.cpp \
	SemanticAnalyzer.cpp \
	StringPool.cpp \
	Symbol.cpp \
	SymbolFactory.cpp \
	SyntaxNode.cpp \
	TypeDescriptor.cpp \
	TypeDescriptorFactory.cpp

#Lexer.cpp \
#Parser.cpp \
#Register.cpp \

PARSER	= Parser.yy
LEXER	= Lexer.l
OBJECTS	= $(SOURCES:%.cpp=%.o) $(PARSER:%.yy=%.o) $(LEXER:%.l=%.o)
SECOUND	= Parser.cpp Parser.hpp Lexer.cpp

.SUFFIXES: .yy .l .cpp .o
.SECOUNDLY: $(SECOUND)

$(TARGET): $(SOURCES) $(PARSER) $(LEXER) $(SECOUND) $(OBJECTS)
	$(LK) $(LFLAGS) $(COVERAGE) -o $(TARGET) $(OBJECTS) -ldl -lm

.yy.cpp:
	$(YACC) $(YFLAGS) -o $@ $<

.l.cpp:
	$(LEX) $(LXFLAGS) -o$@ $<

.cpp.o:
	$(CC) $(CFLAGS) $(COVERAGE) -o $@ $<

all:
	$(TARGET)

clean:
	$(RM) $(TARGET) $(SECOUND) $(OBJECTS)

depend:
	makedepend -Y -- $(CFLAGS) -- $(SOURCES)

cppcheck:
	cppcheck -D __unix__ --enable=all $(SOURCES)

test:
	./$(TARGET) --debug ../sample/sample.mn
	cd ../test/ && python3 test.py

version:
	python3 Version.py

# DO NOT DELETE

CodeBuffer.o: CodeBuffer.h ../runner/common/Platform.h
CodeBuffer.o: ../runner/common/Type.h ../runner/common/FileFormat.h
CodeBuffer.o: ../runner/common/Platform.h ../runner/common/Memory.h
CodeBuffer.o: ../runner/common/Noncopyable.h ../runner/common/OutputStream.h
CodeBuffer.o: ../runner/common/String.h ../runner/common/OutputStream.inl
CodeGenerator.o: CodeGenerator.h ../runner/common/Setup.h
CodeGenerator.o: ../runner/common/Platform.h ../runner/common/Type.h
CodeGenerator.o: ../runner/common/Assert.h ../runner/common/String.h
CodeGenerator.o: ../runner/common/FileFormat.h ../runner/common/Memory.h
CodeGenerator.o: ../runner/common/Math.h ../runner/common/Noncopyable.h
CodeGenerator.o: ../runner/common/OutputStream.h
CodeGenerator.o: ../runner/common/OutputStream.inl
CodeGenerator.o: ../runner/common/Stopwatch.h CodeBuffer.h
CodeGenerator.o: ../runner/common/Platform.h ../runner/common/FileFormat.h
CodeGenerator.o: ../runner/common/Noncopyable.h DataBuffer.h
CodeGenerator.o: GlobalAddressResolver.h GlobalSemanticAnalyzer.h
CodeGenerator.o: SemanticAnalyzer.h Symbol.h TypeDescriptor.h SymbolFactory.h
CodeGenerator.o: StringPool.h TypeDescriptorFactory.h SyntaxNode.h
CodeGenerator.o: LocalAddressResolver.h LocalSemanticAnalyzer.h
CodeGenerator.o: ErrorHandler.h IntermediateLanguage.h
CodeGenerator.o: ../runner/common/Memory.h
DataBuffer.o: DataBuffer.h ../runner/common/Platform.h
DataBuffer.o: ../runner/common/Type.h ../runner/common/Noncopyable.h
DataBuffer.o: ../runner/common/OutputStream.h ../runner/common/Platform.h
DataBuffer.o: ../runner/common/String.h ../runner/common/OutputStream.inl
ErrorHandler.o: ../runner/common/Setup.h ../runner/common/Platform.h
ErrorHandler.o: ../runner/common/Type.h ../runner/common/Assert.h
ErrorHandler.o: ../runner/common/String.h ../runner/common/FileFormat.h
ErrorHandler.o: ../runner/common/Memory.h ../runner/common/Math.h
ErrorHandler.o: ../runner/common/Noncopyable.h
ErrorHandler.o: ../runner/common/OutputStream.h
ErrorHandler.o: ../runner/common/OutputStream.inl
ErrorHandler.o: ../runner/common/Stopwatch.h ErrorHandler.h Lexer.h Main.h
GlobalAddressResolver.o: GlobalAddressResolver.h ../runner/common/Setup.h
GlobalAddressResolver.o: ../runner/common/Platform.h ../runner/common/Type.h
GlobalAddressResolver.o: ../runner/common/Assert.h ../runner/common/String.h
GlobalAddressResolver.o: ../runner/common/FileFormat.h
GlobalAddressResolver.o: ../runner/common/Memory.h ../runner/common/Math.h
GlobalAddressResolver.o: ../runner/common/Noncopyable.h
GlobalAddressResolver.o: ../runner/common/OutputStream.h
GlobalAddressResolver.o: ../runner/common/OutputStream.inl
GlobalAddressResolver.o: ../runner/common/Stopwatch.h CodeBuffer.h
GlobalAddressResolver.o: ../runner/common/Platform.h
GlobalAddressResolver.o: ../runner/common/FileFormat.h
GlobalAddressResolver.o: ../runner/common/Noncopyable.h Symbol.h
GlobalAddressResolver.o: TypeDescriptor.h
GlobalSemanticAnalyzer.o: GlobalSemanticAnalyzer.h SemanticAnalyzer.h
GlobalSemanticAnalyzer.o: ../runner/common/Setup.h
GlobalSemanticAnalyzer.o: ../runner/common/Platform.h ../runner/common/Type.h
GlobalSemanticAnalyzer.o: ../runner/common/Assert.h ../runner/common/String.h
GlobalSemanticAnalyzer.o: ../runner/common/FileFormat.h
GlobalSemanticAnalyzer.o: ../runner/common/Memory.h ../runner/common/Math.h
GlobalSemanticAnalyzer.o: ../runner/common/Noncopyable.h
GlobalSemanticAnalyzer.o: ../runner/common/OutputStream.h
GlobalSemanticAnalyzer.o: ../runner/common/OutputStream.inl
GlobalSemanticAnalyzer.o: ../runner/common/Stopwatch.h Symbol.h
GlobalSemanticAnalyzer.o: TypeDescriptor.h ../runner/common/Platform.h
GlobalSemanticAnalyzer.o: SymbolFactory.h StringPool.h
GlobalSemanticAnalyzer.o: ../runner/common/Noncopyable.h
GlobalSemanticAnalyzer.o: TypeDescriptorFactory.h SyntaxNode.h ErrorHandler.h
IntermediateLanguage.o: IntermediateLanguage.h ../runner/common/Platform.h
IntermediateLanguage.o: ../runner/common/Type.h ../runner/common/Memory.h
IntermediateLanguage.o: CodeBuffer.h ../runner/common/FileFormat.h
IntermediateLanguage.o: ../runner/common/Platform.h ../runner/common/Memory.h
IntermediateLanguage.o: ../runner/common/Noncopyable.h
IntermediateLanguage.o: ../runner/common/Setup.h ../runner/common/Assert.h
IntermediateLanguage.o: ../runner/common/String.h
IntermediateLanguage.o: ../runner/common/FileFormat.h ../runner/common/Math.h
IntermediateLanguage.o: ../runner/common/Noncopyable.h
IntermediateLanguage.o: ../runner/common/OutputStream.h
IntermediateLanguage.o: ../runner/common/OutputStream.inl
IntermediateLanguage.o: ../runner/common/Stopwatch.h
LocalAddressResolver.o: LocalAddressResolver.h ../runner/common/Setup.h
LocalAddressResolver.o: ../runner/common/Platform.h ../runner/common/Type.h
LocalAddressResolver.o: ../runner/common/Assert.h ../runner/common/String.h
LocalAddressResolver.o: ../runner/common/FileFormat.h
LocalAddressResolver.o: ../runner/common/Memory.h ../runner/common/Math.h
LocalAddressResolver.o: ../runner/common/Noncopyable.h
LocalAddressResolver.o: ../runner/common/OutputStream.h
LocalAddressResolver.o: ../runner/common/OutputStream.inl
LocalAddressResolver.o: ../runner/common/Stopwatch.h CodeBuffer.h
LocalAddressResolver.o: ../runner/common/Platform.h
LocalAddressResolver.o: ../runner/common/FileFormat.h
LocalAddressResolver.o: ../runner/common/Noncopyable.h SyntaxNode.h
LocalAddressResolver.o: TypeDescriptor.h CodeGenerator.h DataBuffer.h
LocalAddressResolver.o: GlobalAddressResolver.h GlobalSemanticAnalyzer.h
LocalAddressResolver.o: SemanticAnalyzer.h Symbol.h SymbolFactory.h
LocalAddressResolver.o: StringPool.h TypeDescriptorFactory.h
LocalAddressResolver.o: LocalSemanticAnalyzer.h ErrorHandler.h
LocalSemanticAnalyzer.o: LocalSemanticAnalyzer.h SemanticAnalyzer.h
LocalSemanticAnalyzer.o: ../runner/common/Setup.h ../runner/common/Platform.h
LocalSemanticAnalyzer.o: ../runner/common/Type.h ../runner/common/Assert.h
LocalSemanticAnalyzer.o: ../runner/common/String.h
LocalSemanticAnalyzer.o: ../runner/common/FileFormat.h
LocalSemanticAnalyzer.o: ../runner/common/Memory.h ../runner/common/Math.h
LocalSemanticAnalyzer.o: ../runner/common/Noncopyable.h
LocalSemanticAnalyzer.o: ../runner/common/OutputStream.h
LocalSemanticAnalyzer.o: ../runner/common/OutputStream.inl
LocalSemanticAnalyzer.o: ../runner/common/Stopwatch.h Symbol.h
LocalSemanticAnalyzer.o: TypeDescriptor.h ../runner/common/Platform.h
LocalSemanticAnalyzer.o: SymbolFactory.h StringPool.h
LocalSemanticAnalyzer.o: ../runner/common/Noncopyable.h
LocalSemanticAnalyzer.o: TypeDescriptorFactory.h SyntaxNode.h ErrorHandler.h
Main.o: Main.h CodeGenerator.h ../runner/common/Setup.h
Main.o: ../runner/common/Platform.h ../runner/common/Type.h
Main.o: ../runner/common/Assert.h ../runner/common/String.h
Main.o: ../runner/common/FileFormat.h ../runner/common/Memory.h
Main.o: ../runner/common/Math.h ../runner/common/Noncopyable.h
Main.o: ../runner/common/OutputStream.h ../runner/common/OutputStream.inl
Main.o: ../runner/common/Stopwatch.h CodeBuffer.h ../runner/common/Platform.h
Main.o: ../runner/common/FileFormat.h ../runner/common/Noncopyable.h
Main.o: DataBuffer.h GlobalAddressResolver.h GlobalSemanticAnalyzer.h
Main.o: SemanticAnalyzer.h Symbol.h TypeDescriptor.h SymbolFactory.h
Main.o: StringPool.h TypeDescriptorFactory.h SyntaxNode.h
Main.o: LocalAddressResolver.h LocalSemanticAnalyzer.h ErrorHandler.h Lexer.h
Main.o: Path.h ParsingDriver.h Parser.hpp Version.h ../runner/Mana.h
Main.o: ../runner/common/Setup.h ../runner/Actor.h ../runner/Buffer.h
Main.o: ../runner/common/Platform.h ../runner/Buffer.inl ../runner/Stack.h
Main.o: ../runner/common/Assert.h ../runner/common/Noncopyable.h
Main.o: ../runner/common/OutputStream.h ../runner/Stack.inl
Main.o: ../runner/Actor.inl ../runner/VM.h ../runner/VM.inl
Main.o: ../runner/Plugin.h ../runner/Plugin.inl ../runner/common/FileFormat.h
Main.o: ../runner/Function.h
ParsingDriver.o: ../runner/common/Setup.h ../runner/common/Platform.h
ParsingDriver.o: ../runner/common/Type.h ../runner/common/Assert.h
ParsingDriver.o: ../runner/common/String.h ../runner/common/FileFormat.h
ParsingDriver.o: ../runner/common/Memory.h ../runner/common/Math.h
ParsingDriver.o: ../runner/common/Noncopyable.h
ParsingDriver.o: ../runner/common/OutputStream.h
ParsingDriver.o: ../runner/common/OutputStream.inl
ParsingDriver.o: ../runner/common/Stopwatch.h ParsingDriver.h
ParsingDriver.o: ../runner/common/Noncopyable.h Parser.hpp SyntaxNode.h
ParsingDriver.o: TypeDescriptor.h ../runner/common/Platform.h CodeBuffer.h
ParsingDriver.o: ../runner/common/FileFormat.h CodeGenerator.h DataBuffer.h
ParsingDriver.o: GlobalAddressResolver.h GlobalSemanticAnalyzer.h
ParsingDriver.o: SemanticAnalyzer.h Symbol.h SymbolFactory.h StringPool.h
ParsingDriver.o: TypeDescriptorFactory.h LocalAddressResolver.h
ParsingDriver.o: LocalSemanticAnalyzer.h
Path.o: Path.h ../runner/common/Setup.h ../runner/common/Platform.h
Path.o: ../runner/common/Type.h ../runner/common/Assert.h
Path.o: ../runner/common/String.h ../runner/common/FileFormat.h
Path.o: ../runner/common/Memory.h ../runner/common/Math.h
Path.o: ../runner/common/Noncopyable.h ../runner/common/OutputStream.h
Path.o: ../runner/common/OutputStream.inl ../runner/common/Stopwatch.h
Runner.o: ../runner/Mana.h ../runner/common/Setup.h
Runner.o: ../runner/common/Platform.h ../runner/common/Type.h
Runner.o: ../runner/common/Assert.h ../runner/common/String.h
Runner.o: ../runner/common/FileFormat.h ../runner/common/Memory.h
Runner.o: ../runner/common/Math.h ../runner/common/Noncopyable.h
Runner.o: ../runner/common/OutputStream.h ../runner/common/OutputStream.inl
Runner.o: ../runner/common/Stopwatch.h ../runner/Actor.h ../runner/Buffer.h
Runner.o: ../runner/common/Platform.h ../runner/Buffer.inl ../runner/Stack.h
Runner.o: ../runner/common/Assert.h ../runner/common/Noncopyable.h
Runner.o: ../runner/common/OutputStream.h ../runner/Stack.inl
Runner.o: ../runner/Actor.inl ../runner/VM.h ../runner/VM.inl
Runner.o: ../runner/Plugin.h ../runner/Plugin.inl
Runner.o: ../runner/common/FileFormat.h ../runner/Function.h
SemanticAnalyzer.o: SemanticAnalyzer.h ../runner/common/Setup.h
SemanticAnalyzer.o: ../runner/common/Platform.h ../runner/common/Type.h
SemanticAnalyzer.o: ../runner/common/Assert.h ../runner/common/String.h
SemanticAnalyzer.o: ../runner/common/FileFormat.h ../runner/common/Memory.h
SemanticAnalyzer.o: ../runner/common/Math.h ../runner/common/Noncopyable.h
SemanticAnalyzer.o: ../runner/common/OutputStream.h
SemanticAnalyzer.o: ../runner/common/OutputStream.inl
SemanticAnalyzer.o: ../runner/common/Stopwatch.h Symbol.h TypeDescriptor.h
SemanticAnalyzer.o: ../runner/common/Platform.h SymbolFactory.h StringPool.h
SemanticAnalyzer.o: ../runner/common/Noncopyable.h TypeDescriptorFactory.h
SemanticAnalyzer.o: SyntaxNode.h Lexer.h ErrorHandler.h
StringPool.o: StringPool.h ../runner/common/Platform.h
StringPool.o: ../runner/common/Type.h ../runner/common/Noncopyable.h
Symbol.o: Symbol.h ../runner/common/Setup.h ../runner/common/Platform.h
Symbol.o: ../runner/common/Type.h ../runner/common/Assert.h
Symbol.o: ../runner/common/String.h ../runner/common/FileFormat.h
Symbol.o: ../runner/common/Memory.h ../runner/common/Math.h
Symbol.o: ../runner/common/Noncopyable.h ../runner/common/OutputStream.h
Symbol.o: ../runner/common/OutputStream.inl ../runner/common/Stopwatch.h
Symbol.o: TypeDescriptor.h ../runner/common/Platform.h ErrorHandler.h
SymbolFactory.o: SymbolFactory.h ../runner/common/Setup.h
SymbolFactory.o: ../runner/common/Platform.h ../runner/common/Type.h
SymbolFactory.o: ../runner/common/Assert.h ../runner/common/String.h
SymbolFactory.o: ../runner/common/FileFormat.h ../runner/common/Memory.h
SymbolFactory.o: ../runner/common/Math.h ../runner/common/Noncopyable.h
SymbolFactory.o: ../runner/common/OutputStream.h
SymbolFactory.o: ../runner/common/OutputStream.inl
SymbolFactory.o: ../runner/common/Stopwatch.h StringPool.h
SymbolFactory.o: ../runner/common/Platform.h ../runner/common/Noncopyable.h
SymbolFactory.o: Symbol.h TypeDescriptor.h TypeDescriptorFactory.h
SymbolFactory.o: CodeBuffer.h ../runner/common/FileFormat.h CodeGenerator.h
SymbolFactory.o: DataBuffer.h GlobalAddressResolver.h
SymbolFactory.o: GlobalSemanticAnalyzer.h SemanticAnalyzer.h SyntaxNode.h
SymbolFactory.o: LocalAddressResolver.h LocalSemanticAnalyzer.h
SymbolFactory.o: ErrorHandler.h Path.h
SyntaxNode.o: SyntaxNode.h ../runner/common/Setup.h
SyntaxNode.o: ../runner/common/Platform.h ../runner/common/Type.h
SyntaxNode.o: ../runner/common/Assert.h ../runner/common/String.h
SyntaxNode.o: ../runner/common/FileFormat.h ../runner/common/Memory.h
SyntaxNode.o: ../runner/common/Math.h ../runner/common/Noncopyable.h
SyntaxNode.o: ../runner/common/OutputStream.h
SyntaxNode.o: ../runner/common/OutputStream.inl ../runner/common/Stopwatch.h
SyntaxNode.o: Lexer.h Main.h Symbol.h TypeDescriptor.h
SyntaxNode.o: ../runner/common/Platform.h TypeDescriptorFactory.h
SyntaxNode.o: ../runner/common/Noncopyable.h
TypeDescriptor.o: TypeDescriptor.h ../runner/common/Platform.h
TypeDescriptor.o: ../runner/common/Type.h ErrorHandler.h Symbol.h
TypeDescriptor.o: ../runner/common/Setup.h ../runner/common/Platform.h
TypeDescriptor.o: ../runner/common/Assert.h ../runner/common/String.h
TypeDescriptor.o: ../runner/common/FileFormat.h ../runner/common/Memory.h
TypeDescriptor.o: ../runner/common/Math.h ../runner/common/Noncopyable.h
TypeDescriptor.o: ../runner/common/OutputStream.h
TypeDescriptor.o: ../runner/common/OutputStream.inl
TypeDescriptor.o: ../runner/common/Stopwatch.h
TypeDescriptorFactory.o: TypeDescriptorFactory.h ../runner/common/Platform.h
TypeDescriptorFactory.o: ../runner/common/Type.h
TypeDescriptorFactory.o: ../runner/common/Noncopyable.h TypeDescriptor.h
