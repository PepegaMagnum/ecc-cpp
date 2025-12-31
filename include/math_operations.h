#ifndef MATH_OPERATIONS_H
#define MATH_OPERATIONS_H

#include <bits/stdc++.h>
#include <bit>
#include <bitset>
#include <array>

typedef std::vector<std::bitset<32>> bitset32Vec;
typedef std::vector<std::bitset<16>> bitset16Vec;

bitset32Vec binAdd(bitset32Vec  &a, bitset32Vec  &b, uint32_t m);
bitset32Vec binMult(bitset32Vec &a, bitset32Vec &b, uint32_t m);
bitset32Vec binSquare(bitset32Vec &a, bitset16Vec &preComputedTable);
bitset16Vec computeExpansionTable();
void binReduc(bitset32Vec &a, bitset32Vec fz, uint32_t m);

// uint32_t binInv(uint32_t a, uint32_t f);
// uint32_t binReduc(uint32_t c, uint32_t fz, uint32_t m);

#endif