#ifndef MATH_OPERATIONS_H
#define MATH_OPERATIONS_H

#include <bits/stdc++.h>
#include <bit>
#include <bitset>
#include <array>

uint32_t modInv(uint32_t a, uint32_t m);
uint32_t binAdd(uint32_t a, uint32_t b);
uint32_t binMult(uint32_t a, uint32_t b);
uint32_t binSquare(uint32_t a);
uint32_t binInv(uint32_t a, uint32_t f);
uint32_t binReduc(uint32_t c, uint32_t fz, uint32_t m);

#endif