#include <iostream>
#include "include/math_operations.h"
#include "include/Point.h"
#include "include/Curve.h"

int main() {
    //std::vector<uint32_t> a = {83}, b = {202} , fz = {283};

    const uint32_t m = 8, W=32;
    const auto t = ceil(double(m) / W);

    std::vector<std::bitset<32>> a(t, std::bitset<32>(0));
    std::vector<std::bitset<32>> b(t, std::bitset<32>(0));

    a.reserve(1);
    b.reserve(1);

    a[0] = std::bitset<W>(83);
    b[0] = std::bitset<W>(202);

    auto addResult = binAdd(a, b, m);

    std::cout << "Binary Addition: " <<a[0] <<" + " <<b[0] <<" = " <<addResult[0] <<std::endl;

    auto mulres = binMult(a, b, m);

    std::cout << "Binary Multiplication: " <<a[0] <<" * " <<b[0] << " = " <<mulres[0] <<std::endl;
    auto precomputedTable = computeExpansionTable();
    auto aSquared = binSquare(a, precomputedTable);
    std::cout << "Binary Square of " <<a[0] <<" is " << aSquared[1] <<aSquared[0] <<std::endl;
    // std:: cout << "Reducing " <<std::bitset<32>(mulres) <<" by "
    //     << std::bitset<32>(fz) << " equals " <<binReduc(mulres, fz, m) <<std::endl;
    // std::cout << "Inversion of " << b <<" " << fz <<" is " <<binInv(b, fz) <<std::endl;
    //
    // uint32_t f = (1<<163) + std::stoi("11001001");
    // std::cout << "f in int: " <<f <<std::endl;
    // std::cout << "f in binary: " << std::bitset<32>(f) << std::endl;
    //
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