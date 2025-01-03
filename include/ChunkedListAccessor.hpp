#pragma once

#include "ChunkedList.hpp"

template<typename T, size_t ChunkSize, bool ShouldCopy>
class ChunkedListAccessor final : public ChunkedList<T, ChunkSize, ShouldCopy> {
  using DerivedChunkedList = ChunkedList<T, ChunkSize, ShouldCopy>;

  public:
    using Chunk = typename DerivedChunkedList::Chunk;

    size_t getChunkCount();

    Chunk *getFront();

    Chunk *getBack();
};

#include "../src/ChunkedListAccessor.tpp"