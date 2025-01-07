#pragma once

#include <type_traits>
#include <cstddef>

template<typename T, size_t ChunkSize>
class ChunkedList;

namespace ChunkedListUtility {
  enum SortType {
    BubbleSort,
    SelectionSort,
    InsertionSort,
    MergeSort,
    QuickSort,
    HeapSort,
  };

  template<typename OutputStream, typename T>
  concept CanInsert = requires(OutputStream os, T obj)
  {
    { os << obj } -> std::same_as<OutputStream &>;
  };

  /**
   * @brief Calls the given sort function on the chunked list
   */
  template<typename Compare, SortType Sort, typename T, size_t ChunkSize>
  void sort(ChunkedList<T, ChunkSize> &chunkedList);

  namespace SortFunctions {
    template<typename Compare, typename T, size_t ChunkSize>
    void bubbleSort(ChunkedList<T, ChunkSize> &chunkedList);

    template<typename Compare, typename T, size_t ChunkSize>
    void selectionSort(ChunkedList<T, ChunkSize> &chunkedList);

    template<typename Compare, typename T, size_t ChunkSize>
    void insertionSort(ChunkedList<T, ChunkSize> &chunkedList);

    template<typename Compare, typename T, size_t ChunkSize>
    void mergeSort(ChunkedList<T, ChunkSize> &chunkedList);

    template<typename Compare, typename T, size_t ChunkSize>
    void quickSort(typename ChunkedList<T, ChunkSize>::Iterator start,
                   typename ChunkedList<T, ChunkSize>::Iterator end);

    template<typename Compare, typename T, size_t ChunkSize>
    void heapSort(ChunkedList<T, ChunkSize> &chunkedList);
  }
}

#include "../src/ChunkedListUtility.tpp"
