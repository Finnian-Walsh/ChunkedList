#pragma once

#include <algorithm>
#include <queue>
#include <StandardChunkedList.hpp>

#include "ChunkedListUtility.hpp"

using namespace ChunkedListUtility;

template<typename Compare, SortType Sort = QuickSort, typename T, size_t ChunkSize, bool ShouldCopy>
void ChunkedListUtility::sort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  switch (Sort) {
    case BubbleSort: return SortFunctions::bubbleSort<Compare>(chunkedList);
    case SelectionSort: return SortFunctions::selectionSort<Compare>(chunkedList);
    case InsertionSort: return SortFunctions::insertionSort<Compare>(chunkedList);
    case MergeSort: return SortFunctions::mergeSort<Compare>(chunkedList);
    case QuickSort: return SortFunctions::quickSort<Compare>(chunkedList);
    case HeapSort: return SortFunctions::heapSort<Compare>(chunkedList);
  }
}


template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
void SortFunctions::bubbleSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  if (chunkedList.size() <= 1) return;

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
  if (chunkedList.size() <= 1) return;

  Compare compare;

  auto startingIt = chunkedList.begin();
  const size_t range = chunkedList.size() - 1;

  for (size_t i{}; i < range; ++i) {
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

template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
void SortFunctions::insertionSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  if (chunkedList.size() <= 1) return;

  Compare compare;

  auto end = chunkedList.end();

  for (auto startingIt = std::next(chunkedList.begin()); startingIt != end; ++startingIt) {
    T value = ShouldCopy ? *startingIt : std::move(*startingIt);

    auto it = std::prev(startingIt);

    while (it != chunkedList.begin()) {
      if (compare(value, *it)) {
        if constexpr (ShouldCopy) {
          *std::next(it) = *it;
        } else {
          *std::next(it) = std::move(*it);
        }
      }
    }

  }

  // TODO: Finish Insertion Sort
}

template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
void SortFunctions::mergeSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  if (chunkedList.size() <= 1) return;

  // TODO: Implement Merge Sort
}



template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
void SortFunctions::quickSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  if (chunkedList.size() <= 1) return;

  Compare compare;

  auto pivot = chunkedList.end();



  // TODO: Finish Quick Sort
}

template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
void SortFunctions::quickSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList, size_t start, size_t end) {

  // TODO: Implement quick sort

}


template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
void SortFunctions::heapSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  if (chunkedList.size() <= 1) return;

  std::priority_queue<T, std::vector<T>, Compare> heap{};

  for (auto it = chunkedList.begin(); it != chunkedList.end(); ++it) {
    heap.push(*it);
  }

  for (auto iterator = chunkedList.begin(); !heap.empty() && iterator != chunkedList.end(); ++iterator) {
    *iterator = heap.top();
    heap.pop();
  }
}
