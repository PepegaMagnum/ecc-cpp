#include "../include/math_operations.h"

void binMult(mpz_t a, mpz_t b, mpz_t c, uint32_t m) {
    if (mpz_tstbit(a, 0) == 1) {
        mpz_set(c, b);
    } else {
        mpz_set_d(c, 0);
    }

    for (int i = 1; i < m; i++) {
        mpz_mul_2exp(b, b, 1);
        if (mpz_tstbit(a, i) == 1) {
            mpz_xor(c, c, b);
        }
    }
}

void binSquare(mpz_t c, const mpz_t a) {
    mpz_set_ui(c, 0);

    size_t bit_count = mpz_sizeinbase(a, 2);
    // std::cout << "bit_count: " << bit_count << std::endl;

    for (size_t i = 0; i < bit_count; i++) {
        if (mpz_tstbit(a, i)) {
            mpz_setbit(c, 2 * i);
        }
    }
}

void binReduc(mpz_t c, mpz_t fz, uint32_t m) {
    int k;
    mpz_t tmp;
    mpz_init(tmp);
    for (int i = mpz_sizeinbase(c, 2) - 1; i > m-1; i--) {
        if (mpz_tstbit(c, i)) {
            k = i-m;
            mpz_mul_2exp(tmp, fz, k);
            mpz_xor(c,c,tmp);
        }
    }
}

void binInv(mpz_t a, mpz_t fz, uint32_t m) {
    int j;
    mpz_t u, v, g1, g2;
    mpz_t zjv, zjg2, zj;
    mpz_inits(u, v, g1, g2, zj, zjv, zjg2);

    mpz_set(u, a);
    mpz_set(v, fz);
    mpz_set_d(g1, 1);
    mpz_set_d(g2, 0);

    while (mpz_get_d(u) != 1) {
        j = mpz_sizeinbase(u, 2) - mpz_sizeinbase(v, 2);
        if (j < 0) {
            mpz_swap(v, u);
            mpz_swap(g1, g2);
            j = -j;
        }
        mpz_set_d(zj, 1 << j);
        binMult(zj, v, zjv, m);
        mpz_xor(u, u, zjv);
        binMult(zj, g2, zjg2, m);
        mpz_xor(g1, g1, zjg2);
    }
}