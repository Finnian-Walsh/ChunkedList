#pragma once

#include "ChunkedList.hpp"
#include "ChunkedListMacros.hpp"

/*

  _____ _                 _
 |_   _| |               | |
   | | | |_ ___ _ __ __ _| |_ ___  _ __
   | | | __/ _ \ '__/ _` | __/ _ \| '__|
  _| |_| ||  __/ | | (_| | || (_) | |
 |_____|\__\___|_|  \__,_|\__\___/|_|

 */

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::Iterator(Chunk *chunk) : chunkIterator(chunk) {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::Iterator(Chunk *chunk, size_t index) : chunkIterator(chunk),
                                                                                        index(index) {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::Iterator(Chunk &chunk) : chunkIterator(&chunk) {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::Iterator(Chunk &chunk, size_t index) : chunkIterator(&chunk),
                                                                                        index(index) {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::Iterator(ChunkIterator chunkIterator) : chunkIterator(
*chunkIterator) {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::Iterator(ChunkIterator chunkIterator, size_t index) : chunkIterator(
*chunkIterator), index(index) {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator++() {
  if (index == (ChunkSize - 1)) {
    index = 0;
    ++chunkIterator;
  } else {
    ++index;
  }
  
  return *this;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator++(int) {
  Iterator original = *this;
  
  if (index == (ChunkSize - 1)) {
    index = 0;
    ++chunkIterator;
  } else {
    ++index;
  }
  
  return original;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator--() {
  if (index == 0) {
    index = ChunkSize - 1;
    --chunkIterator;
  } else {
    --index;
  }
  
  return *this;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator--(int) {
  Iterator original = *this;
  
  if (index == 0) {
    index = ChunkSize - 1;
    --chunkIterator;
  } else {
    --index;
  }
  
  return original;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator+(size_t offset) {
  size_t chunkOffset = offset / ChunkSize;
  offset %= ChunkSize;
  
  size_t iteratorIndex = offset + index;
  
  if (iteratorIndex >= ChunkSize) {
    iteratorIndex %= ChunkSize;
    ++chunkOffset;
  }
  
  return Iterator{&(*chunkIterator + chunkOffset), iteratorIndex};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator-(size_t offset) {
  size_t chunkOffset = offset / ChunkSize;
  offset %= ChunkSize;
  
  size_t iteratorIndex = offset - index;
  
  if (iteratorIndex >= ChunkSize) {
    iteratorIndex %= ChunkSize;
    ++chunkOffset;
  }
  
  return Iterator{&(*chunkIterator - chunkOffset), iteratorIndex};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator+=(size_t offset) {
  return *this = operator+(offset);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator-=(size_t offset) {
  return *this = operator-(offset);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
T &ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator*() {
  return chunkIterator[index];
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
T *ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator->() {
  return &chunkIterator[index];
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const T &ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator*() const {
  return chunkIterator[index];
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const T *ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator->() const {
  return &chunkIterator[index];
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator==(const Iterator other) const {
  return other.chunkIterator == chunkIterator && other.index == index;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator==(const ConstIterator other) const {
  return other.chunkIterator == chunkIterator && other.index == index;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator!=(const Iterator other) const {
  return other.chunkIterator != chunkIterator || other.index != index;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator!=(const ConstIterator other) const {
  return other.chunkIterator != chunkIterator || other.index != index;
}

/*

   _____                _   _____ _                 _
  / ____|              | | |_   _| |               | |
 | |     ___  _ __  ___| |_  | | | |_ ___ _ __ __ _| |_ ___  _ __
 | |    / _ \| '_ \/ __| __| | | | __/ _ \ '__/ _` | __/ _ \| '__|
 | |___| (_) | | | \__ \ |_ _| |_| ||  __/ | | (_| | || (_) | |
  \_____\___/|_| |_|___/\__|_____|\__\___|_|  \__,_|\__\___/|_|

 */

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::ConstIterator(Chunk *chunk)
: chunkIterator{chunk} {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::ConstIterator(Chunk *chunk, size_t index)
: chunkIterator{chunk}, index{index} {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::ConstIterator(Chunk &chunk)
: chunkIterator{chunk} {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::ConstIterator(Chunk &chunk, size_t index)
: chunkIterator{chunk}, index{index} {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::ConstIterator(ChunkIterator chunkIterator)
: chunkIterator{chunkIterator} {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::ConstIterator(ChunkIterator chunkIterator, size_t index)
: chunkIterator{chunkIterator}, index{index} {}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator++() {
  if (index == (ChunkSize - 1)) {
    index = 0;
    ++chunkIterator;
  } else {
    ++index;
  }
  
  return *this;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator++(int) {
  ConstIterator original = *this;
  
  if (index == (ChunkSize - 1)) {
    index = 0;
    ++chunkIterator;
  } else {
    ++index;
  }
  
  return original;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator--() {
  if (index == 0) {
    index = ChunkSize - 1;
    --chunkIterator;
  } else {
    --index;
  }
  
  return *this;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator--(int) {
  ConstIterator original = *this;
  
  if (index == 0) {
    index = ChunkSize - 1;
    --chunkIterator;
  } else {
    --index;
  }
  
  return original;
}


template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator+(size_t offset) const {
  size_t chunkOffset = offset / ChunkSize;
  offset %= ChunkSize;
  
  size_t iteratorIndex = offset + index;
  
  if (iteratorIndex >= ChunkSize) {
    iteratorIndex %= ChunkSize;
    ++chunkOffset;
  }
  
  return ConstIterator{&(*chunkIterator + chunkOffset), iteratorIndex};
}


template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator-(size_t offset) const {
  size_t chunkOffset = offset / ChunkSize;
  offset %= ChunkSize;
  
  size_t iteratorIndex = offset - index;
  
  if (iteratorIndex >= ChunkSize) {
    iteratorIndex %= ChunkSize;
    ++chunkOffset;
  }
  
  return ConstIterator{&(*chunkIterator - chunkOffset), iteratorIndex};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator+=(size_t offset) {
  return *this = operator+(offset);
}


template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator-=(size_t offset) {
  return *this = operator-(offset);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const T &ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator*() const {
  return chunkIterator[index];
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const T *ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator->() const {
  return &chunkIterator[index];
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator==(const ConstIterator other) const {
  return other.chunkIterator == chunkIterator && other.index == index;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator==(const Iterator other) const {
  return other.chunkIterator == chunkIterator && other.index == index;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator!=(const ConstIterator other) const {
  return other.chunkIterator != chunkIterator || other.index != index;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator!=(const Iterator other) const {
  return other.chunkIterator != chunkIterator || other.index != index;
}

