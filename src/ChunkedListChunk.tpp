#pragma once

#include "ChunkedList.hpp"
#include "DebuggingMacros.hpp"

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::Chunk(const T *array, int size, Chunk *nextChunk,
                                                    Chunk *prevChunk) : nextIndex(size),
                                                                        nextChunk(nextChunk),
                                                                        prevChunk(prevChunk) {
  DEBUG_LOG("range: " << size << std::endl)
  
  for (int index = 0; index < size; ++index) {
    DEBUG_LOG(array[index] << ' ')
    if constexpr (ShouldCopy)
      data[index] = array[index];
    else
      data[index] = std::move(array[index]);
  }
  
  DEBUG_LINE(true)
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::Chunk(const std::initializer_list<T> &initializerList) : nextIndex(
initializerList.size()) {
  for (int index = 0; index < initializerList.size(); ++index) {
    DEBUG_LOG(initializerList.begin()[index] << ' ')
    if constexpr (ShouldCopy)
      data[index] = initializerList.begin()[index];
    else
      data[index] = std::move(initializerList.begin()[index]);
  }
  
  DEBUG_LINE(true)
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::Chunk() = default;

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::~Chunk() = default;
