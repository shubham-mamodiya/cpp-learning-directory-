#include "random_mt.h"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <type_traits>
#include <vector>

template <typename T> class Sort {
    static_assert(std::is_arithmetic_v<T>, "!Sort only works with primitive data types.");

  private:
    std::vector<T> data{};
    std::vector<T> buffer{};

    static constexpr std::size_t cutoff{7};

    void merge(int lo, int mid, int hi) {
        assert(lo >= 0 && hi < static_cast<int>(data.size()) && lo <= hi);

        int i = lo, j = mid + 1;
        for (int k = lo; k <= hi; k++) {
            if (i > mid) {
                buffer[k] = data[j++];
            } else if (j > hi) {
                buffer[k] = data[i++];
            } else if (less_than(data[j], data[i])) {
                buffer[k] = data[j++];
            } else {
                buffer[k] = data[i++];
            }
        }

        for (int i = lo; i <= hi; ++i) {
            data[i] = buffer[i];
        }
    }

    bool is_sorted(std::vector<T>& a, int lo, int hi) {
        assert(!(lo < 0 || hi < 0));
        if (a.size() <= 1) {
            return true;
        }
        for (int i{lo}; i < hi; ++i) {
            if (less_than(a[i + 1], a[i])) {
                return false;
            }
        }
        return true;
    }

    bool less_than(const T& lhs, const T& rhs) { return compare(lhs, rhs) < 0; }

    int compare(const T& lhs, const T& rhs) {
        if (lhs < rhs) {
            return -1;
        } else if (lhs > rhs) {
            return 1;
        } else {
            return 0;
        }
    }

    void selection_sort(int lo, int hi) {
        int smallest{};
        for (int i{lo}; i <= hi; ++i) {
            smallest = i;
            for (int j{i}; j <= hi; ++j) {
                if (less_than(data[j], data[smallest])) {
                    smallest = j;
                }
            }
            std::swap(data[i], data[smallest]);
        }
    }

    void sort(int lo, int hi) {

        if (hi <= lo + CUTOFF - 1) {
            selection_sort(lo, hi);
            return;
        }

        int mid{lo + (hi - lo) / 2};
        sort(lo, mid);
        sort(mid + 1, hi);
        merge(lo, mid, hi);
    }

  public:
    Sort() = default;

    Sort(std::vector<T>& other) : data{other}, buffer(other.size()) {}

    std::size_t size() const noexcept { return data.size(); }

    bool empty() const noexcept { return data.empty(); }

    void merge_sort() {
        if (!data.empty()) {
            sort(0, data.size() - 1);
        }
    }

    void resize(int size) {
        assert(size >= 0);
        data.resize(static_cast<std::size_t>(size));
        buffer.resize(static_cast<std::size_t>(size));
    }

    bool is_sorted() {
        if (data.size() <= 1) {
            return true;
        }
        for (std::size_t i{1}; i < data.size(); ++i) {
            if (less_than(data[i], data[i - 1])) {
                return false;
            }
        }

        return true;
    }

    void print() {
        for (auto& item : data) {
            std::cout << item << "\n";
        }
    }
};

int main() {
    std::size_t size{20};
    std::vector<int> unsorted_data(size);
    for (std::size_t i{}; i < size; ++i) {
        unsorted_data[i] = Random::get(0, size);
    }
    Sort<int> data{unsorted_data};
    data.merge_sort();
    data.print();
    return 0;
}
