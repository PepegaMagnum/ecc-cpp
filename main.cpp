#include <iostream>
#include "include/math_operations.h"
#include "include/Point.h"
#include "include/Curve.h"

int main() {

    // field parameters
    mpz_t fz_int, a, b;
    uint32_t m = 32;
    bitset32Vec fz;
    mpz_set_ui(fz_int, 1);

    hex_string_to_mpz(a, "b46574af");
    hex_string_to_mpz(b, "f6c71ed1");

    mpz_mul_2exp(fz_int, fz_int, m);
    mpz_add_ui(fz_int, fz_int, std::stoi("10001101", nullptr, 2));

    Curve myCurve = Curve(a, b, m, fz_int);

    printBitset32Vec(myCurve.getFz(), true);

    Point G = Point("32682f63","42b5226d");

    std::cout <<myCurve.isPointOnCurve(G);

    return 0;
}