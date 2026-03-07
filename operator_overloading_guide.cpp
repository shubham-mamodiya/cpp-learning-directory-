/*
  Operator Overloading in C++ — An Extensive, Practical Guide
  ==========================================================

  This file is meant to be read like a tutorial. It also compiles and runs.
  You can use it as a reference while learning or while implementing operators
  in your own types.

  Build examples:
    g++ -std=c++20 -Wall -Wextra -Wpedantic -O2 operator_overloading_guide.cpp -o guide
    ./guide

  Table of contents
  -----------------
  1) Why operator overloading exists
  2) Big rules: what you can and cannot overload
  3) Member vs non-member vs friend
  4) Return types and const correctness
  5) Arithmetic operators: +, -, *, /, % (and friends)
  6) Assignment operators: =, +=, -=, ...
  7) Comparisons: ==, <, <=, <=> and ordering best practices
  8) Increment/decrement: ++ and -- (prefix vs postfix)
  9) Stream operators: << and >> (i/o)
 10) Subscript, call, and other “syntax sugar” operators: [], ()
 11) Conversion operators and explicitness
 12) “Danger zone” operators you usually should not overload
 13) Common mistakes (with fixes)
 14) A checklist you can apply to your own types


  1) Why operator overloading exists
  ---------------------------------
  Operator overloading lets user-defined types participate in expressions with
  familiar syntax:
    Rational r = a + b;
    if (x < y) { ... }
    std::cout << r;

  The goal is not “cute” syntax. The goal is:
  - readability (the expression matches the math / domain)
  - correctness (operators obey expectations)
  - efficiency (avoid unnecessary temporaries)


  2) Big rules: what you can and cannot overload
  ---------------------------------------------
  You CAN overload most operators for user-defined types. But:

  You CANNOT overload:
    - .   (member access)
    - .*  (pointer-to-member access)
    - ::  (scope resolution)
    - ?:  (ternary conditional)
    - sizeof, typeid, alignof
    - the named casts: static_cast, dynamic_cast, const_cast, reinterpret_cast
    - co_await, co_yield, co_return are language keywords (not overloadable)

  You also cannot change:
    - operator precedence
    - operator arity (binary stays binary, unary stays unary)
    - short-circuiting semantics of built-in && and ||

  You can overload:
    - arithmetic (+ - * / %)
    - comparisons (== < <= > >= <=>)
    - assignment (= += -= ...)
    - indexing []
    - function call ()
    - stream insertion/extraction (<< >>)
    - dereference * and arrow -> (for smart pointers)
    - logical !, bitwise &,|,^,~, shifts << >> (careful)
    - comma operator (almost always a bad idea)
    - new/delete (advanced)


  3) Member vs non-member vs friend
  --------------------------------
  A quick decision guide:

  Prefer NON-MEMBER operators when:
  - the operator should allow implicit conversions on the left operand
    (e.g. `2 * rational` where 2 is an int)
  - you want symmetry: `a + b` treats both operands equally
  - the operation conceptually doesn’t “belong” to one operand

  Prefer MEMBER operators when:
  - the operator must be a member: operator=, operator[], operator(), operator->
  - it mutates the left operand: operator+=, operator*=, etc.

  Use FRIEND sparingly:
  - when a non-member needs access to private representation for efficiency
  - or when you want to define the operator inline in the class definition

  A very common pattern:
    - implement compound assignment as members (+=, -=, ...)
    - implement the corresponding binary operators as non-members in terms of
      compound assignment (operator+ uses operator+=)


  4) Return types and const correctness
  ------------------------------------
  Typical expectations:
  - operator+= returns *this by reference: T&
  - operator+ returns a NEW value: T
  - comparison operators return bool
  - operator[] returns a reference for mutable indexing, and a const reference
    for const objects

  Const correctness:
  - “query” operators should be const member functions
  - avoid returning references to temporaries


  5) Arithmetic operators: the preferred implementation style
  -----------------------------------------------------------
  Preferred:
    T& T::operator+=(const T& rhs);
    friend T operator+(T lhs, const T& rhs) { lhs += rhs; return lhs; }

  Why `operator+(T lhs, const T& rhs)` (by value for lhs)?
  - It naturally creates the result object (lhs)
  - It enables move elision / NRVO
  - It keeps a single implementation of the actual math in operator+=


  6) Assignment operators
  ----------------------
  - operator= is special: the compiler generates it if possible.
  - Provide it only when you need custom copy/move semantics.
  - Compound assignment should return T&.


  7) Comparisons and ordering
  --------------------------
  Modern C++ (C++20) encourages:
    auto operator<=>(const T&) const = default;   // if your fields support it
    bool operator==(const T&) const = default;

  Benefits:
  - Generates all comparisons consistently
  - Avoids mistakes in < vs == logic

  If you implement ordering manually:
  - Ensure strict weak ordering for < (or use <=>)
  - Keep == consistent with ordering (a==b implies !(a<b) && !(b<a))


  8) ++ and --
  -----------
  Prefix:
    T& operator++();
  Postfix (dummy int distinguishes it):
    T operator++(int);

  Conventional behavior:
  - prefix increments and returns the updated object by reference
  - postfix returns the old value by value


  9) Stream operators (<< and >>)
  -------------------------------
  Usually defined as non-members:
    std::ostream& operator<<(std::ostream&, const T&);
    std::istream& operator>>(std::istream&, T&);

  Tips:
  - Return the stream to allow chaining: `std::cout << a << b;`
  - For operator>>: validate input; leave object unchanged if parse fails


  10) [], (), ->
  -------------
  - operator[] must be a member.
  - Provide both const and non-const overloads if your type can be const.
  - operator() is useful for “callable objects” and multidimensional indexing.
  - operator-> is used by smart pointers to emulate pointer access.


  11) Conversion operators
  -----------------------
  C++ lets you define conversions:
    explicit operator bool() const;
    explicit operator double() const;

  Strong recommendations:
  - Prefer named functions (to_double()) when conversions could surprise.
  - Mark conversion operators explicit unless you are very sure.
  - `explicit operator bool()` is the classic safe conversion (for “is valid”).


  12) “Danger zone” operators
  --------------------------
  These are legal to overload, but often create confusing code:
  - operator&& and operator|| : you lose short-circuiting; evaluation order
    becomes like a normal function call.
  - operator, (comma): very surprising; almost never worth it.
  - bitwise operators for non-bitset-like types: can be misleading.
  - operator& (address-of): breaks expectations; avoid.
  - operator new/delete: advanced; do it only with clear ownership reasoning.


  13) Common mistakes
  ------------------
  Mistake: returning references to temporaries
    T& operator+(...) { T tmp; return tmp; }   // BUG
  Fix: return by value.

  Mistake: forgetting const correctness
    bool operator==(T& rhs);                  // can’t compare const objects
  Fix: `bool operator==(const T& rhs) const;`

  Mistake: defining operator+ as a member and losing symmetry
    Rational operator+(int) const;            // 2 + r fails
  Fix: define operator+ as a non-member (often friend) so conversions work on
       both sides.

  Mistake: inconsistent == and <
  Fix: implement == and <=> consistently (or default them if possible).

  Mistake: implementing many operators independently
  Fix: implement a small base set and derive others:
       += plus +; -= plus -; <=> generates <,<=,>,>=.


  14) Checklist
  ------------
  - Does the operator reflect real domain meaning (not just clever syntax)?
  - Is behavior unsurprising and consistent with built-in types?
  - Are operators symmetric when they should be?
  - Are return types conventional (T&, T, bool, stream&)?
  - Are overloads const-correct?
  - Are you avoiding dangerous operators unless justified?
  - If you provide < or <=>, is it consistent with ==?
  - Are you minimizing duplication by implementing compound assignment first?
*/

// <compare> and <=> are C++20 features.
// This guide can still compile in C++17 by providing fallback comparisons.
#if defined(__cpp_impl_three_way_comparison) && (__cpp_impl_three_way_comparison >= 201907L) &&    \
    defined(__cpp_lib_three_way_comparison)
#include <compare>
#define GUIDE_HAS_SPACESHIP 1
#else
#define GUIDE_HAS_SPACESHIP 0
#endif

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <string>
#include <utility>

namespace guide {

// A small Rational type demonstrates best practices:
// - invariants (normalized fraction, denominator positive)
// - compound assignment as members
// - binary operators as non-members in terms of compound assignment
// - comparisons via <=> (when available) and ==
// - stream insertion/extraction
// - explicit conversions
class Rational {
  public:
    using rep = std::int64_t;

    constexpr Rational() = default;
    constexpr Rational(rep n) : num_(n), den_(1) {}
    constexpr Rational(rep n, rep d) : num_(n), den_(d) { normalize(); }

    constexpr rep num() const { return num_; }
    constexpr rep den() const { return den_; }

    // Compound assignment operators
    constexpr Rational& operator+=(const Rational& rhs) {
        // a/b + c/d = (ad + bc) / bd
        // We keep it simple here; production code may want overflow handling.
        num_ = num_ * rhs.den_ + rhs.num_ * den_;
        den_ = den_ * rhs.den_;
        normalize();
        return *this;
    }

    constexpr Rational& operator-=(const Rational& rhs) {
        num_ = num_ * rhs.den_ - rhs.num_ * den_;
        den_ = den_ * rhs.den_;
        normalize();
        return *this;
    }

    constexpr Rational& operator*=(const Rational& rhs) {
        num_ *= rhs.num_;
        den_ *= rhs.den_;
        normalize();
        return *this;
    }

    constexpr Rational& operator/=(const Rational& rhs) {
        if (rhs.num_ == 0) {
            throw std::domain_error("Rational: division by zero");
        }
        num_ *= rhs.den_;
        den_ *= rhs.num_;
        normalize();
        return *this;
    }

    // Unary operators
    constexpr Rational operator+() const { return *this; }
    constexpr Rational operator-() const { return Rational{-num_, den_}; }

    // Comparisons
    // We normalize all values, so (num_, den_) is canonical for equality.
    friend constexpr bool operator==(const Rational& a, const Rational& b) {
        return a.num_ == b.num_ && a.den_ == b.den_;
    }

#if GUIDE_HAS_SPACESHIP
    friend constexpr std::strong_ordering operator<=>(const Rational& a, const Rational& b) {
        // Compare a.num/a.den vs b.num/b.den by cross multiplication.
        // Note: could overflow for large reps.
        const auto left = a.num_ * b.den_;
        const auto right = b.num_ * a.den_;
        if (left < right)
            return std::strong_ordering::less;
        if (left > right)
            return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }
#else
    friend constexpr bool operator<(const Rational& a, const Rational& b) {
        const auto left = a.num_ * b.den_;
        const auto right = b.num_ * a.den_;
        return left < right;
    }
    friend constexpr bool operator!=(const Rational& a, const Rational& b) { return !(a == b); }
    friend constexpr bool operator<=(const Rational& a, const Rational& b) { return !(b < a); }
    friend constexpr bool operator>(const Rational& a, const Rational& b) { return b < a; }
    friend constexpr bool operator>=(const Rational& a, const Rational& b) { return !(a < b); }
#endif

    // Explicit conversions
    explicit constexpr operator double() const {
        return static_cast<double>(num_) / static_cast<double>(den_);
    }

    // "Safe bool" conversion is a common pattern in resource types.
    // For Rational, it's arguably not necessary, but shows the idiom.
    explicit constexpr operator bool() const { return num_ != 0; }

    // Stream insertion/extraction
    friend std::ostream& operator<<(std::ostream& os, const Rational& r) {
        if (r.den_ == 1)
            return os << r.num_;
        return os << r.num_ << '/' << r.den_;
    }

    friend std::istream& operator>>(std::istream& is, Rational& out) {
        // Parse either "n" or "n/d".
        // If parse fails, keep out unchanged.
        rep n{};
        rep d{1};

        // Read numerator
        if (!(is >> n))
            return is;

        // Check if next char is '/'
        if (is.peek() == '/') {
            is.get();
            if (!(is >> d)) {
                is.setstate(std::ios::failbit);
                return is;
            }
        }

        try {
            Rational tmp{n, d};
            out = tmp;
        } catch (...) {
            // For invalid denominator (0), mark fail and keep out.
            is.setstate(std::ios::failbit);
        }

        return is;
    }

  private:
    rep num_ = 0;
    rep den_ = 1;

    constexpr void normalize() {
        if (den_ == 0) {
            throw std::domain_error("Rational: denominator is zero");
        }
        if (den_ < 0) {
            den_ = -den_;
            num_ = -num_;
        }
        const rep g = std::gcd(num_ < 0 ? -num_ : num_, den_);
        num_ /= g;
        den_ /= g;
    }
};

// Non-member binary arithmetic operators, implemented in terms of compound ops.
// This preserves symmetry and enables implicit conversions on both sides.
constexpr Rational operator+(Rational lhs, const Rational& rhs) {
    lhs += rhs;
    return lhs;
}
constexpr Rational operator-(Rational lhs, const Rational& rhs) {
    lhs -= rhs;
    return lhs;
}
constexpr Rational operator*(Rational lhs, const Rational& rhs) {
    lhs *= rhs;
    return lhs;
}
constexpr Rational operator/(Rational lhs, const Rational& rhs) {
    lhs /= rhs;
    return lhs;
}

// A Vector2 shows the same patterns and a typical operator[] design.
struct Vec2 {
    double x{};
    double y{};

    constexpr Vec2& operator+=(const Vec2& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    constexpr Vec2& operator-=(const Vec2& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    constexpr Vec2& operator*=(double s) {
        x *= s;
        y *= s;
        return *this;
    }
    constexpr Vec2& operator/=(double s) {
        x /= s;
        y /= s;
        return *this;
    }

    // Indexing operator: member-only.
    // Provide const and non-const overloads.
    constexpr double& operator[](std::size_t i) {
        if (i == 0)
            return x;
        if (i == 1)
            return y;
        throw std::out_of_range("Vec2 index must be 0 or 1");
    }
    constexpr const double& operator[](std::size_t i) const {
        if (i == 0)
            return x;
        if (i == 1)
            return y;
        throw std::out_of_range("Vec2 index must be 0 or 1");
    }
};

constexpr Vec2 operator+(Vec2 lhs, const Vec2& rhs) {
    lhs += rhs;
    return lhs;
}
constexpr Vec2 operator-(Vec2 lhs, const Vec2& rhs) {
    lhs -= rhs;
    return lhs;
}
constexpr Vec2 operator*(Vec2 v, double s) {
    v *= s;
    return v;
}
constexpr Vec2 operator*(double s, Vec2 v) {
    v *= s;
    return v;
}
constexpr Vec2 operator/(Vec2 v, double s) {
    v /= s;
    return v;
}

// Comparisons for Vec2 are domain-dependent.
// There is no universally correct "ordering" for vectors.
// Provide equality only unless you have a clear ordering definition.
constexpr bool operator==(const Vec2& a, const Vec2& b) { return a.x == b.x && a.y == b.y; }

std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    return os << '(' << v.x << ", " << v.y << ')';
}

// A small counter type to demonstrate ++/--.
class Counter {
  public:
    explicit constexpr Counter(int v = 0) : v_(v) {}

    // Prefix
    constexpr Counter& operator++() {
        ++v_;
        return *this;
    }
    // Postfix
    constexpr Counter operator++(int) {
        Counter old = *this;
        ++(*this);
        return old;
    }

    constexpr Counter& operator--() {
        --v_;
        return *this;
    }
    constexpr Counter operator--(int) {
        Counter old = *this;
        --(*this);
        return old;
    }

    constexpr int value() const { return v_; }

#if GUIDE_HAS_SPACESHIP
    friend constexpr bool operator==(const Counter&, const Counter&) = default;
    friend constexpr auto operator<=>(const Counter&, const Counter&) = default;
#else
    friend constexpr bool operator==(const Counter& a, const Counter& b) { return a.v_ == b.v_; }
    friend constexpr bool operator<(const Counter& a, const Counter& b) { return a.v_ < b.v_; }
    friend constexpr bool operator!=(const Counter& a, const Counter& b) { return !(a == b); }
    friend constexpr bool operator<=(const Counter& a, const Counter& b) { return !(b < a); }
    friend constexpr bool operator>(const Counter& a, const Counter& b) { return b < a; }
    friend constexpr bool operator>=(const Counter& a, const Counter& b) { return !(a < b); }
#endif

  private:
    int v_;
};

std::ostream& operator<<(std::ostream& os, const Counter& c) { return os << c.value(); }

// A callable object demonstrating operator().
struct Linear {
    double m{};
    double b{};

    constexpr double operator()(double x) const { return m * x + b; }
};

// A minimal "handle" type showing explicit bool conversion.
// In real code this could wrap a file descriptor, socket, etc.
class Handle {
  public:
    constexpr Handle() = default;
    explicit constexpr Handle(int id) : id_(id) {}

    constexpr int id() const { return id_; }

    // This enables: if (h) { ... }
    explicit constexpr operator bool() const { return id_ >= 0; }

#if GUIDE_HAS_SPACESHIP
    friend constexpr bool operator==(const Handle&, const Handle&) = default;
#else
    friend constexpr bool operator==(const Handle& a, const Handle& b) { return a.id_ == b.id_; }
#endif

  private:
    int id_ = -1;
};

std::ostream& operator<<(std::ostream& os, const Handle& h) {
    if (h)
        return os << "Handle(" << h.id() << ")";
    return os << "Handle(invalid)";
}

} // namespace guide

int main() {
    using guide::Counter;
    using guide::Handle;
    using guide::Linear;
    using guide::Rational;
    using guide::Vec2;

    // Demonstrate Rational operators
    Rational a{1, 3};
    Rational b{2, 5};
    Rational c = a + b;
    Rational d = a * b;
    Rational e = Rational{2} + a; // implicit conversion on lhs works
    Rational f = a + Rational{2}; // and on rhs

    std::cout << "Rational\n";
    std::cout << "  a=" << a << " b=" << b << "\n";
    std::cout << "  a+b=" << c << "  a*b=" << d << "\n";
    std::cout << "  2+a=" << e << "  a+2=" << f << "\n";
    std::cout << "  (a < b)=" << (a < b) << "  (a == b)=" << (a == b) << "\n";
    std::cout << "  double(a)=" << static_cast<double>(a) << "\n";

    // Demonstrate Vec2
    Vec2 v{1.0, 2.0};
    Vec2 w{0.5, -3.0};
    Vec2 u = v + w;
    Vec2 s = 2.0 * v;
    std::cout << "\nVec2\n";
    std::cout << "  v=" << v << " w=" << w << "\n";
    std::cout << "  v+w=" << u << "  2*v=" << s << "\n";
    std::cout << "  v[0]=" << v[0] << " v[1]=" << v[1] << "\n";

    // Demonstrate ++/--
    Counter cnt{10};
    std::cout << "\nCounter\n";
    std::cout << "  cnt=" << cnt << "\n";
    std::cout << "  cnt++ returns " << (cnt++) << ", cnt now " << cnt << "\n";
    std::cout << "  ++cnt returns " << (++cnt) << ", cnt now " << cnt << "\n";

    // Demonstrate operator()
    Linear line{2.0, 1.0};
    std::cout << "\nLinear\n";
    std::cout << "  f(x)=2x+1, f(3)=" << line(3.0) << "\n";

    // Demonstrate explicit bool
    Handle h1;
    Handle h2{42};
    std::cout << "\nHandle\n";
    std::cout << "  h1=" << h1 << " h2=" << h2 << "\n";
    std::cout << "  if (h1) ... => " << static_cast<bool>(h1) << "\n";
    std::cout << "  if (h2) ... => " << static_cast<bool>(h2) << "\n";

    // A tiny note about parsing with operator>>.
    // Example manual test:
    //   echo "3/4" | ./guide
    // We don't actually read stdin in this demo to keep it deterministic.

    return 0;
}
