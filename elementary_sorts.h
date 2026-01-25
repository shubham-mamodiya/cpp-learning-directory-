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
        // TODO: implement knuth shuffle
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
        for (std::size_t index{}; index < m_data.size(); ++index) {
            for (std::size_t j_index{}; j_index < m_data.size(); ++j_index) {
                if (m_data[j_index] > m_data[index]) {
                    std::swap(m_data[j_index], m_data[index]);
                }
            }
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

    int8_t compare_to(const T& lhs, const T& rhs) {
        if (lhs > rhs) {
            return -1;

        } else if (lhs < rhs) {
            return 1;
        } else {
            return 0;
        }
    }
    bool less(const T& lhs, const T& rhs) { return (compare_to(lhs, rhs) < 0); }
};
#endif // !ELEMENTARY_SORTS_H
