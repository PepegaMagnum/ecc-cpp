//
// Created by shen on 19.09.2025.
//

#ifndef ECC_CPP_POINT_H
#define ECC_CPP_POINT_H
#include "math_operations.h"
#include <gmpxx.h>

class Point
{
    mpz_class m_x;
    mpz_class m_y;
    bool      isInfinity = false;
public:

    Point() = default;

    Point(const mpz_t a_x, const mpz_t a_y, bool a_isInfinity);
    Point(const int a_x, const int a_y, bool a_isInfinity);
    Point(const char* hex_x, const char* hex_y);
    void print() const;
    const __mpz_struct* getX() const {
        return m_x.get_mpz_t();
    }
    const __mpz_struct* getY() const {
        return m_y.get_mpz_t();
    }
    void setX(mpz_t x) {
        mpz_set(m_x.get_mpz_t(), x);
    }

    void setY(mpz_t y) {
        mpz_set(m_y.get_mpz_t(), y);
    }

    void setInfinity(bool a_sInfinity) {
        isInfinity = a_sInfinity;
    }

    bool getIsInfinity() const {
        return isInfinity;
    }

    bool operator==(const Point & point) const;

    bool operator!=(const Point& P) const;
};

#endif //ECC_CPP_POINT_H