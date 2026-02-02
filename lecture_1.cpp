#include <iostream>

int main() {
    int first_num{};
    std::cout << "\n provide any number: ";
    std::cin >> first_num;
    std::cout << "\n provide any number: ";
    int second_num{};
    std::cin >> second_num;

    std::cout << "\nsum: " << first_num + second_num;

    return 0;
}
