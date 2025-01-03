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

    for (auto it = chunkedList.begin(); ++it != chunkedList.end(); ++it) {
      if (compare(*++it, *it)) {
        sorted = false;
        std::swap(*it, *++it);
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

  for (size_t i{}; i < range; ++i) {
    auto min = startingIt;

    for (auto it = ++startingIt; it != chunkedList.end(); ++it) {
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

template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
void SortFunctions::insertionSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  if (1 >= chunkedList.size()) return;

  Compare compare;

  auto end = chunkedList.end();

  for (auto startingIt = ++chunkedList.begin(); startingIt != end; ++startingIt) {
    T value = ShouldCopy ? *startingIt : *++startingIt;

    auto it = --startingIt;

    while (it != chunkedList.begin()) {
      if (compare(value, *it)) {
        if constexpr (ShouldCopy) {
          *++it = *it;
        } else {
          *++it = std::move(*it);
        }
      }
    }
  }

  // TODO: Finish Insertion Sort
}

template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
void SortFunctions::mergeSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  if (1 >= chunkedList.size()) return;

  // TODO: Implement Merge Sort
}


template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
void SortFunctions::quickSort(typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator start,
                              typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator end) {
  if (start == end || start == --end) return;  // Base case: single element or empty range

  Compare compare;

  using Iterator = typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator;

  Iterator itemFromLeft = start, itemFromRight = end;

  while (true) {
    while (compare(*itemFromLeft, *end)) {
      ++itemFromLeft;
      if (itemFromLeft == end) break;
    }

    while (compare(*end, *itemFromRight)) {
      --itemFromRight;
      if (itemFromRight == start) break;
    }

    if (itemFromLeft == itemFromRight || itemFromLeft == end || itemFromRight == start) {
      break;
    }

    std::swap(*itemFromLeft, *itemFromRight);
  }

  std::swap(*itemFromLeft, *end);

  quickSort<Compare, T, ChunkSize, ShouldCopy>(start, itemFromLeft);
  quickSort<Compare, T, ChunkSize, ShouldCopy>(++itemFromLeft, end);  // Start right partition after pivot
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
