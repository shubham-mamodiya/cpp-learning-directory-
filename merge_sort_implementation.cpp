
#include <cassert>
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
    vector<T> m_data{};

    void merge(int lo, int hi) {
        assert(lo > 0 || hi > 0 || hi < static_cast<int>(m_data.capacity()) ||
               lo < static_cast<int>(m_data.capacity()));

        int mid{(lo + hi) / 2};

        // assert(!is_sorted(m_data, lo, mid));
        //
        // assert(!is_sorted(m_data, mid + 1, hi));

        vector<T> aux(hi - lo + 1);
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

    void sort_v1() { return; }

  public:
    Sort() = default;

    std::size_t size() const noexcept { return m_data.size; }

    bool empty() const noexcept { return m_data.empty(); }

    bool less(const T& lhs, const T& rhs) { return compare(lhs, rhs) < 0; }

    // void merge_sort() { sort_v1(); }

    void resize(int size) {
        assert(size >= 0);
        m_data.resize(static_cast<std::size_t>(size));
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
};

int main() {

    Sort<int> first_half{};
    first_half.resize(5);
    first_half.m_data = {1, 6, 2, 3, 5};
    first_half.merge(0, 4);
    for (int i{}; i < static_cast<int>(first_half.m_data.size()); ++i) {
        cout << "\n" << first_half.m_data[i];
    }
    return 0;
}
