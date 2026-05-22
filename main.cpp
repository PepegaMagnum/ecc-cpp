#include <iostream>
#include "include/math_operations.h"
#include "include/Point.h"
#include "include/Curve.h"

int main() {
    mpz_class a, b, fz;
    uint32_t m = 16;

    mpz_set_str(a.get_mpz_t(), "2905", 16);
    mpz_set_str(b.get_mpz_t(), "886f", 16);
    mpz_set_str(fz.get_mpz_t(), "10000000000101011", 2);

    Point G("ba04", "9b3b");
    G.print();
    Curve myCurve(a.get_mpz_t(), b.get_mpz_t(), m, fz.get_mpz_t());

    bool isGenOnCurve = myCurve.isPointOnCurve(G);
    std::cout << isGenOnCurve << std::endl;



    return 0;
}