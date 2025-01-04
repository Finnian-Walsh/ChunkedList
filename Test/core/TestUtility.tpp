#pragma once

#include "TestUtility.hpp"
#include "ChunkedListAccessor.hpp"

#include <cstdlib>

using namespace TestUtility;

inline RandomNumberGenerator::RandomNumberGenerator() : engine{std::random_device{}()} {
}

inline int RandomNumberGenerator::operator()(const int min, const int max) {
  return std::uniform_int_distribution{min, max}(engine);
}

inline TestData::TestData(const char *str) : source{str}, nullSource{false} {
}

inline TestData::TestData(std::string &&str) : source{std::move(str)}, nullSource{false} {
}


inline void TestData::setSource(const char *str) {
  source = str;
  nullSource = false;
}

inline void TestData::setSource(std::string &&str) {
  source = std::move(str);
}

inline void TestData::setTask(const char *str) {
  task = str;
  nullTask = false;
}

inline void TestData::setTask(std::string &&str) {
  task = std::move(str);
  nullTask = false;
}

inline const std::string &TestData::getTest() const {
  return test;
}

inline const std::string &TestData::getSource() const {
  return source;
}

inline const std::string &TestData::getTask() const {
  return task;
}

inline void TestData::newTest(const std::string &str) {
  test = str;
  nullSource = true;
  nullTask = true;
}

inline bool TestData::sourceIsNull() const {
  return nullSource;
}

inline bool TestData::taskIsNull() const {
  return nullTask;
}

inline void TestUtility::callFunction(const char *functionName, void (*functionPtr)()) {
  std::cout << "Test " << testNumber << ": " << functionName << '\n';
  ++testNumber;

  try {
    testData.newTest(functionName);
    functionPtr();
  } catch (const std::exception &e) {
    throw std::runtime_error(
      std::string{"Call to "}.operator+=(functionName).operator+=(" failed\nSource: ").operator+=(
        testData.sourceIsNull() ? "NULL" : testData.getSource()).
      operator+=("\nTask: ").operator+=(testData.taskIsNull() ? "NULL" : testData.getTask()).operator+=("\nError: ").
      operator+=(e.what()));
  }

  std::cout << "Test " << testNumber << " successful\n" << std::endl;
}

inline void TestUtility::performTask(const char *taskName, const int logLevel) {
  testData.setTask(taskName);
  if (logLevel <= LogLevel) {
    std::cout << taskName << '\n';
  }
}

template<template <typename, size_t> typename ChunkedListType, size_t ChunkSize>
void Tests::FrontAndBack() {
  performTask("Creating chunked list with initializer list");

  using ListType = ChunkedListType<DefaultT, ChunkSize>;
  using AccessorType = ChunkedListAccessor<DefaultT, ChunkSize>;

  ListType chunkedList{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  auto front = reinterpret_cast<AccessorType *>(&chunkedList)->getFront();
  auto back = reinterpret_cast<AccessorType *>(&chunkedList)->getBack();

  THROW_IF((*front)[0] != 1, "First item is not 1")
  THROW_IF((*back)[back->nextIndex - 1] != 10, "Last item is not 10")

  performTask("Calling size()");
  const size_t size = chunkedList.size();

  THROW_IF(size != 10, std::string{"Unexpected size; got "}+=std::to_string(size)+=" but expected 10");

  performTask("Indexing");
  THROW_IF(chunkedList[size - 1] != 10, "Last item is not 10")
}

template<template <typename, size_t> typename ChunkedListType, size_t ChunkSize>
void Tests::Insertion() {
  using ListType = ChunkedListType<DefaultT, ChunkSize>;

  ListType chunkedList{5, 10, 15};
  std::string expectedOutput{"[5, 10, 15"};

  RandomNumberGenerator rng;

  for (int i = 0; i < 1000; ++i) {
    int num = rng(-1000, 1000);
    expectedOutput += ", ";
    expectedOutput += std::to_string(num);
    VARIANT_CODE(chunkedList.push, chunkedList.push_back)(num);
  }

  expectedOutput.push_back(']');

  performTask("Inserting ChunkedList into ostream");
  std::ostringstream os{};
  os << chunkedList;

  THROW_IF(os.str() != expectedOutput,
           std::string("ostream insertion ran incorrectly\nGot: ") += os.str())
}

template<SortType SortingAlgorithm, template<typename, size_t> typename ChunkedListType>
void Tests::Sorting() {
  performTask("List creation");
  ChunkedListType<DefaultT, DefaultChunkSize> list;

  performTask("Random Number Generator creation");
  RandomNumberGenerator rng;

  performTask("Pushing or using the RNG");
  for (int i{}; i < 100; ++i) {
    VARIANT_CODE(list.push, list.push_back)(rng(1, 100));
  }

  performTask("Sorting");
  list.template sort<std::less<DefaultT>, SortingAlgorithm>();

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

template<template <typename, size_t> typename ChunkedListType, size_t ChunkSize>
void Tests::PushingAndPopping() {
  using ListType = ChunkedListType<DefaultT, ChunkSize>;
  ListType chunkedList{};

  performTask("Pushing");
  for (int i = 0; i < 10; ++i) {
    VARIANT_CODE(chunkedList.push, chunkedList.push_back)('a');
    VARIANT_CODE(chunkedList.push, chunkedList.push_back)('b');
  }

  for (int i = 20; i > 1; --i) {
    performTask("Calling size()");
    THROW_IF(i != chunkedList.size(), "Unexpected ChunkedList size")

    performTask("Popping");
    VARIANT_CODE(chunkedList.pop, chunkedList.pop_back)();
  }

  performTask("Indexing");
  const char firstItem = chunkedList[0];

  performTask("Comparing first item");
  THROW_IF(firstItem != 'a', "First item is not 'a'")
}

template<template <typename, size_t> typename ChunkedListType, size_t ChunkSize>
void Tests::Iterators() {
  using ListType = ChunkedListType<DefaultT, ChunkSize>;

  performTask("List creation");
  ListType chunkedList{1, 2, 3, 4, 5};
  int total{};

  performTask("List iteration (implicit)");
  for (const int num: chunkedList)
    total += num;

  THROW_IF(total != 15, "List sum is not 15")
}

template<template <typename, size_t> typename ChunkedListType, size_t ChunkSize>
void Tests::ConcatenationAndIndexing() {
  using ListType = ChunkedListType<DefaultT, ChunkSize>;
  ListType chunkedList;

  performTask("Pushing");
  for (int i = 0; i < 10; ++i)
    VARIANT_CODE(chunkedList.push, chunkedList.push_back)(i); {
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

template<template <typename, size_t> typename ChunkedListType, size_t ChunkSize>
void Tests::EqualityAndInequality() {
  using ListType = ChunkedListType<DefaultT, ChunkSize>;

  ListType list1{};
  ListType list2{};

  performTask("Pushing");

  for (int i = 1; i < 4; ++i) {
    VARIANT_CODE(list1.push, list1.push_back)(i);
    VARIANT_CODE(list2.push, list2.push_back)(i);
  }

  VARIANT_CODE(list1.push, list1.push_back)(4);
  VARIANT_CODE(list2.push, list2.push_back)(3);

  THROW_IF(list1 == list2, "List comparison 1 failed")

  performTask("Popping");
  VARIANT_CODE(list2.pop, list2.pop_back)();

  performTask("Pushing");
  VARIANT_CODE(list2.push, list2.push_back)(4);

  performTask("ChunkedList inequality");
  THROW_IF(list1 != list2, "List comparison 2 failed")

  performTask("Pushing");
  for (int i = 0; i < 80; ++i) {
    VARIANT_CODE(list1.push, list1.push_back)(i);
    VARIANT_CODE(list2.push, list2.push_back)(i);
  }

  VARIANT_CODE(list1.push, list1.push_back)(1);
  VARIANT_CODE(list2.push, list2.push_back)(1);

  performTask("ChunkedList inequality");
  THROW_IF(list1 != list2, "List comparison 3 failed")

  performTask("Popping");
  VARIANT_CODE(list2.pop, list2.pop_back)();

  performTask("Pushing");
  VARIANT_CODE(list2.push, list2.push_back)(2);

  performTask("List equality");
  THROW_IF(list1 == list2, "List comparison 4 failed")
}
