#ifndef MATH_OPERATIONS_H
#define MATH_OPERATIONS_H

#include <bits/stdc++.h>
#include <gmp.h>
#include <gmpxx.h>
#include <bitset>

typedef std::vector<std::bitset<32>> bitset32Vec;
typedef std::vector<std::bitset<16>> bitset16Vec;

bitset32Vec binAdd(bitset32Vec  a, bitset32Vec  b, uint32_t m);
bitset32Vec binMult(bitset32Vec &a, bitset32Vec &b, uint32_t m);

bitset32Vec &binSquare(bitset32Vec &a, bitset16Vec &preComputedTable);
bitset16Vec computeExpansionTable();
void binReduc(bitset32Vec &a, bitset32Vec fz, uint32_t m);
bitset32Vec binInv(bitset32Vec &a, bitset32Vec fz, uint32_t m);
void mpz_to_bitset32Vec(const mpz_t mpz_value, bitset32Vec& bitsetVec);
mpz_class bitset_vector_to_mpz(const std::vector<std::bitset<32>>& bits);
#endif