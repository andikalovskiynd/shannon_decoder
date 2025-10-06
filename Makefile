UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    GTEST_INC_DIR ?= /opt/homebrew/opt/googletest/include
    GTEST_LIB_DIR ?= /opt/homebrew/opt/googletest/lib
else
    GTEST_INC_DIR ?= /usr/local/include
    GTEST_LIB_DIR ?= /usr/local/lib
endif

CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic
TEST_CXXFLAGS = $(CXXFLAGS) -I$(GTEST_INC_DIR) -DBUILDING_TESTS

ifdef CI
    LDFLAGS = -L$(GTEST_LIB_DIR) -lgtest_main -lgtest -pthread
else
    LDFLAGS = -L$(GTEST_LIB_DIR) -lgtest_main -lgtest -pthread -fsanitize=address
endif

MAIN_TARGET = shannon
TEST_TARGET = test_shannon

SOURCES = $(wildcard *.cpp)
TEST_SOURCES = $(filter test%.cpp, $(SOURCES))
MAIN_SOURCES = $(filter-out $(TEST_SOURCES), $(SOURCES))

all: $(MAIN_TARGET)

$(MAIN_TARGET): $(MAIN_SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_TARGET): $(TEST_SOURCES) $(filter-out main.cpp,$(MAIN_SOURCES))
	$(CXX) $(TEST_CXXFLAGS) $^ $(LDFLAGS) -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(MAIN_TARGET) $(TEST_TARGET) *.o *.bin *.csv

.PHONY: all test clean