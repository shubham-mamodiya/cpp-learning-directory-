
#include "elementary_sorts.h"

int main() {
    // Sorts<int> data{};
    // data.resize(5);
    // data.print();
    //
    // data.initialize_randomly();
    // data.print();

    Sorts<int> data_2{};
    data_2.resize(10);
    // data_2.print();

    data_2.initialize_randomly();
    // data_2.print();

    std::cout << "\n=======================";
    // data_2.selection_sort();
    // data_2.insertion_sort();
    data_2.shell_sort();
    data_2.print();
}
