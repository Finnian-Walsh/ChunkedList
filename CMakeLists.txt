cmake_minimum_required(VERSION 3.28)
project(ChunkedList)

set(CMAKE_CXX_STANDARD 20)

set(SRC src/ChunkedList.hpp src/ChunkedList.hpp)

add_library(ChunkedList STATIC ${SRC})
