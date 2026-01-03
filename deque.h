
/* This is an implementation of queue that can be created with mostly any
 * Primitive data types*/

#ifndef DEQUE_H
#define DEQUE_H

#include <cstddef>

template <typename T> class Deque {
private:
  struct Node {
    T data{};
    Node *next = nullptr;
    Node *prev = nullptr;

    Node() = default;
    Node(T value) : data{value} {}
    Node(T value, Node *input_next, Node *input_prev)
        : data{value}, next{input_next}, prev{input_prev} {}
  };

  Node *m_head{};
  Node *m_tail{};
  std::size_t m_size{};

public:
  // because our node initializes everything itself we are not putting
  // anything in class default constructor
  Deque() = default;

  // TODO: distructor

  // TODO: move semantics

  bool empty() const { return m_size == 0; }
  auto length() const { return m_size; }

  void push_front(T item) { Node *new_node = new Node{item}; }
};

/*deque psudocode
 * use template
 * create Deque class
 * define constructor
 * and distructor
 * copy & move semantics
 * implement empty method that returns boolean that is container is empty or not
 * implement size function
 *
 * Modifiers push_front and push_back
 * return type T pop_front and pop_back
 *
 * iterator support
 * class iterator
 * iterato begin()
 * iterator end()
 */
#endif // DEQUE_H
