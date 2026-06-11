//
// Created by bgn09 on 01-Jun-26.
//

#include "../include/RhoPollard.h"
#include <random>
#include <utility>
#include <gmpxx.h>

Point RhoPollard::funcF(Point Xi, Point P, Point Q) {

    mpz_mod(m_mod3Result.get_mpz_t(), Xi.getX(), m_fieldDivideMod.get_mpz_t());

    if (m_mod3Result == 0) {
        return m_curve.pointAddition(Xi, Q);
    }
    if (m_mod3Result == 1) {
        mpz_class multiplier(2);
        return m_curve.pointMultiplication(Xi, multiplier.get_mpz_t());
    }

    return m_curve.pointAddition(Xi, P);

}

void RhoPollard::funcG(mpz_t result, mpz_t a, const Point& P, const Point& Xi) {
    if (m_curve.isPointOnCurve(P) != true) {
        return;
    }

    mpz_mod(m_mod3Result.get_mpz_t(), Xi.getX(), m_fieldDivideMod.get_mpz_t());

    if (m_mod3Result == 2) {
        mpz_set_ui(m_resultCoeff.get_mpz_t(), 1);
        mpz_xor(result, a, m_resultCoeff.get_mpz_t());
    } else if (m_mod3Result == 1) {
        mpz_set_ui(m_resultCoeff.get_mpz_t(), 2);
        binMult(m_resultCoeff.get_mpz_t(), a, result, m_curve.m_m);
        binReduc(result, m_curve.m_fz.get_mpz_t(), m_curve.m_m);
    } else {
        mpz_set_ui(m_resultCoeff.get_mpz_t(), 1);
        mpz_xor(result, a, m_resultCoeff.get_mpz_t());
        binReduc(result, m_curve.m_fz.get_mpz_t(), m_curve.m_m);
    }

}

void RhoPollard::funcH(mpz_t result, mpz_t b, const Point& P, const Point& Xi) {

    if (m_curve.isPointOnCurve(P) != true) {
        return;
    }

    mpz_mod(m_mod3Result.get_mpz_t(), Xi.getX(), m_fieldDivideMod.get_mpz_t());

    if (m_mod3Result == 0) {
        mpz_set_ui(m_resultCoeff.get_mpz_t(), 1);
        mpz_xor(result, b, m_resultCoeff.get_mpz_t());
    } else if (m_mod3Result == 1) {
        mpz_set_ui(m_resultCoeff.get_mpz_t(), 2);
        binMult(m_resultCoeff.get_mpz_t(), b, result, m_curve.m_m);
        binReduc(result, m_curve.m_fz.get_mpz_t(), m_curve.m_m);
    } else {
        mpz_set_ui(m_resultCoeff.get_mpz_t(), 1);
        mpz_xor(result, b, m_resultCoeff.get_mpz_t());
    }
}

void RhoPollard::computeLog(const Point& P, const Point& Q, mpz_t result) {

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


    for (int j = 0; j <= 4; j++) {
        int i = 0;
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

        mpf_class nSqrt;
        mpf_class nSqrtFloor;
        nSqrt = sqrt(m_n);
        mpf_floor(nSqrtFloor.get_mpf_t(), nSqrt.get_mpf_t());

        while (i <= nSqrtFloor.get_d() ) {

            // Single Step
            funcG(a_i.get_mpz_t(), a_i.get_mpz_t(), P, X_i);
            funcH(b_i.get_mpz_t(), b_i.get_mpz_t(), P, X_i);
            X_i = funcF(X_i, P, Q);

            // Double Step
            X2iTmp = funcF(X_2i, P, Q);

            funcG(a2iTmp.get_mpz_t(), a_2i.get_mpz_t(), P, X_2i);
            funcH(b2iTmp.get_mpz_t(), b_2i.get_mpz_t(), P, X_2i);

            funcG(a_2i.get_mpz_t(), a2iTmp.get_mpz_t(), P, X2iTmp);
            funcH(b_2i.get_mpz_t(), b2iTmp.get_mpz_t(), P, X2iTmp);
            X_2i = funcF(X2iTmp, P, Q);

            if (X_i == X_2i) {
                std::cout <<"Result found in " <<j+1 <<"th execution of Rho Pollard, iteration: " <<i <<std::endl;

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
    }
}