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

template<typename T, size_t ChunkSize = 32, bool ShouldCopy = true>
class ChunkedList {
    static_assert(ChunkSize > 0, "Chunk Size must be greater than 0");
    
    size_t chunkCount{1};
    
    class Chunk {
        T data[ChunkSize]{};
      
      public:
        Chunk(Chunk *nextChunk, Chunk *prevChunk);
        
        Chunk(const T *array, size_t size, Chunk *nextChunk = nullptr, Chunk *prevChunk = nullptr);
        
        /**
         * @functionality does not check for overflows
         **/
        Chunk(const std::initializer_list<T> &initializerList);
        
        Chunk() = default;
        
        ~Chunk() = default;
        
        Chunk &operator+(size_t offset);
        
        Chunk &operator-(size_t offset);
        
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
    
    class ConstChunkIterator;
    
    class ChunkIterator {
      public:
        explicit ChunkIterator(Chunk *chunk);
        
        ~ChunkIterator() = default;
        
        ChunkIterator operator++();
        
        ChunkIterator operator++(int);
        
        ChunkIterator operator--();
        
        ChunkIterator operator--(int);
        
        ChunkIterator operator+(size_t offset) const;
        
        ChunkIterator operator-(size_t offset) const;
        
        ChunkIterator operator+=(size_t offset);
        
        ChunkIterator operator-=(size_t offset);
        
        Chunk &operator*();
        
        Chunk *operator->();
        
        virtual const Chunk &operator*() const;
        
        virtual const Chunk *operator->() const;
        
        bool operator==(ChunkIterator other) const;
        
        bool operator==(ConstChunkIterator other) const;
        
        bool operator!=(ChunkIterator other) const;
        
        bool operator!=(ConstChunkIterator other) const;
        
        T &operator[](size_t index);
        
        virtual const T &operator[](size_t index) const;
      
      protected:
        Chunk *chunk;
    };
    
    class ConstChunkIterator final : private ChunkIterator {
      public:
        explicit ConstChunkIterator(Chunk *chunk);
        
        ~ConstChunkIterator() = default;
        
        using ChunkIterator::operator++;
        
        using ChunkIterator::operator--;
        
        using ChunkIterator::operator+;
        
        using ChunkIterator::operator-;
        
        using ChunkIterator::operator+=;
        
        using ChunkIterator::operator-=;
        
        const Chunk &operator*() const override;
        
        const Chunk *operator->() const override;
        
        using ChunkIterator::operator==;
        
        using ChunkIterator::operator!=;
        
        const T &operator[](size_t index) const override;
        
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
        
        virtual const T &operator*() const;
        
        virtual const T *operator->() const;
        
        bool operator==(Iterator other) const;
        
        bool operator==(ConstIterator other) const;
        
        bool operator!=(Iterator other) const;
        
        bool operator!=(ConstIterator other) const;
      
      protected:
        ChunkIterator chunkIterator{};
        size_t index{0};
    };
    
    class ConstIterator : private Iterator {
      public:
        explicit ConstIterator(const Chunk *chunk);
        
        ConstIterator(const Chunk *chunk, size_t index);
        
        explicit ConstIterator(const Chunk &chunk);
        
        ConstIterator(const Chunk &chunk, size_t index);
        
        explicit ConstIterator(ChunkIterator chunkIterator);
        
        ConstIterator(ChunkIterator chunkIterator, size_t index);
        
        ~ConstIterator() = default;
        
        using Iterator::operator++;
        
        using Iterator::operator--;
        
        ConstIterator operator+(size_t offset) const;
        
        ConstIterator operator-(size_t offset) const;
        
        using Iterator::operator+=;
        
        using Iterator::operator-=;
        
        const T &operator*() const override;
        
        const T *operator->() const override;
        
        using Iterator::operator==;
        
        using Iterator::operator!=;
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

#endif