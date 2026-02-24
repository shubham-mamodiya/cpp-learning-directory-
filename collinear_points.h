#ifndef COLLINEAR_POINTS_H

class Point {
    float x{};
    float y{};

  public:
    Point() = default;

    Point(const float& x, const float& y) : x{x}, y{y} {}

    Point(const Point& that) {
        this->x = that.x;
        this->y = that.y;
    }

    Point& operator=(const Point& that) {
        this->x = that.x;
        this->y = that.y;
        return *this;
    }
    Point& operator=(const float& x, const float& y) {}
};
#endif // !collinear_points_h
// RandomizedQueue(const RandomizedQueue&) = delete;
// RandomizedQueue(RandomizedQueue&&) = delete;
// RandomizedQueue& operator=(const RandomizedQueue&) = delete;
// RandomizedQueue& operator=(RandomizedQueue&&) = delete;
