//
// Created by bgn09 on 10-Jun-26.
//
#include <catch2/catch_test_macros.hpp>
#include "Curve.h"

TEST_CASE("Check multiplication", "[Curve]") {
    mpz_class a, b, fz;
    uint32_t m = 16;

    std::random_device rd;
    if (rd.entropy() > 0) {
        std::cout << "Entropia: " << rd.entropy() << std::endl;
    }

    std::mt19937_64 generator(rd());
    mpz_class a_i;
    std::uniform_int_distribution<uint32_t> dis(1, 65920-1);
    mpz_set_d(a_i.get_mpz_t(), dis(generator));
    mpz_set_str(a.get_mpz_t(), "2905", 16);
    mpz_set_str(b.get_mpz_t(), "886f", 16);
    mpz_set_str(fz.get_mpz_t(), "10000000000101011", 2);

    Curve myCurve(a.get_mpz_t(), b.get_mpz_t(), m, fz.get_mpz_t());
    Point G("ba04", "9b3b");

    SECTION ("GENERATOR CHECK") {
        CHECK(myCurve.isPointOnCurve(G) == true);
    }

    SECTION ("CHECK IF MULTIPLICATION IS EQUAL TO N ADDITION") {
        a_i = 13;
        std::cout << a_i << " size is " <<mpz_sizeinbase(a_i.get_mpz_t(),2) << std::endl;

        Point nG = myCurve.pointMultiplication(G, a_i.get_mpz_t());
        CHECK(myCurve.isPointOnCurve(nG) == true);
        Point nGAdd = G;
        for (int i = 0; i < 13-1; i++) {
            // std::cout << "Addition " << i+1 <<std::endl;
            nGAdd = myCurve.pointAddition(nGAdd, G);
            // nGAdd.print();
        }

        std::cout << "Multiplied Point:" <<std::endl;
        nG.print();
        std:: cout << "Addition Point" <<std::endl;
        nGAdd.print();

        CHECK(nG == nGAdd);
    }
}