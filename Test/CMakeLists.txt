cmake_minimum_required(VERSION 3.10)

project(ChunkedListTest VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_FILES ChunkedListTest.cpp
        ../src/ChunkedList.tpp
        ../src/ChunkedListIterator.tpp
        ../src/ChunkedListChunk.tpp
        ../src/ChunkedListUtility.tpp
)

include_directories(../include)

add_executable(${PROJECT_NAME} ${SRC_FILES})
