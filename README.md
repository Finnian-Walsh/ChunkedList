# Chunked List

A basic C++ list data structure, which uses **Chunks** to store data.

```cpp
template<typename T, size_t ChunkSize = 32, bool ShouldCopy = false>
class ChunkedList
```

Each **Chunk** contains a fixed array, of size ChunkSize, and the **Chunk** class has 3 constructors. However,
**Chunks** are abstracted away and hidden from the user's view.

Furthermore, each **Chunk** keep track of its next index, incrementing and decrementing it by 1 each push and pop respectively.
Therefore, calling the pop method on a **ChunkedList** doesn't deallocate anything unless the next index of the front **Chunk** is equal to 0, causing the entire Chunk and its contents to be deallocated.

# How to use:

- Download the libChunkedList.a file
- Create a new directory in your project, where you store your libraries, named ChunkedList
- Add another directory inside the ChunkedList directory named include, as well as lib
- Move the libChunkedList.a file inside the lib directory
- Paste the ChunkedList.hpp source code into a new header file in your project, stored within the include directory
- Include the new header file you created