//
// Created by shen on 3.01.2026.
//

#include "../include/Point.h"

Point::Point(const mpz_t a_x, const mpz_t a_y) {
    mpz_inits(m_x.get_mpz_t(), m_y.get_mpz_t());
    mpz_set(m_x.get_mpz_t(), a_x);
    mpz_set(m_y.get_mpz_t(), a_y);
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
