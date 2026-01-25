
#include "elementary_sorts.h"

#include <cstddef>

int main() {
    // Sorts<int> data{};
    // data.resize(5);
    // data.print();
    //
    // data.initialize_randomly();
    // data.print();

    Sorts<int> data{};
    // Sorts<long> data{};
    // Sorts<float> data{};
    // Sorts<double> data{};
    // Sorts<std::size_t> data{};
    data.resize(10);
    // data_2.print();

    // data.initialize_consistent();
    data.initialize_randomly();
    // data.shuffle();
    // data_2.print();

    std::cout << "\n=======================";
    // data.selection_sort();
    data.insertion_sort();
    // data.shell_sort();
    data.print();
}
