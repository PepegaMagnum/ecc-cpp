#ifndef MATH_OPERATIONS_H
#define MATH_OPERATIONS_H

#include <bits/stdc++.h>
#include <bit>
#include <bitset>
#include <array>

std::vector<std::bitset<32>> binAdd(std::vector<std::bitset<32>>  &a, std::vector<std::bitset<32>>  &b, uint32_t m);

std::vector<std::bitset<32>> binMult(std::vector<std::bitset<32>> &a, std::vector<std::bitset<32>> &b, uint32_t m);
std::vector<std::bitset<32>> binSquare(std::vector<std::bitset<32>> &a, std::vector<std::bitset<16>> &preComputedTable);
std::vector<std::bitset<16>> computeExpansionTable();
// uint32_t binInv(uint32_t a, uint32_t f);
// uint32_t binReduc(uint32_t c, uint32_t fz, uint32_t m);

#endif