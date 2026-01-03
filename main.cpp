#include <iostream>
#include "include/math_operations.h"
#include "include/Point.h"
#include "include/Curve.h"

int main() {
    bitset32Vec fz;

    mpz_t fz_int;
    mpz_set_ui(fz_int, 1);
    mpz_mul_2exp(fz_int, fz_int, 163);
    mpz_add_ui(fz_int, fz_int, std::stoi("11001001", nullptr, 2));

    // std::cout << fz_int << std::endl;

    mpz_to_bitset32Vec(fz_int, fz);
    // for (int i = fz.size(); i >= 0; i--) {
    //     std::cout << fz[i];
    // }

    // Curve k163 = Curve(1, 1, 163, f);
    //
    // uint32_t x = std::stoul("02fe13c0537bbc11acaa07d793de4e6d5e5c94eee8", nullptr, 16);
    // uint32_t y = std::stoul("0289070fb05d38ff58321f2e800536d538ccdaa3d9", nullptr, 16);
    //
    // Point g1 = Point(x, y);
    //
    // std::cout << "Point g1 is on the Curve: " <<k163.isPointOnCurve(g1) <<std::endl;
    return 0;
}