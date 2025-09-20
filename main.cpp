#include <iostream>
#include "include/math_operations.h"
#include "include/Point.h"
#include "include/Curve.h"

int main() {
    uint32_t a = 83, b = 202 , fz = 283, m = 8;

    std::cout <<"Modular multiplicative inverse of " << a <<" and " << b << " is " << modInv(a, b) <<std::endl;

    std::cout << "Binary Addition: " <<a <<" + " <<b <<" = " <<binAdd(a, b) <<std::endl;

    uint32_t mulres = binMult(a, b);

    std::cout << "Binary Multiplication: " <<a <<" * " <<b << " = " <<mulres <<std::endl;

    std::cout << "Binary Square of " <<a <<" is " << binSquare(a) <<std::endl;
    std:: cout << "Reducing " <<std::bitset<32>(mulres) <<" by " 
        << std::bitset<32>(fz) << " equals " <<binReduc(mulres, fz, m) <<std::endl;
    std::cout << "Inversion of " << b <<" " << fz <<" is " <<binInv(b, fz) <<std::endl;

    uint32_t f = (1<<163) + std::stoi("11001001");
    std::cout << "f in int: " <<f <<std::endl;
    std::cout << "f in binary: " << std::bitset<32>(f) << std::endl;

    Curve k163 = Curve(1, 1, 163, f);

    uint32_t x = std::stoul("02fe13c0537bbc11acaa07d793de4e6d5e5c94eee8", nullptr, 16);
    uint32_t y = std::stoul("0289070fb05d38ff58321f2e800536d538ccdaa3d9", nullptr, 16);

    Point g1 = Point(x, y);

    std::cout << "Point g1 is on the Curve: " <<k163.isPointOnCurve(g1) <<std::endl;



    return 0;
}