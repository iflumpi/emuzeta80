
$PROJECT_NAME := EmuZeta80_Tests

CC := g++

TEST_SOURCE := emuzeta80_tests.cpp
TEST_BINARY := emuzeta80_tests.o

#INCLUDE_GMOCK_DIR := ../thirdparty/gmock/include
#LIB_GMOCK_DIR := ../thirdparty/gmock/lib
INCLUDE_GTEST_DIR := /usr/src/googletest/googletest/include
INCLUDE_GMOCK_DIR := /usr/src/googletest/googlemock/include 
LIB_GTEST_DIR := /usr/src/googletest/googletest
LIB_GMOCK_DIR := /usr/src/googletest/googlemock
LIB_EMUZETA80_DIR := ../build

all:
	$(CC) $(TEST_SOURCE) -o $(TEST_BINARY) -std=c++11 -I$(INCLUDE_GTEST_DIR) -I$(INCLUDE_GMOCK_DIR) -I../src/z80 -L$(LIB_GTEST_DIR) -L$(LIB_GMOCK_DIR) -L$(LIB_EMUZETA80_DIR) -g -fprofile-arcs -ftest-coverage -lgtest -lgmock -lgcov -lpthread -lz80
	export LD_LIBRARY_PATH=$(LIB_EMUZETA80_DIR):$(LIB_GMOCK_DIR); ./$(TEST_BINARY)
