CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic -Werror
TARGET = shannon

SOURCES = $(wildcard *.cpp)
TEMP = $(wildcard *.bin)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS) $(TEMP)

.PHONY: all clean