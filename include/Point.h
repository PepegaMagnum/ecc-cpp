//
// Created by shen on 19.09.2025.
//

#ifndef ECC_CPP_POINT_H
#define ECC_CPP_POINT_H
#include "math_operations.h"

class Point
{
    bitset32Vec m_x;
    bitset32Vec m_y;
public:

    Point() = default;

    Point(const bitset32Vec& a_x, const bitset32Vec& a_y);
    Point(const mpz_t a_x, const mpz_t a_y);
    void print() const;

    [[nodiscard]] bitset32Vec getX() const {return m_x;}
    void setX(const bitset32Vec& x) {m_x = x;}

    [[nodiscard]] bitset32Vec getY() const {return m_y;}
    void setY(const bitset32Vec& y) {m_y = y;}

};

#endif //ECC_CPP_POINT_H