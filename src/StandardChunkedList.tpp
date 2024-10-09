#include "StandardChunkedList.hpp"

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedListUtility::ChunkedListImplementation::ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator
begin(StandardChunkedList<T, ChunkSize, ShouldCopy> &standardChunkedList) {
  return standardChunkedList.begin();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedListUtility::ChunkedListImplementation::ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator
end(StandardChunkedList<T, ChunkSize, ShouldCopy> &standardChunkedList) {
  return standardChunkedList.begin();
}