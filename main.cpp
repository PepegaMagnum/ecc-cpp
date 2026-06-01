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
    // G.print();
    Curve myCurve(a.get_mpz_t(), b.get_mpz_t(), m, fz.get_mpz_t());

    // bool isGenOnCurve = myCurve.isPointOnCurve(G);
    // printf("isGenOnCurve = %d\n", isGenOnCurve);

    Point G2;
    G2 = myCurve.pointDoubling(G);
    // G2.print();
    bool isGenOnCurve2 = myCurve.isPointOnCurve(G2);
    // printf("isGenOnCurve2 = %d\n", isGenOnCurve2);

    Point G3;
    G3 = myCurve.pointAddition(G, G2);
    // G3.print();

    bool isG3OnCurve = myCurve.isPointOnCurve(G3);
    // printf("isG3OnCurve = %d\n", isG3OnCurve);

    mpz_class n(3);

    Point g3Mult = myCurve.pointMultiplication(G,n.get_mpz_t());
    // G3.print();
    g3Mult.print();
    // bool isG3MultOnCurve = myCurve.isPointOnCurve(g3Mult);
    // printf("isG3MultOnCurve = %d\n", isG3MultOnCurve);

    return 0;
}