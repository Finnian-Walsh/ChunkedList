
#include "core/TestUtility.hpp"
#include "StandardChunkedList.hpp"

template<size_t ChunkSize = 32, typename T = int, bool ShouldCopy = false>
using TestChunkedList = StandardChunkedList<T, ChunkSize, ShouldCopy>;

int main() {
  TestChunkedList<> testList{};
}