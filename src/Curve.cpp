//
// Created by shen on 4.01.2026.
//

#include <utility>

#include "../include/Curve.h"

Curve::Curve(mpz_t a, mpz_t b, uint32_t m, mpz_t fz) {

    const auto t = ceil(double(m) / 32);

    mpz_to_bitset32Vec(a, m_a);
    mpz_to_bitset32Vec(b, m_b);
    mpz_to_bitset32Vec(fz, m_fz);
    m_m = m;
    preCompSquaringTable = computeExpansionTable();
}

Curve::Curve(bitset32Vec a, bitset32Vec b, uint32_t m, bitset32Vec fz) {
    m_a = std::move(a);
    m_b = std::move(b);
    m_m = m;
    m_fz = std::move(fz);
    preCompSquaringTable = computeExpansionTable();
}

bool Curve::isPointOnCurve(Point p) {
    bitset32Vec x = p.getX();
    bitset32Vec y = p.getY();

    std::cout <<"x" <<std::endl;
    printBitset32Vec(x);
    std::cout <<"y" <<std::endl;
    printBitset32Vec(y);

//    bitset32Vec y2 = binSquare(y, preCompSquaringTable);

   // y2 = binReduc(y2,m_fz,m_m, false);
    // std::cout <<"y2: " <<std::endl;
    // printBitset32Vec(y2, true);

    bitset32Vec xy = binMult(x, y, m_m, true);
    std::cout <<"xy" <<std::endl;
    printBitset32Vec(xy);
    xy = binReduc(xy,m_fz,m_m);
    printBitset32Vec(xy);
    //
    // bitset32Vec x2 = binSquare(x, preCompSquaringTable);
    // x2 = binReduc(x2,m_fz,m_m);
    //
    // bitset32Vec x3 = binMult(x2, x, m_m);
    // x3 = binReduc(x3,m_fz,m_m);
    //
    // bitset32Vec ax2 = binMult(m_a, x2, m_m);
    // ax2 = binReduc(ax2,m_fz,m_m);
    //
    // bitset32Vec leftSideEq = binAdd(y2,xy, m_m);
    // bitset32Vec rightSideEq = binAdd(binAdd(x3,ax2, m_m), m_b, m_m);

     // printBitset32Vec(y2);
     // printBitset32Vec(x3);
     // printBitset32Vec(xy);
     // printBitset32Vec(x2);
     // printBitset32Vec(ax2);

    // printBitset32Vec(leftSideEq);
    // printBitset32Vec(rightSideEq);

    // if ( bitset_vector_to_mpz(leftSideEq) == bitset_vector_to_mpz(rightSideEq) ) {
    //     return true;
    // }
    return false;
}
