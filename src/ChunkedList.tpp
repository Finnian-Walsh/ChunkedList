#pragma once

#include <iostream>

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
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkedList(const std::initializer_list<T> &initializerList) {
  if (initializerList.size() == 0) {
    front = back = new Chunk{};
    return;
  }

  DEBUG_LOG("Initializer list size = " << initializerList.size())

  DEBUG_EXECUTE({
    int index = 0;
    DEBUG_LOG(" (")

    for (; index < initializerList.size() - 1; ++index) {
    DEBUG_LOG(initializerList.begin()[index] << ", ")
    }

    DEBUG_LOG(initializerList.begin()[index] << ")")
    DEBUG_LINE(true)
    })

  if (ChunkSize >= initializerList.size()) {
    front = back = new Chunk{initializerList.begin(), initializerList.size()};
    return;
  }

  front = back = new Chunk{initializerList.begin(), ChunkSize};

  chunkCount = (initializerList.size() + ChunkSize - 1) / ChunkSize;

  for (int offset = 1; offset < chunkCount - 1; ++offset) {
    pushChunk(new Chunk{initializerList.begin() + (offset * ChunkSize), ChunkSize});
  }

  const int remainingItems = initializerList.size() % ChunkSize;
  pushChunk(
    new Chunk{
      initializerList.begin() + (chunkCount - 1) * ChunkSize, remainingItems == 0 ? ChunkSize : remainingItems
    });
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::~ChunkedList() {
  do {
    Chunk *newBack = back->prevChunk;
    delete back;
    back = newBack;
  } while (back);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
T &ChunkedList<T, ChunkSize, ShouldCopy>::operator[](const size_t index) {
  size_t chunkIndex = index / ChunkSize;

  Chunk *chunk = front;

  while (chunkIndex-- > 0) {
    chunk = chunk->nextChunk;
  }

  return (*chunk)[index % ChunkSize];
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const T &ChunkedList<T, ChunkSize, ShouldCopy>::operator[](const size_t index) const {
  return const_cast<ChunkedList>(*this).operator[](index);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator ChunkedList<T, ChunkSize, ShouldCopy>::begin() {
  return Iterator{front, 0};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator ChunkedList<T, ChunkSize, ShouldCopy>::begin() const {
  return ConstIterator{front, 0};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator ChunkedList<T, ChunkSize, ShouldCopy>::end() {
  return Iterator{back, back->nextIndex};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator ChunkedList<T, ChunkSize, ShouldCopy>::end() const {
  return ConstIterator{back, back->nextIndex};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator ChunkedList<T, ChunkSize, ShouldCopy>::beginChunk() {
  return ChunkIterator{front};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator ChunkedList<T, ChunkSize, ShouldCopy>::endChunk() {
  return ChunkIterator{nullptr};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
void ChunkedList<T, ChunkSize, ShouldCopy>::push(ValueType value) {
  if (back->nextIndex == ChunkSize) {
    if constexpr (ShouldCopy) {
      auto *nextChunk = new Chunk{value};
      ++chunkCount;

      nextChunk->prevChunk = back;
      back->nextChunk = nextChunk;
      back = nextChunk;
    } else {
      auto *nextChunk = new Chunk{std::move(value)};
      ++chunkCount;

      nextChunk->prevChunk = back;
      back->nextChunk = nextChunk;
      back = nextChunk;
    }
  } else {
    if constexpr (ShouldCopy)
      (*back)[back->nextIndex] = value;
    else
      (*back)[back->nextIndex] = std::move(value);
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
  if (Chunk *newBack = back->prevChunk; newBack) {
    delete back;
    back = newBack;
    --chunkCount;
    --back->nextIndex;
  }
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
template<typename Compare, SortType Sort>
void ChunkedList<T, ChunkSize, ShouldCopy>::sort() {
  DEBUG_LOG("Calling sort utility function using " << typeid(Compare).name() << " class to compare")
  ChunkedListUtility::sort<Compare, Sort>(*this);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
size_t ChunkedList<T, ChunkSize, ShouldCopy>::size() const {
  return (chunkCount - 1) * ChunkSize + back->nextIndex;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::empty() const {
  return back->nextIndex == 0;
}

/**
 * @brief returns true if every item in the other chunked list is the same as in the given chunked list
 */
template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::operator==(const ChunkedList &other) const {
  if (size() != other.size())
    return false;

  for (ConstIterator thisIterator = begin(), otherIterator = other.begin();
       thisIterator != end(); ++thisIterator, ++otherIterator)
    if (*thisIterator != *otherIterator)
      return false;

  return true;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::operator!=(const ChunkedList &other) const {
  if (size() != other.size())
    return true;

  for (ConstIterator thisIterator = begin(), otherIterator = other.begin();
       thisIterator != end(); ++thisIterator, ++otherIterator)
    if (*thisIterator != *otherIterator)
      return true;

  return false;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
std::ostream &operator<<(std::ostream &os, ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) {
  os << '[';

  auto iterator = chunkedList.begin(), lastIterator = chunkedList.end() - 1;

  for (; iterator != lastIterator; ++iterator) {
    os << *iterator << ", ";
  }

  os << *lastIterator;

  os << "]";

  return os;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
template<typename OutputStream, typename StringType, typename DelimiterType, StringType(*ConversionCall)(
  OutputStream &)>
StringType ChunkedList<T, ChunkSize, ShouldCopy>::concat(const DelimiterType delimiter) {
  static_assert(ChunkedListUtility::has_insertion_operator_v<OutputStream, StringType>,
                "OutputStream cannot handle StringType");
  static_assert(ChunkedListUtility::has_insertion_operator_v<OutputStream, StringType>,
                "OutputStream cannot handle SeparatorType");

  OutputStream stream;

  Iterator it = begin(), lastIt = end() - 1;

  if (it == lastIt) {
    DEBUG_LOG("Empty ChunkedList" << std::endl)
    return StringType{};
  }

  for (; it != lastIt; ++it) {
    stream << *it << delimiter;

    DEBUG_LOG(stream.str() << '\n');
  }

  stream << *lastIt;

  if constexpr (ConversionCall == nullptr) {
    return StringType{stream.str()};
  } else {
    return ConversionCall(stream);
  }
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator begin(
  ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) noexcept {
  return chunkedList.begin();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator begin(
  const ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) noexcept {
  return chunkedList.begin();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator end(
  ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) noexcept {
  return chunkedList.end();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator end(
  const ChunkedList<T, ChunkSize, ShouldCopy> &chunkedList) noexcept {
  return chunkedList.end();
}
