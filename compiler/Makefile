# mana (compiler)
# Copyright (c) 2003 Shun Moriya
#
# アプリケーションへ組み込めるコンパイラ本体です。
# コマンドラインインターフェースは ../driver にあります。
#

LIBRARY	= libmana.a

# Debug では表明を有効なまま残し、Release では NDEBUG で無効にします。
# 片方でしか回さないと、表明が無いと見えない不具合を取り逃します。
CONFIG	?= Release

#CC	= g++
CC	= clang++
CFLAGS	= --std=c++17 -Wall -c
ifeq ($(CONFIG),Debug)
CFLAGS	+= -O0 -g
else
CFLAGS	+= -O3 -D NDEBUG
endif

AR	= ar
ARLIBFLAGS = rcs

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
	Compiler.cpp \
	DataBuffer.cpp \
	ErrorHandler.cpp \
	GlobalAddressResolver.cpp \
	GlobalSemanticAnalyzer.cpp \
	IntermediateLanguage.cpp \
	LocalAddressResolver.cpp \
	LocalSemanticAnalyzer.cpp \
	NamespaceRegistry.cpp \
	ParsingDriver.cpp \
	Path.cpp \
	SemanticAnalyzer.cpp \
	SourceResolver.cpp \
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

$(LIBRARY): $(SOURCES) $(PARSER) $(LEXER) $(SECOUND) $(OBJECTS)
	$(RM) $(LIBRARY)
	$(AR) $(ARLIBFLAGS) $(LIBRARY) $(OBJECTS)

.yy.cpp:
	$(YACC) $(YFLAGS) -o $@ $<

.l.cpp:
	$(LEX) $(LXFLAGS) -o$@ $<

.cpp.o:
	$(CC) $(CFLAGS) $(COVERAGE) -o $@ $<

all:
	$(LIBRARY)

clean:
	$(RM) $(LIBRARY) $(SECOUND) $(OBJECTS)

depend:
	makedepend -Y -- $(CFLAGS) -- $(SOURCES)

cppcheck:
	cppcheck -D __unix__ --enable=all $(SOURCES) $(SECOUND)

version:
	python3 Version.py

# DO NOT DELETE

SourceResolver.o: SourceResolver.h Path.h ../runner/common/Setup.h
SourceResolver.o: ../runner/common/Platform.h ../runner/common/Type.h

CodeBuffer.o: CodeBuffer.h ../runner/common/Platform.h
CodeBuffer.o: ../runner/common/Type.h ../runner/common/FileFormat.h
CodeBuffer.o: ../runner/common/Platform.h ../runner/common/Memory.h
CodeBuffer.o: ../runner/common/Noncopyable.h ../runner/common/OutputStream.h
CodeBuffer.o: ../runner/common/OutputStream.inl ../runner/common/String.h
CodeGenerator.o: CodeGenerator.h ../runner/common/Setup.h
CodeGenerator.o: ../runner/common/Platform.h ../runner/common/Type.h
CodeGenerator.o: ../runner/common/Assert.h ../runner/common/String.h
CodeGenerator.o: ../runner/common/FileFormat.h ../runner/common/Math.h
CodeGenerator.o: ../runner/common/Noncopyable.h ../runner/common/Memory.h
CodeGenerator.o: ../runner/common/OutputStream.h
CodeGenerator.o: ../runner/common/OutputStream.inl
CodeGenerator.o: ../runner/common/Stopwatch.h CodeBuffer.h
CodeGenerator.o: ../runner/common/Platform.h ../runner/common/FileFormat.h
CodeGenerator.o: ../runner/common/Memory.h ../runner/common/Noncopyable.h
CodeGenerator.o: DataBuffer.h GlobalAddressResolver.h
CodeGenerator.o: GlobalSemanticAnalyzer.h SemanticAnalyzer.h Symbol.h
CodeGenerator.o: TypeDescriptor.h SymbolFactory.h StringPool.h
CodeGenerator.o: TypeDescriptorFactory.h SyntaxNode.h LocalAddressResolver.h
CodeGenerator.o: LocalSemanticAnalyzer.h ErrorHandler.h
CodeGenerator.o: IntermediateLanguage.h
DataBuffer.o: DataBuffer.h ../runner/common/Platform.h
DataBuffer.o: ../runner/common/Type.h ../runner/common/Noncopyable.h
DataBuffer.o: ../runner/common/OutputStream.h ../runner/common/Platform.h
DataBuffer.o: ../runner/common/OutputStream.inl ../runner/common/String.h
ErrorHandler.o: ../runner/common/Setup.h ../runner/common/Platform.h
ErrorHandler.o: ../runner/common/Type.h ../runner/common/Assert.h
ErrorHandler.o: ../runner/common/String.h ../runner/common/FileFormat.h
ErrorHandler.o: ../runner/common/Math.h ../runner/common/Noncopyable.h
ErrorHandler.o: ../runner/common/Memory.h ../runner/common/OutputStream.h
ErrorHandler.o: ../runner/common/OutputStream.inl
ErrorHandler.o: ../runner/common/Stopwatch.h ErrorHandler.h Lexer.h
GlobalAddressResolver.o: GlobalAddressResolver.h ../runner/common/Setup.h
GlobalAddressResolver.o: ../runner/common/Platform.h ../runner/common/Type.h
GlobalAddressResolver.o: ../runner/common/Assert.h ../runner/common/String.h
GlobalAddressResolver.o: ../runner/common/FileFormat.h
GlobalAddressResolver.o: ../runner/common/Math.h
GlobalAddressResolver.o: ../runner/common/Noncopyable.h
GlobalAddressResolver.o: ../runner/common/Memory.h
GlobalAddressResolver.o: ../runner/common/OutputStream.h
GlobalAddressResolver.o: ../runner/common/OutputStream.inl
GlobalAddressResolver.o: ../runner/common/Stopwatch.h CodeBuffer.h
GlobalAddressResolver.o: ../runner/common/Platform.h
GlobalAddressResolver.o: ../runner/common/FileFormat.h
GlobalAddressResolver.o: ../runner/common/Memory.h
GlobalAddressResolver.o: ../runner/common/Noncopyable.h Symbol.h
GlobalAddressResolver.o: TypeDescriptor.h
GlobalSemanticAnalyzer.o: GlobalSemanticAnalyzer.h SemanticAnalyzer.h
GlobalSemanticAnalyzer.o: ../runner/common/Setup.h
GlobalSemanticAnalyzer.o: ../runner/common/Platform.h ../runner/common/Type.h
GlobalSemanticAnalyzer.o: ../runner/common/Assert.h ../runner/common/String.h
GlobalSemanticAnalyzer.o: ../runner/common/FileFormat.h
GlobalSemanticAnalyzer.o: ../runner/common/Math.h
GlobalSemanticAnalyzer.o: ../runner/common/Noncopyable.h
GlobalSemanticAnalyzer.o: ../runner/common/Memory.h
GlobalSemanticAnalyzer.o: ../runner/common/OutputStream.h
GlobalSemanticAnalyzer.o: ../runner/common/OutputStream.inl
GlobalSemanticAnalyzer.o: ../runner/common/Stopwatch.h Symbol.h
GlobalSemanticAnalyzer.o: TypeDescriptor.h ../runner/common/Platform.h
GlobalSemanticAnalyzer.o: SymbolFactory.h StringPool.h
GlobalSemanticAnalyzer.o: TypeDescriptorFactory.h
GlobalSemanticAnalyzer.o: ../runner/common/Noncopyable.h SyntaxNode.h
GlobalSemanticAnalyzer.o: ErrorHandler.h NamespaceRegistry.h
IntermediateLanguage.o: IntermediateLanguage.h ../runner/common/Platform.h
IntermediateLanguage.o: ../runner/common/Type.h CodeBuffer.h
IntermediateLanguage.o: ../runner/common/FileFormat.h
IntermediateLanguage.o: ../runner/common/Platform.h ../runner/common/Memory.h
IntermediateLanguage.o: ../runner/common/Noncopyable.h
LocalAddressResolver.o: LocalAddressResolver.h ../runner/common/Setup.h
LocalAddressResolver.o: ../runner/common/Platform.h ../runner/common/Type.h
LocalAddressResolver.o: ../runner/common/Assert.h ../runner/common/String.h
LocalAddressResolver.o: ../runner/common/FileFormat.h ../runner/common/Math.h
LocalAddressResolver.o: ../runner/common/Noncopyable.h
LocalAddressResolver.o: ../runner/common/Memory.h
LocalAddressResolver.o: ../runner/common/OutputStream.h
LocalAddressResolver.o: ../runner/common/OutputStream.inl
LocalAddressResolver.o: ../runner/common/Stopwatch.h CodeBuffer.h
LocalAddressResolver.o: ../runner/common/Platform.h
LocalAddressResolver.o: ../runner/common/FileFormat.h
LocalAddressResolver.o: ../runner/common/Memory.h
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
LocalSemanticAnalyzer.o: ../runner/common/Math.h
LocalSemanticAnalyzer.o: ../runner/common/Noncopyable.h
LocalSemanticAnalyzer.o: ../runner/common/Memory.h
LocalSemanticAnalyzer.o: ../runner/common/OutputStream.h
LocalSemanticAnalyzer.o: ../runner/common/OutputStream.inl
LocalSemanticAnalyzer.o: ../runner/common/Stopwatch.h Symbol.h
LocalSemanticAnalyzer.o: TypeDescriptor.h ../runner/common/Platform.h
LocalSemanticAnalyzer.o: SymbolFactory.h StringPool.h TypeDescriptorFactory.h
LocalSemanticAnalyzer.o: ../runner/common/Noncopyable.h SyntaxNode.h
LocalSemanticAnalyzer.o: ErrorHandler.h NamespaceRegistry.h
NamespaceRegistry.o: NamespaceRegistry.h StringPool.h
NamespaceRegistry.o: ../runner/common/Platform.h ../runner/common/Type.h
ParsingDriver.o: ../runner/common/Setup.h ../runner/common/Platform.h
ParsingDriver.o: ../runner/common/Type.h ../runner/common/Assert.h
ParsingDriver.o: ../runner/common/String.h ../runner/common/FileFormat.h
ParsingDriver.o: ../runner/common/Math.h ../runner/common/Noncopyable.h
ParsingDriver.o: ../runner/common/Memory.h ../runner/common/OutputStream.h
ParsingDriver.o: ../runner/common/OutputStream.inl
ParsingDriver.o: ../runner/common/Stopwatch.h ParsingDriver.h
ParsingDriver.o: ../runner/common/Noncopyable.h CodeBuffer.h
ParsingDriver.o: ../runner/common/Platform.h ../runner/common/FileFormat.h
ParsingDriver.o: ../runner/common/Memory.h CodeGenerator.h DataBuffer.h
ParsingDriver.o: GlobalAddressResolver.h GlobalSemanticAnalyzer.h
ParsingDriver.o: SemanticAnalyzer.h Symbol.h TypeDescriptor.h SymbolFactory.h
ParsingDriver.o: StringPool.h TypeDescriptorFactory.h SyntaxNode.h
ParsingDriver.o: LocalAddressResolver.h LocalSemanticAnalyzer.h
ParsingDriver.o: ErrorHandler.h NamespaceRegistry.h
Path.o: Path.h ../runner/common/Setup.h ../runner/common/Platform.h
Path.o: ../runner/common/Type.h ../runner/common/Assert.h
Path.o: ../runner/common/String.h ../runner/common/FileFormat.h
Path.o: ../runner/common/Math.h ../runner/common/Noncopyable.h
Path.o: ../runner/common/Memory.h ../runner/common/OutputStream.h
Path.o: ../runner/common/OutputStream.inl ../runner/common/Stopwatch.h
SemanticAnalyzer.o: SemanticAnalyzer.h ../runner/common/Setup.h
SemanticAnalyzer.o: ../runner/common/Platform.h ../runner/common/Type.h
SemanticAnalyzer.o: ../runner/common/Assert.h ../runner/common/String.h
SemanticAnalyzer.o: ../runner/common/FileFormat.h ../runner/common/Math.h
SemanticAnalyzer.o: ../runner/common/Noncopyable.h ../runner/common/Memory.h
SemanticAnalyzer.o: ../runner/common/OutputStream.h
SemanticAnalyzer.o: ../runner/common/OutputStream.inl
SemanticAnalyzer.o: ../runner/common/Stopwatch.h Symbol.h TypeDescriptor.h
SemanticAnalyzer.o: ../runner/common/Platform.h SymbolFactory.h StringPool.h
SemanticAnalyzer.o: TypeDescriptorFactory.h ../runner/common/Noncopyable.h
SemanticAnalyzer.o: SyntaxNode.h Lexer.h ErrorHandler.h
StringPool.o: StringPool.h ../runner/common/Platform.h
StringPool.o: ../runner/common/Type.h
Symbol.o: Symbol.h ../runner/common/Setup.h ../runner/common/Platform.h
Symbol.o: ../runner/common/Type.h ../runner/common/Assert.h
Symbol.o: ../runner/common/String.h ../runner/common/FileFormat.h
Symbol.o: ../runner/common/Math.h ../runner/common/Noncopyable.h
Symbol.o: ../runner/common/Memory.h ../runner/common/OutputStream.h
Symbol.o: ../runner/common/OutputStream.inl ../runner/common/Stopwatch.h
Symbol.o: TypeDescriptor.h ../runner/common/Platform.h ErrorHandler.h
SymbolFactory.o: SymbolFactory.h ../runner/common/Setup.h
SymbolFactory.o: ../runner/common/Platform.h ../runner/common/Type.h
SymbolFactory.o: ../runner/common/Assert.h ../runner/common/String.h
SymbolFactory.o: ../runner/common/FileFormat.h ../runner/common/Math.h
SymbolFactory.o: ../runner/common/Noncopyable.h ../runner/common/Memory.h
SymbolFactory.o: ../runner/common/OutputStream.h
SymbolFactory.o: ../runner/common/OutputStream.inl
SymbolFactory.o: ../runner/common/Stopwatch.h StringPool.h
SymbolFactory.o: ../runner/common/Platform.h Symbol.h TypeDescriptor.h
SymbolFactory.o: TypeDescriptorFactory.h ../runner/common/Noncopyable.h
SymbolFactory.o: CodeBuffer.h ../runner/common/FileFormat.h
SymbolFactory.o: ../runner/common/Memory.h CodeGenerator.h DataBuffer.h
SymbolFactory.o: GlobalAddressResolver.h GlobalSemanticAnalyzer.h
SymbolFactory.o: SemanticAnalyzer.h SyntaxNode.h LocalAddressResolver.h
SymbolFactory.o: LocalSemanticAnalyzer.h ErrorHandler.h
SyntaxNode.o: SyntaxNode.h ../runner/common/Setup.h
SyntaxNode.o: ../runner/common/Platform.h ../runner/common/Type.h
SyntaxNode.o: ../runner/common/Assert.h ../runner/common/String.h
SyntaxNode.o: ../runner/common/FileFormat.h ../runner/common/Math.h
SyntaxNode.o: ../runner/common/Noncopyable.h ../runner/common/Memory.h
SyntaxNode.o: ../runner/common/OutputStream.h
SyntaxNode.o: ../runner/common/OutputStream.inl ../runner/common/Stopwatch.h
SyntaxNode.o: Lexer.h Symbol.h TypeDescriptor.h ../runner/common/Platform.h
SyntaxNode.o: TypeDescriptorFactory.h ../runner/common/Noncopyable.h
TypeDescriptor.o: TypeDescriptor.h ../runner/common/Platform.h
TypeDescriptor.o: ../runner/common/Type.h ErrorHandler.h Symbol.h
TypeDescriptor.o: ../runner/common/Setup.h ../runner/common/Platform.h
TypeDescriptor.o: ../runner/common/Assert.h ../runner/common/String.h
TypeDescriptor.o: ../runner/common/FileFormat.h ../runner/common/Math.h
TypeDescriptor.o: ../runner/common/Noncopyable.h ../runner/common/Memory.h
TypeDescriptor.o: ../runner/common/OutputStream.h
TypeDescriptor.o: ../runner/common/OutputStream.inl
TypeDescriptor.o: ../runner/common/Stopwatch.h
TypeDescriptorFactory.o: TypeDescriptorFactory.h ../runner/common/Platform.h
TypeDescriptorFactory.o: ../runner/common/Type.h
TypeDescriptorFactory.o: ../runner/common/Noncopyable.h TypeDescriptor.h
Compiler.o: Compiler.h ErrorHandler.h ../runner/common/Setup.h
Compiler.o: ../runner/common/Platform.h ../runner/common/Type.h
Compiler.o: ../runner/common/Assert.h ../runner/common/String.h
Compiler.o: ../runner/common/FileFormat.h ../runner/common/Math.h
Compiler.o: ../runner/common/Noncopyable.h ../runner/common/Memory.h
Compiler.o: ../runner/common/OutputStream.h ../runner/common/OutputStream.inl
Compiler.o: ../runner/common/Stopwatch.h CodeGenerator.h CodeBuffer.h
Compiler.o: DataBuffer.h GlobalAddressResolver.h GlobalSemanticAnalyzer.h
Compiler.o: SemanticAnalyzer.h Symbol.h TypeDescriptor.h SymbolFactory.h
Compiler.o: StringPool.h TypeDescriptorFactory.h SyntaxNode.h
Compiler.o: LocalAddressResolver.h LocalSemanticAnalyzer.h Lexer.h
Compiler.o: ParsingDriver.h Path.h
