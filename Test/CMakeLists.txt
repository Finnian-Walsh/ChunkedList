cmake_minimum_required(VERSION 3.10)

project(ChunkedListTests VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_FILES)

include_directories(../include)

add_executable(ChunkedListTest
        ${SRC_FILES}
        ChunkedListTest.cpp
)

add_executable(StandardChunkedListTest
        ${SRC_FILES}
        StandardChunkedListTest.cpp
)

add_executable(CopyTest
        ${SRC_FILES}
        CopyTests.cpp
)
