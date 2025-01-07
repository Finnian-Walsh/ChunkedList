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

  template<typename, typename, typename = void>
  class has_insertion_operator : public std::false_type {
  };

  template<typename Class, typename Parameter>
  class has_insertion_operator<Class, Parameter, std::void_t<decltype(
    std::declval<Class &>() << std::declval<Parameter>())> > : public std::true_type {
  };

  template<typename Class, typename Parameter>
  constexpr bool has_insertion_operator_v = has_insertion_operator<Class, Parameter>::value;

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
