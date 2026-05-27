//
// Created by shen on 3.01.2026.
//

#include "../include/Point.h"
Point::Point(const mpz_t a_x, const mpz_t a_y, bool a_isInfinity)
    : m_x(a_x), m_y(a_y), isInfinity(a_isInfinity)
{
    // The body remains completely empty!
    // The initializer list above safely and efficiently copies the mpz_t data.
}

Point::Point(const int a_x, const int a_y, bool a_isInfinity) {
    mpz_set_d(m_x.get_mpz_t(), a_x);
    mpz_set_d(m_y.get_mpz_t(), a_y);
    isInfinity = a_isInfinity;
}

Point::Point(const char* hex_x, const char* hex_y) {
    mpz_set_str(m_x.get_mpz_t(), hex_x, 16);
    mpz_set_str(m_y.get_mpz_t(), hex_y, 16);
}

void Point::print() const {
    std::cout << "x coordinate: ";
    gmp_printf("0x%ZX\n", m_x.get_mpz_t());
    std::cout << std::endl;

    std::cout << "y coordinate: ";
    gmp_printf("0x%ZX\n", m_y.get_mpz_t());
    std::cout << std::endl;
}

bool Point::operator==(const Point& P) const {
    return (m_x == P.m_x) && (m_y == P.m_y);
}

bool Point::operator!=(const Point& P) const {
    return !(*this == P);
}
