
/*
===============================================================================
                                LAMBDAS IN C++
===============================================================================

This file is an extensive, comment-based guide to lambda functions in modern C++.
You can treat it like living documentation.

Recommended standard: C++17 or later
-------------------------------------------------------------------------------
WHAT IS A LAMBDA?
-------------------------------------------------------------------------------

A lambda is an *anonymous function object*.
That means:
- It has no name
- It can be created inline
- It can capture variables from its surrounding scope

General syntax:

[capture](parameters) -> return_type {
    body
};

Notes:
- return_type is often omitted (auto-deduced)
- parameters can use auto (generic lambdas)
- capture controls access to outer variables

===============================================================================
*/

#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main() {
    using std::cout;
    using std::string;
    using std::vector;

    /*
    ===========================================================================
    1. BASIC LAMBDA (NO CAPTURE)
    ===========================================================================
    */

    // A simple lambda that adds two integers
    auto add{[](int a, int b) { return a + b; }};

    cout << "add(2, 3) = " << add(2, 3) << '\n';

    /*
    - `auto` is used because lambdas have unique, unnamed types
    - This lambda captures NOTHING
    */

    /*
    ===========================================================================
    2. IMPLICIT RETURN TYPE
    ===========================================================================
    */

    auto square{[](int x) {
        return x * x; // return type deduced as int
    }};

    cout << "square(5) = " << square(5) << '\n';

    /*
    ===========================================================================
    3. EXPLICIT RETURN TYPE (RARE, BUT IMPORTANT)
    ===========================================================================
    */

    auto divide{[](int a, int b) -> double { return static_cast<double>(a) / b; }};

    cout << "divide(5, 2) = " << divide(5, 2) << '\n';

    /*
    Use explicit return types when:
    - Multiple return statements return different types
    - You want to force precision
    */

    /*
    ===========================================================================
    4. CAPTURE CLAUSE BASICS
    ===========================================================================
    */

    int factor{3};

    // Capture by VALUE
    auto multiply_by_value{[factor](int x) { return x * factor; }};

    // Capture by REFERENCE
    auto multiply_by_reference{[&factor](int x) { return x * factor; }};

    cout << "value capture: " << multiply_by_value(10) << '\n';
    cout << "reference capture: " << multiply_by_reference(10) << '\n';

    factor = 5;

    // Value capture does NOT change
    cout << "value capture after change: " << multiply_by_value(10) << '\n';
    // Reference capture DOES change
    cout << "reference capture after change: " << multiply_by_reference(10) << '\n';

    /*
    ===========================================================================
    5. DEFAULT CAPTURES (USE CAREFULLY)
    ===========================================================================
    */

    int a{1};
    int b{2};

    auto by_value{[=]() { return a + b; }};

    auto by_reference{[&]() {
        a++;
        b++;
    }};

    cout << "by_value sum = " << by_value() << '\n';
    by_reference();
    cout << "a = " << a << ", b = " << b << '\n';

    /*
    Best practice:
    - Prefer explicit captures [x, &y]
    - Avoid [=] and [&] in large lambdas
    */

    /*
    ===========================================================================
    6. LAMBDAS WITH STL ALGORITHMS
    ===========================================================================
    */

    vector<int> numbers{1, 2, 3, 4, 5, 6};

    // Print using for_each
    std::for_each(numbers.begin(), numbers.end(), [](int n) { cout << n << ' '; });
    cout << '\n';

    // Count even numbers
    auto even_count{
        std::count_if(numbers.begin(), numbers.end(), [](int n) { return n % 2 == 0; })};

    cout << "Even numbers: " << even_count << '\n';

    /*
    ===========================================================================
    7. GENERIC LAMBDAS (auto PARAMETERS)
    ===========================================================================
    */

    auto max_value{[](auto x, auto y) { return (x > y) ? x : y; }};

    cout << "max(3, 7) = " << max_value(3, 7) << '\n';
    cout << "max(2.5, 1.1) = " << max_value(2.5, 1.1) << '\n';

    /*
    Generic lambdas behave like function templates.
    Introduced in C++14.
    */

    /*
    ===========================================================================
    8. LAMBDAS AS FUNCTION PARAMETERS
    ===========================================================================
    */

    auto apply{[](int x, const std::function<int(int)>& fn) { return fn(x); }};

    cout << "apply square: " << apply(4, square) << '\n';

    /*
    Note:
    - std::function adds overhead
    - Prefer templates when performance matters
    */

    /*
    ===========================================================================
    9. IMMEDIATELY INVOKED LAMBDA (IIFE STYLE)
    ===========================================================================
    */

    int result{[] {
        int x{10};
        int y{20};
        return x + y;
    }()};

    cout << "Immediate lambda result = " << result << '\n';

    /*
    Useful for:
    - Scoped initialization
    - Complex const initialization
    */

    /*
    ===========================================================================
    10. MUTABLE LAMBDAS
    ===========================================================================
    */

    int counter{0};

    auto increment{[counter]() mutable {
        counter++;
        return counter;
    }};

    cout << "increment = " << increment() << '\n';
    cout << "increment again = " << increment() << '\n';

    /*
    Without `mutable`, captured-by-value variables are const.
    */

    /*
    ===========================================================================
    11. RETURNING LAMBDAS
    ===========================================================================
    */

    auto make_multiplier{[](int m) { return [m](int x) { return x * m; }; }};

    auto times_three{make_multiplier(3)};
    cout << "3 * 7 = " << times_three(7) << '\n';

    /*
    ===========================================================================
    12. LAMBDA VS FUNCTION VS FUNCTOR
    ===========================================================================
    */

    /*
    Prefer lambdas when:
    - Logic is short
    - Used locally
    - Passed to algorithms

    Prefer functions when:
    - Logic is reusable
    - Needs a name

    Prefer functors when:
    - Stateful and reused
    - Performance critical
    */

    /*
    ===========================================================================
    13. COMMON MISTAKES
    ===========================================================================
    */

    /*
    ❌ Capturing dangling references
    ❌ Overusing std::function
    ❌ Very large lambdas
    ❌ Default captures in complex scopes
    */

    /*
    ===========================================================================
    14. MODERN BEST PRACTICES (SUMMARY)
    ===========================================================================
    */

    /*
    ✔ Use auto for lambdas
    ✔ Prefer explicit captures
    ✔ Use list initialization {}
    ✔ Keep lambdas small
    ✔ Use generic lambdas instead of templates when local
    */

    return 0;
}
