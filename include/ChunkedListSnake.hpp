#pragma once

#include <ostream>

#include "ChunkedList.hpp"

template<typename T, size_t ChunkSize = 32>
class chunked_list final : ChunkedList<T, ChunkSize> {
  using derived_chunked_list = ChunkedList<T, ChunkSize>;

  public:
    using iterator = typename derived_chunked_list::Iterator;

    using const_iterator = typename derived_chunked_list::ConstIterator;

    using chunk_iterator = typename derived_chunked_list::ConstIterator;

    using derived_chunked_list::ChunkedList;

    using derived_chunked_list::operator[];

    using derived_chunked_list::begin;

    chunk_iterator begin_chunk();

    using derived_chunked_list::end;

    chunk_iterator end_chunk();

    void push_back(T value);

    void pop_back();

    void pop_back_chunk();

    using derived_chunked_list::sort;

    using derived_chunked_list::size;

    size_t length() const;

    using derived_chunked_list::empty;

    bool operator==(const chunked_list &other) const;

    bool operator!=(const chunked_list &other) const;

    using derived_chunked_list::concat;

    template<typename, size_t, bool>
    friend std::ostream &operator<<(std::ostream &os, chunked_list &chunkedList);
};

#include "../src/ChunkedListSnake.tpp"
