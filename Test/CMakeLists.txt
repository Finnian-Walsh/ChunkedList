cmake_minimum_required(VERSION 3.10)

project(ChunkedListTest VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} ChunkedListTest.cpp
        ../src/ChunkedList.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ../include)