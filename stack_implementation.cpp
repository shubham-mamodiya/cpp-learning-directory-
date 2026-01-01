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
  /*
   * locations on this stack that are not used is a Empty string.
   * It does not free those locations.
   * */

  // why capacity is 16? Because 16 times 2 or divided by 2 is always even.
  // But odd is also fine
  std::size_t m_capacity{16};

  // pointer to dynamic array of strings
  std::string *m_stack{new std::string[m_capacity]{}};

  // Index for the latest added string
  std::size_t m_top{};

  void resize(size_t m_new_capacity) {
    std::string *m_new_stack = new std::string[m_new_capacity]{};

    for (std::size_t index = 0; index < m_top; ++index) {
      m_new_stack[index] = std::move(m_stack[index]);
    }

    delete[] m_stack;
    m_stack = m_new_stack;
    m_capacity = m_new_capacity;
  }

public:
  // data is managed by user so a Naturally empty stack
  ArrayStack() = default;

  ~ArrayStack() { delete[] m_stack; }

  /*
   * Deep copy constructor.
   * called when:
   * ArrayStack stack {some other stack}
   * */
  ArrayStack(const ArrayStack &other)
      : m_capacity{other.m_capacity},
        m_stack{new std::string[other.m_capacity]{}}, m_top{other.m_top} {
    for (std::size_t index = 0; index < m_capacity; ++index) {
      m_stack[index] = other.m_stack[index];
    }
  }

  /*
   * Deep copy assignment.
   * Called when:
   * ArrayStack a;
   * a = some other ArrayStack b;
   * */
  ArrayStack &operator=(const ArrayStack &other) {
    if (this != &other) {
      delete[] m_stack;

      m_capacity = other.m_capacity;
      m_top = other.m_top;
      m_stack = new std::string[m_capacity]{};

      for (std::size_t index{}; index < m_capacity; ++index) {
        m_stack[index] = other.m_stack[index];
      }
    }
    return *this;
  }

  /*
   * Deep move constructor for std::move()
   * Called when:
   * ArrayStack a = std::move(some ArrayStack b)
   * */
  ArrayStack(ArrayStack &&other) noexcept
      : m_capacity{other.m_capacity}, m_stack{other.m_stack},
        m_top{other.m_top} {
    other.m_stack = nullptr;
    other.m_top = 0;
    other.m_capacity = 0;
  }

  /*
   * Deep move assignment.
   * Called when:
   * ArrayStack a;
   * a = std::move(some ArrayStack b)
   * */
  ArrayStack &operator=(ArrayStack &&other) noexcept {
    if (this != &other) {
      delete[] m_stack;

      m_stack = other.m_stack;
      m_capacity = other.m_capacity;
      m_top = other.m_top;

      other.m_stack = nullptr;
      other.m_top = 0;
      other.m_capacity = 0;
    }
    return *this;
  }

  void push(std::string item) {
    if (m_top >= m_capacity) {
      resize(m_capacity * 2);
    }
    m_stack[m_top++] = std::move(item);
  }

  std::string pop() {
    if (is_empty()) {
      throw std::runtime_error("Stack is empty!");
    }

    // because top is a count of elements not index and starts from 1 not 0.
    std::string value = std::move(m_stack[--m_top]);

    // shrink to half when stack is a quarter full
    if (m_top > 0 && m_top <= (m_capacity / 4)) {
      resize(m_capacity / 2);
    }

    return value;
  }

  bool is_empty() const { return m_top == 0; }

  auto size() const { return m_top; }
  auto capacity() const { return m_capacity; }

  void reverse() {
    for (std::size_t i{}; i < m_top; ++i) {
      std::swap(m_stack[i], m_stack[m_top - 1 - i]);
    }
  }

  void print() const {
    for (std::size_t i{}; i < m_top; ++i) {
      std::cout << m_stack[i] << " ";
    }
  }
};

// Linked list implementation for stack
typedef struct Node {
  std::string item{};
  Node *next = nullptr;

  Node(const std::string &word, Node *next_node) : item{word}, next{next_node} {
    // Nothing in constructor body
    //
  }
} Node_t;

/*
 * Asks for a line to the user then pushes words to the stack
 * pops when '-' is given
 * */
int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  ArrayStack words;
  std::string input{};
  std::string temp_word{};
  char white_space = ' ';

  std::cout << "Enter text ('-' to pop) \n";
  std::getline(std::cin >> std::ws, input);
  for (char c : input) {
    if (c == '-') {
      if (!words.is_empty()) {
        if (!temp_word.empty()) {
          words.push(temp_word);
          temp_word.clear();
        }
        words.pop();
      }
    } else if (c == white_space) {
      if (!temp_word.empty()) {
        words.push(temp_word);
        temp_word.clear();
      }
    } else {
      temp_word += c;
    }
  }
  if (!temp_word.empty()) {
    words.push(temp_word);
  }
  words.reverse();

  words.print();
  return 0;
}
