

#define CHUNKED_LIST_DEBUGGING
#include "StandardChunkedList.hpp"

#include "core/TestUtility.hpp"

template<size_t ChunkSize = 32, typename T = int, bool ShouldCopy = false>
using TestChunkedList = StandardChunkedList<T, ChunkSize, ShouldCopy>;

int main() {
  BEGIN
  
  CALL_TEST("List creation", []() -> Result {
    potentialError = "List creation";
    std::cout << "Creating list";
    TestChunkedList<> testList{};
    std::cout << "\nCreated";
    
    return Result{true};
  })
  
  SUCCESS
}