#pragma once

#include "ChunkedList.hpp"

template<typename T, size_t ChunkSize>
class ChunkedListAccessor final : public ChunkedList<T, ChunkSize> {
  using DerivedChunkedList = ChunkedList<T, ChunkSize>;

  public:
    using Chunk = typename DerivedChunkedList::Chunk;

    size_t getChunkCount();

    Chunk *getFront();

    Chunk *getBack();
};

#include "../src/ChunkedListAccessor.tpp"