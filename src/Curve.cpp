//
// Created by shen on 4.01.2026.
//

#include <utility>

#include "../include/Curve.h"

Curve::Curve(mpz_t a, mpz_t b, uint32_t m, mpz_t fz) {
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
    bitset32Vec y2, x3, xy, x2, ax2;

    y2 = binSquare(y,preCompSquaringTable);
    binReduc(y2,m_fz,m_m);
    x3 = binMult(binSquare(x, preCompSquaringTable), x, m_m);
    binReduc(x3,m_fz,m_m);
    x2 = binSquare(x, preCompSquaringTable);
    binReduc(x2,m_fz,m_m);
    ax2 = binMult(m_a,x2,m_m);
    binReduc(ax2,m_fz,m_m);

    bitset32Vec eq = binAdd(binAdd(binAdd(binAdd(y2,xy, m_m), x3, m_m), ax2, m_m), m_b, m_m);
    if (bitset_vector_to_mpz(eq) == 0) {
        return true;
    }
    return false;
}
