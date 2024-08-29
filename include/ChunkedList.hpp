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

        Chunk& operator+(size_t offset);

        Chunk& operator-(size_t offset);

        int nextIndex{0};
        
        Chunk *nextChunk{nullptr};
        Chunk *prevChunk{nullptr};
        
        T &operator[](int index);
        
        const T &operator[](int index) const;
    };

    Chunk *front{nullptr};
    Chunk *back{nullptr};
    
    void pushChunk(Chunk *chunk);
    
    using ValueType = decltype(ShouldCopy ? T : const T &>);

  public:
    ChunkedList();
    
    ChunkedList(const std::initializer_list<T> &initializerList);
    
    ~ChunkedList();
    
    
    class Iterator {
      public:
        ~Iterator();
        
        Iterator operator++();
        
        Iterator operator++(int);
        
        Iterator operator--();
        
        Iterator operator--(int);

        Iterator operator+(size_t offset);

        Iterator operator-(size_t offset);

        inline Iterator operator+=(size_t offset);
        
        inline Iterator operator-=(size_t offset);
        
        T &operator*();
        
        inline bool operator==(Iterator other);
        
        inline bool operator!=(Iterator other);
      private:
        struct IteratorDummy {
          Chunk* chunk{nullptr};
          int index{0};
        };

        Chunk *chunk{nullptr};
        int index{0};
    };
    
    class ChunkIterator {
      public:
        ~ChunkIterator();
        
        ChunkIterator operator++();
        
        ChunkIterator operator++(int);

        ChunkIterator operator--();

        ChunkIterator operator--(int);

        ChunkIterator operator+(size_t offset);

        ChunkIterator operator-(size_t offset);

        inline ChunkIterator operator+=(size_t offset);

        inline ChunkIterator operator-=(size_t offset);

        inline Chunk& operator*();

        inline bool operator==(ChunkIterator other);

        inline bool operator!=(ChunkIterator other);
      private:
        struct ChunkIteratorDummy {
          Chunk* chunk{nullptr};
        };

        Chunk* chunk{nullptr};
    };

    T &operator[](int index);
    
    const T &operator[](int index) const;
    
    Iterator begin();
    
    Iterator end();
    
    ChunkIterator beginChunk();
    
    ChunkIterator endChunk();

    void push(ValueType);
    
    void pop();
    
    void popChunk();
    
    [[nodiscard]] size_t length() const;
    
    [[nodiscard]] size_t size() const;
    
    bool operator==(const ChunkedList &other);
    
    bool operator!=(const ChunkedList &other);
    
    template<typename, size_t, bool>
    friend std::ostream &operator<<(std::ostream &os, ChunkedList &chunkedList);
    
    const char *concat(const char *delimiter);

    std::string concat(const std::string& delimiter);
};

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator begin(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList);

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator end(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList);