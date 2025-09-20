//
// Created by shen on 19.09.2025.
//

#include "../include/Curve.h"

Curve::Curve(uint32_t a, uint32_t b, uint32_t m, uint32_t fz) : m_a(a), m_b(b), m_m(m), m_fz(fz){};
bool Curve::isPointOnCurve(Point pointP)
{
    const uint32_t y = pointP.getY();
    const uint32_t x = pointP.getX();

    uint32_t y2 = binReduc(binSquare(y), m_fz, m_m);
    uint32_t x3 = binReduc(binMult(binSquare(x), x), m_fz, m_m);
    uint32_t xy = binReduc(binMult(x, y), m_fz, m_m);
    uint32_t x2 = binReduc(binSquare(x), m_fz, m_m);
    uint32_t ax2 = binReduc(binMult(m_a, x2), m_fz, m_m);

    uint32_t equation = binAdd(binAdd(binAdd(binAdd(y2, xy), x3), ax2), m_b);

    if (equation == 0)
    {
        return true;
    }
    return false;
}




