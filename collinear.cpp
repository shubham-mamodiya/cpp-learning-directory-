#include "points.h"

#include <iomanip> // for setprecision
#include <iostream>

std::ostream& operator<<(std::ostream& out, const Point_2d& p) {
    return out << std::fixed << std::setprecision(2) << "Point(" << p.m_x << ", " << p.m_y << ")";
}

std::istream& operator>>(std::istream& in, Point_2d& point) {

    double x{};
    double y{};

    in >> x >> y;
    point = in ? Point_2d{x, y} : Point_2d{};

    return in;
}

int main() {
    [[maybe_unused]] Point_2d p1{4.5, 5.4};
    [[maybe_unused]] Point_2d p2{p1};
    [[maybe_unused]] Point_2d p3 = p2;
    std::cin >> p3;
    std::cout << p1 << "\n" << p3;
    return 0;
}
