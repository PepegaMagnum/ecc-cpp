//
// Created by shen on 3.01.2026.
//

#include "../include/Curve.h"

Point::Point(const mpz_t a_x, const mpz_t a_y) {
    mpz_to_bitset32Vec(a_x, m_x);
    mpz_to_bitset32Vec(a_y, m_y);
}

Point::Point(const bitset32Vec& a_x, const bitset32Vec& a_y) {
    m_x = a_x;
    m_y = a_y;
}

void Point::print() const {
    std::cout << "x coordinate: 0x";
    for (int i = m_x.size(); i >= 0 ; i--) {
        std::cout << std::hex << m_x[i].to_string();
    }
    std::cout << std::endl;

    std::cout << "y coordinate: 0x";
    for (int i = m_y.size(); i >= 0 ; i--) {
        std::cout << std::hex << m_y[i].to_string();
    }
    std::cout << std::endl;
}
