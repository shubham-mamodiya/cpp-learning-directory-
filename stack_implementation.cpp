#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

/* Stack API
 * Warm-up API. Stack of strings data type.
 * Stack Of Strings , Create an empty stack
 * void push(String item), insert a new element onto the stack
 * String pop(), remove and return the string most recently added
 * bool is Empty(), is stack empty
 * int size, number of strings on the stack
 */
class ArrayStack {
private:
  std::size_t m_capacity{16}; // why 16? Because 16 times 2 or divided by 2 is
                              // always even. But odd is also fine
  std::string *m_stack = new std::string[m_capacity]{};
  std::size_t m_top{};

  void resize(size_t m_new_capacity) {
    std::string *m_new_stack = new std::string[m_new_capacity];

    for (std::size_t index = 0; index < m_top; ++index) {
      m_new_stack[index] = std::move(m_stack[index]);
    }

    delete[] m_stack;
    m_stack = m_new_stack;
    m_capacity = m_new_capacity;
  }

public:
  ~ArrayStack() { delete[] m_stack; }

  void push(std::string_view item) {
    if (m_top >= m_capacity) {
      resize(m_capacity * 2);
    }
    m_stack[m_top++] = item;
  }

  std::string pop() {
    if (is_empty()) {
      throw std::runtime_error("Stack is empty!");
    }

    if (m_top > 0 && m_top <= (m_capacity / 2)) {
      resize(m_capacity / 2);
    }
    return std::move(
        m_stack[--m_top]); // because top is a count of elements not index
  }

  bool is_empty() const { return m_top == 0; }

  auto size() const { return m_capacity; }
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  ArrayStack words;
  std::string input{};
  int count{1};
  std::cout << "Enter ('-' to pop, '=' for size) \n";
  while (true) {
    std::cout << "\nEnter a word " << count << ": ";
    std::cin >> input;
    if (input == "-") {
      std::cout << "\n Popped Item: " << words.pop() << "\n";
      --count;
    } else if (input == "=") {
      std::cout << "\n Size: " << words.size() << "\n";
    } else {
      words.push(input);
      ++count;
    }
  }
  return 0;
}
