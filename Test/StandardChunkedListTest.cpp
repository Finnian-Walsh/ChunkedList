#define CHUNKED_LIST_DEBUGGING
#include "StandardChunkedList.hpp"

#include "core/TestUtility.hpp"

using namespace TestUtility;

int main() {
  BEGIN

  using namespace Tests;

    callFunction("Front and back", [] {
    testData.setSource("Test 1");
    FrontAndBack<chunked_list, 1>();

    testData.setSource("Test 2");
    FrontAndBack<chunked_list, 2>();

    testData.setSource("Test 3");
    FrontAndBack<chunked_list, 3>();

    testData.setSource("Test 4");
    FrontAndBack<chunked_list, 4>();

    testData.setSource("Test 5");
    FrontAndBack<chunked_list, 5>();
  });

  callFunction("Insertion", [] {
    testData.setSource("Test 1");
    Insertion<chunked_list, 1>();

    testData.setSource("Test 2");
    Insertion<chunked_list, 2>();

    testData.setSource("Test 3");
    Insertion<chunked_list, 3>();

    testData.setSource("Test 4");
    Insertion<chunked_list, 4>();

    testData.setSource("Test 5");
    Insertion<chunked_list, 5>();
  });

  callFunction("Sorting", [] {
    testData.setSource("Bubble Sorting");
    Sorting<BubbleSort, chunked_list>();

    testData.setSource("Selection Sorting");
    Sorting<SelectionSort, chunked_list>();

    testData.setSource("Insertion Sorting");
    Sorting<InsertionSort, chunked_list>();

    testData.setSource("Merge Sorting");
    Sorting<MergeSort, chunked_list>();

    testData.setSource("Quick Sorting");
    Sorting<QuickSort, chunked_list>();

    testData.setSource("Heap Sorting");
    Sorting<HeapSort, chunked_list>();
  });

  callFunction("Iterators", [] {
    testData.setSource("Test 1");
    Iterators<chunked_list, 1>();

    testData.setSource("Test 2");
    Iterators<chunked_list, 2>();

    testData.setSource("Test 3");
    Iterators<chunked_list, 3>();

    testData.setSource("Test 4");
    Iterators<chunked_list, 4>();

    testData.setSource("Test 5");
    Iterators<chunked_list, 5>();
  });

  callFunction("Pushing and popping", [] {
    testData.setSource("Test 1");
    PushingAndPopping<chunked_list, 1>();

    testData.setSource("Test 2");
    PushingAndPopping<chunked_list, 2>();

    testData.setSource("Test 3");
    PushingAndPopping<chunked_list, 3>();

    testData.setSource("Test 4");
    PushingAndPopping<chunked_list, 4>();

    testData.setSource("Test 5");
    PushingAndPopping<chunked_list, 5>();
  });

  callFunction("Equality and inequality", [] {
    testData.setSource("Test 1");
    EqualityAndInequality<chunked_list, 1>();

    testData.setSource("Test 2");
    EqualityAndInequality<chunked_list, 2>();

    testData.setSource("Test 3");
    EqualityAndInequality<chunked_list, 3>();

    testData.setSource("Test 4");
    EqualityAndInequality<chunked_list, 4>();

    testData.setSource("Test 5");
    EqualityAndInequality<chunked_list, 5>();
  });

  callFunction("Concatenation and indexing", [] {
    testData.setSource("Test 1");
    ConcatenationAndIndexing<chunked_list, 1>();

    testData.setSource("Test 2");
    ConcatenationAndIndexing<chunked_list, 2>();

    testData.setSource("Test 3");
    ConcatenationAndIndexing<chunked_list, 3>();

    testData.setSource("Test 4");
    ConcatenationAndIndexing<chunked_list, 4>();

    testData.setSource("Test 5");
    ConcatenationAndIndexing<chunked_list, 5>();
  });
  
  SUCCESS
}