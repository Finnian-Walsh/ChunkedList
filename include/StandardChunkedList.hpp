#pragma once

#include <ostream>

#include "ChunkedList.hpp"

template<typename T, size_t ChunkSize = 32, bool ShouldCopy = true>
class chunked_list final : ChunkedList<T, ChunkSize, ShouldCopy> {
  using derived_chunked_list = ChunkedList<T, ChunkSize, ShouldCopy>;

  public:
    using value_type = typename derived_chunked_list::ValueType;

    using iterator = typename derived_chunked_list::Iterator;

    using const_iterator = typename derived_chunked_list::ConstIterator;

    using chunk_iterator = typename derived_chunked_list::ConstIterator;

    using derived_chunked_list::ChunkedList;

    using derived_chunked_list::operator[];

    using derived_chunked_list::begin;

    chunk_iterator begin_chunk();

    using derived_chunked_list::end;

    chunk_iterator end_chunk();

    void push_back(value_type value);

    void pop_back();

    void pop_back_chunk();

    using derived_chunked_list::sort;

    using derived_chunked_list::size;

    size_t length() const;

    using derived_chunked_list::empty;

    using derived_chunked_list::operator==;

    using derived_chunked_list::operator!=;

    using derived_chunked_list::concat;

    template<typename, size_t, bool>
    friend std::ostream &operator<<(std::ostream &os, chunked_list &chunkedList);
};

#include "../src/StandardChunkedList.tpp"
