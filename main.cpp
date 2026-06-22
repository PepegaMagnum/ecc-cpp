#include <iostream>
#include <unistd.h>

#include "include/math_operations.h"
#include "include/Point.h"
#include "include/Curve.h"
#include "include/RhoPollard.h"

static std::string readParameter(const char* prompt) {
    if (isatty(0)) std::cout << prompt << std::flush;
    std::string tok;
    if (!(std::cin >> tok)) {
        std::cerr << "ERROR: ran out of input while reading: " << prompt << "\n";
        std::exit(1);
    }
    return tok;
}

int main() {

    uint32_t m = (uint32_t)std::stoul(readParameter("m (decimal): "));
    std::string a_hex  = readParameter("a  (hex): ");
    std::string b_hex  = readParameter("b  (hex): ");
    std::string fz_hex = readParameter("fz (hex): ");
    std::string Gx_hex = readParameter("Gx (hex): ");
    std::string Gy_hex = readParameter("Gy (hex): ");
    std::string n_dec  = readParameter("n  (decimal): ");
    std::string k_dec  = readParameter("k  (decimal, target = k*G): ");

    uint32_t fieldDivider = 3;

    mpz_class a, b, fz, n, k;
    mpz_set_str(a.get_mpz_t(),  a_hex.c_str(),  16);
    mpz_set_str(b.get_mpz_t(),  b_hex.c_str(),  16);
    mpz_set_str(fz.get_mpz_t(), fz_hex.c_str(), 16);
    mpz_set_str(n.get_mpz_t(),  n_dec.c_str(),  10);
    mpz_set_str(k.get_mpz_t(),  k_dec.c_str(),  10);

    Curve myCurve(a.get_mpz_t(), b.get_mpz_t(), m, fz.get_mpz_t());
    Point G(Gx_hex.c_str(), Gy_hex.c_str());

    if (!myCurve.isPointOnCurve(G)) {
        std::cerr << "ERROR: generator G is not on the curve. "
        "Check a, b, fz, Gx, Gy" << std::endl;
        return 1;
    }

    Point kG = myCurve.pointMultiplication(G, k.get_mpz_t());
    std::cout << "Computing target Q = " << k << " * G" <<std::endl;
    kG.print();

    if (!myCurve.isPointOnCurve(kG)) {
        std::cerr << "ERROR: k*G is not on the curve." << std::endl;
        return 1;
    }
    std::cout << "Q is on the curve." << std::endl;


    RhoPollard rhoPol(myCurve, n, fieldDivider, 1, 0);
    mpz_class result;
    Point pointResult;

    for (int i = 0; i < 1000; i++) {
        rhoPol.computeLog(G, kG, result.get_mpz_t());
        if (result != 0) {
            pointResult = myCurve.pointMultiplication(G, result.get_mpz_t());

            if (pointResult == kG) {
                std::cout << "Valid result found: " << result << std::endl;
                return 0;
            }
            std::cout << "Algorithm executed but no valid result was found: " <<std::endl;
            std::cout << "Calculated result: " <<result << std::endl;

            std::cout << "Q: " <<std::endl;
            kG.print();
            std::cout << "Calculated Q: " <<std::endl;
            pointResult.print();
            return 1;
        }
    }
    std::cout << "No result after 1000 runs of pollard's rho." << std::endl;
    return 1;
}