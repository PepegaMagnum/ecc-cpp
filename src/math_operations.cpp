#include "../include/math_operations.h"

constexpr size_t WORD_SIZE = 32;

void shiftBitsetVectorLeft(std::vector<std::bitset<WORD_SIZE>>& v, uint32_t t)
{
    bool carry = false;

    for (size_t i = 0; i < t; ++i)
    {
        bool new_carry = v[i][WORD_SIZE - 1];

        v[i] <<= 1;
        v[i][0] = carry;

        carry = new_carry;
    }
}

bitset32Vec binAdd(bitset32Vec  &a, bitset32Vec  &b, uint32_t m){
    bitset32Vec result;
    const auto t = ceil(double(m) / 32);
    result.reserve(static_cast<int>(t));

    for (int i = 0 ; i <= t; i++)
    {
        //std::cout <<i;
        result.push_back(a[i] ^ b[i]);
    }
    // std::cout<< result[0] <<std::endl;
    return result;
}

bitset32Vec binMult(bitset32Vec &a, bitset32Vec &b, uint32_t m, bool debug)
{
    const auto t = ceil(double(m) / 32);
    bitset32Vec c(t);
    auto b_copy = b;
    if (debug) {
        std::cout <<"Multiplying" <<std::endl;
        std::cout <<"a:" <<std::endl;
        printBitset32Vec(a, true);
        std::cout <<"b:" <<std::endl;
        printBitset32Vec(b, true);
    }

    if (a[0][0] == true) {
        c = b;
    }

    for (int i = 1 ; i < m ; i++) {
        shiftBitsetVectorLeft(b_copy, t+1);
        if (debug) {
            std::cout <<"i: " <<i <<std::endl;
            std::cout <<"B*z mod fz: " <<std::endl;
            printBitset32Vec(b_copy, true);
            std::cout <<"c:" <<std::endl;
            printBitset32Vec(c, true);
        }

        if (a[i/32][i%32] == 1) {
            c = binAdd(c,b_copy, m);
            if (debug) {
                std::cout << "Result" <<std::endl;
                printBitset32Vec(c, true);
            }
        }

    }
    return c;
}

std::bitset<16> expandBits(const std::bitset<8>& bits) {
    std::bitset<16> result;
    for (int i = 0; i < 8; ++i) {
        result[2 * i] = bits[i];  // copy each bit to every other position
    }
    return result;
}

bitset16Vec computeExpansionTable() {
    bitset16Vec table(256);

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

bitset32Vec binSquare(bitset32Vec &a, std::vector<std::bitset<16> > &preComputedTable)
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

bitset32Vec binReduc(bitset32Vec &a, bitset32Vec fz, uint32_t m, bool debug) {
    if (debug) {
        std::cout <<"Reducing: " <<std::endl;
        printBitset32Vec(a);
        std::cout << "Reduction polynomial" << std::endl;
        printBitset32Vec(fz);
        std::cout <<std::endl;
    }
    bitset32Vec fz_copy;

    const auto t = ceil(double(m) / 32);
    bitset32Vec fz_tmp;
    for (int i = 2*(m-1); i > m-1; i--) {
        if (debug) {
            std::cout <<"Bit: " <<i <<std::endl;
            printBitset32Vec(a, false);
        }
        size_t vector_idx = i / 32;
        size_t bit_idx = i % 32;

        // std::cout << "Vector: " <<vector_idx <<std::endl;
        // std::cout <<"Bit: " <<bit_idx <<std::endl;

        if (a[vector_idx][bit_idx] == true) {
            if (debug)
                std::cout <<"Adding" <<std::endl;
            fz_copy = fz;
            for (int j = 0; j < i-m; j++) {
                shiftBitsetVectorLeft(fz_copy, t);
            }
            if (debug)
                printBitset32Vec(fz_copy, false);
            a = binAdd(a, fz_copy, 2*m);
        }
    }
    std::vector result(a.begin(), a.begin() + t);
    return result;
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
    for (int i = 0 ; i < j ; i++) {
        shiftBitsetVectorLeft(z,t);
    }
    return z;
}

bitset32Vec binInv(bitset32Vec &a, bitset32Vec fz, uint32_t m) {
    bitset32Vec u = a;
    bitset32Vec v = std::move(fz);

    bitset32Vec g1;
    bitset32Vec g2;

    g1.emplace_back(1);
    g2.emplace_back(0);

    while (bitset_vector_to_mpz(u) != 1) {
        int j = degree(u) - degree(v);

        if (j < 0) {
            std::swap(u,v);
            std::swap(g1,g2);
            j = -j;
        }

        bitset32Vec zj = generateZJ(j, m);
        bitset32Vec zjv = binMult(zj, v, m);
        bitset32Vec zjg2 = binMult(zj, g2, m);

        u = binAdd(u,zjv, m);
        g1 = binAdd(g1, zjg2, m);
    }
    return g1;
}

void mpz_to_bitset32Vec(const mpz_t mpz_value, bitset32Vec& bitsetVec) {
    size_t bitCount = mpz_sizeinbase(mpz_value, 2);
    size_t wordCount = (bitCount + 32 - 1) / 32;

    std::vector<uint32_t> words(wordCount, 0);

    mpz_export(
        words.data(),
        nullptr,
        -1,
        sizeof(uint32_t),
        0,
        0,
        mpz_value
        );
    std::vector<std::bitset<32>> result(wordCount);
    for (size_t i = 0; i < wordCount; ++i) {
        result[i] = std::bitset<32>(words[i]);
    }
    bitsetVec = result;
}

void hex_string_to_mpz(mpz_t result, const char* hex_str) {
    // Base 16 for hexadecimal
    if (mpz_set_str(result, hex_str, 16) != 0) {
        throw std::invalid_argument("Invalid hexadecimal string");
    }
}

void printBitset32Vec(const bitset32Vec& bitsetVec, bool sections) {
    for (int i = bitsetVec.size(); i >= 0 ; i--) {
        if (sections) {
            std::cout <<"[" <<i <<"] :" << bitsetVec[i] << std::endl;
        } else {
            std::cout << bitsetVec[i];
        }


    }
    std::cout << std::endl;
}