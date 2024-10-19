
#define CHUNKED_LIST_DEBUGGING
#include "ChunkedList.hpp"

#include <iostream>
#include <functional>
#include <random>
#include <cstdlib>
#include <csignal>
#include <memory>

template<size_t ChunkSize = 32, typename T = int, bool ShouldCopy = false>
using TestChunkedList = ChunkedList<T, ChunkSize, ShouldCopy>;

#define BEGIN int testNumber{1}; std::cout << "Starting tests..." << std::endl
#define CALL_TEST(functionName, functionPtr) std::cout << "Test " << testNumber << ":\n"; callFunction(functionName, functionPtr); std::cout << "Test " << testNumber << " successful" << std::endl; ++testNumber;
#define SUCCESS std::cout << "All " << testNumber - 1 << " tests have been ran."; return EXIT_SUCCESS
#define RETURN_IF(condition, str) if (condition) return Result{str};

class PotentialError {
  private:
    std::string error;
    bool null{true};
  public:
    
    PotentialError() = default;
    
    [[maybe_unused]] explicit PotentialError(const char *str) : error{str}, null{false} {}
    
    [[maybe_unused]] explicit PotentialError(std::string &&str) : error{std::move(str)}, null{false} {}
    
    [[maybe_unused]] explicit PotentialError(const std::string &str) : error{str}, null{false} {}
    
    [[maybe_unused]] void set(const char *str) {
      error = str;
    }
    
    [[maybe_unused]] void set(const std::string &str) {
      error = str;
    }
    
    [[nodiscard]] const std::string &get() const {
      return error;
    }
    
    PotentialError &operator=(const char *str) {
      error = str;
      return *this;
    }
    
    PotentialError &operator=(const std::string &str) {
      error = str;
      return *this;
    }
    
    void setNull() {
      null = true;
    }
    
    [[nodiscard]] bool isNull() const {
      return null;
    }
  
} potentialError;

class Result {
  public:
    const std::string message{};
    const bool status{false};
    
    explicit Result(bool status) : status{status} {}
    
    explicit Result(std::string &&message) : message{std::move(message)} {}
    
    explicit Result(const std::string &message) : message{message} {}
    
    explicit Result(const char *message) : message{message} {}
    
    bool operator!() const {
      return status;
    }
    
    operator bool() const {
      return status;
    }
};

void callFunction(const char *functionName, Result(*functionPtr)()) {
  std::signal(SIGSEGV, [](int signalNumber) -> void {
    std::cerr << "Potential error: " << potentialError.get() << "\nSegmentation fault: " << signalNumber << std::endl;
    std::terminate();
  });
  
  std::unique_ptr<Result> result{};
  
  try {
    potentialError.setNull();
    result = std::make_unique<Result>(functionPtr());
  } catch (const std::exception &e) {
    if (potentialError.isNull())
      std::cerr << "Call to " << functionName << "failed\nUnknown error" << std::endl;
    else
      std::cerr << "Call to " << functionName << "failed\nPotential error: " << potentialError.get() << '\n' << e.what()
                << std::endl;
    std::terminate();
  }
  
  if (!result->status)
    throw std::runtime_error(
    ((std::string{"Function call failed:\n"} += functionName) += '\n') += result->message);
}

int main() {
  BEGIN;
  
  CALL_TEST("Front and back", ([]() -> Result {
    using List = TestChunkedList<2>;
    List chunkedList{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    int chunkIndex{};
    
    {
      potentialError = "Inserting ChunkedList into ostream";
      std::ostringstream os{};
      os << chunkedList;
      
      RETURN_IF(os.str() != "[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]",
                std::string("ostream insertion ran incorrectly\nGot: ").operator+=(os.str()))
    }
    
    potentialError = "calling beginChunk()";
    auto chunkIt = chunkedList.beginChunk();
    
    potentialError = "calling endChunk()";
    auto endChunk = chunkedList.endChunk();
    
    potentialError = "ChunkIterator inequality";
    for (; chunkIt != endChunk; ++chunkIt, ++chunkIndex) {
      potentialError = "Dereferencing ChunkIterator or Chunk indexing";
      int item = chunkIt->operator[](chunkedList.chunkSize - 1);
      
      potentialError = "String concatenation";
      RETURN_IF(item & 1,
                std::string{"Unexpected item found: "}.operator+=(item).operator+=(" at chunk ").operator+=(
                chunkIndex + '0'))
      
      potentialError = "ChunkIterator operator++(int)";
    }
    
    potentialError = "Indexing";
    RETURN_IF(chunkedList[chunkedList.size() - 1] != 10, "Last item is not 10")
    
    return Result{true};
  }))
  
  CALL_TEST("Sorting", ([]() -> Result {
    int num{};
    
    potentialError = "List creation";
    TestChunkedList list{};
    
    potentialError = "stl";
    std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> distribution{1, 100};
    
    potentialError = "Pushing";
    for (int i{}; i < 100; ++i) {
      list.push(distribution(gen));
    }
    
    potentialError = "Sorting";
    list.sort();
    
    potentialError = "Indexing";
    int last = list[0];
    
    potentialError = "calling begin()";
    auto iterator = list.begin();
    
    potentialError = "Iterator operator+=()";
    iterator += 1;
    
    potentialError = "calling end()";
    auto end = list.end();
    
    potentialError = "Iterator inequality";
    for (; iterator != end; ++iterator) {
      potentialError = "Iterator dereferencing";
      if (*iterator < last) {
        return Result{"Sorting failed!"};
      }
      
      last = *iterator;
      
      potentialError = "Iterator operator++(int)";
    }
    
    return Result{true};
  }))
  
  CALL_TEST("Iterators", ([]() -> Result {
    potentialError = "List creation";
    TestChunkedList chunkedList{1, 2, 3, 4, 5};
    int total{};
    
    potentialError = "List iteration (implicit)";
    for (int num: chunkedList)
      total += num;
    
    return Result{total == 15};
  }))
  
  CALL_TEST("Pushing and popping", ([]() -> Result {
    TestChunkedList<4, char> chunkedList{};
    
    potentialError = "Pushing";
    for (int i = 0; i < 10; ++i) {
      chunkedList.push('a');
      chunkedList.push('b');
    }
    
    for (int i = 20; i > 1; ++i) {
      potentialError = "Calling size()";
      RETURN_IF(i != chunkedList.size(), "Unexpected ChunkedList size")
      
      potentialError = "Popping";
      chunkedList.pop();
    }
    
    potentialError = "Indexing";
    return chunkedList[0] == 'a' ? Result{true} : Result{std::string{"First item is not 'a'"}};
  }))
  
  CALL_TEST("Equality and inequality", ([]() -> Result {
    TestChunkedList list1{};
    TestChunkedList list2;
    
    potentialError = "Pushing";
    
    list1.push(1);
    list1.push(2);
    list1.push(3);
    list1.push(4);
    
    list2.push(1);
    list2.push(2);
    list2.push(3);
    list2.push(3);
    
    RETURN_IF(list1 == list2, "List comparison failed")
    
    potentialError = "Popping";
    list2.pop();
    
    potentialError = "Pushing";
    list2.push(4);
    
    potentialError = "ChunkedList inequality";
    RETURN_IF(list1 != list2, "List comparison failed")
    
    potentialError = "Pushing";
    for (int i = 0; i < 80; ++i) {
      list1.push(i);
      list2.push(i);
    }
    
    list1.push(1);
    list2.push(1);
    
    potentialError = "ChunkedList inequality";
    RETURN_IF(list1 != list2, "List comparison failed")
    
    potentialError = "Popping";
    list2.pop();
    
    potentialError = "Pushing";
    list2.push(2);
    
    potentialError = "List equality";
    RETURN_IF(list1 == list2, "List comparison failed")
    
    return Result{true};
  }))
  
  CALL_TEST("Concatenation and indexing", ([]() -> Result {
    TestChunkedList<2> chunkedList;
    
    potentialError = "Pushing";
    for (int i{}; i < 10; ++i)
      chunkedList.push(i);
    
    potentialError = "Concatenation";
    RETURN_IF((chunkedList.concat(std::string{" "})) != "0 1 2 3 4 5 6 7 8 9", "Concatenation failed")
    
    potentialError = "Indexing";
    for (int i{}; i < 10; ++i)
      RETURN_IF(chunkedList[i] != i, "")
    
    return Result{true};
  }))
  
  // callFunction("", []() -> bool {
  //   return true;
  // }, bool{true});
  
  SUCCESS;
}
