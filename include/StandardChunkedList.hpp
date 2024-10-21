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
    using Iterator = DerivedChunkedList::Iterator;
    
    using ConstIterator = DerivedChunkedList::ConstIterator;
    
    using ChunkIterator = DerivedChunkedList::ConstIterator;
    
    using DerivedChunkedList::ChunkedList;
    
    using DerivedChunkedList::operator[];
    
    using DerivedChunkedList::begin;
    
    ChunkIterator begin_chunk();
    
    using DerivedChunkedList::end;
    
    ChunkIterator end_chunk();
    
    void push_back(DerivedChunkedList::ValueType value);
    
    void pop_back();
    
    void pop_back_chunk();
    
    using DerivedChunkedList::sort;
    
    using DerivedChunkedList::size;
    
    [[nodiscard]] size_t length() const;
    
    using DerivedChunkedList::empty;
    
    using DerivedChunkedList::operator==;
    
    using DerivedChunkedList::operator!=;
    
    using DerivedChunkedList::concat;
    
    template<typename, size_t, bool>
    friend std::ostream &operator<<(std::ostream &os, ChunkedList &chunkedList);
};

#include "../src/StandardChunkedList.tpp"

#endif
