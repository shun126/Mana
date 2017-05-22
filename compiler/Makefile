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
	SymbolTable.cpp \
	SyntaxNode.cpp \
	SyntaxNodeFactory.cpp \
	SystemHolder.cpp \
	TypeDescriptor.cpp \
	TypeDescriptorFactory.cpp

#DatalinkGenerator.cpp \
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

test:
	./$(TARGET) --debug ../sample/sample.mn
	cd ../test/ && python3 test.py

version:
	python3 Version.py

# DO NOT DELETE
