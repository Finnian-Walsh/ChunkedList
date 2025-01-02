// #define CHUNKED_LIST_DEBUGGING

#include "ChunkedList.hpp"

#define LOG_LEVEL 0

#include "core/TestUtility.hpp"

template<size_t ChunkSize = 32, typename T = int, bool ShouldCopy = true>
using TestChunkedList = ChunkedList<T, ChunkSize, ShouldCopy>;

using namespace TestUtility;

namespace Tests {
  template<size_t ChunkSize>
  void FrontAndBack() {
    using List = TestChunkedList<ChunkSize>;
    performTask("Creating chunked list with initializer list");
    List chunkedList{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto front = reinterpret_cast<ChunkedListAccessor<int, ChunkSize, true>*>(&chunkedList)->getFront();
    auto back = reinterpret_cast<ChunkedListAccessor<int, ChunkSize, true>*>(&chunkedList)->getBack();

    THROW_IF((*front)[0] != 1, "First item is not 1")
    THROW_IF((*back)[back->nextIndex - 1] != 10, "Last item is not 10")

    performTask("Calling size()");
    const size_t size = chunkedList.size();

    THROW_IF(size != 10, std::string{"Unexpected size; got "}+=std::to_string(size)+=" but expected 10");

    performTask("Indexing");
    THROW_IF(chunkedList[size - 1] != 10, "Last item is not 10")
  }

  template<size_t ChunkSize>
  void ConcatenationIndex() {
    TestChunkedList<ChunkSize> chunkedList;

    performTask("Pushing");
    for (int i = 0; i < 10; ++i)
      chunkedList.push(i); {
      performTask("Concatenation");
      std::string &&str = chunkedList.concat(" ");
      THROW_IF(str != "0 1 2 3 4 5 6 7 8 9",
               (std::string{"Concatenation failed; expected \"0 1 2 3 4 5 6 7 8 9\" but got \""}+=str)+='"')
    }

    performTask("Indexing");
    for (int i = 0; i < 10; ++i) {
      const int num = chunkedList[i];
      THROW_IF(num != i,
               (std::string{"Index "}+=std::to_string(i)+=" was expected to be ")+=std::to_string(i)+=std::string{
               " but received "}+=std::to_string(num));
    }
  }

  template<size_t ChunkSize>
  void EqualityInequality() {
    TestChunkedList<ChunkSize> list1{};
    TestChunkedList<ChunkSize> list2{};

    performTask("Pushing");

    for (int i = 1; i < 4; ++i) {
      list1.push(i);
      list2.push(i);
    }

    list1.push(4);
    list2.push(3);

    THROW_IF(list1 == list2, "List comparison 1 failed")

    performTask("Popping");
    list2.pop();

    performTask("Pushing");
    list2.push(4);

    performTask("ChunkedList inequality");
    THROW_IF(list1 != list2, "List comparison 2 failed")

    performTask("Pushing");
    for (int i = 0; i < 80; ++i) {
      list1.push(i);
      list2.push(i);
    }

    list1.push(1);
    list2.push(1);

    performTask("ChunkedList inequality");
    THROW_IF(list1 != list2, "List comparison 3 failed")

    performTask("Popping");
    list2.pop();

    performTask("Pushing");
    list2.push(2);

    performTask("List equality");
    THROW_IF(list1 == list2, "List comparison 4 failed")
  }

  template<SortType SortingAlgorithm>
  void Sorting() {
    performTask("List creation");
    TestChunkedList<> list{};

    performTask("Random Number Generator creation");
    RandomNumberGenerator rng;

    performTask("Pushing or using the RNG");
    for (int i{}; i < 100; ++i) {
      list.push(rng(1, 100));
    }

    performTask("Sorting");
    list.sort<std::less<int>, SortingAlgorithm>();

    performTask("Indexing");
    int last = list[0];

    performTask("Calling begin()");
    auto iterator = list.begin();

    performTask("Iterator operator+=()");
    iterator += 1;

    performTask("Calling end()");
    const auto end = list.end();

    performTask("Iterator inequality");
    for (; iterator != end; ++iterator) {
      performTask("Iterator dereferencing");
      THROW_IF(*iterator < last, "Sorting failed!")

      last = *iterator;

      performTask("Iterator operator++(int)");
    }
  }

  template<size_t ChunkSize>
  void Insertion() {
    TestChunkedList<ChunkSize> chunkedList{5, 10, 15};
    std::string expectedOutput{"[5, 10, 15"};

    RandomNumberGenerator rng;

    for (int i = 0; i < 1000; ++i) {
      int num = rng(-1000, 1000);
      expectedOutput += ", ";
      expectedOutput += std::to_string(num);
      chunkedList.push(num);
    }

    expectedOutput.push_back(']');

    performTask("Inserting ChunkedList into ostream");
    std::ostringstream os{};
    os << chunkedList;

    THROW_IF(os.str() != expectedOutput,
             std::string("ostream insertion ran incorrectly\nGot: ") += os.str())
  }
}


int main() {
  BEGIN

  using namespace Tests;

  callFunction("Front and back", [] {
    testData.setSource("Test 1");
    FrontAndBack<1>();

    testData.setSource("Test 2");
    FrontAndBack<2>();

    testData.setSource("Test 3");
    FrontAndBack<3>();

    testData.setSource("Test 4");
    FrontAndBack<4>();

    testData.setSource("Test 5");
    FrontAndBack<5>();
  });

  callFunction("Insertion", [] {
    testData.setSource("Test 1");
    Insertion<1>();

    testData.setSource("Test 2");
    Insertion<2>();

    testData.setSource("Test 3");
    Insertion<3>();

    testData.setSource("Test 4");
    Insertion<4>();

    testData.setSource("Test 5");
    Insertion<5>();
  });

  callFunction("Sorting", [] {
    testData.setSource("Bubble Sorting");
    Sorting<BubbleSort>();

    testData.setSource("Selection Sorting");
    Sorting<SelectionSort>();

    testData.setSource("Insertion Sorting");
    Sorting<InsertionSort>();

    testData.setSource("Merge Sorting");
    Sorting<MergeSort>();

    testData.setSource("Quick Sorting");
    Sorting<QuickSort>();

    testData.setSource("Heap Sorting");
    Sorting<HeapSort>();
  });

  callFunction("Iterators", [] {
    performTask("List creation");
    TestChunkedList<> chunkedList{1, 2, 3, 4, 5};
    int total{};

    performTask("List iteration (implicit)");
    for (const int num: chunkedList)
      total += num;

    THROW_IF(total != 15, "List sum is not 15")
  });

  callFunction("Pushing and popping", [] {
    TestChunkedList<4, char> chunkedList{};

    performTask("Pushing");
    for (int i = 0; i < 10; ++i) {
      chunkedList.push('a');
      chunkedList.push('b');
    }

    for (int i = 20; i > 1; --i) {
      performTask("Calling size()");
      const size_t size = chunkedList.size();
      THROW_IF(i != size,
               ((std::string{"Unexpected ChunkedList size; expected "}+=std::to_string(i))+=" but got ")+=std::
               to_string(size));

      performTask("Popping");
      chunkedList.pop();
    }

    performTask("Indexing");
    const char firstItem = chunkedList[0];
    THROW_IF(firstItem != 'a', std::string{"First item is not 'a'; got "}+=firstItem);
  });

  callFunction("Equality and inequality", [] {
    testData.setSource("Test 1");
    EqualityInequality<1>();

    testData.setSource("Test 2");
    EqualityInequality<2>();

    testData.setSource("Test 3");
    EqualityInequality<3>();

    testData.setSource("Test 4");
    EqualityInequality<4>();

    testData.setSource("Test 5");
    EqualityInequality<5>();
  });

  callFunction("Concatenation and indexing", [] {
    testData.setSource("Test 1");
    ConcatenationIndex<1>();

    testData.setSource("Test 2");
    ConcatenationIndex<2>();

    testData.setSource("Test 3");
    ConcatenationIndex<3>();

    testData.setSource("Test 4");
    ConcatenationIndex<4>();

    testData.setSource("Test 5");
    ConcatenationIndex<5>();
  });

  SUCCESS
}
