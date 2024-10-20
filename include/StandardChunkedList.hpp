#ifndef STANDARD_CHUNKED_LIST
#define STANDARD_CHUNKED_LIST

#define IMPLEMENT_CHUNKED_LIST_PRIVATELY

#include "ChunkedList.hpp"

#undef IMPLEMENT_CHUNKED_LIST_PRIVATELY

template<typename T, size_t ChunkSize = 32, bool ShouldCopy = true>
class StandardChunkedList final
: private ChunkedListUtility::ChunkedListImplementation::ChunkedList<T, ChunkSize, ShouldCopy> {
  private:
    using DerivedChunkedList = ChunkedListUtility::ChunkedListImplementation::ChunkedList<T, ChunkSize, ShouldCopy>;
  
  public:
    using DerivedChunkedList::ChunkedList;
    
    using DerivedChunkedList::operator[];
    
    using DerivedChunkedList::begin;
    
    using begin_chunk = DerivedChunkedList::beginChunk;
    
    using DerivedChunkedList::end;
    
    using end_chunk = DerivedChunkedList::endChunk;
    
    using push_back = DerivedChunkedList::push;
    
    using pop_back = DerivedChunkedList::pop;
    
    using pop_back_chunk = DerivedChunkedList::popChunk;
    
    using DerivedChunkedList::sort;
    
    using DerivedChunkedList::size;
    
    using length = DerivedChunkedList::size;
    
    using DerivedChunkedList::empty;
    
    using DerivedChunkedList::operator==;
    
    using DerivedChunkedList::operator!=;
    
    using DerivedChunkedList::operator<<;
    
    using DerivedChunkedList::concat;
    
    using Iterator = DerivedChunkedList::Iterator;
    
    using ConstIterator = DerivedChunkedList::ConstIterator;
    
    using ChunkIterator = DerivedChunkedList::ConstIterator;
};

#include "../src/StandardChunkedList.tpp"

#endif
