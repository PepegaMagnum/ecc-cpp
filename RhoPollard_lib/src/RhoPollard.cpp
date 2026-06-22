//
// Created by bgn09 on 01-Jun-26.
//

#include "../include/RhoPollard.h"

#include <chrono>
#include <random>
#include <utility>
#include <gmpxx.h>

int RhoPollard::partition(const Point &X) {
    mpz_mod(m_mod3Result.get_mpz_t(), X.getX(), m_fieldDivideMod.get_mpz_t());
    return static_cast<int>(mpz_get_ui(m_mod3Result.get_mpz_t()));
}

Point RhoPollard::funcF(Point &Xi, Point &P, Point &Q, int branch) {
    if (branch == 0) return m_curve.pointAddition(Xi, Q);
    if (branch == 1) return m_curve.pointDoubling(Xi);
    return m_curve.pointAddition(Xi, P);

}

void RhoPollard::funcG(mpz_t result, mpz_t a, int branch) {
    // a is the P-coefficient; mirror funcF's action on the point
    if (branch == 0) {            // funcF added Q -> a unchanged
        mpz_set(result, a);
    } else if (branch == 1) {     // funcF doubled -> a -> 2a mod n
        mpz_mul_ui(result, a, 2);
        mpz_mod(result, result, m_n.get_mpz_t());
    } else {                      // branch 2: funcF added P -> a -> a+1 mod n
        mpz_add_ui(result, a, 1);
        mpz_mod(result, result, m_n.get_mpz_t());
    }
}

void RhoPollard::funcH(mpz_t result, mpz_t b, int branch) {
    // b is the Q-coefficient
    if (branch == 0) {            // funcF added Q -> b -> b+1 mod n
        mpz_add_ui(result, b, 1);
        mpz_mod(result, result, m_n.get_mpz_t());
    } else if (branch == 1) {     // funcF doubled -> b -> 2b mod n
        mpz_mul_ui(result, b, 2);
        mpz_mod(result, result, m_n.get_mpz_t());
    } else {                      // branch 2: funcF added P -> b unchanged
        mpz_set(result, b);
    }
}

void RhoPollard::computeLog(Point &P, Point &Q, mpz_t result) {
    std::cout << "Starting Rho Pollard..." <<std::endl;
    // initial parameters
    mpz_class a_i, b_i, a_2i, b_2i;
    mpz_class a2iTmp;
    mpz_class b2iTmp;
    mpz_class gcd;
    mpz_class aCoeffSub;
    mpz_class bCoeffSub;
    mpz_class bCoeffSubInv;
    Point X2iTmp;

    Point X_i, X_2i;
    Point a_iP, b_iQ;
    Point a_2iP, b_2iQ;

    gmp_randstate_t state;
    gmp_randinit_mt(state);

    mpf_class nSqrt;
    mpf_class nSqrtFloor;
    nSqrt = sqrt(m_n);
    mpf_floor(nSqrtFloor.get_mpf_t(), nSqrt.get_mpf_t());

    double totalMs = 0.0;
    int    attempts = 0;

    for (int j = 0; j <= 4; j++) {
        bool collided = false;
        bool found = false;
        int i = 0;
        auto attemptStart = std::chrono::steady_clock::now();
        // mpz_set_d(a_i.get_mpz_t(), dis(generator));
        mpz_class randomRange;
        randomRange = m_n - 1;
        mpz_urandomm(a_i.get_mpz_t(), state, randomRange.get_mpz_t());
        mpz_urandomm(b_i.get_mpz_t(), state, randomRange.get_mpz_t());
        mpz_urandomm(a_2i.get_mpz_t(), state, randomRange.get_mpz_t());
        mpz_urandomm(b_2i.get_mpz_t(), state, randomRange.get_mpz_t());

        a_iP = m_curve.pointMultiplication(P, a_i.get_mpz_t());
        b_iQ = m_curve.pointMultiplication(Q, b_i.get_mpz_t());
        a_2iP = m_curve.pointMultiplication(P, a_2i.get_mpz_t());
        b_2iQ = m_curve.pointMultiplication(Q, b_2i.get_mpz_t());

        X_i = m_curve.pointAddition(a_iP, b_iQ);
        X_2i = m_curve.pointAddition(a_2iP, b_2iQ);

        double iterTotalUs = 0.0;
        long iterCount = 0;
        double iterMinUs = std::numeric_limits<double>::max();
        double iterMaxUs = 0.0;

        while (i <= nSqrtFloor) {
            auto iterStart = std::chrono::steady_clock::now();
            // Single Step
            int branch_i = partition(X_i);
            funcG(a_i.get_mpz_t(), a_i.get_mpz_t(), branch_i);
            funcH(b_i.get_mpz_t(), b_i.get_mpz_t(), branch_i);
            X_i = funcF(X_i, P, Q, branch_i);

            // Double Step
            int branch_2i = partition(X_2i);
            X2iTmp = funcF(X_2i, P, Q, branch_2i);

            funcG(a2iTmp.get_mpz_t(), a_2i.get_mpz_t(), branch_2i);
            funcH(b2iTmp.get_mpz_t(), b_2i.get_mpz_t(), branch_2i);

            int branch_tmp = partition(X2iTmp);
            funcG(a_2i.get_mpz_t(), a2iTmp.get_mpz_t(), branch_tmp);
            funcH(b_2i.get_mpz_t(), b2iTmp.get_mpz_t(), branch_tmp);
            X_2i = funcF(X2iTmp, P, Q, branch_tmp);

            auto iterEnd = std::chrono::steady_clock::now();
            double thisUs = std::chrono::duration<double, std::micro>(iterEnd - iterStart).count();
            iterTotalUs += thisUs;
            if (iterCount > m_skipIterReportCount) {
                if (thisUs < iterMinUs) iterMinUs = thisUs;
                if (thisUs > iterMaxUs) iterMaxUs = thisUs;
            }
            ++iterCount;
            if (iterCount % m_iterNReport == 0) {
                std::cout << "iter " << iterCount
                  << " | avg: " << (iterTotalUs / iterCount) << " us"
                  << " | min: " << iterMinUs << " us"
                  << " | max: " << iterMaxUs << " us"
                  << std::endl;
            }

            if (X_i == X_2i) {
                std::cout <<"Collision found during " <<j+1 <<"th execution of Rho Pollard, iteration: " <<i <<std::endl;
                if (mpz_cmp(b_i.get_mpz_t(), b_2i.get_mpz_t()) == 0)
                    break;

                mpz_sub(bCoeffSub.get_mpz_t(), b_2i.get_mpz_t(), b_i.get_mpz_t());

                mpz_gcd(gcd.get_mpz_t(), bCoeffSub.get_mpz_t(), m_n.get_mpz_t());

                if (mpz_cmp_ui(gcd.get_mpz_t(), 1) == 0) {
                    mpz_sub(aCoeffSub.get_mpz_t(), a_i.get_mpz_t(), a_2i.get_mpz_t());
                    mpz_invert(bCoeffSubInv.get_mpz_t(), bCoeffSub.get_mpz_t(), m_n.get_mpz_t());
                    mpz_mul(result, aCoeffSub.get_mpz_t(), bCoeffSubInv.get_mpz_t());
                    mpz_mod(result, result, m_n.get_mpz_t());
                }
            } else {
                i++;
            }

        }

        auto attemptEnd = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> elapsed = attemptEnd - attemptStart;
        totalMs += elapsed.count();
        ++attempts;
        std::cout << "  attempt " << (j + 1)
                  << ": " << elapsed.count() << " ms"
                  << " | running avg: " << (totalMs / attempts) << " ms"
                  << std::endl;
    }
    gmp_randclear(state);
}