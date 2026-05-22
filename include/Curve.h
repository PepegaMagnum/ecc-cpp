//
// Created by shen on 19.09.2025.
//

#ifndef ECC_CPP_CURVE_H
#define ECC_CPP_CURVE_H

#include "math_operations.h"
#include "Point.h"

class Curve
{
    mpz_class m_a;
    mpz_class m_b;
    uint32_t m_m;
    mpz_class m_fz;
    Point m_generator;

public:
    Curve() = default;
    Curve(mpz_t a, mpz_t b, uint32_t m, mpz_t fz);
    bool isPointOnCurve(Point p);
    static Point pointNeg(Point& p);
    Point pointAddition(Point P, Point Q);
    // Point pointDoubling(Point P);
    // Point pointMultiplication(Point P, uint32_t a);

    const __mpz_struct* getFz() const{
        return m_fz.get_mpz_t();
    }
};


#endif //ECC_CPP_CURVE_H