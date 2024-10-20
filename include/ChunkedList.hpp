#ifndef CHUNKED_LIST_HPP
#define CHUNKED_LIST_HPP

#include <sstream>
#include <initializer_list>
#include <cstring>
#include <algorithm>
#include <functional>
#include <queue> // for std::priority_queue
#include <vector> // for std::vector

#ifdef CHUNKED_LIST_DEBUGGING

#include <iostream> // for character output

#endif

#ifdef IMPLEMENT_CHUNKED_LIST_PRIVATELY

namespace ChunkedListUtility {
  namespace ChunkedListImplementation {

#endif

template<typename T, size_t ChunkSize = 32, bool ShouldCopy = true>
class ChunkedList {
    static_assert(ChunkSize > 0, "Chunk Size must be greater than 0");
    
    size_t chunkCount{1};
    
    class Chunk {
        T data[ChunkSize]{};
      
      public:
        Chunk(Chunk *nextChunk, Chunk *prevChunk) : nextChunk(nextChunk), prevChunk(prevChunk) {}
        
        Chunk(const T *array, int size, Chunk *nextChunk = nullptr, Chunk *prevChunk = nullptr);
        
        Chunk(const std::initializer_list<T> &initializerList);
        
        Chunk() = default;
        
        ~Chunk() = default;
        
        Chunk &operator+(size_t offset) {
          Chunk *chunk{this};
          
          for (size_t i{0}; i < offset; ++i)
            chunk = chunk->nextChunk;
          
          return *chunk;
        }
        
        Chunk &operator-(size_t offset) {
          Chunk *chunk{this};
          
          for (size_t i{0}; i < offset; ++i)
            chunk = chunk->nextChunk;
          
          return *chunk;
        }
        
        size_t nextIndex{0};
        
        Chunk *nextChunk{nullptr};
        Chunk *prevChunk{nullptr};
        
        T &operator[](size_t index);
        
        const T &operator[](size_t index) const;
        
        bool operator==(const Chunk &other) const;
        
        bool operator!=(const Chunk &other) const;
    };
    
    Chunk *front{nullptr};
    Chunk *back{nullptr};
    
    void pushChunk(Chunk *chunk);
  
  public:
    using ValueType = typename std::conditional_t<ShouldCopy, T, const T &>;
    
    ChunkedList();
    
    ChunkedList(const std::initializer_list<T> &initializerList);
    
    ~ChunkedList();
    
    class ChunkIterator {
      public:
        explicit ChunkIterator(Chunk *chunk) : chunk(chunk) {}
        
        ~ChunkIterator() = default;
        
        ChunkIterator operator++() {
          chunk = chunk->nextChunk;
          return *this;
        }
        
        ChunkIterator operator++(int) {
          Iterator original = *this;
          chunk = chunk->nextChunk;
          return original;
        }
        
        ChunkIterator operator--() {
          chunk = chunk->prevChunk;
          return *this;
        }
        
        ChunkIterator operator--(int) {
          Iterator original = *this;
          chunk = chunk->prevChunk;
          return original;
        }
        
        ChunkIterator operator+(size_t offset) {
          Chunk *ptr{chunk};
          
          for (; offset > 0; --offset)
            ptr = ptr->nextChunk;
          
          return ChunkIterator{ptr};
        }
        
        ChunkIterator operator-(size_t offset) {
          Chunk *ptr{chunk};
          
          for (; offset > 0; --offset)
            ptr = ptr->prevChunk;
          
          return ChunkIterator{ptr};
        }
        
        ChunkIterator operator+=(const size_t offset) {
          return *this = operator+(offset);
        }
        
        ChunkIterator operator-=(const size_t offset) {
          return *this = operator-(offset);
        }
        
        Chunk &operator*() {
          return *chunk;
        }
        
        const Chunk &operator*() const {
          return *chunk;
        }
        
        Chunk *operator->() {
          return chunk;
        }
        
        const Chunk *operator->() const {
          return chunk;
        }
        
        bool operator==(const ChunkIterator other) const {
          return other.chunk == chunk;
        }
        
        bool operator!=(const ChunkIterator other) const {
          return other.chunk != chunk;
        }
        
        T &operator[](const size_t index) {
          return (*chunk)[index];
        }
        
        const T &operator[](const size_t index) const {
          return (*chunk)[index];
        }
        
        bool operator==(const ChunkIterator other) {
          return chunk == other.chunk;
        }
        
        bool operator!=(const ChunkIterator other) {
          return chunk == other.chunk;
        }
      
      private:
        Chunk *chunk;
    };
    
    class ConstIterator;
    
    class Iterator {
      public:
        explicit Iterator(Chunk *chunk);
        
        Iterator(Chunk *chunk, size_t index);
        
        explicit Iterator(Chunk &chunk);
        
        Iterator(Chunk &chunk, size_t index);
        
        explicit Iterator(ChunkIterator chunkIterator);
        
        Iterator(ChunkIterator chunkIterator, size_t index);
        
        ~Iterator() = default;
        
        Iterator operator++();
        
        Iterator operator++(int);
        
        Iterator operator--();
        
        Iterator operator--(int);
        
        Iterator operator+(size_t offset);
        
        Iterator operator-(size_t offset);
        
        Iterator operator+=(size_t offset);
        
        Iterator operator-=(size_t offset);
        
        T &operator*();
        
        T *operator->();
        
        const T &operator*() const;
        
        const T *operator->() const;
        
        bool operator==(Iterator other) const;
        
        bool operator==(ConstIterator other) const;
        
        bool operator!=(Iterator other) const;
        
        bool operator!=(ConstIterator other) const;
      
      private:
        ChunkIterator chunkIterator;
        size_t index{0};
    };
    
    class ConstIterator {
      public:
        explicit ConstIterator(Chunk *chunk);
        
        ConstIterator(Chunk *chunk, size_t index);
        
        explicit ConstIterator(Chunk &chunk);
        
        ConstIterator(Chunk &chunk, size_t index);
        
        explicit ConstIterator(ChunkIterator chunkIterator);
        
        ConstIterator(ChunkIterator chunkIterator, size_t index);
        
        ~ConstIterator() = default;
        
        ConstIterator operator++();
        
        ConstIterator operator++(int);
        
        ConstIterator operator--();
        
        ConstIterator operator--(int);
        
        ConstIterator operator+(size_t offset) const;
        
        ConstIterator operator-(size_t offset) const;
        
        ConstIterator operator+=(size_t offset);
        
        ConstIterator operator-=(size_t offset);
        
        const T &operator*() const;
        
        const T *operator->() const;
        
        bool operator==(ConstIterator other) const;
        
        bool operator==(Iterator other) const;
        
        bool operator!=(ConstIterator other) const;
        
        bool operator!=(Iterator other) const;
      
      private:
        ChunkIterator chunkIterator;
        size_t index{0};
    };
    
    T &operator[](size_t index);
    
    const T &operator[](size_t index) const;
    
    Iterator begin();
    
    [[nodiscard]] ConstIterator begin() const;
    
    Iterator end();
    
    [[nodiscard]] ConstIterator end() const;
    
    ChunkIterator beginChunk();
    
    ChunkIterator endChunk();
    
    void push(ValueType value);
    
    void pop();
    
    void popChunk();
    
    template<bool AscendingOrder = true>
    void sort();
    
    void sort(bool ascendingOrder);
    
    [[nodiscard]] size_t size() const;
    
    [[nodiscard]] bool empty() const;
    
    bool operator==(const ChunkedList &other) const;
    
    bool operator!=(const ChunkedList &other) const;
    
    template<typename, size_t, bool>
    friend std::ostream &operator<<(std::ostream &os, ChunkedList &chunkedList);
    
    template<
    typename OutputStream = std::ostringstream,
    typename StringType = std::string,
    typename SeparatorType = std::string,
    StringType(*ConversionCall)(OutputStream &) = nullptr
    >
    StringType concat(SeparatorType delimiter = ", ");
};

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator
begin(const ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) noexcept;

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator
end(const ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) noexcept;

#undef DEBUG_LOG
#undef DEBUG_LINE
#undef DEBUG_EXECUTE

#include "../src/ChunkedList.tpp"
#include "../src/ChunkedListChunk.tpp"
#include "../src/ChunkedListIterator.tpp"

#ifdef IMPLEMENT_CHUNKED_LIST_PRIVATELY
}}
#endif

#endif