
#include "elementary_sorts.h"

int main() {
    // Sorts<int> data{};
    // data.resize(5);
    // data.print();
    //
    // data.initialize_randomly();
    // data.print();

    Sorts<int> data_2{};
    data_2.resize(5);
    // data_2.print();

    data_2.initialize_consistent();
    data_2.shuffle();
    data_2.print();
}
