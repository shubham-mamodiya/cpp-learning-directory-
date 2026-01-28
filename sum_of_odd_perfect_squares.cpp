#include <cstddef>
#include <iostream>

int main() {

    std::size_t sum{};
    long long maximum{};
    std::cout << "Give the maximum exponent you desire: ";
    std::cin >> maximum;
    std::size_t maximum_needed{static_cast<std::size_t>(maximum)};
    sum = maximum_needed * ((4 * (maximum_needed * maximum_needed) - 1) / 3);
    std::cout << "Answer: " << sum;
    return 0;
}
