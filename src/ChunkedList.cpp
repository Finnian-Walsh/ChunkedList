#include "ChunkedList.hpp"

#include <sstream>

#ifdef CHUNKED_LIST_DEBUGGING
#include <iostream>
#define DEBUG_LINE std::cout << std::endl;
#define DEBUG_LOG(value) std::cout << value;
#define DEBUG_EXECUTE(source) source
#else
#define DEBUG_LINE
#define DEBUG_LOG(value)
#define DEBUG_EXECUTE(source)
#endif

// ChunkedList::Chunk

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::Chunk(Chunk *nextChunk, Chunk *prevChunk) : nextChunk(nextChunk),
                                                                                          prevChunk(prevChunk) {
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::Chunk(const T *array, const int size, Chunk *nextChunk, Chunk *prevChunk)
: nextIndex(size), nextChunk(nextChunk), prevChunk(prevChunk) {
  DEBUG_LOG("range: " << range << std::endl)
  
  for (int index = 0; index < size; ++index) {
    DEBUG_LOG(pointerStart[index] << ' ')
    if constexpr (ShouldCopy)
      data[index] = array[index];
    else
      data[index] = std::move(array[index]);
  }
  
  DEBUG_LINE
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::Chunk(const std::initializer_list<T> &initializerList) : nextIndex(
initializerList.size()) {
  for (int index = 0; index < initializerList.size(); ++index) {
    DEBUG_LOG(initializerList.begin()[index] << ' ')
    if constexpr (ShouldCopy)
      data[index] = initializerList.begin()[index];
    else
      data[index] = std::move(initializerList.begin()[index]);
  }
  
  DEBUG_LINE
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::Chunk() = default;

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::~Chunk() = default;

template <typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Chunk &ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::operator+(size_t offset) {
  Chunk& chunk{*this};

  for (size_t i = 0; i < offset; ++i)
    chunk = chunk.nextChunk;

  return chunk;
}

template <typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Chunk &ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::operator-(size_t offset) {
  Chunk& chunk{*this};

  for (size_t i = 0; i < offset; ++i)
    chunk = chunk.nextChunk;

  return chunk;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
T &ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::operator[](int index) {
  return data[index];
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const T &ChunkedList<T, ChunkSize, ShouldCopy>::Chunk::operator[](int index) const {
  return data[index];
}

// ChunkedList::Iterator

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::~Iterator() = default;

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator++() {
  if (index == ChunkSize) {
    chunk = chunk->nextChunk;
    index = 0;
  } else {
    ++index;
  }
  return *this;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator++(int) {
  Iterator original = *this;
  if (index == ChunkSize) {
    index = 0;
    chunk = chunk->nextChunk;
  } else {
    ++index;
  }
  return original;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator--() {
  if (index == 0) {
    index = ChunkSize - 1;
    chunk = chunk->prevChunk;
  } else {
    --index;
  }
  return *this;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator--(int) {
  Iterator original = *this;
  if (index == 0) {
    index = ChunkSize - 1;
    chunk = chunk->prevChunk;
  } else {
    --index;
  }
  return original;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator+(size_t offset) {
  size_t chunkOffset = offset / ChunkSize;
  offset %= ChunkSize;

  size_t iteratorIndex = offset + index;

  if (indexOffset >= ChunkSize) {
    newIndex %= ChunkSize;
    ++chunkOffset;
  }

  return {&chunk->operator+(chunkOffset), iteratorIndex};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator-(size_t offset) {
  size_t chunkOffset = offset / ChunkSize;
  offset %= ChunkSize;

  size_t iteratorIndex = offset + index;

  if (indexOffset >= ChunkSize) {
    newIndex %= ChunkSize;
    ++chunkOffset;
  }

  return {&chunk->operator+(chunkOffset), iteratorIndex};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
inline ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator+=(size_t offset) {
  return *this = operator+(offset);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
inline ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator-=(size_t offset) {
  return *this = operator-(offset);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
T &ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator*() {
  return (*chunk)[index];
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
inline bool ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator==(Iterator other) {
  IteratorDummy& dummy = *reinterpret_cast<IteratorDummy*>(&other);
  return dummy.chunk == chunk && dummy.index == index;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
inline bool ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator!=(Iterator other) {
  IteratorDummy& dummy = *reinterpret_cast<IteratorDummy*>(&other);
  return dummy.chunk != chunk || dummy.index != index;
}

// ChunkIterator

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::~ChunkIterator() = default;

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator++() {
  chunk = chunk->nextChunk;
  return *this;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator++(int) {
  Iterator original = *this;
  chunk = chunk->nextChunk;
  return original;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator--() {
  chunk = chunk->prevChunk;
  return *this;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator--(int) {
  Iterator original = *this;
  chunk = chunk->prevChunk;
  return original;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator+(size_t offset) {
  Chunk& chunkRef{chunk};

  for (size_t i = 0; i < offset; ++i)
    chunkRef = chunkRef.nextChunk;

  return chunkRef;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator-(size_t offset) {
  Chunk& chunkRef{chunk};

  for (size_t i = 0; i < offset; ++i)
    chunkRef = chunkRef.prevChunk;

  return chunkRef;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
inline ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator+=(size_t offset) {
  return *this = operator+(offset);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
inline ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator-=(size_t offset) {
  return *this = operator-(offset);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
inline ChunkedList<T, ChunkSize, ShouldCopy>::Chunk &ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator*() {
  return *chunk;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
inline bool ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator==(ChunkIterator other) {
  return reinterpret_cast<ChunkIteratorDummy*>(&other)->chunk == chunk;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
inline bool ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator!=(ChunkIterator other) {
  return reinterpret_cast<Chunk*>(other) != chunk;
}

// ChunkedList

template<typename T, size_t ChunkSize, bool ShouldCopy>
void ChunkedList<T, ChunkSize, ShouldCopy>::pushChunk(Chunk *chunk) {
  back->nextChunk = chunk;
  chunk->prevChunk = back;
  back = chunk;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkedList() {
  front = back = new Chunk();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkedList(const std::initializer_list<T> &initializerList) {
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

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::~ChunkedList() {
  while (back)
    popChunk();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
T &ChunkedList<T, ChunkSize, ShouldCopy>::operator[](int index) {
  const int chunkIndex = index / ChunkSize;
  Chunk *chunk = front;
  
  for (int i = 0; i < chunkIndex; ++i)
    chunk = chunk->nextChunk;
  
  return chunk->operator[](index % ChunkSize);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const T &ChunkedList<T, ChunkSize, ShouldCopy>::operator[](int index) const {
  const int chunkIndex = index / ChunkSize;
  Chunk *chunk = front;
  
  for (int i = 0; i < chunkIndex; ++i)
    chunk = chunk->nextChunk;
  
  return (*chunk)[](index % ChunkSize);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator ChunkedList<T, ChunkSize, ShouldCopy>::begin() {
  return {back};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator ChunkedList<T, ChunkSize, ShouldCopy>::end() {
  return {front, front->nextIndex - 1};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator ChunkedList<T, ChunkSize, ShouldCopy>::beginChunk() {
  return {back};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator ChunkedList<T, ChunkSize, ShouldCopy>::endChunk() {
  return {front + 1};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
void ChunkedList<T, ChunkSize, ShouldCopy>::push(ValueType value) {
  if constexpr (ShouldCopy)
    if (back->nextIndex == ChunkSize) {
      auto *nextChunk = new Chunk{value};
      nextChunk->prevChunk = back;
      back->nextChunk = nextChunk;
      back = nextChunk;
    } else {
      (*back)[back->nextIndex] = value;
      ++back->nextIndex;
    }
  else if (back->nextIndex == ChunkSize) {
    auto *nextChunk = new Chunk{std::move(value)};
    nextChunk->prevChunk = back;
    back->nextChunk = nextChunk;
    back = nextChunk;
  } else {
    back[back->nextIndex] = std::move(value);
    ++back->nextIndex;
  }
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
void ChunkedList<T, ChunkSize, ShouldCopy>::pop() {
  if (back->nextIndex == 0)
    popChunk();
  else
    --back->nextIndex;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
void ChunkedList<T, ChunkSize, ShouldCopy>::popChunk() {
  Chunk *newBack = back->prevChunk;
  delete back;
  back = newBack;
}

#define RET_SIZE return (chunkCount * ChunkSize) + back->nextIndex - 1;

template<typename T, size_t ChunkSize, bool ShouldCopy>
size_t ChunkedList<T, ChunkSize, ShouldCopy>::length() const { RET_SIZE }

template<typename T, size_t ChunkSize, bool ShouldCopy>
size_t ChunkedList<T, ChunkSize, ShouldCopy>::size() const { RET_SIZE }

#undef RET_SIZE

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::operator==(const ChunkedList<T, ChunkSize, ShouldCopy> &other) {
  if (this == &other)
    return true;
  if (length() != other.length())
    return false;
  
  for (typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator thisIterator = begin(), otherIterator = other.begin();
       thisIterator != endChunk(); ++thisIterator, ++otherIterator)
    if (thisIterator != otherIterator)
      return false;
  
  return true;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::operator!=(const ChunkedList<T, ChunkSize, ShouldCopy> &other) {
  if (this == &other)
    return false;
  if (length() != other.length())
    return true;
  
  for (typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator thisIterator = begin(), otherIterator = other.begin();
       thisIterator != endChunk(); ++thisIterator, ++otherIterator)
    if (thisIterator != otherIterator)
      return true;
  
  return false;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
std::ostream &operator<<(std::ostream &os, ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
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

template<typename T, size_t ChunkSize, bool ShouldCopy>
const char* ChunkedList<T, ChunkSize, ShouldCopy>::concat(const char* delimiter) {
  std::ostringstream concatenation;
  
  Iterator it{begin()};

  for (; it != end() - 1; ++it)
    concatenation << v << delimiter;
  
  concatenation << *++it;

  return concatenation.view().cbegin();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
std::string ChunkedList<T, ChunkSize, ShouldCopy>::concat(const std::string& delimiter) {
  std::ostringstream concatenation;
  
  Iterator it{begin()};

  for (; it != end() - 1; ++it)
    concatenation << v << delimiter;

  concatenation << *++it;

  return concatenation.str();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator begin(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  return chunkedList.begin();
}


template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator end(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  return chunkedList.end();
}
