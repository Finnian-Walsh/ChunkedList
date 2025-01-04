#include "../core/TestUtility.hpp"

class DataClass;

using T = std::vector<int>;
using ListType = ChunkedList<T, 1>;

class DataClass {
  public:
    int *memory{};

    DataClass() : memory{new int()} {
    }

    explicit DataClass(const int i) : memory{new int{i}} {
    }

    ~DataClass() {
      if (memory) {
        delete memory;
      }
    }
};


int main() {
  ListType list{};

  T vec{1};

  vec.emplace_back(1);

  list.emplace(std::initializer_list{1, 2, 3});

  const T &element = list[0];

  std::ranges::for_each(element, [](const int n) -> void {
    std::cout << n << ' ';
  });

  return 0;
}
