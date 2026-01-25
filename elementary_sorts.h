/*NOTE: please don't use this file. Because there are 1 or 2 bugs
 * in it and I did not fix them as it was just for learning the algorithms
 * not to make a useable library*/
#ifndef ELEMENTARY_SORTS_H

#define ELEMENTARY_SORTS_H

#include "random_mt.h"

#include <cstddef>
#include <cstdint>
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

    bool empty() const noexcept { return m_data.empty(); }

    void initialize_randomly() {
        for (std::size_t index{}; index < m_data.size(); ++index) {
            m_data[index] = Random::random<T>();
        }
    }

    void initialize_consistent() {
        for (std::size_t index{}; index < m_data.size(); ++index) {
            m_data[index] = index;
        }
    }

    void shuffle() {

        if (empty()) {
            return;
        }
        std::size_t first_index{0};
        for (std::size_t i{}; i < m_data.size(); ++i) {

            std::size_t random_num = Random::get(first_index, i);
            std::swap(m_data[i], m_data[random_num]);
        }
    }
    void print() {
        for (auto item : m_data) {
            std::cout << "\n " << item;
        }
    }

    void selection_sort() {
        if (empty()) {
            return;
        }
        std::size_t smallest{};
        for (std::size_t i{}; i < m_data.size(); ++i) {
            smallest = i;
            for (std::size_t j{i}; j < m_data.size(); ++j) {
                if (less(m_data[smallest], m_data[j])) {
                    smallest = j;
                }
            }
            std::swap(m_data[i], m_data[smallest]);
        }
    }

    void insertion_sort() {
        std::size_t size{m_data.size()};
        for (std::size_t i{}; i < size; ++i) {
            for (long long j{static_cast<long long>(i)}; j > 0; --j) {
                if (less(m_data[j - 1], m_data[j])) {
                    std::swap(m_data[j - 1], m_data[j]);
                } else {
                    break;
                }
            }
        }
    }

    void shell_sort() {
        // weights
        std::size_t size{m_data.size()};

        // knuth gap sequence
        std::size_t gap{};
        while (gap < (size / 3)) {
            gap = 3 * gap + 1;
        }

        while (gap >= 1) {
            for (std::size_t i{gap}; i < size; ++i) {
                for (std::size_t j{i}; j >= gap && less(m_data[j - gap], m_data[j]); j -= gap) {
                    std::swap(m_data[j - gap], m_data[j]);
                }
            }
            gap /= 3;
        }
    }

    // Empty container is always sorted
    void is_sorted() {
        if (m_data.size() <= 1) {
            std::cout << "\nTrue";
            return;
        }
        for (std::size_t i{1}; i < m_data.size(); ++i) {
            if (less(m_data[i - 1], m_data[i])) {
                std::cout << "\nFalse";
                return;
            }
        }
        std::cout << "\nTrue";
    }
    int8_t compare_to(const T& lhs, const T& rhs) {
        if (lhs < rhs) {
            return 1;

        } else if (lhs > rhs) {
            return -1;
        } else {
            return 0;
        }
    }
    bool less(const T& lhs, const T& rhs) { return (compare_to(lhs, rhs) < 0); }
};
#endif // !ELEMENTARY_SORTS_H
