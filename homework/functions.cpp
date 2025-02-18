#include "Header.hpp"

std::vector<double> solve(double a, double b, double c) {
    if (a == 0) {
        throw invalid
    }
    std::vector<double>res;
    double diskr = b * b - 4 * a * c;

    if (diskr < 0) {
        std::exception ex("Roots are not real, diskriminant < 0");
        cout << ex.what();
        exit(1);
    }
    double diskr = b * b - 4 * a * c;
    res.push_back((-b + sqrt(diskr)) / (2 * a));
    res.push_back((-b - sqrt(diskr)) / (2 * a));
    return res;
}