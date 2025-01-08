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

  template<template <typename...> typename TemplateT, typename T>
  class IsTemplateOf {
    template<typename>
    class Impl : public std::false_type {
    };

    template<typename... Args>
    class Impl<TemplateT<Args...> > : public std::true_type {
    };

    public:
      static constexpr bool value = Impl<T>::value;
  };

  template<template <typename...> class Template, typename T>
  concept TemplateOf = IsTemplateOf<Template, T>::value;

  template<typename ChunkedListT, typename T>
  concept IsGenericIterator = TemplateOf<ChunkedListT::template GenericIterator, T>;

  template<typename ChunkedListT, typename T>
  concept IsGenericChunkIterator = TemplateOf<ChunkedListT::template GenericChunkIterator, T>;

  template<typename OutputStream, typename T>
  concept CanInsert = requires(OutputStream os, T obj)
  {
    { os << obj } -> std::convertible_to<std::ostream &>;
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
