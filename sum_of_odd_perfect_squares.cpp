#include <cstddef>
#include <iostream>

int main() {

    std::size_t sum{};
    std::size_t maximum_needed{799'000U};
    for (std::size_t i = 1; i <= maximum_needed; i += 2) {
        sum += i * i;
    }
    std::cout << "Answer: " << sum;
    return 0;
}
