#include "deque.h"
// #include <cstddef>
#include <string>

void get_input(Deque<std::string> *container);

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
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
 * founds any instruction symbol. Also the previous
 * The default push is at the head.
 * it does not pop when container is empty.
 * */
void get_input(Deque<std::string> *container) {
    using namespace std;
    string input{};
    string temp_word{};
    bool pop_condition{};
    // because '`' pushes the next word to the tail and other symbols takes
    // care of previous word
    bool push_forward{true};

    char white_space = ' ';
    char backtick = '`';
    char tilde = '~';
    char hyphen = '-';


    getline(cin >> ws, input);
    for (char c : input) {
        pop_condition = container->is_empty() ? false : true;

        // it changes the direction of push and also pushes the stored
        // buffer to the container based on the previous direction
        if (c == backtick) {
            if (!temp_word.empty()) {
                if (push_forward) {
                    container->push_front(temp_word);
                } else {
                    container->push_back(temp_word);
                }
            }
            push_forward = false;
        }
        // popping and pushing
        else if (c == hyphen || c == tilde || c == white_space) {
            // push the previous buffer
            if (!temp_word.empty()) {
                if (push_forward) {
                    container->push_front(temp_word);
                } else {
                    container->push_back(temp_word);
                }
                // After every push set direction to default (true)
                push_forward = true;
            }
            // pop only if container is not empty
            if (pop_condition) {
                if (c == hyphen) {
                    container->pop_back();
                } else if (c == tilde) {
                    container->pop_front();
                }
            }
        } else {
            temp_word += c;
        }
    }
}
