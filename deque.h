
/* This is an implementation of queue that can be created with mostly any
 * Primitive data types*/

#ifndef DEQUE_H
#define DEQUE_H

#include <cstddef>
#include <iostream>
#include <stdexcept>

// generic Double ended queue object
template <typename T> class Deque {
  private:
    struct Node_t {
        // initialized to nothing by default
        T data{};
        Node_t* next = nullptr;
        Node_t* prev = nullptr;

        Node_t() = default;
        Node_t(T value) : data{value} {}

        // this constructor is redundant
        Node_t(T value, Node_t* input_next, Node_t* input_prev)
            : data{value}, next{input_next}, prev{input_prev} {}
    };

    /* Both are pointing to nullptr by default
     * and this is the state they should be when size is 0.
     * */
    Node_t* m_head{};
    Node_t* m_tail{};
    std::size_t m_size{};

  public:
    // because our node initializes everything itself we are not putting
    // anything in class default constructor
    Deque() = default;

    ~Deque() {
        while (m_head) {
            pop_front();
        }
    }

    // TODO: move semantics

    bool is_empty() const {
        return m_size == 0;
    }
    auto get_size() const {
        return m_size;
    }

    void push_front(T item) {
        auto* new_node = new Node_t{item};

        if (is_empty()) {

            m_head = m_tail = new_node;
        } else {
            new_node->next = m_head;
            m_head->prev = new_node;
            m_head = new_node;
        }
        ++m_size;
    }

    void push_back(T item) {
        auto* new_node = new Node_t{item};

        if (is_empty()) {
            m_head = m_tail = new_node;
        } else {
            new_node->prev = m_tail;

            m_tail->next = new_node;
            m_tail = new_node;
        }
        ++m_size;
    }

    void pop_front() {
        if (is_empty()) {
            throw std::runtime_error("Deque Container is already empty!");
        }

        Node_t* temp_node = m_head;
        m_head = m_head->next;
        if (m_head) {
            m_head->prev = nullptr;
        } else {
            m_tail = nullptr; // deque container is empty
        }
        delete temp_node;
        --m_size;
    }

    void pop_back() {
        if (is_empty()) {
            throw std::runtime_error("Deque container is already empty!");
        }

        Node_t* temp_node = m_tail;
        m_tail = m_tail->prev;
        if (m_tail) {
            m_tail->next = nullptr;
        } else {
            m_head = nullptr; // deque container is empty
        }

        delete temp_node;
        --m_size;
    }

    auto get_front() -> T {
        if (is_empty()) {
            throw std::runtime_error("Deque container is already empty!");
        }
        return m_head->data;
    }

    auto get_back() -> T {
        if (is_empty()) {
            throw std::runtime_error("Deque container is already empty!");
        }

        return m_tail->data;
    }

    // prints data from head to tail
    // 1 2 3 then 4
    void print_forward() {

        Node_t* current = m_head;

        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }

        std::cout << '\n';
    }

    // prints data form tail to head
    // 4 3 2 then 1
    void print_backwards() {
        Node_t* current = m_tail;
        while (current) {
            std::cout << current->data << " ";
            current = current->prev;
        }
        std::cout << '\n';
    }
};

#endif // DEQUE_H
