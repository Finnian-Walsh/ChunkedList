#pragma once

#include <algorithm>
#include <queue>

#include "internal/ChunkedListUtility.hpp"

using namespace ChunkedListUtility;

/**
 * @brief Calls the given sort function on the chunked list
 */
template<typename Compare, SortType Sort = QuickSort, typename T, size_t ChunkSize, bool ShouldCopy>
void ChunkedListUtility::sort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  using namespace SortFunctions;

  switch (Sort) {
    case BubbleSort: return bubbleSort<Compare>(chunkedList);
    case SelectionSort: return selectionSort<Compare>(chunkedList);
    case InsertionSort: return insertionSort<Compare>(chunkedList);
    case MergeSort: return mergeSort<Compare>(chunkedList);
    case QuickSort: return quickSort<Compare, T, ChunkSize, ShouldCopy>(chunkedList.begin(), chunkedList.end());
    case HeapSort: return heapSort<Compare>(chunkedList);
  }
}

template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
void SortFunctions::bubbleSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  if (1 >= chunkedList.size()) return;

  Compare compare;

  bool sorted{};

  do {
    sorted = true;

    for (auto it = chunkedList.begin(); std::next(it) != chunkedList.end(); ++it) {
      if (compare(*std::next(it), *it)) {
        sorted = false;
        std::swap(*it, *std::next(it));
      }
    }
  } while (!sorted);
}

template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
void SortFunctions::selectionSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  if (1 >= chunkedList.size()) return;

  Compare compare;

  auto startingIt = chunkedList.begin();
  const size_t range = chunkedList.size() - 1;

  for (size_t i = 0; i < range; ++i) {
    auto min = startingIt;

    for (auto it = std::next(startingIt); it != chunkedList.end(); ++it) {
      if (compare(*it, *min)) {
        min = it;
      }
    }

    if (min != startingIt) {
      std::swap(*min, *startingIt);
    }

    ++startingIt;
  }
}

template <typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
void SortFunctions::insertionSort(ChunkedList<T, ChunkSize, ShouldCopy>& chunkedList) {
  if (chunkedList.size() <= 1) return;

  using CaptureType = std::conditional_t<ShouldCopy, T, T&&>;

  Compare compare;

  auto begin = chunkedList.begin(), end = chunkedList.end();

  for (auto startingIt = std::next(begin); startingIt != end; ++startingIt) {
    CaptureType value = *startingIt;

    auto it = startingIt;

    while (it != begin) {
      auto prevIt = std::prev(it);

      if (!compare(value, *prevIt)) break; // value < *prevIt

      *it = std::move(*prevIt);
      it = prevIt;
    }

    *it = std::move(value);
  }
}

template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
void SortFunctions::mergeSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  if (1 >= chunkedList.size()) return;

  // TODO: Implement Merge Sort
}

template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
void SortFunctions::quickSort(typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator start,
                              typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator end) {
  if (start == end || std::next(start) == end) return;

  Compare compare;

  using Iterator = typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator;

  Iterator pivot = end;
  --pivot;

  Iterator left = start;
  Iterator right = pivot;

  while (true) {
    while (left != pivot && compare(*left, *pivot)) {
      ++left;
    }

    while (right != start && compare(*pivot, *right)) {
      --right;
    }

    if (left == right || std::next(right) == left) break;

    std::swap(*left, *right);
    ++left;
    --right;
  }

  std::swap(*left, *pivot);

  quickSort<Compare, T, ChunkSize, ShouldCopy>(start, left); // Left partition
  quickSort<Compare, T, ChunkSize, ShouldCopy>(std::next(left), end); // Right partition
}

template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
void SortFunctions::heapSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  if (1 >= chunkedList.size()) return;

  std::priority_queue<T, std::vector<T>, Compare> heap{};

  for (auto it = chunkedList.begin(); it != chunkedList.end(); ++it) {
    heap.push(*it);
  }

  for (auto iterator = chunkedList.end() - 1;; --iterator) {
    *iterator = heap.top();
    heap.pop();

    if (iterator == chunkedList.begin()) break;
  }
}
