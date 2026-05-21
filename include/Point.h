//
// Created by shen on 19.09.2025.
//

#ifndef ECC_CPP_POINT_H
#define ECC_CPP_POINT_H
#include "math_operations.h"

class Point
{
    mpz_class m_x;
    mpz_class m_y;
public:

    Point() = default;

    Point(const mpz_t a_x, const mpz_t a_y);
    Point(const char* hex_x, const char* hex_y);
    void print() const;
    const __mpz_struct* getX() const {
        return m_x.get_mpz_t();
    }
    const __mpz_struct* getY() const {
        return m_y.get_mpz_t();
    }

};

#endif //ECC_CPP_POINT_H