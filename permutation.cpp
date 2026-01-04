#include "deque.h"
#include <cstddef>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  std::size_t size{10};
  Deque<std::size_t> container{};
  for (std::size_t x{}; x < size; ++x) {
    container.push_front(x);
  }
  container.print_backwards();
  std::cout << container.get_size() << std::endl;
  return 0;
}
