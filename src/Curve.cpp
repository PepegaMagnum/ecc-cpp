//
// Created by shen on 4.01.2026.
//

#include <utility>

#include "../include/Curve.h"

Curve::Curve(mpz_t a, mpz_t b, uint32_t m, mpz_t fz) {
    mpz_set(m_a.get_mpz_t(),a);
    mpz_set(m_b.get_mpz_t(),b);
    mpz_set(m_fz.get_mpz_t(),fz);
    m_m = m;
}

bool Curve::isPointOnCurve(Point p) {
    mpz_t x;
    mpz_t y;
    mpz_class y2;

    mpz_init_set(x, p.getX());
    mpz_init_set(y, p.getY());

    gmp_printf("x: 0x%ZX\n", x);
    gmp_printf("y: 0x%ZX\n", y);

    binSquare(y2.get_mpz_t(), y);
    // gmp_printf("y2: 0x%ZX\n", y2.get_mpz_t());
    binReduc(y2.get_mpz_t(),m_fz.get_mpz_t(),m_m);
    std::cout << y2.get_mpz_t() << std::endl;

    mpz_class xy;
    binMult(x, y, xy.get_mpz_t(), m_m);
    binReduc(xy.get_mpz_t(),m_fz.get_mpz_t(),m_m);
    std::cout << xy.get_mpz_t() << std::endl;

    mpz_class x2;
    binSquare(x2.get_mpz_t(), x);
    binReduc(x2.get_mpz_t(),m_fz.get_mpz_t(),m_m);
    std::cout << x2.get_mpz_t() << std::endl;

    mpz_class x3;
    binMult(x2.get_mpz_t(), x,x3.get_mpz_t(), m_m);
    binReduc(x3.get_mpz_t(),m_fz.get_mpz_t(),m_m);
    std::cout << x3.get_mpz_t() << std::endl;

    mpz_class ax2;
    // std::cout <<"a: " << m_a.get_mpz_t() << std::endl;
    // std::cout <<"b: " << x2.get_mpz_t() << std::endl;
    binMult(m_a.get_mpz_t(), x2.get_mpz_t(),ax2.get_mpz_t(), m_m);
    binReduc(ax2.get_mpz_t(),m_fz.get_mpz_t(),m_m);
    std::cout << ax2.get_mpz_t() << std::endl;

    mpz_class equation;

    mpz_xor(equation.get_mpz_t(), y2.get_mpz_t(), xy.get_mpz_t());
    mpz_xor(equation.get_mpz_t(), equation.get_mpz_t(), x3.get_mpz_t());
    mpz_xor(equation.get_mpz_t(), equation.get_mpz_t(), ax2.get_mpz_t());
    mpz_xor(equation.get_mpz_t(), equation.get_mpz_t(), m_b.get_mpz_t());

    std::cout << "EQUATION EQUAL TO (insert drum roll): " <<equation.get_mpz_t() << std::endl;

    if ( mpz_cmp_ui(equation.get_mpz_t(),0) == 0) {
        return true;
    }
    std::cout << "Pierdole ci matke.pl" << std::endl;
    return false;
}
