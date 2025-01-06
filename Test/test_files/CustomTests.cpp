#include "../core/TestUtility.hpp"

#include <chrono>
#include <ranges>
#include <numeric>

using namespace TestUtility;

int main() {
  callFunction("Integer push time test", [] {
    ChunkedList<int, 1'000'000'000> testList;

    std::cout << "starting..." << std::endl;

    const auto start = std::chrono::high_resolution_clock::now();

    constexpr size_t pushes = 1'000'000'000;

    for (int i = 0; i < pushes; i++) {
      testList.push(i);
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    const auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Total time taken to push " << pushes << " integers: " << duration_ms.count() << " milliseconds\n";
    std::cout << "Average time to push 1 integer: " << duration_ns.count() / pushes << " nanoseconds" << std::endl;
  });

  return 0;
}
