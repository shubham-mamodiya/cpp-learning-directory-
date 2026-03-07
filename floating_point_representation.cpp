
#include <cmath>
#include <iostream>
#include <limits>

int main() {
    double pos_zero = 0.0;
    double neg_zero = -0.0;

    double pos_inf = std::numeric_limits<double>::infinity();
    double neg_inf = -std::numeric_limits<double>::infinity();

    double nan = std::numeric_limits<double>::quiet_NaN();

    std::cout << "Values:\n";
    std::cout << "+0  = " << pos_zero << '\n';
    std::cout << "-0  = " << neg_zero << '\n';
    std::cout << "+inf = " << pos_inf << '\n';
    std::cout << "-inf = " << neg_inf << '\n';
    std::cout << "NaN = " << nan << '\n';

    std::cout << "\nChecking signs:\n";
    std::cout << "signbit(+0) = " << std::signbit(pos_zero) << '\n';
    std::cout << "signbit(-0) = " << std::signbit(neg_zero) << '\n';

    std::cout << "\nDivision behavior:\n";
    std::cout << "1 / +0 = " << 1 / pos_zero << '\n';
    std::cout << "1 / -0 = " << 1 / neg_zero << '\n';

    std::cout << "\nInfinity checks:\n";
    std::cout << "isinf(+inf) = " << std::isinf(pos_inf) << '\n';
    std::cout << "isinf(-inf) = " << std::isinf(neg_inf) << '\n';

    std::cout << "\nNaN checks:\n";
    std::cout << "isnan(NaN) = " << std::isnan(nan) << '\n';

    std::cout << "\nNaN comparison:\n";
    std::cout << "NaN == NaN → " << (nan == nan) << '\n';

    std::cout << "0 / 0 = " << 0.0 / 0.0 << '\n';
}
