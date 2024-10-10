#include "ChunkedListUtility.hpp"

template<typename T, size_t ChunkSize, bool ShouldCopy>
void ChunkedListUtility::ascendingSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  if (chunkedList.empty()) return;
  
  std::priority_queue<T, std::vector<T>, std::less<T>> heap{};
  
  for (auto it = chunkedList.begin(); it != chunkedList.end(); ++it) {
    heap.push(*it);
  }
  
  for (auto iterator = chunkedList.begin(); !heap.empty() && iterator != chunkedList.end(); ++iterator) {
    *iterator = heap.top();
    heap.pop();
  }
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
void ChunkedListUtility::descendingSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  if (chunkedList.empty()) return;
  
  std::priority_queue<T, std::vector<T>, std::greater<T>> heap{};
  
  for (auto it = chunkedList.begin(); it != chunkedList.end(); ++it) {
    heap.push(*it);
  }
  
  for (auto iterator = chunkedList.begin(); !heap.empty() && iterator != chunkedList.end(); ++iterator) {
    *iterator = heap.top();
    heap.pop();
  }
}
