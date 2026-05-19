#include <iostream>
#include "include/math_operations.h"
#include "include/Point.h"
#include "include/Curve.h"

int main() {

    mpz_class a(83);
    mpz_class b(202);
    mpz_class c;

    binMult(a.get_mpz_t(), b.get_mpz_t(), c.get_mpz_t(), 8);

    std::cout << c << std::endl;
    binSquare(c.get_mpz_t(), a.get_mpz_t());

    std::cout << c << std::endl;

    return 0;
}