cmake_minimum_required(VERSION 3.0)
 
project(EMUZETA80)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES_Z80
	src/emuzeta80/CPU.cpp
	src/emuzeta80/RAM.cpp
	src/emuzeta80/ALU.cpp
    src/emuzeta80/IOPorts.cpp)

include_directories(src/emuzeta80)

add_library(emuzeta80 SHARED ${SOURCES_Z80})
target_compile_options(emuzeta80 PUBLIC -std=c++11 -O3)
