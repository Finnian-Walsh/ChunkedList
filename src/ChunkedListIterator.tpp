#pragma once

#include "ChunkedList.hpp"
#include "ChunkedListMacros.hpp"

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::Iterator(Chunk *chunk, size_t index) : chunkIterator(chunk),
                                                                                        index(index) {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::Iterator(Chunk &chunk) : chunkIterator(&chunk) {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::Iterator(Chunk &chunk, size_t index) : chunkIterator(&chunk),
                                                                                        index(index) {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::Iterator(ChunkIterator chunkIterator) : chunkIterator(
*chunkIterator) {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::Iterator(ChunkIterator chunkIterator, size_t index) : chunkIterator(
*chunkIterator), index(index) {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::~Iterator() = default;

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator++() {
  if (index == ChunkSize) {
    index = 0;
    ++chunkIterator;
  } else {
    ++index;
  }
  
  return *this;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator++(int) {
  Iterator original = *this;
  
  if (index == ChunkSize) {
    index = 0;
    ++chunkIterator;
  } else {
    ++index;
  }
  return original;
}


template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator--() {
  if (index == 0) {
    index = ChunkSize - 1;
    --chunkIterator;
  } else {
    --index;
  }
  return *this;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator--(int) {
  Iterator original = *this;
  if (index == 0) {
    index = ChunkSize - 1;
    --chunkIterator;
  } else {
    --index;
  }
  return original;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator+(size_t offset) {
  size_t chunkOffset = offset / ChunkSize;
  offset %= ChunkSize;
  
  size_t iteratorIndex = offset + index;
  
  if (iteratorIndex >= ChunkSize) {
    iteratorIndex %= ChunkSize;
    ++chunkOffset;
  }
  
  return Iterator{&(*chunkIterator + chunkOffset), iteratorIndex};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator-(size_t offset) {
  size_t chunkOffset = offset / ChunkSize;
  offset %= ChunkSize;
  
  size_t iteratorIndex = offset - index;
  
  if (iteratorIndex >= ChunkSize) {
    iteratorIndex %= ChunkSize;
    ++chunkOffset;
  }
  
  return Iterator{&(*chunkIterator - chunkOffset), iteratorIndex};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator+=(size_t offset) {
  return *this = operator+(offset);
}