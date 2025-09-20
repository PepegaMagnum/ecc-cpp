//
// Created by shen on 19.09.2025.
//

#ifndef ECC_CPP_CURVE_H
#define ECC_CPP_CURVE_H

#include "math_operations.h"
#include "Point.h"

class Curve
{
    uint32_t m_a;
    uint32_t m_b;
    uint32_t m_m;
    uint32_t m_fz;

public:
    Curve() = default;
    Curve(uint32_t a, uint32_t b, uint32_t m, uint32_t fz);
    bool isPointOnCurve(Point p);
    Point pointAddition(Point P, Point Q);
    Point pointDoubling(Point P);
    Point pointMultiplication(Point P, uint32_t a);

};


#endif //ECC_CPP_CURVE_H