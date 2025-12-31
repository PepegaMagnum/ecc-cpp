#include "../include/math_operations.h"
#include <gmpxx.h>
#include <gmp.h>

bitset32Vec binAdd(bitset32Vec  &a, bitset32Vec  &b, uint32_t m){
    bitset32Vec result;
    const auto t = ceil(double(m) / 32);
    result.reserve(static_cast<int>(t));

    for (int i = 0 ; i < t ; i++)
    {
        //std::cout <<i;
        result.push_back(a[i] ^ b[i]);
    }
    // std::cout<< result[0] <<std::endl;
    return result;
}

bitset32Vec binMult(bitset32Vec &a, bitset32Vec &b, uint32_t m)
{
    const auto t = ceil(double(m) / 32);
    //std::cout <<t <<std::endl;
    auto b_copy = b;
    bitset32Vec result(t, std::bitset<32>(0));

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

bitset32Vec binSquare(bitset32Vec &a, std::vector<std::bitset<16>> &preComputedTable)
{
    uint32_t t = a.size();
    bitset32Vec result(2*t);

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

void shiftBitsetVectorRight(bitset32Vec& v) {
    bool carry = false;

    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        bool new_carry = (*it)[0]; // LSB
        *it >>= 1;
        (*it)[32 - 1] = carry;
        carry = new_carry;
    }
}

void shiftBitsetVectorLeft(bitset32Vec& v) {
    bool carry = false;

    for (auto& b : v) {
        bool new_carry = b[32 - 1]; // MSB
        b <<= 1;
        b[0] = carry;
        carry = new_carry;
    }
}

std::vector<bitset32Vec> precomputeUK (bitset32Vec fz, uint32_t m) {
    bitset32Vec rz = fz;
    const auto t = static_cast<int>(ceil(double(m) / 32));
    rz[t][m%32] = false;
    std::vector<bitset32Vec> uk;
    bitset32Vec zk;
    zk.emplace_back(1);

    uk.push_back(binMult(zk, rz, m));

    for (int i = 1 ; i < 32 ; i++) {
        shiftBitsetVectorLeft(zk);
        uk.push_back(binMult(zk, rz, m));
    }

    return uk;
}

void binReduc(bitset32Vec &a, bitset32Vec fz, uint32_t m) {
    std::vector<bitset32Vec> uk = precomputeUK(fz, m);
    bitset32Vec result;
    const auto t = ceil(double(m) / 32);
    result.reserve(static_cast<int>(t));

    for (int l = t; l >= 0; l--) {
        for (int i = 2*(m-1); i >= m; i--) {
            if (a[l][i % 32] == true) {
                int j = floor((i-m)/32);
                int k = (i-m) - 32*j;
                a = binAdd(a, uk[k], 2*(m-1));
            }
        }
    }
}

std::vector<uint32_t> bitsets_to_words(const std::vector<std::bitset<32>>& bits) {
    std::vector<uint32_t> words(bits.size());

    for (size_t i = 0; i < bits.size(); ++i) {
        words[i] = static_cast<uint32_t>(bits[i].to_ulong());
    }

    return words;
}

mpz_class bitset_vector_to_mpz(const std::vector<std::bitset<32>>& bits) {
    std::vector<uint32_t> words = bitsets_to_words(bits);

    mpz_class result;
    mpz_import(
        result.get_mpz_t(),
        words.size(),          // number of words
        -1,                    // order: least significant word first
        sizeof(uint32_t),      // word size
        0,                     // native endianness
        0,                     // no nails
        words.data()
    );

    return result;
}

int degree(const std::vector<std::bitset<32>>& poly) {
    for (size_t w = poly.size(); w-- > 0; ) {
        if (poly[w].any()) {
            for (size_t b = 32; b-- > 0; ) {
                if (poly[w].test(b)) {
                    return static_cast<int>(w * 32 + b);
                }
            }
        }
    }
    return -1;  // zero polynomial
}

bitset32Vec generateZJ(uint32_t j, uint32_t m) {
    bitset32Vec z;
    const auto t = ceil(double(m) / 32);
    z.reserve(static_cast<int>(t));
    z.emplace_back(1);
    for (int i = 1 ; i < j ; i++) {
        shiftBitsetVectorLeft(z);
    }
    return z;
}

bitset32Vec binInv(bitset32Vec &a, bitset32Vec fz, uint32_t m) {
    bitset32Vec u = a;
    bitset32Vec v = fz;

    bitset32Vec g1;
    bitset32Vec g2;

    g1.emplace_back(1);
    g2.emplace_back(0);

    while (bitset_vector_to_mpz(u) != 1) {
        std::cout <<"integer u = "<< bitset_vector_to_mpz(u) <<std::endl;
        std::cout <<"g1  = " <<g1[0] <<std::endl;
        std::cout <<"g2  = " <<g2[0] <<std::endl;
        std::cout <<"u   = " <<u[0] <<std::endl;
        std::cout <<"v   = " <<v[0] <<std::endl;

        int j = degree(u) - degree(v);
        std::cout <<"deg(u) = " <<degree(u) <<std::endl;
        std::cout <<"deg(v) = " <<degree(v) <<std::endl;
        std::cout <<"j  = " <<j<<std::endl;

        if (j < 0) {
            swap(u,v);
            swap(g1,g2);
            j = -j;
        }

        bitset32Vec zj = generateZJ(j, m);
        //
        // std::cout <<"zj = " <<zj[0] <<std::endl;
        // std::cout <<"v  = " <<v[0] <<std::endl;
        // std::cout <<"g2 = " <<g2[0] <<std::endl;

        bitset32Vec zjv = binMult(zj, v, m);
        // binReduc(zj, fz, m);

        bitset32Vec zjg2 = binMult(zj, g2, m);
        // binReduc(zjg2, fz, m);
        //
        // std::cout <<"zjv  = " <<zjv[0] <<std::endl;
        // std::cout <<"zjg2 = " <<zjg2[0] <<std::endl;

        u = binAdd(u,zjv, m);
        // std::cout <<"u  = " <<u[0] <<std::endl;
        g1 = binAdd(g1,zjg2, m);
        // std::cout <<"g1 = " <<g1[0] <<std::endl;
    }
    return g1;
}