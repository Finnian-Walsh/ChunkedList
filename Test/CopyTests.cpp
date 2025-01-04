#include "core/TestUtility.hpp"

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

    // DataClass(DataClass &&other) noexcept : memory{other.memory} {
    //   other.memory = nullptr;
    // }
    //
    // DataClass &operator=(const DataClass &other) {
    //   if (this != &other) {
    //      memory = other.memory;
    //   }
    //
    //   return *this;
    // }

    ~DataClass() {
      if (memory) {
        delete memory;
      }
    }
};


int main() {
  T vec{1};

  ListType list{};

  list.push(std::move(vec));

  // std::cout << &list[0] << std::endl;

  return 0;
}
