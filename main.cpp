#include <iostream>
#include "math_operations/math_operations.h"

int main() {
    uint32_t a = 83, b = 202 , fz = 283, m = 8;

    std::cout <<"Modular multiplicative inverse of " << a <<" and " << b << " is " << modInv(&a, &b) <<std::endl;

    std::cout << "Binary Addition: " <<a <<" + " <<b <<" = " <<binAdd(&a, &b) <<std::endl;

    uint32_t mulres = binMult(&a, &b);

    std::cout << "Binary Multiplication: " <<a <<" * " <<b << " = " <<mulres <<std::endl;

    std::cout << "Binary Square of " <<a <<" is " << binSquare(&a) <<std::endl;
    std:: cout << "Reducing " <<std::bitset<32>(mulres) <<" by " 
        << std::bitset<32>(fz) << " equals " <<binReduc(&mulres, &fz, &m) <<std::endl;
    std::cout << "Inversion of " << b <<" " << fz <<" is " <<binInv(&b, &fz) <<std::endl;
    
    return 0;
}