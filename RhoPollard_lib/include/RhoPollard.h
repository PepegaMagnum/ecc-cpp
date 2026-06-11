//
// Created by bgn09 on 01-Jun-26.
//

#ifndef ECC_CPP_RHOPOLLARD_H
#define ECC_CPP_RHOPOLLARD_H
#include <utility>

#include "Curve.h"

class RhoPollard {

    Curve m_curve;
    mpz_class m_n;
    mpz_class m_fieldDivideMod;
    mpz_class m_mod3Result;
    mpz_class m_resultCoeff;

    Point funcF(Point Xi, Point P, Point Q);
    void funcG(mpz_t result, mpz_t a, const Point& P, const Point& Xi);
    void funcH(mpz_t result, mpz_t b, const Point& P, const Point& Xi);



public:
    RhoPollard() = default;
    RhoPollard(Curve a_curve, mpz_class a_n, uint32_t a_fieldDivideMod) {
        m_curve = std::move(a_curve);
        m_fieldDivideMod = a_fieldDivideMod;
        m_n = std::move(a_n);
        m_mod3Result = 0;
        m_resultCoeff = 0;
    }
    void computeLog(const Point& P, const Point& Q, mpz_t result);
};

#endif //ECC_CPP_RHOPOLLARD_H
