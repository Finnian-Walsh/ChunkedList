#pragma once

#include <ostream>
#include <initializer_list>
#include <cstring>

template<typename T, size_t ChunkSize = 32, bool ShouldCopy = true>
class ChunkedList {
    static_assert(ChunkSize > 0, "Chunk Size must be greater than 0");
    
    size_t chunkCount{1};
    
    class Chunk {
        T data[ChunkSize];
      
      public:
        Chunk(Chunk *nextChunk, Chunk *prevChunk);
        
        Chunk(const T *array, int size, Chunk *nextChunk = nullptr, Chunk *prevChunk = nullptr);
        
        Chunk(const std::initializer_list<T> &initializerList);
        
        Chunk();
        
        ~Chunk();
        
        int nextIndex{0};
        
        Chunk *nextChunk{nullptr};
        Chunk *prevChunk{nullptr};
        
        T &operator[](int index);
        
        const T &operator[](int index) const;
    };
    
    class Iterator {
      public:
        ~Iterator() = default;
        
        Iterator &operator++();
        
        Iterator operator++(int);
        
        Iterator &operator--();
        
        Iterator operator--(int);
        
        T &operator*();
        
        inline bool operator==(Iterator other);
        
        inline bool operator!=(Iterator other);
      
      private:
        Chunk *chunk{nullptr};
        int index{0};
    };
    
    using ChunkIterator = const Chunk *;
    
    Chunk *front{nullptr};
    Chunk *back{nullptr};
    
    void pushChunk(Chunk *chunk);
  
  public:
    ChunkedList();
    
    ChunkedList(const std::initializer_list<T> &initializerList);
    
    ~ChunkedList();
    
    T &operator[](int index);
    
    const T &operator[](int index) const;
    
    Iterator begin();
    
    Iterator end();
    
    ChunkIterator beginChunk();
    
    ChunkIterator endChunk();
    
    void push(typename std::conditional<ShouldCopy, T, const T &>);
    
    void pop();
    
    void popChunk();
    
    [[nodiscard]] size_t length() const;
    
    [[nodiscard]] size_t size() const;
    
    bool operator==(const ChunkedList &other);
    
    bool operator!=(const ChunkedList &other);
    
    template<typename, size_t, bool>
    friend std::ostream &operator<<(std::ostream &os, ChunkedList &chunkedList);
};

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::iterator begin(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList);

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::iterator end(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList);