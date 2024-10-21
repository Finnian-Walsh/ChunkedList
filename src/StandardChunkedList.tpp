#pragma once

#include "StandardChunkedList.hpp"

template<typename T, size_t ChunkSize, bool ShouldCopy>
StandardChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator StandardChunkedList<T, ChunkSize, ShouldCopy>::begin_chunk() {
  return DerivedChunkedList::beginChunk();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
StandardChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator StandardChunkedList<T, ChunkSize, ShouldCopy>::end_chunk() {
  return DerivedChunkedList::endChunk();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
void StandardChunkedList<T, ChunkSize, ShouldCopy>::push_back(DerivedChunkedList::ValueType value) {
  DerivedChunkedList::push(value);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
void StandardChunkedList<T, ChunkSize, ShouldCopy>::pop_back() {
  DerivedChunkedList::pop();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
void StandardChunkedList<T, ChunkSize, ShouldCopy>::pop_back_chunk() {
  DerivedChunkedList::popChunk();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
size_t StandardChunkedList<T, ChunkSize, ShouldCopy>::length() const {
  return DerivedChunkedList::size();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
StandardChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator
begin(StandardChunkedList<T, ChunkSize, ShouldCopy> &standardChunkedList) {
  return standardChunkedList.begin();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
StandardChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator
end(StandardChunkedList<T, ChunkSize, ShouldCopy> &standardChunkedList) {
  return standardChunkedList.begin();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
std::ostream &operator<<(std::ostream &os, StandardChunkedList<T, ChunkSize, ShouldCopy> &standardChunkedList) {
  return operator<<(os, static_cast<ChunkedList<T, ChunkSize, ShouldCopy>>(standardChunkedList));
}