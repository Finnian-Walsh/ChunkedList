cmake_minimum_required(VERSION 3.28)
project(ChunkedList)

set(CMAKE_CXX_STANDARD 20)

add_library(ChunkedList STATIC src/ChunkedList.hpp
        src/ChunkedList.cpp)
