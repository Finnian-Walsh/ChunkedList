# Chunked List

## Concept

A simple C++ list data structure, which uses **Chunks** to store data.

```cpp
template<typename T, size_t ChunkSize = 32, bool ShouldCopy = false>
class ChunkedList
```

The **ChunkedList** data structure is essentially a linked list, where each node is a **Chunk**, as explained below.

## Chunks

Each **Chunk** contains a fixed array, of size ChunkSize, and the **Chunk** class has 3 constructors, each populating
the array in a different way. However,
**Chunks** are abstracted away and from the user's view.

Furthermore, each **Chunk** keep track of its next index, incrementing and decrementing it by 1 each push and pop
respectively.
Therefore, calling the pop method on a **ChunkedList** doesn't deallocate anything unless the next index of the front
**Chunk** is equal to 0, causing the entire Chunk and its contents to be deallocated.

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

If you wish to enable debugging mode for **ChunkedList** development, you can #define CHUNKED_LIST_DEBUGGING in
src/ChunkedList.hpp if you are building the project manually, or download the debug libChunkedList.a file from the
debugging release.
Doing so will cause the **ChunkedList** operations to be outputted in the console.

## How to use

- Download the libChunkedList.a file.
- Create a new directory in your project, where you store your libraries, named ChunkedList.
- Add another directory inside the ChunkedList directory named include, as well as lib.
- Move the libChunkedList.a file inside the lib directory.
- Create a new header file in your include directory and paste the ChunkedList.hpp source code.
- Include the new header file you created in your code.

## Coding examples:

```cpp
#include "ChunkedList.hpp"

#include <iostream>

int main() {
  ChunkedList<int> list{1, 2, 3, 4, 5};
  
  for (int num : list) {
    std::cout << num << '\n';    
  }
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
word in the list together with " ", subsequently outputting the returned **const char***.