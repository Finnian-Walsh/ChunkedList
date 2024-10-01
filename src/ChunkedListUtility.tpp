#include "ChunkedListUtility.hpp"

template<bool AscendingOrder, typename T, size_t ChunkSize, bool ShouldCopy>
void ChunkedListUtility::sort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  using CompareClass = CompareClass<T, AscendingOrder>;
  using PriorityQueue = std::priority_queue<T, std::vector<T>, CompareClass>;
  
  PriorityQueue heap{};
  
  for (typename ChunkedList<T, ChunkSize, ShouldCopy>::ValueType item: chunkedList) {
    heap.push(item);
  }
  
  for (auto iterator = chunkedList.begin(); !heap.empty(); ++iterator) {
    *iterator = heap.top();
    heap.pop();
  }
}
