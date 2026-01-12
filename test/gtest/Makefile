CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -I../../compiler -I../../runner
LDFLAGS ?=
GTEST_CFLAGS ?= $(shell pkg-config --cflags gtest 2>/dev/null)
GTEST_LIBS ?= $(shell pkg-config --libs gtest 2>/dev/null)

TARGET = mana_gtests

SOURCES = \
	../../compiler/Path.cpp \
	../../compiler/StringPool.cpp \
	test_path.cpp \
	test_string_pool.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(GTEST_CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS) $(GTEST_LIBS) -lgtest -pthread

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(GTEST_CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
