# Chunked List

A basic C++ list data structure, which uses **Chunks** to store data.

```cpp
template<typename T, size_t ChunkSize = 32, bool ShouldCopy = false>
class ChunkedList
```


# How to use:

- Download the libChunkedList.a file
- Create a new directory in your project, where you store your libraries, named ChunkedList
- Add another directory inside the ChunkedList directory named include, as well as lib
- Move the libChunkedList.a file inside the lib directory
- Paste the ChunkedList.hpp source code into a new header file in your project, stored within the include directory
- Include the new header file you created