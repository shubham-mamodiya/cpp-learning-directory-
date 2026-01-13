#include "deque.h"
// #include <cstddef>
#include <string>

void get_input(Deque<std::string>* container);

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    using namespace std;

    Deque<string> container{};
    get_input(&container);

    container.print_backwards();
    return 0;
}

/*when char c is in line:
 * when c is a '-' pops form tail.
 * when c is a '~' pops from head.
 * when c is a '`'. This will push the next word at the tail until it
 * founds any instruction symbols or whitespace.
 * The default push is at the head.
 * it does not pop when container is empty.
 * */
void get_input(Deque<std::string>* container) {
    using namespace std;
    string input{};
    string temp_word{};
    bool pop_condition{};

    // because '`' pushes the next word to the tail and other symbols takes
    // care of previous word
    // true -> push on the head
    // false -> push on the tail
    bool push_forward{true};

    // used to decide should we append this char to the temp_word
    bool append{true};

    char white_space = ' ';
    char backtick = '`';
    char tilde = '~';
    char hyphen = '-';
    // TODO: use std::array for instructions

    string instructions{white_space, backtick, tilde, hyphen};

    cout << "Test input for data: \n";
    getline(cin >> ws, input);
    for (char character : input) {
        // check if we have any instructions or not
        for (char instruction : instructions) {
            if (instruction == character) {
                append = false;
            }
        }

        // when container is empty don't pop
        // so, pop_condition is false or vice versa
        pop_condition = container->is_empty() ? false : true;

        if (append) {
            temp_word += character;
        } else {
            // PUSHING
            // push the previous buffer
            // it changes the direction of push and also pushes the stored
            // buffer to the container based on the previous direction
            if (!temp_word.empty()) {
                if (push_forward) {
                    container->push_front(temp_word);
                } else {
                    container->push_back(temp_word);
                }
                // After every push set direction to default (true)
                // it is intended this way because backtick means push
                // the next word at the tail

                temp_word.clear();
            }
            push_forward = (character == backtick) ? false : true;

            // POPPING
            if (character == hyphen || character == tilde) {

                // pop only if container is not empty
                if (pop_condition) {
                    // pop form head
                    if (character == hyphen) {
                        container->pop_front();
                    }
                    // pop form tail
                    else if (character == tilde) {
                        container->pop_back();
                    }
                }
            }
        }
        // means you can append character to temp_word
        append = true;
    }

    // the above code does not try to push the last word from input to container
    // so this code completes that
    // TODO: remove this extra code and make above code work universally for
    // all inputs

    if (!temp_word.empty()) {

        if (push_forward) {
            container->push_front(temp_word);
        } else {
            container->push_back(temp_word);
        }
        temp_word.clear();
    }
}
