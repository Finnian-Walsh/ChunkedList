#pragma once

#include <sstream>
#include <initializer_list>
#include <cstring>
#include <functional>

#include "internal/ChunkedListUtility.hpp"

template<typename T, size_t ChunkSize = 32>
class ChunkedList {
  protected:
    static_assert(ChunkSize > 0, "Chunk Size must be greater than 0");

    size_t chunkCount{1};

    class Chunk {
      T data[ChunkSize]{};

      public:
        Chunk(Chunk *nextChunk, Chunk *prevChunk);

        Chunk(const T *array, size_t size, Chunk *nextChunk = nullptr, Chunk *prevChunk = nullptr);

        explicit Chunk(T value);

        Chunk() = default;

        ~Chunk() = default;

        /**
         * @brief returns the chunk x chunks ahead of the given chunk. Does not account for overflows
         */
        Chunk &operator+(size_t offset);

        /**
         * @brief returns the chunk x chunks behind the given chunk. Does not account for overflows
         */
        Chunk &operator-(size_t offset);

        size_t nextIndex{0};

        Chunk *nextChunk{nullptr};
        Chunk *prevChunk{nullptr};

        bool empty() const;

        T &operator[](size_t index);

        const T &operator[](size_t index) const;

        bool operator==(const Chunk &other) const;

        bool operator!=(const Chunk &other) const;

        void debugData(std::string &str) const;
    };

    Chunk *front{nullptr};
    Chunk *back{nullptr};

    /**
     * @brief simply pushes a chunk to the back, without mutating the chunkCount
     */
    void pushChunk(Chunk *chunk);

    template<typename ChunkT>
    class GenericChunkIterator {
      public:
        // stl compatibility

        using value_type = ChunkT;
        using difference_type = std::ptrdiff_t;
        using pointer = ChunkT *;
        using reference = ChunkT &;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit GenericChunkIterator(ChunkT *chunkPtr);

        explicit GenericChunkIterator(ChunkT &chunkRef);

        ~GenericChunkIterator() = default;

        GenericChunkIterator operator++();

        GenericChunkIterator operator++(int);

        GenericChunkIterator operator--();

        GenericChunkIterator operator--(int);

        GenericChunkIterator operator+(size_t offset) const;

        GenericChunkIterator operator-(size_t offset) const;

        GenericChunkIterator operator+=(size_t offset);

        GenericChunkIterator operator-=(size_t offset);

        template<typename ChunkIteratorT>
        bool operator==(ChunkIteratorT other) const;

        template<typename ChunkIteratorT>
        bool operator!=(ChunkIteratorT other) const;

        ChunkT &operator*();

        const ChunkT &operator*() const;

        ChunkT *operator->();

        const ChunkT *operator->() const;

      protected:
        ChunkT *chunk{nullptr};
    };

    template<typename ChunkT, typename ValueT>
    class GenericIterator {
      using ChunkIteratorT = GenericChunkIterator<ChunkT>;

      public:
        // stl compatibility
        using value_type = ValueT;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueT *;
        using reference = ValueT &;
        using iterator_category = std::bidirectional_iterator_tag;

        explicit GenericIterator(ChunkT *chunkPtr, size_t index = 0);

        explicit GenericIterator(ChunkT &chunkRef, size_t index = 0);

        explicit GenericIterator(ChunkIteratorT chunkIterator, size_t index = 0);

        ~GenericIterator() = default;

        GenericIterator operator++();

        GenericIterator operator++(int);

        GenericIterator operator--();

        GenericIterator operator--(int);

        GenericIterator operator+(size_t offset);

        GenericIterator operator-(size_t offset);

        GenericIterator operator+=(size_t offset);

        GenericIterator operator-=(size_t offset);

        template<typename IteratorT>
        bool operator==(IteratorT other) const;

        template<typename IteratorT>
        bool operator!=(IteratorT other) const;

        ValueT &operator*();

        const ValueT &operator*() const;

        ValueT *operator->();

        const ValueT *operator->() const;

        size_t getIndex() const;

        ChunkIteratorT getChunkIterator() const;

      protected:
        ChunkIteratorT chunkIterator{};
        size_t index{0};
    };

  public:
    ChunkedList();

    ChunkedList(std::initializer_list<T> initializerList);

    ~ChunkedList();

    using ChunkIterator = GenericChunkIterator<Chunk>;

    using ConstChunkIterator = GenericChunkIterator<const Chunk>;

    using Iterator = GenericIterator<Chunk, T>;

    using ConstIterator = GenericIterator<const Chunk, const T>;

    T &operator[](size_t index);

    const T &operator[](size_t index) const;

    Iterator begin();

    ConstIterator begin() const;

    Iterator end();

    ConstIterator end() const;

    ChunkIterator beginChunk();

    ConstChunkIterator beginChunk() const;

    ChunkIterator endChunk();

    ConstChunkIterator endChunk() const;

    void push(T value);

    template<typename... Args>
    void emplace(Args &&... args);

    void pop();

    void popChunk();

    template<typename Compare = std::less<T>, SortType Sort = QuickSort>
    void sort();

    size_t size() const;

    bool empty() const;

    /**
     * @brief returns true if every item in the other chunked list is the same as in the given chunked list
     */
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

template<typename T, size_t ChunkSize>
typename ChunkedList<T, ChunkSize>::ConstIterator
begin(const ChunkedList<T, ChunkSize> &chunkedList) noexcept;

template<typename T, size_t ChunkSize>
typename ChunkedList<T, ChunkSize>::Iterator
begin(ChunkedList<T, ChunkSize> &chunkedList) noexcept;

template<typename T, size_t ChunkSize>
typename ChunkedList<T, ChunkSize>::ConstIterator
end(const ChunkedList<T, ChunkSize> &chunkedList) noexcept;

template<typename T, size_t ChunkSize>
typename ChunkedList<T, ChunkSize>::Iterator
end(ChunkedList<T, ChunkSize> &chunkedList) noexcept;

#undef DEBUG_LOG
#undef DEBUG_LINE
#undef DEBUG_EXECUTE

#include "../src/ChunkedList.tpp"
#include "../src/ChunkedListChunk.tpp"
#include "../src/ChunkedListIterator.tpp"
#include "../src/ChunkedListUtility.tpp"
