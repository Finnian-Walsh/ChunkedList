cmake_minimum_required(VERSION 3.28)
project(ChunkedList)

set(CMAKE_CXX_STANDARD 20)

include_directories(include
)

set(SRC_FILES
        include/ChunkedList.hpp
        src/ChunkedList.cpp)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})