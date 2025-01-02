#pragma once

#include <type_traits>
#include <cstddef>

template<typename T, size_t ChunkSize, bool ShouldCopy>
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
  class has_insertion_operator<Class, Parameter, std::void_t<decltype(std::declval<Class &>()
  << std::declval<Parameter>())>>
  : public std::true_type {
  };
  
  template<typename Class, typename Parameter>
  constexpr bool has_insertion_operator_v = has_insertion_operator<Class, Parameter>::value;

  template<typename Compare, SortType Sort, typename T, size_t ChunkSize, bool ShouldCopy>
  void sort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList);

  namespace SortFunctions {
    template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
    void bubbleSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList);

    template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
    void selectionSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList);

    template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
    void insertionSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList);

    template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
    void mergeSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList);

    template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
    void quickSort(typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator start, typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator end);

    template<typename Compare, typename T, size_t ChunkSize, bool ShouldCopy>
    void heapSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList);
  }
}

#include "../src/ChunkedListUtility.tpp"

