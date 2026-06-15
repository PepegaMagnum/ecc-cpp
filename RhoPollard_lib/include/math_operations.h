#ifndef MATH_OPERATIONS_H
#define MATH_OPERATIONS_H

#include <cstdint>
#include <gmp.h>
#include <iostream>


void binMult(mpz_t a, mpz_t b, mpz_t c, uint32_t m);
void binSquare(mpz_t c, const mpz_t a);
void binInv(mpz_t a, mpz_t fz, uint32_t m);
void binReduc(mpz_t c, mpz_t fz, uint32_t m);
#endif