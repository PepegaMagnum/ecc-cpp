#include <iostream>
#include "include/math_operations.h"
#include "include/Point.h"
#include "include/Curve.h"
#include "include/RhoPollard.h"

int main() {
    mpz_class a, b, fz;
    uint32_t m = 16;

    std::random_device rd;
    if (rd.entropy() > 0) {
        std::cout << "Entropia: " << rd.entropy() << std::endl;
    }

    std::mt19937_64 generator(rd());
    mpz_class a_i;
    std::uniform_int_distribution<uint32_t> dis(1, 65920-1);
    mpz_set_d(a_i.get_mpz_t(), dis(generator));

    mpz_set_str(a.get_mpz_t(), "2905", 16);
    mpz_set_str(b.get_mpz_t(), "886f", 16);
    mpz_set_str(fz.get_mpz_t(), "10000000000101011", 2);

    Point G("ba04", "9b3b");

    // G.print();
    Curve myCurve(a.get_mpz_t(), b.get_mpz_t(), m, fz.get_mpz_t());
    Point nG = myCurve.pointMultiplication(G, a_i.get_mpz_t());
    // nG.print();
    std::cout << "Multiplying by " <<a_i << std::endl;

    bool nGOnCurve = myCurve.isPointOnCurve(nG);
    std::cout << nGOnCurve << std::endl;

    Point nGAdd = G;

    for (mpz_class i = 0; i < a_i-1; i++ ) {
        // std::cout << "Addition: "<< i << std::endl;
        nGAdd = myCurve.pointAddition(nGAdd, G);
    }

    nG.print();
    nGAdd.print();

    if (!(nG == nGAdd)) {
        std::cout << "NOT EQUAL" << std::endl;
    }

    // mpz_class result;
    // Point pointResult;
    //
    // RhoPollard rhoPol(myCurve, 65920);

    // for (int i = 0; i < 100; i++) {
    //     rhoPol.computeLog(G, nG, result.get_mpz_t(), generator);
    //     // if (mpz_cmp_d(result.get_mpz_t(), -1) != 0) {
    //     //     pointResult = myCurve.pointMultiplication(G, result.get_mpz_t());
    //     //     if (pointResult == nG)
    //     //         break;
    //     // }
    // }

    return 0;
}