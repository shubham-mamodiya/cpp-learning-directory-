#ifndef RANDOMIZED_QUEUE_H
#define RANDOMIZED_QUEUE_H

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <utility>

template <typename T> class randomized_queue {
  private:
    struct Node_t {
        T value{};
        Node_t* next{nullptr};

        Node_t() = default;

        explicit Node_t(const T& other_value, Node_t* other_next)
            : value{other_value}, next{other_next} {}
    };

    Node_t* m_fisrt{};
    Node_t* m_last{};
    std::size_t m_size{};

  public:
    randomized_queue() = default;

    // TODO: implement move semantics

    bool empty() const {
        return m_size == 0;
    }

    std::size_t size() const {
        return m_size;
    }

    void enqueue(const T& item) {

        Node_t* new_node = new Node_t(item, nullptr);

        if (empty()) {
            m_fisrt = m_last = new_node;
            ++m_size;
        } else {
            m_fisrt->next = new_node;
            ++m_size;
        }
    }
};
#endif // !RANDOMIZED_QUEUE_H
