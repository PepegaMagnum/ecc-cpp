#ifndef MATH_OPERATIONS_H
#define MATH_OPERATIONS_H

#include <bits/stdc++.h>
#include <gmp.h>
#include <gmpxx.h>
#include <bitset>

void binMult(mpz_t a, mpz_t b, mpz_t c, uint32_t m);
void binSquare(mpz_t c, const mpz_t a);
void binInv(mpz_t a, mpz_t fz, uint32_t m, bool debug);
void binReduc(mpz_t c, mpz_t fz, uint32_t m);
#endif