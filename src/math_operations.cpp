#include "../include/math_operations.h"

std::vector<std::bitset<32>> binAdd(std::vector<std::bitset<32>>  &a, std::vector<std::bitset<32>>  &b, uint32_t m){
    std::vector<std::bitset<32>> result;
    const auto t = ceil(double(m) / 32);
    result.reserve(static_cast<int>(t));

    for (int i = 0 ; i < t ; i++)
    {
        std::cout <<i;
        result[i] = a[i] ^ b[i];
    }
    // std::cout<< result[0] <<std::endl;
    return result;
}

std::vector<std::bitset<32>> binMult(std::vector<std::bitset<32>> &a, std::vector<std::bitset<32>> &b, uint32_t m)
{
    const auto t = ceil(double(m) / 32);
    std::cout <<t <<std::endl;
    auto b_copy = b;
    std::vector<std::bitset<32>> result(t, std::bitset<32>(0));

    for (int k = 0 ; k < 32 ; k++)
    {
        for (int j = 0; j < t ; j++)
        {
            if (a[j][k] == 1)
            {
                if (result[j] == 0)
                    result[j] = b_copy[j];
                else
                    result[j] = result[j] ^ b_copy[j];
            }

            if (k != 31)
                b_copy[j] = b_copy[j] << 1;
        }
    }
    return result;
}

std::bitset<16> expandBits(const std::bitset<8>& bits) {
    std::bitset<16> result;
    for (int i = 0; i < 8; ++i) {
        result[2 * i] = bits[i];  // copy each bit to every other position
    }
    return result;
}

std::vector<std::bitset<16>> computeExpansionTable() {
    std::vector<std::bitset<16>> table(256);

    for (int i = 0; i < 256; ++i) {
        std::bitset<8> input(i);
        table[i] = expandBits(input);
    }

    return table;
}

std::bitset<32> concat16Bitset(std::bitset<16> &a, std::bitset<16> &b)
{
    return std::bitset<32>(a.to_string() + b.to_string());
}

std::vector<std::bitset<32>> binSquare(std::vector<std::bitset<32>> &a, std::vector<std::bitset<16>> &preComputedTable)
{
    uint32_t t = a.size();
    std::vector<std::bitset<32>> result(2*t);

    for (int i=0; i < t; i++)
    {
        std::vector<std::bitset<8>> temp(4);
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 8; k++)
            {
                temp[j][k] = a[i][j*8 + k];
            }
        }
        result[2*i] = concat16Bitset(preComputedTable[temp[1].to_ulong()],
            preComputedTable[temp[0].to_ulong()]);
        result[2*i+1] = concat16Bitset(preComputedTable[temp[3].to_ulong()],
            preComputedTable[temp[2].to_ulong()]);

    }
    return result;
}