#include <iostream>
#include <limits>
#include <vector>

int main() {
    std::vector<int> scoreList{};

    while (true) {
        std::cout << "Enter a score (or -1 to finish): ";
        int x{};
        std::cin >> x;

        if (!std::cin) // handle bad input
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // If we're done, break out of loop
        if (x == -1)
            break;

        // The user entered a valid element, so let's push it on the vector
        scoreList.push_back(x);
    }

    std::cout << "Your list of scores: \n";

    for (const auto& score : scoreList)
        std::cout << score << ' ';

    return 0;
}
