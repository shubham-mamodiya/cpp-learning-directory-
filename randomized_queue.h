#ifndef RANDOMIZED_QUEUE_H
#define RANDOMIZED_QUEUE_H

#include "random_mt.h"

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <utility>

template <typename T> class RandomizedQueue {
  private:
    struct Node_t {
        T value{};
        Node_t* next{nullptr};

        Node_t() = default;

        explicit Node_t(const T& other_value, Node_t* other_next)
            : value{other_value}, next{other_next} {}
    };

    Node_t* m_tail{};
    Node_t* m_head{};
    std::size_t m_size{};

  public:
    RandomizedQueue() = default;

    ~RandomizedQueue() {
        while (!empty()) {
            dequeue();
        }
    }

    // TODO: implement move semantics
    RandomizedQueue(const RandomizedQueue&) = delete;
    RandomizedQueue(RandomizedQueue&&) = delete;
    RandomizedQueue& operator=(const RandomizedQueue&) = delete;
    RandomizedQueue& operator=(RandomizedQueue&&) = delete;
    bool empty() const noexcept {
        return m_size == 0;
    }

    std::size_t size() const noexcept {
        return m_size;
    }

    void enqueue(const T& item) {

        Node_t* new_node = new Node_t(item, nullptr);

        if (empty()) {
            m_tail = m_head = new_node;
            ++m_size;
        } else {
            m_tail->next = new_node;
            m_tail = new_node;
            ++m_size;
        }
    }

    auto dequeue() -> T {
        if (empty()) {
            throw std::logic_error("The container is empty!.");
        }

        auto* temp = m_head;
        auto value = temp->value;

        m_head = m_head->next;
        if (!m_head) {
            // when there is only one node in the list
            // first is also pointing to the some node
            m_tail = m_head = nullptr;
        }
        --m_size;
        delete temp;
        return value;
    }

    auto sample() const -> T {
        if (empty()) {
            throw std::logic_error("The container is empty!.");
        }

        Node_t* current = m_head;
        auto random_num = Random::get<std::size_t>(0, m_size - 1);

        for (std::size_t index = 0; index < random_num; ++index) {
            current = current->next;
        }

        return current->value;
    }
};
#endif // !RANDOMIZED_QUEUE_H
