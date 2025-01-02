#pragma once

#include "StandardChunkedList.hpp"

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename chunked_list<T, ChunkSize, ShouldCopy>::chunk_iterator chunked_list<T, ChunkSize, ShouldCopy>::begin_chunk() {
  return derived_chunked_list::beginChunk();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename chunked_list<T, ChunkSize, ShouldCopy>::chunk_iterator chunked_list<T, ChunkSize, ShouldCopy>::end_chunk() {
  return derived_chunked_list::endChunk();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
void chunked_list<T, ChunkSize, ShouldCopy>::push_back(typename derived_chunked_list::ValueType value) {
  derived_chunked_list::push(value);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
void chunked_list<T, ChunkSize, ShouldCopy>::pop_back() {
  derived_chunked_list::pop();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
void chunked_list<T, ChunkSize, ShouldCopy>::pop_back_chunk() {
  derived_chunked_list::popChunk();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
size_t chunked_list<T, ChunkSize, ShouldCopy>::length() const {
  return derived_chunked_list::size();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
std::ostream &operator<<(std::ostream &os, chunked_list<T, ChunkSize, ShouldCopy> &chunkedList) {
  return operator<<(os, static_cast<ChunkedList<T, ChunkSize, ShouldCopy>>(chunkedList));
}