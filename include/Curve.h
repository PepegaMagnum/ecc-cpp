//
// Created by shen on 19.09.2025.
//

#ifndef ECC_CPP_CURVE_H
#define ECC_CPP_CURVE_H

#include "math_operations.h"
#include "Point.h"

class Curve
{
    bitset32Vec m_a;
    bitset32Vec m_b;
    uint32_t m_m;
    bitset32Vec m_fz;
    bitset16Vec preCompSquaringTable;

public:
    Curve() = default;
    Curve(mpz_t a, mpz_t b, uint32_t m, mpz_t fz);
    Curve(bitset32Vec a, bitset32Vec b, uint32_t m, bitset32Vec fz);
    bool isPointOnCurve(Point p);
    // Point pointAddition(Point P, Point Q);
    // Point pointDoubling(Point P);
    // Point pointMultiplication(Point P, uint32_t a);

    bitset32Vec getFz() {
        return m_fz;
    }
};


#endif //ECC_CPP_CURVE_H