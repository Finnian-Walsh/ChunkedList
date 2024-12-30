#pragma once

/*

  _____ _                 _
 |_   _| |               | |
   | | | |_ ___ _ __ __ _| |_ ___  _ __
   | | | __/ _ \ '__/ _` | __/ _ \| '__|
  _| |_| ||  __/ | | (_| | || (_) | |
 |_____|\__\___|_|  \__,_|\__\___/|_|

*/

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::Iterator(Chunk *chunk) : chunkIterator(chunk) {
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::Iterator(Chunk *chunk, const size_t index) : chunkIterator(chunk),
  index(index) {
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::Iterator(Chunk &chunk) : chunkIterator(&chunk) {
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::Iterator(Chunk &chunk, const size_t index) : chunkIterator(&chunk),
  index(index) {
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::Iterator(ChunkIterator chunkIterator) : chunkIterator{chunkIterator} {
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize,
  ShouldCopy>::Iterator::Iterator(ChunkIterator chunkIterator, const size_t index) : chunkIterator{
  chunkIterator
}, index(index) {
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator++() {
  if (index < ChunkSize - 1) {
    ++index;
  } else if (chunkIterator->nextChunk) {
      ++chunkIterator;
      index = 0;
  } else {
    index = ChunkSize;
  }

  return *this;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator
++(int) {
  Iterator original = *this;
  operator++();
  return original;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator--() {
  if (index == 0) {
    index = ChunkSize - 1;
    --chunkIterator;
  } else {
    --index;
  }

  return *this;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator
--(int) {
  Iterator original = *this;
  operator--();
  return original;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
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
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator-(size_t offset) {
  ChunkIterator newChunkIt = chunkIterator;

  while (offset > ChunkSize) {
    --newChunkIt;
    offset -= ChunkSize;
  }

  if (offset > index) {
    --newChunkIt;
    size_t newIndex = ChunkSize - offset + index;
    return Iterator{newChunkIt, newIndex};
  } else {
    size_t newIndex = index - offset;
    return Iterator{newChunkIt, newIndex};
  }
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
ChunkedList<T, ChunkSize, ShouldCopy>::Iterator::operator+=(size_t offset) {
  return *this = operator+(offset);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Iterator
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
  return reinterpret_cast<const Iterator *>(&other)->chunkIterator != chunkIterator || reinterpret_cast<const Iterator
           *>(&other)->index != index;
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
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::ConstIterator(const Chunk *chunk)
  : Iterator{const_cast<Chunk *>(chunk)} {
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::ConstIterator(const Chunk *chunk, const size_t index)
  : Iterator{const_cast<Chunk *>(chunk), index} {
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::ConstIterator(const Chunk &chunk)
  : Iterator{const_cast<Chunk &>(chunk)} {
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::ConstIterator(const Chunk &chunk, const size_t index)
  : Iterator{const_cast<Chunk &>(chunk), index} {
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::ConstIterator(const ChunkIterator chunkIterator)
  : Iterator{chunkIterator} {
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::ConstIterator(ChunkIterator chunkIterator, const size_t index)
  : Iterator{chunkIterator, index} {
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator+(size_t offset) const {
  return Iterator::operator+(offset);
}


template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator-(size_t offset) const {
  return Iterator::operator-(offset);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const T &ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator*() const {
  return Iterator::operator*();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const T *ChunkedList<T, ChunkSize, ShouldCopy>::ConstIterator::operator->() const {
  return Iterator::operator->();
}


/*

    _____ _                 _    _____ _                 _
  / ____| |               | |  |_   _| |               | |
 | |    | |__  _   _ _ __ | | __ | | | |_ ___ _ __ __ _| |_ ___  _ __
 | |    | '_ \| | | | '_ \| |/ / | | | __/ _ \ '__/ _` | __/ _ \| '__|
 | |____| | | | |_| | | | |   < _| |_| ||  __/ | | (_| | || (_) | |
  \_____|_| |_|\__,_|_| |_|_|\_\_____|\__\___|_|  \__,_|\__\___/|_|

*/

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::ChunkIterator(Chunk *chunk) : chunk(chunk) {
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator++() {
  chunk = chunk->nextChunk;
  return *this;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator++(int) {
  Iterator original = *this;
  chunk = chunk->nextChunk;
  return original;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator--() {
  chunk = chunk->prevChunk;
  return *this;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator--(int) {
  Iterator original = *this;
  chunk = chunk->prevChunk;
  return original;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator+(size_t offset) const {
  Chunk *ptr{chunk};

  for (; offset > 0; --offset)
    ptr = ptr->nextChunk;

  return ChunkIterator{ptr};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator-(size_t offset) const {
  Chunk *ptr{chunk};

  for (; offset > 0; --offset)
    ptr = ptr->prevChunk;

  return ChunkIterator{ptr};
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator+=(const size_t offset) {
  return *this = operator+(offset);
}


template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator
ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator-=(const size_t offset) {
  return *this = operator-(offset);
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Chunk &ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator
*() {
  return *chunk;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
typename ChunkedList<T, ChunkSize, ShouldCopy>::Chunk *ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator
->() {
  return chunk;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const typename ChunkedList<T, ChunkSize, ShouldCopy>::Chunk &ChunkedList<T, ChunkSize,
  ShouldCopy>::ChunkIterator::operator*() const {
  return *chunk;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const typename ChunkedList<T, ChunkSize, ShouldCopy>::Chunk *ChunkedList<T, ChunkSize,
  ShouldCopy>::ChunkIterator::operator->() const {
  return chunk;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator==(const ChunkIterator other) const {
  return other.chunk == chunk;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator==(const ConstChunkIterator other) const {
  return other.chunk == chunk;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator!=(const ChunkIterator other) const {
  return other.chunk != chunk;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
bool ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator!=(const ConstChunkIterator other) const {
  return other.chunk != chunk;
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
T &ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator[](const size_t index) {
  return (*chunk)[index];
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const T &ChunkedList<T, ChunkSize, ShouldCopy>::ChunkIterator::operator[](const size_t index) const {
  return (*chunk)[index];
}

/*

    _____                _    _____ _                 _    _____ _                 _
  / ____|              | |  / ____| |               | |  |_   _| |               | |
 | |     ___  _ __  ___| |_| |    | |__  _   _ _ __ | | __ | | | |_ ___ _ __ __ _| |_ ___  _ __
 | |    / _ \| '_ \/ __| __| |    | '_ \| | | | '_ \| |/ / | | | __/ _ \ '__/ _` | __/ _ \| '__|
 | |___| (_) | | | \__ \ |_| |____| | | | |_| | | | |   < _| |_| ||  __/ | | (_| | || (_) | |
  \_____\___/|_| |_|___/\__|\_____|_| |_|\__,_|_| |_|_|\_\_____|\__\___|_|  \__,_|\__\___/|_|

*/

template<typename T, size_t ChunkSize, bool ShouldCopy>
ChunkedList<T, ChunkSize, ShouldCopy>::ConstChunkIterator::ConstChunkIterator(Chunk *chunk) : ChunkIterator(chunk) {
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const typename ChunkedList<T, ChunkSize, ShouldCopy>::Chunk &ChunkedList<T, ChunkSize,
  ShouldCopy>::ConstChunkIterator::operator*() const {
  return ChunkIterator::operator*();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const typename ChunkedList<T, ChunkSize, ShouldCopy>::Chunk *ChunkedList<T, ChunkSize,
  ShouldCopy>::ConstChunkIterator::operator->() const {
  return ChunkIterator::operator->();
}

template<typename T, size_t ChunkSize, bool ShouldCopy>
const T &ChunkedList<T, ChunkSize, ShouldCopy>::ConstChunkIterator::operator[](size_t index) const {
  return ChunkIterator::operator[](index);
}
