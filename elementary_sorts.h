#ifndef ELEMENTARY_SORTS_H
#define ELEMENTARY_SORTS_H

#include "random_mt.h"

#include <cstddef>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <vector>

/*Mostly for scalar types*/
template <typename T> class Sorts {
    static_assert(std::is_arithmetic_v<T>, "!We can't shuffle non-scalar types yet.");

  private:
    std::vector<T> m_data{};

  public:
    Sorts() = default;

    void resize(int size) {
        if (size < 0) {
            throw std::invalid_argument("Resizing with -ve values is invalid.");
        }
        m_data.resize(static_cast<std::size_t>(size));
    }

    bool empty() const noexcept {
        return m_data.empty();
    }

    void initialize_randomly() {
        for (std::size_t index{}; index < m_data.size(); ++index) {
            m_data[index] = Random::get(0, 0x10000);
        }
    }

    void initialize_consistent() {
        for (std::size_t index{}; index < m_data.size(); ++index) {
            m_data[index] = index;
        }
    }

    void shuffle() {
        // TODO: implement knuth shuffle
        if (empty()) {
            return;
        }
        std::size_t first_index{0};
        for (std::size_t i{}; i < m_data.size(); ++i) {

            std::size_t random_num = Random::get(first_index, i + 1);
            std::swap(m_data[i], m_data[random_num]);
        }
    }
    void print() {
        for (auto item : m_data) {
            std::cout << "\n " << item;
        }
    }
};
#endif // !ELEMENTARY_SORTS_H
