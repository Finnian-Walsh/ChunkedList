#ifndef CHUNKED_LIST_HPP
#define CHUNKED_LIST_HPP

#include <sstream>
#include <initializer_list>
#include <cstring>
#include <algorithm>
#include <functional>
#include <queue>
#include <vector>

#ifdef CHUNKED_LIST_DEBUGGING
#include <iostream>
#endif

template<typename T, size_t ChunkSize = 32, bool ShouldCopy = true>
class ChunkedList {
    static_assert(ChunkSize > 0, "Chunk Size must be greater than 0");
    
    size_t chunkCount{1};
    
    class Chunk {
        T data[ChunkSize];
      
      public:
        Chunk(Chunk *nextChunk, Chunk *prevChunk) : nextChunk(nextChunk), prevChunk(prevChunk) {}
        
        Chunk(const T *array, int size, Chunk *nextChunk = nullptr, Chunk *prevChunk = nullptr);
        
        Chunk(const std::initializer_list<T> &initializerList);
        
        Chunk();
        
        ~Chunk();
        
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
    
    [[maybe_unused]] [[maybe_unused]] ChunkedList(const std::initializer_list<T> &initializerList);
    
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
        
        ChunkIterator operator+=(size_t offset) {
          return *this = operator+(offset);
        }
        
        ChunkIterator operator-=(size_t offset) {
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
        
        bool operator==(ChunkIterator other) const {
          return other.chunk == chunk;
        }
        
        bool operator!=(ChunkIterator other) const {
          return other.chunk != chunk;
        }
        
        T &operator[](size_t index) {
          return (*chunk)[index];
        }
        
        const T &operator[](size_t index) const {
          return (*chunk)[index];
        }
        
        bool operator==(ChunkIterator other) {
          return chunk == other.chunk;
        }
      
        bool operator!=(ChunkIterator other) {
          return chunk == other.chunk;
        }
        
      private:
        Chunk *chunk;
    };
    
    class Iterator {
      public:
        explicit Iterator(Chunk *chunk);
        
        Iterator(Chunk *chunk, size_t index);
        
        explicit Iterator(Chunk &chunk);
        
        Iterator(Chunk &chunk, size_t index);
        
        explicit Iterator(ChunkIterator chunkIterator);
        
        Iterator(ChunkIterator chunkIterator, size_t index);
        
        ~Iterator();
        
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
        
        bool operator!=(Iterator other) const;
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
        
        ~ConstIterator();
        
        ConstIterator operator++() const;
        
        ConstIterator operator++(int) const;
        
        ConstIterator operator--() const;
        
        ConstIterator operator--(int) const;
        
        ConstIterator operator+(size_t offset) const;
        
        ConstIterator operator-(size_t offset) const;
        
        ConstIterator operator+=(size_t offset) const;
        
        ConstIterator operator-=(size_t offset) const {
          return *this = operator-(offset);
        }
        
        const T &operator*() const {
          return chunkIterator[index];
        }
        
        inline bool operator==(const Iterator other) const {
          return other.chunkIterator == chunkIterator && other.index == index;
        }
        
        inline bool operator!=(const Iterator other) const {
          return other.chunkIterator != chunkIterator || other.index != index;
        }
      private:
        ChunkIterator chunkIterator;
        size_t index{0};
    };
    
    [[nodiscard]] constexpr size_t chunk_size() const;
    
    T &operator[](size_t index);
    
    const T &operator[](size_t index) const;
    
    Iterator begin() {
      return Iterator{back, 0};
    }
    
    const Iterator begin() const {
      return Iterator{back, 0};
    }
    
    Iterator end() {
      return Iterator{front, front->nextIndex - 1};
    }
    
    const Iterator end() const {
      return Iterator{front, front->nextIndex - 1};
    }
    
    ChunkIterator beginChunk() {
      return ChunkIterator{back};
    }
    
    const ChunkIterator beginChunk() const {
      return ChunkIterator{back};
    }
    
    ChunkIterator endChunk();
    
    [[nodiscard]] const ChunkIterator endChunk() const;
    
    void push(ValueType value);
    
    void pop();
    
    void popChunk();
    
    [[nodiscard]] size_t size() const;
    
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
    StringType concat(SeparatorType separator);
};

template<typename T, size_t ChunkSize, bool ShouldCopy>
auto begin(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) noexcept -> decltype(chunkedList.begin());

template<typename T, size_t ChunkSize, bool ShouldCopy>
auto begin(const ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) noexcept -> decltype(chunkedList.begin());

template<typename T, size_t ChunkSize, bool ShouldCopy>
auto end(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) noexcept -> decltype(chunkedList.end());

template<typename T, size_t ChunkSize, bool ShouldCopy>
auto end(const ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) noexcept -> decltype(chunkedList.end());

#undef DEBUG_LOG
#undef DEBUG_LINE
#undef DEBUG_EXECUTE

#include "../src/ChunkedList.tpp"
#include "../src/ChunkedListChunk.tpp"
#include "../src/ChunkedListIterator.tpp"

#endif