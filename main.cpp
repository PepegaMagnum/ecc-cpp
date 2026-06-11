#include <iostream>
#include "include/math_operations.h"
#include "include/Point.h"
#include "include/Curve.h"
#include "include/RhoPollard.h"

int main() {
    mpz_class a, b, fz;
    uint32_t m = 163;
    mpz_class n("04000000000000000000020108a2e0cc0d99f8a5ef", 16);

    std::random_device rd;
    if (rd.entropy() > 0) {
        std::cout << "Entropia: " << rd.entropy() << std::endl;
    }

    std::mt19937_64 generator(rd());
    mpz_class a_i;
    a_i = 2342134;

    mpz_set_str(a.get_mpz_t(), "1", 16);
    mpz_set_str(b.get_mpz_t(), "1", 16);
    mpz_set_str(fz.get_mpz_t(), "800000000000000000000000000000000000000C9", 16);

    Point G("02fe13c0537bbc11acaa07d793de4e6d5e5c94eee8", "0289070fb05d38ff58321f2e800536d538ccdaa3d9");

    // G.print();
    Curve myCurve(a.get_mpz_t(), b.get_mpz_t(), m, fz.get_mpz_t());
    Point nG = myCurve.pointMultiplication(G, a_i.get_mpz_t());
    // nG.print();
    std::cout << "Multiplying by " <<a_i << std::endl;

    nG.print();

    if (myCurve.isPointOnCurve(nG)) {
        std::cout << "n*G is on the curve" << std::endl;
    }

    mpz_class result;
    Point pointResult;
    uint32_t fieldDivider = 3;

    RhoPollard rhoPol(myCurve, n, fieldDivider);

    for (int i = 0; i < 1000; i++) {
        rhoPol.computeLog(G, nG, result.get_mpz_t());
        std::cout << "RhoPollard iteration: " << i << std::endl;
        std::cout << "result: " << result << std::endl;
        if (result != 0) {
            pointResult = myCurve.pointMultiplication(G, result.get_mpz_t());

            if (pointResult == nG) {
                std::cout << "Valid result found: " << result << std::endl;
                return 0;
            }
            std::cout << "Algorithm executed but no valid result was found: " <<std::endl;
            std::cout << "Calculated result: " <<result << std::endl;

            std::cout << "nG: " <<std::endl;
            nG.print();
            std::cout << "Calculated nG: " <<std::endl;
            pointResult.print();
            return 1;
        }
    }
    return 1;
}