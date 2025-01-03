#pragma once

#include <type_traits>
#include <utility>

#ifdef CHUNKED_LIST_DEBUGGING
#define DEBUG_LINE(should_flush) if constexpr (should_flush) { std::cout << std::endl; std::cout.flush(); } else { std::cout << '\n'; }
#define DEBUG_LOG(value) std::cout << value;
#define DEBUG_EXECUTE(source) source
#else
#define DEBUG_LINE(flush)
#define DEBUG_LOG(value)
#define DEBUG_EXECUTE(source)
#endif
