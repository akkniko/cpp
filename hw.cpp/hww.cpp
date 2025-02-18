#include "Header.hpp"

int main() {
    double a, b, c;
    std::cout << "Enter coefficients:" << std::endl;
    std::cout << "a, b, c" << std::endl;
    std::cin >> a >> b >> c;

    if (a == 0) {
        std::cout << "This is not a quadratic equation." << std::endl;
        return 1;
    }

    std::cout << "The equation turned out:" << std::endl;
    std::cout << a << "x^2 + " << b << "x + " << c << " = 0" << std::endl;

    std::vector<double> res = solve(a, b, c);

    std::cout << "Roots of the equation: " << std::endl;
    if (res.empty()) {
        std::cout << "No real roots." << std::endl;
    }
    else {
        for (const auto& root : res) {
            std::cout << root << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}