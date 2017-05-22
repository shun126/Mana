# mana (compiler)
# Copyright (c) 2003 Shun Moriya
#

TARGET	= mana

#CC	= g++
CC	= clang++
CFLAGS	= --std=c++14 -c -O3 -Wall -D NDEBUG

LK	= $(CC)
LFLAGS	= --std=c++14 -O3 

COVERAGE=

YACC = bison
YFLAGS	= -d

LEX	= flex
LXFLAGS	= -8 -F

CP = cp -f
RM = rm -f

SOURCES	= CodeGenerator.cpp DatalinkGenerator.cpp ErrorHandler.cpp GlobalSemanticAnalyzer.cpp LoaclAddressResolver.cpp LocalSemanticAnalyzer.cpp Main.cpp Path.cpp SemanticAnalyzer.cpp SymbolEntry.cpp SymbolFactory.cpp SymbolTable.cpp SyntaxNode.cpp TypeDescriptor.cpp TypeDescriptorFactory.cpp
#	Register.cpp ¥
PARSER	= Parser.y
LEXER	= Lexer.l
OBJECTS	= $(SOURCES:%.cpp=%.o) $(PARSER:%.y=%.o) $(LEXER:%.l=%.o)
SECOUND	= Parser.cpp Parser.hpp Lexer.cpp

.SUFFIXES: .y .l .cpp .o
.SECOUNDLY: $(SECOUND)

$(TARGET): $(SOURCES) $(PARSER) $(LEXER) $(OBJECTS)
	$(LK) $(LFLAGS) $(COVERAGE) -o $(TARGET) $(OBJECTS) -ldl -lm

.l.cpp:
	$(LEX) $(LXFLAGS) -o$@ $<

.y.cpp:
	$(YACC) $(YFLAGS) -o $@ $<

.cpp.o:
	$(CC) $(CFLAGS) $(COVERAGE) -o $@ $<

all:
	$(TARGET)

clean:
	$(RM) $(TARGET) $(OBJECTS)

depend:
	makedepend -Y -- $(CFLAGS) -- $(SOURCES)

test:
	./$(TARGET) --debug ../sample/sample.mn
	ruby -C ../test/mana test.rb

version:
	ruby Version.rb

# DO NOT DELETE
