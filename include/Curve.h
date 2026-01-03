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
    bitset32Vec m_m;
    bitset32Vec m_fz;

public:
    Curve() = default;
    Curve(mpz_t a, mpz_t b, mpz_t m, bitset32Vec fz);
    bool isPointOnCurve(Point p);
    Point pointAddition(Point P, Point Q);
    Point pointDoubling(Point P);
    Point pointMultiplication(Point P, uint32_t a);

};


#endif //ECC_CPP_CURVE_H