#pragma once

#include "ChunkedList.hpp"
#include "DebuggingMacros.hpp"

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
template<typename StringT, typename DelimiterT>
StringT ChunkedList<T, ChunkSize, ShouldCopy>::concat(DelimiterT delimiter) {
  std::ostringstream concatenation;
  
  Iterator it{begin()};
  
  for (; it != end() - 1; ++it)
    concatenation << *it << delimiter;
  
  concatenation << *++it;
  
  if constexpr (std::is_same_v<StringT, std::string>) {
    return concatenation.str();
  } else {
    return StringT{concatenation};
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
