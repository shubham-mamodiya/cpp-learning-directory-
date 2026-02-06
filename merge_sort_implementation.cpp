
#include <cassert>
#include <iostream>
#include <type_traits>
#include <vector>

using namespace std;
template <typename T> class Sort {
    static_assert(is_arithmetic_v<T>,
                  "!Sort object only intended to work on premetive data types.");

  private:
    // nothing yet remove if you want

    vector<T> m_data{};
    vector<T> aux{};

    void merge(int lo, int mid, int hi) {
        assert(lo > 0 || hi > 0 || hi < static_cast<int>(m_data.capacity()) ||
               lo < static_cast<int>(m_data.capacity()));

        mid += 1;

        // assert(!is_sorted(m_data, lo, mid));
        //
        // assert(!is_sorted(m_data, mid + 1, hi));

        for (int k = lo; k <= hi; k++) {
            aux[k] = m_data[k];
        }

        int i = lo, j = mid;
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
    }

    bool is_sorted(vector<T> a, int lo, int hi) {
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

    void sort(int lo, int hi) {
        assert(lo >= 0 || hi >= 0);

        if (hi <= lo) {
            return;
        };

        int mid{lo + (hi - lo) / 2};
        --hi;
        sort(lo, mid);
        sort(mid + 1, hi);
        merge(lo, mid, hi);
    }

  public:
    Sort() = default;

    Sort(vector<T> other) {
        m_data = other;
        aux = other;
    }

    std::size_t size() const noexcept { return m_data.size; }

    bool empty() const noexcept { return m_data.empty(); }

    bool less(const T& lhs, const T& rhs) { return compare(lhs, rhs) < 0; }

    // void merge_sort() { sort_v1(); }

    void merge_sort() { sort(0, m_data.size()); }
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
            if (less(m_data[i - 1], m_data[i])) {
                return false;
            }
        }

        return true;
    }

    int compare(const T& lhs, const T& rhs) {
        if (lhs < rhs) {
            return -1;
        } else if (lhs > rhs) {
            return 1;
        } else {
            return 0;
        }
    }

    void print() {
        for (auto& item : m_data) {
            std::cout << item << "\n";
        }
    }
};

int main() {
    vector<int> unsorted_data{5, 6, 3, 4, 6, 3, 9, 2};
    Sort<int> data{unsorted_data};
    data.merge_sort();
    data.print();
    return 0;
}
