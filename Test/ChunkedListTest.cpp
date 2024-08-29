
#include "ChunkedList.hpp"

#include <string>
#include <cstdlib>

template<typename FunctionType, typename OutputType, typename... InputTypes>
void callFunction(const char functionName[], FunctionType functionPtr, const OutputType& expectedOutput, InputTypes&&... input) {
  if (expectedOutput != functionPtr(input...)) {
    throw std::runtime_error(std::string{"Function call produced invalid output:\n"} + functionName);
  }
}

int main() {
  callFunction("Iterators", []() -> int {
    ChunkedList<int> chunkedList{1, 2, 3, 4, 5};
    int total{};

    for (int num : chunkedList)
      total += num;

    return total;
  }, 15);

  callFunction("Pushing and popping", []() -> char {
    ChunkedList<char, 4, false> chunkedList{};
    
    for (int i = 0; i < 10; ++i) {
      chunkedList.push('a');
      chunkedList.push('b');
    }

    for (int i = 0; i < 19; ++i)
      chunkedList.pop();

    return chunkedList[0];
  }, 'a');

  callFunction("Equality and inequality", []() -> bool {
    ChunkedList<int> list1{}, list2{};

    list1.push(1);
    list1.push(2);
    list1.push(3);
    list1.push(4);

    list2.push(1);
    list2.push(2);
    list2.push(3);
    list2.push(3);

    if (list1 != list2)
      return false;
    
    list2.pop();
    list2.push(4);

    if (list1 == list2)
      return false;
    
    for (int i = 0; i < 80; ++i) {
      list1.push(i);
      list2.push(i);
    }

    list1.push(1);
    list2.push(1);

    if (list1 != list2)
      return false;
    
    list2.pop();
    list2.push(2);

    if (list1 != list2)
      return false;

    auto it1 = list1.begin(), &&it2 = list1.begin();

    ++it2;

    if (it1 == it2)
      return false;

    --it2;

    if (it1 != it2)
      return false;

    it1 += 50;

    it2 += 40;

    for (int i = 0; i < 10; ++i)
      ++it2;

    if (it1 != it2)
      return false;

    return it1 == it2;
  }, bool{true});

  callFunction("Concatenation and indexing", []() -> bool {
    ChunkedList<int, 2> chunkedList;

    for (int i{}; i < 10; ++i)
      chunkedList.push(i);

    if (std::string{chunkedList.concat(" ")} != std::string{"1 2 3 4 5 6 7 8 9 10"})
      return false;

    for (int i{}; i < 10; ++i)
      if (chunkedList[i] != i)
        return false;

    return true;

  }, bool{true});

  // callFunction("", []() -> bool {
  //   return true;
  // }, bool{true});

  return 0;
}
