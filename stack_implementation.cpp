#include <iostream>
#include <string>

/* Stack API
 * Warmup API. Stack of strings data type.
 * StackOFStrings , Create an empty stack
 * void push(String item), insert a new element onto the stack
 * String pop(), remove and return the string most recently added
 * bool isEmpty(), is stack empty
 * int size, number of strings on the stack
 */
class ArrayStack {
public:
  std::string stack[10]{};
  void push(std::string word) { stack[0] = word; }
  std::string pop() { return stack[0]; }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  ArrayStack words;
  words.push("hello, world!");
  std::cout << words.pop();
  return 0;
}
