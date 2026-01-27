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

depend:
	makedepend -Y -- $(CFLAGS) -- $(SOURCES)
# DO NOT DELETE

ProgramImageTest.o: ../runner/ProgramImage.h ../runner/common/Setup.h
ProgramImageTest.o: ../runner/common/Platform.h ../runner/common/Type.h
ProgramImageTest.o: ../runner/common/Assert.h ../runner/common/String.h
ProgramImageTest.o: ../runner/common/FileFormat.h ../runner/common/Math.h
ProgramImageTest.o: ../runner/common/Noncopyable.h ../runner/common/Memory.h
ProgramImageTest.o: ../runner/common/OutputStream.h
ProgramImageTest.o: ../runner/common/OutputStream.inl
ProgramImageTest.o: ../runner/common/Stopwatch.h ../runner/ProgramImage.inl
ProgramImageTest.o: ../runner/ProgramImage.h
