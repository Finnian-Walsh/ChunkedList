## Table of Contents

1. [Concept](#concept)
2. [Chunks](#chunks)
3. [Deallocation](#deallocation)
4. [Methods](#methods)
    - [Iteration](#iteration)
    - [Sorting](#sorting)
    - [Private Member Accessing](#private-member-accessing)
5. [Debugging](#debugging)
6. [Installation](#installation)
7. [Examples](#examples)
    - [Basic Usage](#basic-usage)
    - [String Concatenation](#string-concatenation)
8. [Snake Case Variant](#snake-case-variant)

# Chunked List

## Concept

The **ChunkedList** is a simple and efficient C++ list data structure, which uses **Chunks** to store data.

```cpp
template<typename T, size_t ChunkSize = 32>
class ChunkedList;
```

This data structure is essentially a linked list, where each node is a **Chunk**.

### Template Parameters

- `T` the type of data which will be stored in the **ChunkedList**
- `ChunkSize` the size of each **Chunk**

## Chunks

Each **Chunk** contains a fixed array, of size ChunkSize. However, **Chunks** are abstracted away and from the user's
view.

Furthermore, each **Chunk** keeps track of its next index, incrementing and decrementing it by 1 each push and pop,
respectively.
Therefore, calling the pop method on a **ChunkedList** doesn't deallocate anything unless the next index of the back
**Chunk** is equal to 0, causing the entire **Chunk** to be deallocated. Using popped values which haven't been
deallocated is recommended against.

## Deallocation

When a **ChunkedList** instance is deallocated, every **Chunk** gets deallocated, from the `back` to the `front`.

```cpp
template<typename T, size_t ChunkSize>
ChunkedList<T, ChunkSize>::~ChunkedList() {
  do {
    Chunk *newBack = back->prevChunk;
    delete back;
    back = newBack;
  } while (back);
}
```

## Methods

### Iteration

To iterate over a **ChunkedList**, **Iterators** should be used - range-based for-loops are supported. Index based
for-loops should not be used, since each subscript operation performs a O(n/k) search, iterating through each **Chunk**
until the correct value is found.

```cpp
for (T value : chunkedList) {
  ...
}
```

```cpp
template<typename T, size_t ChunkSize>
ChunkedList<T, ChunkSize>::Iterator begin(ChunkedList<T, ChunkSize> &chunkedList);

template<typename T, size_t ChunkSize>
ChunkedList<T, ChunkSize>::Iterator end(ChunkedList<T, ChunkSize> &chunkedList);
```

### Sorting

The **ChunkedList** data structure comes with a built-in sort function, allowing you to sort it with a specified compare
class and an algorithm of your choice with template parameters.

```cpp
template<typename Compare = std::less<T>, SortEnum SortType = HeapSort>
void sort();
```

By default, the sort function uses `std::less<T>` to compare types and `HeapSort` as the Sorting algorithm

### Private member accessing

A **ChunkedListAccessor** class provides safe access to the private members:

- front (first chunk)
- back (last chunk)
- chunkCount (number of chunks)

```cpp
template<typename T, size_t ChunkSize>
class ChunkedListAccessor final : ChunkedList<T, ChunkSize>;
```

Usage:

```cpp
auto &accessor = static_cast<ChunkedListAccessor<T, ChunkSize>

auto front = accessor.getFront();
auto back = accessor.getBack();
size_t chunkCount = accessor.getChunkCount();
```

## Debugging

Enable debugging mode by defining:

```cpp
#define CHUNKED_LIST_DEBUGGING
```

This will log **ChunkedList** operations to the console.

## Installation

Clone the repository or add it as a submodule to your project.

```bash
git submodule add https://github.com/Finnian-Walsh/ChunkedList.git <path>
```

```bash
git clone https://github.com/Finnian-Walsh/ChunkedList.git
```

Then, add the `include` directory to your include directories.

## Examples

### Basic Usage

```cpp
#include <iostream>

#include "ChunkedList.hpp"

int main() {
  ChunkedList<int> list{1, 2, 3, 4, 5};
  
  for (int num: list)
    std::cout << num << '\n';    
    
  return 0;
}
```

Output:

```
1
2
3
4
5
```

### String Concatenation

```cpp
#include <iostream>

#include "ChunkedList.hpp"

int main() {
  ChunkedList<std::string> list{"Hello", "world!"};
  
  std::cout << list.concat(" ") << std::endl;
}
```

Output:

```
Hello world!
```

## Snake Case Variant

For projects where `snake_case` naming conventions are used, include:

```cpp
#include "ChunkedListSnake.hpp"
```

This implements the **chunked_list** class, with identical functionality to the **ChunkedList**.

### Example

```cpp
#include "ChunkedListSnake.hpp"

#include <random>

int main() {
  chunked_list<int, 32> chunkedList{};
  
  std::mt19937 gen{std::random_device{}};
  std::uniform_int_distribution<int> dist{1, 100};
  
  for (int i = 0; i < 1000; ++i) {
    int num = dist(gen);
    chunkedList.push_back(num);
  }

  std::cout << "Unsorted: " << chunkedList << std::endl;
  
  chunkedList.sort<std::less<int>, HeapSort>();
  
  std::cout << "Sorted: " << chunkedList << std::endl;
  
  return 0;
}
```
