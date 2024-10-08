#ifndef CHUNKED_LIST_UTILITY_HPP
#define CHUNKED_LIST_UTILITY_HPP

#include "ChunkedList.hpp"

namespace ChunkedListUtility {
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
  
  template<typename T, size_t ChunkSize, bool ShouldCopy>
  void ascendingSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList);
  
  template<typename T, size_t ChunkSize, bool ShouldCopy>
  void descendingSort(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList);
}

#include "../src/ChunkedListUtility.tpp"

void test_function() {
  ChunkedList<int, 32, false> list;
  
  list.sort();
}

#endif