#include "ChunkedListUtility.hpp"

template<typename T, size_t ChunkSize, bool ShouldCopy>
void ChunkedListUtility::ascendingSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  std::priority_queue<T, std::vector<T>, std::less<T>> heap{};
  
  for (typename ChunkedList<T, ChunkSize, ShouldCopy>::ValueType item: chunkedList) {
    heap.push(item);
  }
  
  for (auto iterator = chunkedList.begin(); !heap.empty(); ++iterator) {
    *iterator = heap.top();
    heap.pop();
  }
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
void ChunkedListUtility::descendingSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  std::priority_queue<T, std::vector<T>, std::greater<T>> heap{};
  
  for (typename ChunkedList<T, ChunkSize, ShouldCopy>::ValueType item: chunkedList) {
    heap.push(item);
  }
  
  for (auto iterator = chunkedList.begin(); !heap.empty(); ++iterator) {
    *iterator = heap.top();
    heap.pop();
  }
}
