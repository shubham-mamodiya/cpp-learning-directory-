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
        long long size{static_cast<long long>(m_data.size())};
        for (long long i_index{}; i_index < size; ++i_index) {
            for (long long j_index{i_index - 1}; j_index > 0; --j_index) {
                if (m_data[j_index] > m_data[i_index]) {
                    std::swap(m_data[j_index], m_data[i_index]);
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
    bool less(const T& lhs, const T& rhs) {
        return (compare_to(lhs, rhs) < 0);
    }
};
#endif // !ELEMENTARY_SORTS_H
