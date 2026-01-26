# mana (test)
#
# Build ProgramImageTest
#

TARGET = ProgramImageTest

CC = clang++
CFLAGS = --std=c++17 -O2 -Wall

SOURCES = \
	ProgramImageTest.cpp

OBJECTS = $(SOURCES:%.cpp=%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJECTS)
