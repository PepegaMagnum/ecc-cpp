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

void hex_to_mpz(mpz_t out, const char* hex) {
    // Strip optional 0x / 0X prefix
    if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
        hex += 2;
    }

    mpz_set_str(out, hex, 16);
}

Point::Point(const char* hex_x, const char* hex_y) {
    mpz_t x, y;

    mpz_inits(x,y, nullptr);

    hex_to_mpz(x, hex_x);
    hex_to_mpz(y, hex_y);

    mpz_to_bitset32Vec(x, m_x);
    mpz_to_bitset32Vec(y, m_y);
    mpz_clears(x, y, nullptr);
}

std::string bitset_to_hex(const std::bitset<32>& b) {
    uint64_t value = b.to_ullong();

    std::ostringstream oss;
    oss << std::hex << std::uppercase << value;
    return oss.str();
}

void Point::print() const {
    std::cout << "x coordinate: 0x";
    for (int i = m_x.size(); i >= 0 ; i--) {
        std::cout << bitset_to_hex(m_x[i]);
    }
    std::cout << std::endl;

    std::cout << "y coordinate: 0x";
    for (int i = m_y.size(); i >= 0 ; i--) {
        std::cout << bitset_to_hex(m_y[i]);
    }
    std::cout << std::endl;
}
