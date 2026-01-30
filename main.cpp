#include <iostream>
#include "include/math_operations.h"
#include "include/Point.h"
#include "include/Curve.h"

int main() {
    bitset32Vec fz;

    mpz_t fz_int, a, b;
    uint32_t m = 163;
    mpz_set_ui(fz_int, 1);
    mpz_set_ui(a, 1);
    mpz_set_ui(b, 1);
    mpz_mul_2exp(fz_int, fz_int, 163);
    mpz_add_ui(fz_int, fz_int, std::stoi("11001001", nullptr, 2));

    // std::cout << fz_int << std::endl;
    Curve k163 = Curve(a, b, m, fz_int);

    Point G = Point("02fe13c0537bbc11acaa07d793de4e6d5e5c94eee8",
        "0289070fb05d38ff58321f2e800536d538ccdaa3d9");

    k163.isPointOnCurve(G);

    // Point g1 = Point(x, y);
    //
    // std::cout << "Point g1 is on the Curve: " <<k163.isPointOnCurve(g1) <<std::endl;
    return 0;
}