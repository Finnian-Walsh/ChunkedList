#define CHUNKED_LIST_DEBUGGING

#include "ChunkedList.hpp"

#define LOG_LEVEL 2
#include "core/TestUtility.hpp"

template<size_t ChunkSize = 32, typename T = int, bool ShouldCopy = false>
using TestChunkedList = ChunkedList<T, ChunkSize, ShouldCopy>;

using namespace TestUtility;

int main() {
  BEGIN

  CALL_TEST("Front and back", ([]() -> Result {
              performOperation("unknown", 2);
              constexpr size_t chunkSize = 2;
              using List = TestChunkedList<chunkSize>;
              performOperation("Creating chunked list with initializer list");
              List chunkedList{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

              int chunkIndex{};

              performOperation("Calling beginChunk()");
              auto chunkIt = chunkedList.beginChunk();

              performOperation("calling endChunk()");
              const auto endChunk = chunkedList.endChunk();

              performOperation("ChunkIterator inequality");
              for (; chunkIt != endChunk; ++chunkIt, ++chunkIndex) {
              performOperation("Dereferencing ChunkIterator or Chunk indexing");
              const int item = chunkIt->operator[](chunkSize - 1);

              performOperation("String concatenation");
              RETURN_IF(item & 1,
                std::string{"Unexpected item found: "}.operator+=(item).operator+=(" at chunk ").operator+=(
                  chunkIndex + '0'))

              performOperation("ChunkIterator operator++(int)");
              }

              performOperation("Indexing");
              RETURN_IF(chunkedList[chunkedList.size() - 1] != 10, "Last item is not 10")

              performOperation("Returning");
              return Result{true};
              }))

  CALL_TEST("Insertion", ([]() -> Result {
              TestChunkedList<> chunkedList{5, 10, 15};
              std::string expectedOutput{"[5, 10, 15"};

              RandomNumberGenerator rng;

              for (int i = 0; i < 1000; ++i) {
              int num = rng(-1000, 1000);
              expectedOutput += ", ";
              expectedOutput += std::to_string(num);
              chunkedList.push(num);
              }

              expectedOutput.push_back(']');

              potentialError = "Inserting ChunkedList into ostream";
              std::ostringstream os{};
              os << chunkedList;

              RETURN_IF(os.str() != expectedOutput,
                std::string("ostream insertion ran incorrectly\nGot: ") += (os.str()))

              return Result{true};
              }))

  CALL_TEST("Sorting", ([]() -> Result {
              int num{};

              potentialError = "List creation";
              TestChunkedList<> list{};

              potentialError = "Random Number Generator creation";
              RandomNumberGenerator rng;

              potentialError = "Pushing or using the RNG";
              for (int i{}; i < 100; ++i) {
              list.push(rng(1, 100));
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
              TestChunkedList<> chunkedList{1, 2, 3, 4, 5};
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
              TestChunkedList<> list1{};
              TestChunkedList<> list2;

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

  SUCCESS
}
