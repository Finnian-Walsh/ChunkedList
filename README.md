# Chunked List

## Concept

A simple C++ list data structure, which uses **Chunks** to store data.

```cpp
template<typename T, size_t ChunkSize = 32, bool ShouldCopy = false>
class ChunkedList;
```

The **ChunkedList** data structure is essentially a linked list, where each node is a **Chunk**, as explained below.

## Chunks

Each **Chunk** contains a fixed array, of size ChunkSize, and the **Chunk** class has 3 constructors, each populating
the array in a different way. However,
**Chunks** are abstracted away and from the user's view.

Furthermore, each **Chunk** keeps track of its next index, incrementing and decrementing it by 1 each push and pop
respectively.
Therefore, calling the pop method on a **ChunkedList** doesn't deallocate anything unless the next index of the back
**Chunk** is equal to 0, causing the entire Chunk and its contents to be deallocated. Although, you should make sure to
be careful with this - using values which have been popped is recommended against.

## Iterating

To iterate over a **ChunkedList**, **Iterators** (or **ChunkIterators**) should be used. Index based for-loops should
not be used, since each subscript operation performs a linear search, iterating through each **Chunk** until the correct
value is found. Additionally, range
based for-loops are supported, which implicitly utilise **Iterators**.

```cpp
for (T value : chunkedList) {
  ...
}
```

```cpp
template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator begin(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList);

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator end(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList);
```

## Debugging

If you wish to enable debugging mode for **ChunkedList** development, you can add

```cpp
#define CHUNKED_LIST_DEBUGGING
```

in `src/ChunkedList.hpp` or before you include the header file. Doing so will cause the **ChunkedList** operations to be
outputted in the console.

## How to use

Using this repository is simple. Either add the repository as a submodule or clone the repository.

```bash
git submodule add https://github.com/Finnian-Walsh/ChunkedList.git <path>
```

```bash
git clone https://github.com/Finnian-Walsh/ChunkedList.git
```

## Coding examples

```cpp
#include "ChunkedList.hpp"

#include <iostream>

int main() {
  ChunkedList<int> list{1, 2, 3, 4, 5};
  
  for (int num : list)
    std::cout << num << '\n';    
    
  return 0;
}
```

Creates a **ChunkedList** of **int**s, populated with 1, 2, 3, 4, 5 then outputs each number in the console with a range
based for-loop.

```cpp
#include "ChunkedList.hpp"

#include <iostream>

int main() {
  ChunkedList<std::string> list{"hello", "world"};
  
  std::cout << list.concat(" ");
}
```

Creates a **ChunkedList** populated with **std::string**s containing "hello" and "world" then joins each
word in the list together with " ", subsequently outputting the returned **std::string**.

## Naming Conventions

The **ChunkedList** data structure uses `camelCase`. However, if you would like to use the **ChunkedList** with
`snake_case`, you can add

```cpp
#include "StandardChunkedList.hpp"
```

to your code instead. This gives you access to the **chunked_list** data structure, which has all the same
methods of the **ChunkedList**, but written in snake case.

### Standard Chunked List Examples

````cpp
#include "StandardChunkedList.hpp"

#include <random>

int main() {
  chunked_list<int, 32> chunkedList{};
  
  std::mt19937 gen{std::random_device{}};
  std::uniform_int_distrubition<int> dist{1, 100};
  
  for (int i = 0; i < 1000; ++i) {
    int num = dist(gen);
    chunkedList.push_back(num);
  }
  
  chunkedList.sort<true>();
  
  std::cout << chunkedList << std::endl;
  
  return 0;
}
````
