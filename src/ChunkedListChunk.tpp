#pragma once

#include "ChunkedList.hpp"
#include "ChunkedListMacros.hpp"

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::Chunk(Chunk *nextChunk, Chunk *prevChunk)
: nextChunk(nextChunk), prevChunk(prevChunk) {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::Chunk(const T *array, size_t size, Chunk *nextChunk,
                                                    Chunk *prevChunk) : nextIndex(size),
                                                                        nextChunk(nextChunk),
                                                                        prevChunk(prevChunk) {
  DEBUG_LOG("range = " << size << " (")

#ifdef CHUNKED_LIST_DEBUGGING
  int index = 0;
  for (; index < size - 1; ++index) {
    DEBUG_LOG(array[index] << ", ")
    if constexpr (ShouldCopy) data[index] = array[index];
    else data[index] = std::move(array[index]);
  }
  
  if constexpr (ShouldCopy) data[index] = array[index];
  else data[index] = std::move(array[index]);
  
  DEBUG_LOG(data[index] << ')')
  DEBUG_LINE(true)
#else
  for (int index = 0; index < size; ++index) {
    if constexpr (ShouldCopy) data[index] = array[index];
    else data[index] = std::move(array[index]);
  }
#endif
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
typename ChunkedList<T, ChunkSize, ShouldCopy>::Chunk &
ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::operator+(const size_t offset) {
  Chunk *chunk{this};
  
  for (size_t i{0}; i < offset; ++i)
    chunk = chunk->nextChunk;
  
  return *chunk;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Chunk &
ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::operator-(const size_t offset) {
  Chunk *chunk{this};
  
  for (size_t i{0}; i < offset; ++i)
    chunk = chunk->nextChunk;
  
  return *chunk;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
T &ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::operator[](size_t index) {
  return data[index];
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const T &ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::operator[](size_t index) const {
  return data[index];
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::operator==(const Chunk &other) const {
  return this == &other;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::operator!=(const Chunk &other) const {
  return this == &other;
}
