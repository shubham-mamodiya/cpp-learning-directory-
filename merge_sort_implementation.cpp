
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <vector>

using namespace std;
template <typename T> class Sort {
    static_assert(is_arithmetic_v<T>,
                  "!Sort object only intended to work on premetive data types.");

  private:
    // nothing yet remove if you want
  public:
    vector<T> data{};

    vector<T> merge(int lo, int hi) {
        int mid{(lo + hi) / 2};
        if (!is_sorted(data, lo, mid)) {
            return;
        }

        if (!is_sorted(data, mid + 1, hi)) {
            return;
        }
        vector<T> aux(data.capacity());
        for (int k = lo; k <= hi; k++) {
            aux[k] = data[k];
        }

        int i = lo, j = mid + 1;
        for (int k = lo; k <= hi; k++) {
            if (i > mid) {
                data[k] = aux[j++];
            } else if (j > hi) {
                data[k] = aux[i++];
            } else if (less(aux[j], aux[i])) {
                data[k] = aux[j++];
            } else {
                data[k] = aux[i++];
            }
        }
        return data;
    }
    bool is_sorted(vector<T> a, int lo, int hi) {
        if (lo < 0 || hi < 0) {
            return false;
        }
        if (a.size() <= 1) {
            std::cout << "\nTrue";
            return true;
        }
        for (int i{lo}; i < hi; ++i) {
            if (less(a[i - 1], a[i])) {
                std::cout << "\nFalse";
                return false;
            }
        }
        std::cout << "\nTrue";
        return true;
    }

    void sort_v1() { return; }

  public:
    Sort() { data.resize(10); }

    bool empty() const noexcept { return data.empty(); }

    bool less(const T& lhs, const T& rhs) { return compare(lhs, rhs) < 0; }

    void merge_sort() { sort_v1(); }

    bool is_sorted() {
        if (data.size() <= 1) {
            std::cout << "\nTrue";
            return true;
        }
        for (std::size_t i{1}; i < data.size(); ++i) {
            if (less(data[i - 1], data[i])) {
                std::cout << "\nFalse";
                return false;
            }
        }
        std::cout << "\nTrue";
        return true;
    }
    int8_t compare(const T& lhs, const T& rhs) {
        if (lhs < rhs) {
            return -1;
        } else if (lhs > rhs) {
            return 1;
        } else {
            return 0;
        }
    }
};

int main() {

    Sort<int> first_half{};

    for (int i = 0; i < (first_half.data.capacity() / 2); ++i) {
        first_half.data[i] = i;
    }

    return 0;
}
