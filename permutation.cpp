#include "deque.h"
// #include <cstddef>
#include <string>

void get_input(Deque<std::string>& container);

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    using namespace std;

    Deque<string> container{};
    get_input(container);

    container.print_backwards();
    return 0;
}

/* When a character appears in the line:
 * - '-' pops from the tail.
 * - '~' pops from the head.
 * - '`' pushes the next word to the tail until an instruction symbol
 *   or whitespace is found.
 * - The default push is at the head.
 * - It does not pop when the container is empty.
 */
void get_input(Deque<std::string>& container) {
    std::string input{};     // To get a line as input
    std::string temp{};      // To derive words from input
    bool push_forward{true}; // for the condition when a Backtick is given so add the next word
    // to the tail

    std::cout << "Give some testing data: ";
    std::getline(std::cin >> std::ws, input);

    auto flush_word{[&]() -> void {
        if (temp.empty()) {
            return;
        }
        if (push_forward) {
            container.push_front(temp); // push on the head
        } else {
            container.push_back(temp); // push on the tail
        }
        temp.clear();
        push_forward = true; // reset to default push direction
    }};

    for (char character : input) {
        switch (character) {
        default:
            temp += character;
            break;

        case ' ':
            flush_word();
            break;

        case '`':
            flush_word();         // previous buffer
            push_forward = false; // next push on the tail
            break;

        case '-':
            flush_word(); // previous buffer (Ex: 1-2, push 1 then pop)
            if (!container.is_empty()) {
                container.pop_front();
            }
            break;

        case '~':
            flush_word();
            if (!container.is_empty()) {
                container.pop_back();
            }
            break;
        }
    }
    flush_word(); // push the last word that happens to be left in the input
}
