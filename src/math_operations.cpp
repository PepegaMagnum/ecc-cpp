#include "../include/math_operations.h"

uint32_t modInv(uint32_t* a, uint32_t* m){
    if(std::__gcd(*a, *m) > 1){
        return -1;
    }

    for(int x = 1 ; x < *m ; x++){
        if( (*a % *m) * (x % *m) % *m == 1){
            return x;
        }
    }
}

uint32_t binAdd(uint32_t* a, uint32_t*b){
    return *a^*b;
}

uint32_t binMult(uint32_t* a, uint32_t*b){
    std::bitset<32> binA, binB, binC;

    binA = *a;
    binB = *b;

    if (binA[0] == 1)
        binC = *b;
    else if (binA[0] == 0)
    {
        binC = 0;
    }

    //std::cout <<"binA: " <<binA << " binB: " << binB 
    //<< " binC " <<binC <<std::endl;

    for(int i = 1; i < binA.size(); i++){
        binB = binB << 1;

        //std::cout <<"iteration: " <<i <<std::endl;

        //std::cout <<"binB " <<binB <<std::endl;
        if (binA[i] == 1)
            binC = binC ^ binB;
        
        //std::cout <<"binC "<<binC <<std::endl;
            
    }

    return binC.to_ullong();
}

uint32_t binSquare(uint32_t* a){
    std::bitset<32> binA;
    std::bitset<32> binC;

    binA = *a;

    if (binA[31] == 1)
        binC = 1;
    else if (binA[31] == 0)
    {
        binC = 0;
    }

    int shiftCnt;
    for(int k = binA.size() - 1; k >= 0; k--){
        if (binA[k] == 1){
            shiftCnt = k;
            break;
        } 
    }

    //std::cout << shiftCnt <<std::endl;

    for (int i = 1; i <= shiftCnt + 1 ; i++){
        binC = binC << 2;
        if (binA[i] == 1)
            binC ^= 1;
    }
    return binC.to_ullong();
}

uint32_t binInv(uint32_t* a, uint32_t* f){
    uint32_t u = *a;
    uint32_t v = *f;
    int j;
    uint32_t zj;
    uint32_t zjv;
    uint32_t zjg2;

    uint32_t g1 = 1;
    uint32_t g2 = 0;

    while (u != 1){
        j = std::__bit_width(u) - std::__bit_width(v);
        //std::cout <<std::__bit_width(u) 
        //    << " - " <<std::__bit_width(v) <<" = " <<j <<std::endl;
        if(j < 0){
            std::swap(u,v);
            std::swap(g1,g2);
            j = -j;
        }
        zj = 1 << j;
        zjv = binMult(&zj, &v);
        u = binAdd(&u, &zjv);
        //std::cout <<std::bitset<32>(u) <<std::endl;
        zjg2 = binMult(&zj, &g2);
        g1 = binAdd(&g1, &zjg2);
    }

    return g1; 
}

uint32_t binReduc(uint32_t* c, uint32_t* fz, uint32_t* m){
    uint32_t k;
    for(int i = 32; i --> *m-1;){
        if( (*c & (1<<i)) >> i == 1){
            k = i-*m;
            *c = *c^(*fz<<k);
        }
    }
    return *c;
}