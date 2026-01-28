
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <vector>

using namespace std;
template <typename T> class Sort {
    static_assert(is_arithmetic_v<T>,
                  "!Sort object only intended to work on premetive data types.");

  private:
    vector<T> data{};

    void merge() { return; }
    void sort_v1() { return; }

  public:
    bool empty() const noexcept { return data.empty(); }

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
int main() { return 0; }
