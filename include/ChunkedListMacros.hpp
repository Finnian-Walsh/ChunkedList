#pragma once

#ifdef CHUNKED_LIST_DEBUGGING
#define DEBUG_LINE(flush) if constexpr (flush) std::cout << std::endl; else std::cout << '\n';
#define DEBUG_LOG(value) std::cout << value;
#define DEBUG_EXECUTE(source) source
#else
#define DEBUG_LINE(flush)
#define DEBUG_LOG(value)
#define DEBUG_EXECUTE(source)
#endif
