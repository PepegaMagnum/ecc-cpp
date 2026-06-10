//
// Created by bgn09 on 01-Jun-26.
//

#include "../include/RhoPollard.h"
#include <random>
#include <utility>

Point RhoPollard::funcF(Point Xi, Point P, Point Q) {
    mpz_class result;
    mpz_class modulo(3);

    mpz_mod(result.get_mpz_t(), Xi.getX(), modulo.get_mpz_t());

    if (mpz_cmp_ui(result.get_mpz_t(), 0) == 0) {
        return m_curve.pointAddition(Xi, std::move(Q));
    } else if(mpz_cmp_ui(result.get_mpz_t(), 1) == 0) {
        mpz_class multiplier(2);
        return m_curve.pointMultiplication(Xi, multiplier.get_mpz_t());
    } else {
        return m_curve.pointAddition(Xi, std::move(P));
    }
}

void RhoPollard::funcG(mpz_t result, mpz_t a, Point P, Point Xi) {
    mpz_class modulo3;
    mpz_class modulo(3);
    mpz_class n;

    if (m_curve.isPointOnCurve(std::move(P)) != true) {
        return;
    }

    mpz_mod(modulo3.get_mpz_t(), Xi.getX(), modulo.get_mpz_t());

    if (mpz_cmp_ui(modulo3.get_mpz_t(), 2) == 0) {
        mpz_set_ui(n.get_mpz_t(), 1);
        mpz_xor(result, a, n.get_mpz_t());

    } else if (mpz_cmp_ui(modulo3.get_mpz_t(), 1) == 0) {
        mpz_set_ui(n.get_mpz_t(), 2);
        binMult(n.get_mpz_t(), a, result, m_curve.m_m);
        binReduc(result, m_curve.m_fz.get_mpz_t(), m_curve.m_m);

    } else {
        mpz_set_ui(n.get_mpz_t(), 1);
        mpz_xor(result, a, n.get_mpz_t());
        binReduc(result, m_curve.m_fz.get_mpz_t(), m_curve.m_m);

    }

}

void RhoPollard::funcH(mpz_t result, mpz_t b, Point P, Point Xi) {
    mpz_class modulo3;
    mpz_class modulo(3);
    mpz_class n;

    mpz_mod(modulo3.get_mpz_t(), Xi.getX(), modulo.get_mpz_t());

    if (mpz_cmp_ui(modulo3.get_mpz_t(), 0) == 0) {
        mpz_set_ui(n.get_mpz_t(), 1);
        mpz_xor(result, b, n.get_mpz_t());
    } else if (mpz_cmp_ui(modulo3.get_mpz_t(), 1) == 0) {
        mpz_set_ui(n.get_mpz_t(), 2);
        binMult(n.get_mpz_t(), b, result, m_curve.m_m);
        binReduc(result, m_curve.m_fz.get_mpz_t(), m_curve.m_m);
    } else {
        mpz_set_ui(n.get_mpz_t(), 1);
        mpz_xor(result, b, n.get_mpz_t());
    }
}

void RhoPollard::computeLog(const Point& P, const Point& Q, mpz_t result, std::mt19937_64 generator) {

    // initial parameters
    mpz_class a_i, b_i, a_2i, b_2i;
    mpz_class a2iTmp;
    mpz_class b2iTmp;
    mpz_class gcd;
    mpz_class subTmp, subTmp2;
    Point X2iTmp;

    Point X_i, X_2i;
    Point a_iP, b_iQ;
    Point a_2iP, b_2iQ;

    std::uniform_int_distribution<uint32_t> dis(1, 65920-1);

    for (int j = 0; j <= 4; j++) {
        int i = 0;
        mpz_set_d(a_i.get_mpz_t(), dis(generator));
        mpz_set_d(b_i.get_mpz_t(), dis(generator));
        mpz_set_d(a_2i.get_mpz_t(), dis(generator));
        mpz_set_d(b_2i.get_mpz_t(), dis(generator));

        a_iP = m_curve.pointMultiplication(P, a_i.get_mpz_t());
        b_iQ = m_curve.pointMultiplication(Q, b_i.get_mpz_t());
        a_2iP = m_curve.pointMultiplication(P, a_2i.get_mpz_t());
        b_2iQ = m_curve.pointMultiplication(Q, b_2i.get_mpz_t());

        X_i = m_curve.pointAddition(a_iP, b_iQ);
        X_2i = m_curve.pointAddition(a_2iP, b_2iQ);

        while (i <= std::floor(sqrt(65920)) ) { //replace with std::floor(sqrt(65920))
            // std::cout << "Iteracja: " << i << std::endl;

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
                std::cout << "AYE LMAOOOOOOO" << std::endl;
                // if (mpz_cmp(b_i.get_mpz_t(), b_2i.get_mpz_t()) == 0)
                //     break;
                // mpz_gcd(gcd.get_mpz_t(), b_i.get_mpz_t(), b_2i.get_mpz_t());
                // if (mpz_cmp_ui(gcd.get_mpz_t(), 0) == 0) {
                //     std::cout <<"LAMBO" << std::endl;
                //     mpz_sub(subTmp.get_mpz_t(), a_i.get_mpz_t(), a_2i.get_mpz_t());
                //     mpz_sub(subTmp2.get_mpz_t(), b_i.get_mpz_t(), b_2i.get_mpz_t());
                //     mpz_invert(subTmp2.get_mpz_t(), subTmp2.get_mpz_t(), m_n.get_mpz_t());
                //     mpz_mul(result, subTmp.get_mpz_t(), subTmp2.get_mpz_t());
                // } else {
                //     std::cout << "SZAMBO" << std::endl;
                //     mpz_set_d(result, -1);
                //     break;
                // }
            } else {
                i++;
            }
        }
    }
}