#pragma once
#include <iostream>

class Point_2d {
  public:
    double m_x{};
    double m_y{};

    Point_2d() = default;

    Point_2d(const double& x, const double& y) : m_x{x}, m_y{y} {}

    Point_2d(const Point_2d& that) {
        this->m_x = that.m_x;
        this->m_y = that.m_y;
    }

    Point_2d& operator=(const Point_2d& that) {
        this->m_x = that.m_x;
        this->m_y = that.m_y;
        return *this;
    }

    friend Point_2d operator+(const Point_2d& p1, const Point_2d& p2) {
        return Point_2d{p1.m_x + p2.m_x, p1.m_y + p2.m_y};
    }

    friend Point_2d operator-(const Point_2d& p1, const Point_2d& p2) {
        return Point_2d{p1.m_x - p2.m_x, p1.m_y - p2.m_y};
    }

    friend bool operator==(const Point_2d& p1, const Point_2d& p2) {
        return p1.m_x == p2.m_x && p1.m_y == p2.m_y;
    }

    friend bool operator!=(const Point_2d& p1, const Point_2d& p2) {
        return p1.m_x != p2.m_x || p1.m_y != p2.m_y;
    }
};

std::ostream& operator<<(std::ostream&, const Point_2d&);
std::istream& operator>>(std::istream&, Point_2d&);

/* just copy this to any cpp file for using it

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
*/
