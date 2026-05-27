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
    // printf("isGenOnCurve = %d\n", isGenOnCurve);
    Point G2;

    // mpz_class x;
    // mpz_class invX;
    // mpz_class xInvX;
    //
    // mpz_set_str(x.get_mpz_t(), "ba04", 16);
    // mpz_set(invX.get_mpz_t(), x.get_mpz_t());
    //
    // binInv(invX.get_mpz_t(), fz.get_mpz_t(), 16);
    // gmp_printf("x = 0x%Zx\n", invX.get_mpz_t());
    // std::cout << "invx = " << invX.get_mpz_t() << std::endl;
    // binMult(x, invX, xInvX, 16);
    // std::cout << "xInvX = " << xInvX << std::endl;

    G2 = myCurve.pointDoubling(G);
    G2.print();
    bool isGenOnCurve2 = myCurve.isPointOnCurve(G2);
    printf("isGenOnCurve2 = %d\n", isGenOnCurve2);

    return 0;
}