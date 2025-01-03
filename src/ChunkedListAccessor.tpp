#pragma once

#include "ChunkedListAccessor.hpp"

template<typename T, size_t ChunkSize, bool ShouldCopy>
size_t ChunkedListAccessor<T, ChunkSize, ShouldCopy>::getChunkCount() {
  return this->chunkCount;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedListAccessor<T, ChunkSize, ShouldCopy>::Chunk *ChunkedListAccessor<T, ChunkSize,
  ShouldCopy>::getFront() {
  return this->front;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedListAccessor<T, ChunkSize, ShouldCopy>::Chunk *ChunkedListAccessor<T, ChunkSize,
  ShouldCopy>::getBack() {
  return this->back;
}
