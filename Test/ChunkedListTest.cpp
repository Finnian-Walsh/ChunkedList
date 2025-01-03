// #define CHUNKED_LIST_DEBUGGING

#include "ChunkedList.hpp"
#include "ChunkedListAccessor.hpp"

#define LOG_LEVEL 0

#include "core/TestUtility.hpp"

using namespace TestUtility;

int main() {
  BEGIN

  using namespace Tests;

  callFunction("Front and back", [] {
    testData.setSource("Test 1");
    FrontAndBack<ChunkedList, 1>();

    testData.setSource("Test 2");
    FrontAndBack<ChunkedList, 2>();

    testData.setSource("Test 3");
    FrontAndBack<ChunkedList, 3>();

    testData.setSource("Test 4");
    FrontAndBack<ChunkedList, 4>();

    testData.setSource("Test 5");
    FrontAndBack<ChunkedList, 5>();
  });

  callFunction("Insertion", [] {
    testData.setSource("Test 1");
    Insertion<ChunkedList, 1>();

    testData.setSource("Test 2");
    Insertion<ChunkedList, 2>();

    testData.setSource("Test 3");
    Insertion<ChunkedList, 3>();

    testData.setSource("Test 4");
    Insertion<ChunkedList, 4>();

    testData.setSource("Test 5");
    Insertion<ChunkedList, 5>();
  });

  callFunction("Sorting", [] {
    testData.setSource("Bubble Sorting");
    Sorting<BubbleSort, ChunkedList>();

    testData.setSource("Selection Sorting");
    Sorting<SelectionSort, ChunkedList>();

    testData.setSource("Insertion Sorting");
    Sorting<InsertionSort, ChunkedList>();

      // testData.setSource("Merge Sorting");
      // Sorting<MergeSort, ChunkedList>();

    testData.setSource("Quick Sorting");
    Sorting<QuickSort, ChunkedList>();

    testData.setSource("Heap Sorting");
    Sorting<HeapSort, ChunkedList>();
  });

  callFunction("Iterators", [] {
    testData.setSource("Test 1");
    Iterators<ChunkedList, 1>();

    testData.setSource("Test 2");
    Iterators<ChunkedList, 2>();

    testData.setSource("Test 3");
    Iterators<ChunkedList, 3>();

    testData.setSource("Test 4");
    Iterators<ChunkedList, 4>();

    testData.setSource("Test 5");
    Iterators<ChunkedList, 5>();
  });

  callFunction("Pushing and popping", [] {
    testData.setSource("Test 1");
    PushingAndPopping<ChunkedList, 1>();

    testData.setSource("Test 2");
    PushingAndPopping<ChunkedList, 2>();

    testData.setSource("Test 3");
    PushingAndPopping<ChunkedList, 3>();

    testData.setSource("Test 4");
    PushingAndPopping<ChunkedList, 4>();

    testData.setSource("Test 5");
    PushingAndPopping<ChunkedList, 5>();
  });

  callFunction("Equality and inequality", [] {
    testData.setSource("Test 1");
    EqualityAndInequality<ChunkedList, 1>();

    testData.setSource("Test 2");
    EqualityAndInequality<ChunkedList, 2>();

    testData.setSource("Test 3");
    EqualityAndInequality<ChunkedList, 3>();

    testData.setSource("Test 4");
    EqualityAndInequality<ChunkedList, 4>();

    testData.setSource("Test 5");
    EqualityAndInequality<ChunkedList, 5>();
  });

  callFunction("Concatenation and indexing", [] {
    testData.setSource("Test 1");
    ConcatenationAndIndexing<ChunkedList, 1>();

    testData.setSource("Test 2");
    ConcatenationAndIndexing<ChunkedList, 2>();

    testData.setSource("Test 3");
    ConcatenationAndIndexing<ChunkedList, 3>();

    testData.setSource("Test 4");
    ConcatenationAndIndexing<ChunkedList, 4>();

    testData.setSource("Test 5");
    ConcatenationAndIndexing<ChunkedList, 5>();
  });

  SUCCESS
}
