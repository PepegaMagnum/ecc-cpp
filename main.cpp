#include "include/math_operations.h"
#include "include/Point.h"
#include "include/Curve.h"
#include "RhoPollard_lib/include/RhoPollard.h"
// #include <random>

int main() {
    mpz_class a, b, fz;
    uint32_t m = 4;
    mpz_class n("B", 16);

    // std::random_device rd;
    // if (rd.entropy() > 0) {
    //     std::cout << "Entropia: " << rd.entropy() << std::endl;
    // }
    //
    // std::mt19937_64 generator(rd());
    mpz_class a_i;
    a_i = 7;

    mpz_set_str(a.get_mpz_t(), "8", 16);
    mpz_set_str(b.get_mpz_t(), "9", 16);
    mpz_set_str(fz.get_mpz_t(), "13", 16);

    Point G("8", "1");

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
        }
    }
    std::cout << "No result after 1000 runs of pollard's rho." << std::endl;
    return 1;
}