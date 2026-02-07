#include "random_mt.h"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <type_traits>
#include <vector>

template <typename T> class Sort {
    static_assert(std::is_arithmetic_v<T>,
                  "!Sort object only intended to work on premetive data types.");

  private:
    std::vector<T> m_data{};
    std::vector<T> aux{};
    const int CUTOFF{7};

    void merge(int lo, int mid, int hi) {
        if (lo >= 0 && hi < static_cast<int>(m_data.size()) && lo <= hi) {

            for (int k = lo; k <= hi; k++) {
                aux[k] = m_data[k];
            }

            int i = lo, j = mid + 1;
            for (int k = lo; k <= hi; k++) {
                if (i > mid) {
                    m_data[k] = aux[j++];
                } else if (j > hi) {
                    m_data[k] = aux[i++];
                } else if (less(aux[j], aux[i])) {
                    m_data[k] = aux[j++];
                } else {
                    m_data[k] = aux[i++];
                }
            }
        } else {
            return;
        }
    }

    bool is_sorted(std::vector<T>& a, int lo, int hi) {
        assert(!(lo < 0 || hi < 0));
        if (a.size() <= 1) {
            return true;
        }
        for (int i{lo}; i < hi; ++i) {
            if (less(a[i + 1], a[i])) {
                return false;
            }
        }
        return true;
    }

    bool less(const T& lhs, const T& rhs) { return compare(lhs, rhs) < 0; }

    int compare(const T& lhs, const T& rhs) {
        if (lhs < rhs) {
            return -1;
        } else if (lhs > rhs) {
            return 1;
        } else {
            return 0;
        }
    }

    void insertion_sort(int lo, int hi) {
        int smallest{};
        for (int i{lo}; i <= hi; ++i) {
            smallest = i;
            for (int j{i}; j <= hi; ++j) {
                if (less(m_data[j], m_data[i])) {
                    smallest = j;
                }
            }
            std::swap(m_data[i], m_data[smallest]);
        }
    }
    void sort(int lo, int hi) {

        if (hi <= lo + CUTOFF - 1) {
            insertion_sort(lo, hi);
            return;
        }

        int mid{lo + (hi - lo) / 2};
        sort(lo, mid);
        sort(mid + 1, hi);
        merge(lo, mid, hi);
    }

  public:
    Sort() = default;

    Sort(std::vector<T>& other) : m_data{other}, aux(other.size()) {}

    std::size_t size() const noexcept { return m_data.size(); }

    bool empty() const noexcept { return m_data.empty(); }
    void merge_sort() {
        if (!m_data.empty()) {
            sort(0, m_data.size() - 1);
        }
    }

    void resize(int size) {
        assert(size >= 0);
        m_data.resize(static_cast<std::size_t>(size));
        aux.resize(static_cast<std::size_t>(size));
    }

    bool is_sorted() {
        if (m_data.size() <= 1) {
            return true;
        }
        for (std::size_t i{1}; i < m_data.size(); ++i) {
            if (less(m_data[i], m_data[i - 1])) {
                return false;
            }
        }

        return true;
    }

    void print() {
        for (auto& item : m_data) {
            std::cout << item << "\n";
        }
    }
};

int main() {
    std::size_t size{10};
    std::vector<int> unsorted_data(size);
    for (std::size_t i{}; i < size; ++i) {
        unsorted_data[i] = Random::get(0, size);
    }
    Sort<int> data{unsorted_data};
    data.merge_sort();
    data.print();
    return 0;
}
