#pragma once

#include <ostream>
#include <initializer_list>
#include <cstring>

#ifdef CHUNKED_LIST_DEBUGGING
#include <iostream>
#define DEBUG_LINE(flush) if constexpr (flush) std::cout << std::endl; else std::cout << '\n';
#define DEBUG_LOG(value) std::cout << value;
#define DEBUG_EXECUTE(source) source
#else
#define DEBUG_LINE(flush)
#define DEBUG_LOG(value)
#define DEBUG_EXECUTE(source)
#endif


template<typename T, size_t ChunkSize = 32, bool ShouldCopy = true>
class ChunkedList {
    static_assert(ChunkSize > 0, "Chunk Size must be greater than 0");
    
    size_t chunkCount{1};
    
    class Chunk {
        T data[ChunkSize];
      
      public:
        Chunk(Chunk *nextChunk, Chunk *prevChunk) : nextChunk(nextChunk), prevChunk(prevChunk) {}
        
        Chunk(const T *array, int size, Chunk *nextChunk = nullptr, Chunk *prevChunk = nullptr) : nextIndex(size),
                                                                                                  nextChunk(nextChunk),
                                                                                                  prevChunk(prevChunk) {
          DEBUG_LOG("range: " << size << std::endl)
          
          for (int index = 0; index < size; ++index) {
            DEBUG_LOG(array[index] << ' ')
            if constexpr (ShouldCopy)
              data[index] = array[index];
            else
              data[index] = std::move(array[index]);
          }
          
          DEBUG_LINE(true)
        }
        
        Chunk(const std::initializer_list<T> &initializerList) : nextIndex(initializerList.size()) {
          for (int index = 0; index < initializerList.size(); ++index) {
            DEBUG_LOG(initializerList.begin()[index] << ' ')
            if constexpr (ShouldCopy)
              data[index] = initializerList.begin()[index];
            else
              data[index] = std::move(initializerList.begin()[index]);
          }
          
          DEBUG_LINE(true)
        }
        
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
        
        int nextIndex{0};
        
        Chunk *nextChunk{nullptr};
        Chunk *prevChunk{nullptr};
        
        T &operator[](size_t index) {
          return data[index];
        }
        
        const T &operator[](size_t index) const {
          return data[index];
        }
    };
    
    Chunk *front{nullptr};
    Chunk *back{nullptr};
    
    void pushChunk(Chunk *chunk) {
      back->nextChunk = chunk;
      chunk->prevChunk = back;
      back = chunk;
    }
    
    using ValueType = typename std::conditional<ShouldCopy, T, const T &>::type;
  public:
    ChunkedList() {
      front = back = new Chunk();
    }
    
    [[maybe_unused]] ChunkedList(const std::initializer_list<T> &initializerList) {
      if (initializerList.size() == 0) {
        front = back = new Chunk{};
        return;
      }
      
      DEBUG_LOG("Initializer list size: " << initializerList.size() << std::endl)
      
      DEBUG_EXECUTE({
                      for (int i = 0; i < initializerList.size(); ++i)
                        DEBUG_LOG(*(initializerList.begin() + i) << ' ');
                      DEBUG_LINE
                    })
      
      if (ChunkSize >= initializerList.size()) {
        front = back = new Chunk{initializerList.begin(), static_cast<int>(initializerList.size())};
        return;
      }
      
      const int initializerListSizeModChunkSize = initializerList.size() % ChunkSize;
      chunkCount =
      initializerListSizeModChunkSize == 0
      ? initializerList.size() / ChunkSize
      : initializerList.size() / ChunkSize + 1;
      front = back = new Chunk{initializerList.begin(), static_cast<int>(ChunkSize)};
      
      for (int index = 1; index < chunkCount - 1; ++index) {
        pushChunk(new Chunk{initializerList.begin() + index * ChunkSize, static_cast<int>(ChunkSize)});
      }
      
      pushChunk(new Chunk{
      initializerList.begin() + (chunkCount - 1) * ChunkSize,
      ChunkSize > initializerListSizeModChunkSize ? initializerListSizeModChunkSize : static_cast<int>(ChunkSize)
      });
    }
    
    ~ChunkedList() {
      while (back) popChunk();
    }
    
    class ChunkIterator {
      public:
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
            ptr = chunkRef->nextChunk;
          
          return {ptr};
        }
        
        ChunkIterator operator-(size_t offset) {
          Chunk *ptr{chunk};
          
          for (; offset > 0; --offset)
            ptr = ptr->prevChunk;
          
          return {ptr};
        }
        
        inline ChunkIterator operator+=(size_t offset) {
          return *this = operator+(offset);
        }
        
        inline ChunkIterator operator-=(size_t offset) {
          return *this = operator-(offset);
        }
        
        inline Chunk &operator*() {
          return *chunk;
        }
        
        inline bool operator==(ChunkIterator other) {
          return reinterpret_cast<Chunk *>(&other) == chunk;
        }
        
        inline bool operator!=(ChunkIterator other) {
          return reinterpret_cast<Chunk *>(&other) != chunk;
        }
      
      private:
        struct Dummy {
          Chunk *chunk{nullptr};
        };

        Chunk *chunk{nullptr};
    };
    
    class Iterator {
      public:
        ~Iterator() = default;
        
        Iterator operator++() {
          if (index == ChunkSize) {
            chunk = chunk->nextChunk;
            index = 0;
          } else {
            ++index;
          }
          return *this;
        }
        
        Iterator operator++(int) {
          Iterator original = *this;
          if (index == ChunkSize) {
            index = 0;
            chunk = chunk->nextChunk;
          } else {
            ++index;
          }
          return original;
        }
        
        Iterator operator--() {
          if (index == 0) {
            index = ChunkSize - 1;
            chunk = chunk->prevChunk;
          } else {
            --index;
          }
          return *this;
        }
        
        Iterator operator--(int) {
          Iterator original = *this;
          if (index == 0) {
            index = ChunkSize - 1;
            chunk = chunk->prevChunk;
          } else {
            --index;
          }
          return original;
        }
        
        Iterator operator+(size_t offset) {
          size_t chunkOffset = offset / ChunkSize;
          offset %= ChunkSize;
          
          size_t iteratorIndex = offset + index;
          
          if (iteratorIndex >= ChunkSize) {
            iteratorIndex %= ChunkSize;
            ++chunkOffset;
          }
          
          return {&chunk->operator+(chunkOffset), iteratorIndex};
        }
        
        Iterator operator-(size_t offset) {
          size_t chunkOffset = offset / ChunkSize;
          offset %= ChunkSize;
          
          size_t iteratorIndex = offset - index;
          
          if (iteratorIndex >= ChunkSize) {
            iteratorIndex %= ChunkSize;
            ++chunkOffset;
          }
          
          return {&chunk->operator-(chunkOffset), iteratorIndex};
        }
        
        inline Iterator operator+=(size_t offset) {
          return *this = operator+(offset);
        }
        
        inline Iterator operator-=(size_t offset) {
          return *this = operator-(offset);
        }
        
        T &operator*() {
          return (*chunk)[index];
        }
        
        inline bool operator==(Iterator other) {
          // return static_cast<Dummy *>(&other)->chunk == chunk && static_cast<Dummy *>(&other)->index == index;
          return other.chunk == chunk && other.index == index;
        }
        
        inline bool operator!=(Iterator other) {
          // return static_cast<Dummy *>(&other)->chunk != chunk || static_cast<Dummy *>(&other)->index != index;
          return other.chunk != chunk || other.index != index;
        }

        Iterator(Chunk *chunk) : chunkIterator{chunk} {}

        Iterator(Chunk *chunk, size_t index) : chunkIterator{chunk}, index{index} {}

        Iterator(Chunk &chunk) : chunkIterator{&chunk} {}

        Iterator(Chunk &chunk, size_t index) : chunkIterator(&chunk), index{index} {}

        Iterator(ChunkIterator chunkIterator) : chunkIterator{chunkIterator} {}

        Iterator(ChunkIterator chunkIterator, size_t index) : chunkIterator{chunkIterator}, index{index} {}
      private:
        ChunkIterator chunkIterator;
        size_t index{0};
    };

    T &operator[](size_t index) {
      const int chunkIndex = index / ChunkSize;
      Chunk *chunk = front;
      
      for (int i = 0; i < chunkIndex; ++i)
        chunk = chunk->nextChunk;
      
      return chunk->operator[](index % ChunkSize);
    }
    
    const T &operator[](size_t index) const {
      const int chunkIndex = index / ChunkSize;
      Chunk *chunk = front;
      
      for (int i = 0; i < chunkIndex; ++i)
        chunk = chunk->nextChunk;
      
      return (*chunk)(index % ChunkSize);
    }
    
    Iterator begin() {
      return {back, 0};
    }
    
    Iterator end() {
      return {front, front->nextIndex - 1};
    }
    
    ChunkIterator beginChunk() {
      return {back};
    }
    
    ChunkIterator endChunk() {
      return {front + 1};
    }
    
    void push(ValueType value) {
      if (back->nextIndex == ChunkSize) {
        if constexpr (ShouldCopy) {
          auto *nextChunk = new Chunk{std::move(value)};
          nextChunk->prevChunk = back;
          back->nextChunk = nextChunk;
          back = nextChunk;
        } else {
          auto *nextChunk = new Chunk{value};
          nextChunk->prevChunk = back;
          back->nextChunk = nextChunk;
          back = nextChunk;
        }
      } else {
        if constexpr (ShouldCopy)
          (*back)[back->nextIndex] = std::move(value);
        else
          (*back)[back->nextIndex] = value;
        ++back->nextIndex;
      }
    }
    
    [[maybe_unused]] void pop() {
      if (back->nextIndex == 0)
        popChunk();
      else
        --back->nextIndex;
    }
    
    void popChunk() {
      Chunk *newBack = back->prevChunk;
      delete back;
      back = newBack;
    }
    
    [[nodiscard]] size_t size() const {
      return (chunkCount * ChunkSize) + back->nextIndex - 1;
    }
    
    bool operator==(const ChunkedList &other) const {
      if (size() != other.size())
        return false;
      
      for (typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator thisIterator = begin(), otherIterator = other.begin();
           thisIterator != endChunk(); ++thisIterator, ++otherIterator)
        if (thisIterator != otherIterator)
          return false;
      
      return true;
    }
    
    bool operator!=(const ChunkedList &other) const {
      if (size() != other.size())
        return true;
      
      for (typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator thisIterator = begin(), otherIterator = other.begin();
           thisIterator != endChunk(); ++thisIterator, ++otherIterator)
        if (thisIterator != otherIterator)
          return true;
      
      return false;
    }
    
    template<typename, size_t, bool>
    friend std::ostream &operator<<(std::ostream &os, ChunkedList &chunkedList) {
      DEBUG_EXECUTE(os << "Chunked List: ";)
      os << '[';
      DEBUG_EXECUTE({ os << '\n'; })
      auto *chunk = chunkedList.front;
      
      while (chunk) {
        DEBUG_LOG("Next index: " << chunk->nextIndex << '\n')
        
        for (int i = 0; i < chunk->nextIndex; ++i)
          os << ' ' << (*chunk)[i] << ',';
        
        chunk = chunk->nextChunk;
        DEBUG_EXECUTE({ os << '\n'; })
      }
      
      os << " ]";
      
      return os;
    }
    
    const char *concat(const char *delimiter) {
      std::ostringstream concatenation;
      
      Iterator it{begin()};
      
      for (; it != end() - 1; ++it)
        concatenation << *it << delimiter;
      
      concatenation << *++it;
      
      return concatenation.str().c_str();
    }
    
    std::string concat(const std::string &delimiter) {
      std::ostringstream concatenation;
      
      Iterator it{begin()};
      
      for (; it != end() - 1; ++it)
        concatenation << *it << delimiter;
      
      concatenation << *++it;
      
      return concatenation.str();
    }
};

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator begin(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  return chunkedList.begin();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator end(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  return chunkedList.end();
}

#undef DEBUG_LOG
#undef DEBUG_LINE
#undef DEBUG_EXECUTE
