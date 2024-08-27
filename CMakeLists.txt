cmake_minimum_required(VERSION 3.28)
project(ChunkedList)

set(CMAKE_CXX_STANDARD 20)

set(SRC src/ChunkedList.cpp include/ChunkedList.hpp)

add_library(ChunkedList STATIC ${SRC})

set_target_properties(ChunkedList PROPERTIES LINKER_LANGUAGE CXX)
