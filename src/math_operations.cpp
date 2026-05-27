#include "../include/math_operations.h"

void binMult(mpz_t a, mpz_t b, mpz_t c, uint32_t m) {

    mpz_t tmp;
    mpz_init(tmp);

    mpz_set(tmp, b);

    if (mpz_tstbit(a, 0) == 1) {
        mpz_set(c, tmp);
    } else {
        mpz_set_d(c, 0);
    }

    for (int i = 1; i < m; i++) {
        mpz_mul_2exp(tmp, tmp, 1);
        if (mpz_tstbit(a, i) == 1) {
            mpz_xor(c, c, tmp);
        }
    }
    mpz_clear(tmp);
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
    int iter;
    mpz_t u, v, g1, g2;
    mpz_t zjv, zjg2, zj;
    mpz_inits(u, v, g1, g2, NULL);
    mpz_init_set_ui(zj, 0);
    mpz_init_set_ui(zjv, 0);
    mpz_init_set_ui(zjg2, 1);

    mpz_set(u, a);
    mpz_set(v, fz);
    mpz_set_d(g1, 1);
    mpz_set_d(g2, 0);

    while (mpz_cmp_d(u, 1) != 0) {
        std::cout << "it's my fault again" << std::endl;
        // gmp_printf("u = 0x%Zx\n", u);
        // gmp_printf("v = 0x%Zx\n", v);
        j = mpz_sizeinbase(u, 2) - mpz_sizeinbase(v, 2);
        // std::cout << mpz_sizeinbase(u, 2) <<" - " <<mpz_sizeinbase(v, 2) << " = " << j << std::endl;
        if (j < 0) {
            mpz_swap(v, u);
            mpz_swap(g1, g2);
            j = -j;
            // std::cout <<"After swap" << std::endl;
            // gmp_printf("u = 0x%Zx\n", u);
            // gmp_printf("v = 0x%Zx\n", v);
        }
        mpz_set_ui(zj,1);
        // gmp_printf("zj = %Zx\n", zj);
        mpz_mul_2exp(zj, zj, j);
        // gmp_printf("zj = 0x%Zx\n", zj);

        binMult(zj, v, zjv, m);
        // gmp_printf("zjv = 0x%Zx\n", zjv);

        mpz_xor(u, u, zjv);
        // gmp_printf("u xor zjv = 0x%Zx\n", u);
        binMult(zj, g2, zjg2, m);

        mpz_xor(g1, g1, zjg2);
        // gmp_printf("g1 = 0x%Zx\n", g1);
        iter++;
    }
    mpz_set(a, g1);
    // gmp_printf("a = 0x%Zx\n", a);
    mpz_clears(u, v, g1, g2, zjv, zjg2, NULL);
}