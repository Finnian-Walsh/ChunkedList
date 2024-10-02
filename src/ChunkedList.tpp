#pragma once

#include "ChunkedList.hpp"
#include "ChunkedListMacros.hpp"
#include "ChunkedListUtility.hpp"

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
[[maybe_unused]] ChunkedList<T, ChunkSize, ShouldCopy>::ChunkedList(const std::initializer_list<T> &initializerList) {
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
  while (back) popChunk();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
constexpr size_t ChunkedList<T, ChunkSize, ShouldCopy>::chunk_size() const {
  return ChunkSize;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
T &ChunkedList<T, ChunkSize, ShouldCopy>::operator[](size_t index) {
  const int chunkIndex = index / ChunkSize;
  Chunk *chunk = front;
  
  for (int i = 0; i < chunkIndex; ++i)
    chunk = chunk->nextChunk;
  
  return chunk->operator[](index % ChunkSize);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const T &ChunkedList<T, ChunkSize, ShouldCopy>::operator[](size_t index) const {
  return const_cast<ChunkedList>(*this).operator[](index);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator ChunkedList<T, ChunkSize, ShouldCopy>::endChunk() const {
  return ChunkIterator{front + 1};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator ChunkedList<T, ChunkSize, ShouldCopy>::endChunk() {
  return ChunkIterator{front + 1};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
void ChunkedList<T, ChunkSize, ShouldCopy>::push(ValueType value) {
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

template<typename T, size_t ChunkSize, bool ShouldCopy>
size_t ChunkedList<T, ChunkSize, ShouldCopy>::size() const {
  return (chunkCount * ChunkSize) + back->nextIndex - 1;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::operator==(const ChunkedList &other) const {
  if (size() != other.size())
    return false;
  
  for (typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator thisIterator = begin(), otherIterator = other.begin();
       thisIterator != end(); ++thisIterator, ++otherIterator)
    if (*thisIterator != *otherIterator)
      return false;
  
  return true;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::operator!=(const ChunkedList &other) const {
  if (size() != other.size())
    return true;
  
  for (typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator thisIterator = begin(), otherIterator = other.begin();
       thisIterator != end(); ++thisIterator, ++otherIterator)
    if (thisIterator != otherIterator)
      return true;
  
  return false;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
std::ostream &operator<<(std::ostream &os, ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  DEBUG_EXECUTE(os << "Chunked List: ";)
  os << '[';
  DEBUG_EXECUTE({ os << '\n'; })
  
  for (auto chunkIterator = chunkedList.beginChunk(); chunkIterator != chunkedList.endChunk(); ++chunkIterator) {
    DEBUG_LOG("Next index: " << chunk->nextIndex << '\n')
    
    for (int i = 0; i < chunkIterator->nextIndex; ++i)
      os << ' ' << (*chunkIterator)[i] << ',';
    
    DEBUG_EXECUTE({ os << '\n'; })
  }
  
  os << " ]";
  
  return os;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
template<typename OutputStream, typename StringType, typename SeparatorType, StringType(*ConversionCall)(
OutputStream &)>
StringType ChunkedList<T, ChunkSize, ShouldCopy>::concat(const SeparatorType separator) {
  static_assert(ChunkedListUtility::has_insertion_operator_v<OutputStream, StringType>,
                "OutputStream cannot handle StringType");
  static_assert(ChunkedListUtility::has_insertion_operator_v<OutputStream, StringType>,
                "OutputStream cannot handle SeparatorType");
  
  OutputStream stream;
  
  Iterator it = begin(), lastIt = end() - 1;
  
  if (it == lastIt) return StringType{};
  
  std::for_each(it, lastIt, [&stream, separator](ValueType v) mutable {
    stream << v << separator;
  });
  
  stream << *++lastIt;
  
  if constexpr (ConversionCall == nullptr) {
    return StringType{stream.str()};
  } else {
    return ConversionCall(stream);
  }
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
auto begin(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) noexcept -> decltype(chunkedList.begin()) {
  return chunkedList.begin();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
auto begin(const ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) noexcept -> decltype(chunkedList.begin()) {
  return chunkedList.begin();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
auto end(ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) noexcept -> decltype(chunkedList.end()) {
  return chunkedList.end();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
auto end(const ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) noexcept -> decltype(chunkedList.end()) {
  return chunkedList.end();
}
