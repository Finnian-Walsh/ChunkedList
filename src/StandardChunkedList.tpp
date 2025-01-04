#pragma once

#include "StandardChunkedList.hpp"

template<typename T, size_t ChunkSize>
typename chunked_list<T, ChunkSize>::chunk_iterator chunked_list<T, ChunkSize>::begin_chunk() {
  return derived_chunked_list::beginChunk();
}

template<typename T, size_t ChunkSize>
typename chunked_list<T, ChunkSize>::chunk_iterator chunked_list<T, ChunkSize>::end_chunk() {
  return derived_chunked_list::endChunk();
}

template<typename T, size_t ChunkSize>
void chunked_list<T, ChunkSize>::push_back(typename derived_chunked_list::ValueType value) {
  derived_chunked_list::push(value);
}

template<typename T, size_t ChunkSize>
void chunked_list<T, ChunkSize>::pop_back() {
  derived_chunked_list::pop();
}

template<typename T, size_t ChunkSize>
void chunked_list<T, ChunkSize>::pop_back_chunk() {
  derived_chunked_list::popChunk();
}

template<typename T, size_t ChunkSize>
size_t chunked_list<T, ChunkSize>::length() const {
  return derived_chunked_list::size();
}

template<typename T, size_t ChunkSize>
std::ostream &operator<<(std::ostream &os, chunked_list<T, ChunkSize> &chunkedList) {
  return operator<<(os, static_cast<ChunkedList<T, ChunkSize>>(chunkedList));
}
