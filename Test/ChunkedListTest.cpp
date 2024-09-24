
#include "ChunkedList.hpp"

#include <iostream>
#include <functional>

template<size_t ChunkSize = 32, typename T = int, bool ShouldCopy = false>
using StandardChunkedList = ChunkedList<T, ChunkSize, ShouldCopy>;

#define BEGIN int testNumber{1}; std::cout << "Starting tests..." << std::endl
#define CALL_TEST(functionName, functionPtr) callFunction(functionName, functionPtr); std::cout << "Test " << testNumber << " successful." << std::endl; ++testNumber;
#define SUCCESS std::cout << "All " << testNumber << " tests have been ran."; return EXIT_SUCCESS
#define RETURN_IF(condition, str) if (condition) return Result{std::string{str}};

class Result {
  public:
    const std::string message{};
    const bool status{};
    
    explicit Result(bool status) : status{status} {}
    
    explicit Result(std::string &&message) : message{std::move(message)} {}
    
    explicit Result(const std::string &message) : message{message} {}
};

void callFunction(const char functionName[], Result(*functionPtr)()) {
  Result &&result = functionPtr();
  if (!result.status)
    throw std::runtime_error(
    std::string{"Function call failed:\n"}.operator+=(functionName).operator+=('\n').operator+=(result.message));
}

int main() {
  BEGIN;
  
  CALL_TEST("Front and back", ([]() -> Result {
    using List = StandardChunkedList<2>;
    List chunkedList{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    int chunkIndex{};
    
    for (auto chunkIt = chunkedList.beginChunk(); chunkIt != chunkedList.endChunk(); ++chunkIt, ++chunkIndex) {
      int item = chunkIt->operator[](chunkedList.chunk_size() - 1);
      
      RETURN_IF(item % 2, "")
      
      if (item % 2)
        return Result{std::string{"Unexpected item found: "}.operator+=(item).operator+=(" at chunk ").operator+=(chunkIndex + '0')};
    }
    
    RETURN_IF((*--chunkedList.endChunk())[chunkedList.chunk_size() - 1] != 10, "Last item is not 10")
    
    return Result{true};
  }))
  
  CALL_TEST("Iterators", ([]() -> Result {
    std::cout << "creating list\n";
    StandardChunkedList chunkedList{1, 2, 3, 4, 5};
    int total{};
    
    std::cout << "iterating...\n";
    for (int num: chunkedList)
      total += num;
    
    std::cout << "finished iterating...\n";
    return Result{total == 15};
  }))
  
  CALL_TEST("Pushing and popping", ([]() -> Result {
    StandardChunkedList<4, char> chunkedList{};
    
    for (int i = 0; i < 10; ++i) {
      chunkedList.push('a');
      chunkedList.push('b');
    }
    
    for (int i = 20; i > 1; ++i) {
      RETURN_IF(i != chunkedList.size(), "Unexpected ChunkedList size")
      
      chunkedList.pop();
    }
    
    return chunkedList[0] == 'a' ? Result{true} : Result{std::string{"First item is not 'a'"}};
  }))
  
  CALL_TEST("Equality and inequality", ([]() -> Result {
    StandardChunkedList list1{}, list2{};
    
    list1.push(1);
    list1.push(2);
    list1.push(3);
    list1.push(4);
    
    list2.push(1);
    list2.push(2);
    list2.push(3);
    list2.push(3);
    
    RETURN_IF(list1 != list2, "List comparison failed")
    
    list2.pop();
    list2.push(4);
    
    RETURN_IF(list1 == list2, "List comparison failed")
    
    for (int i = 0; i < 80; ++i) {
      list1.push(i);
      list2.push(i);
    }
    
    list1.push(1);
    list2.push(1);
    
    RETURN_IF(list1 != list2, "List comparison failed")
    
    list2.pop();
    list2.push(2);
    
    RETURN_IF(list1 != list2, "List comparison failed")
    
    auto it1 = list1.begin(), &&it2 = list1.begin();
    
    ++it2;
    
    RETURN_IF(it1 == it2, "Iterator comparison failed")
    
    --it2;
    
    RETURN_IF(it1 != it2, "Iterator comparison failed")
    
    it1 += 50;
    
    it2 += 40;
    
    for (int i = 0; i < 10; ++i)
      ++it2;
    
    return it1 == it2 ? Result{true} : Result{std::string{"Iterator comparison failed"}};
  }))
  
  CALL_TEST("Concatenation and indexing", ([]() -> Result {
    StandardChunkedList<2> chunkedList;
    
    for (int i{}; i < 10; ++i)
      chunkedList.push(i);
    
    RETURN_IF(std::string{"1 2 3 4 5 6 7 8 9 10"} != chunkedList.concat(" "), "Concatenation failed")
    
    for (int i{}; i < 10; ++i)
      RETURN_IF(chunkedList[i] != i, "")
    
    return Result{true};
  }))
  
  // callFunction("", []() -> bool {
  //   return true;
  // }, bool{true});
  
  SUCCESS;
}
