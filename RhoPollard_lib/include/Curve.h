//
// Created by shen on 19.09.2025.
//

#ifndef ECC_CPP_CURVE_H
#define ECC_CPP_CURVE_H

#include "math_operations.h"
#include "Point.h"
#include <optional>

class Curve
{
    mpz_class m_a;
    mpz_class m_b;
    Point m_generator;

public:
    uint32_t m_m;
    mpz_class m_fz;
    Curve() = default;
    Curve(mpz_t a, mpz_t b, uint32_t m, mpz_t fz);
    bool isPointOnCurve(const Point& p);
    static Point pointNeg(Point& p);
    Point pointAddition(Point &P, Point &Q);
    Point pointDoubling(const Point& P);
    Point pointMultiplication(const Point &P, mpz_t a);
};


#endif //ECC_CPP_CURVE_H