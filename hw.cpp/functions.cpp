#include "Header.hpp"

std::vector<double> solve(double a, double b, double c) {
    if (a == 0) {
        throw std::invalid_argument("Coefficient 'a' can't be zero.");
    }
    std::vector<double>res;
    double diskr = b * b - 4 * a * c;
    if (diskr < 0) {
        std::exception ex("The roots are not real, D < 0\n");
        std::cout << ex.what();

        exit(1);
    }
    res.push_back((-b + sqrt(diskr)) / (2 * a));
    res.push_back((-b - sqrt(diskr)) / (2 * a));
    return res;
}