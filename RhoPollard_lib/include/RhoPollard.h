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
    uint32_t  m_iterNReport;
    uint32_t  m_skipIterReportCount;

    Point funcF(Point &Xi, Point &P, Point &Q, int branch);
    void funcG(mpz_t result, mpz_t a, int branch);
    void funcH(mpz_t result, mpz_t b, int branch);
    int partition(const Point& X);


public:
    RhoPollard() = default;
    RhoPollard(Curve a_curve, mpz_class a_n, uint32_t a_fieldDivideMod, uint32_t a_iterNReport, uint32_t a_skipIterReportCount) {
        m_curve = std::move(a_curve);
        m_fieldDivideMod = a_fieldDivideMod;
        m_n = std::move(a_n);
        m_mod3Result = 0;
        m_resultCoeff = 0;
        m_iterNReport = a_iterNReport;
        m_skipIterReportCount = a_skipIterReportCount;
    }
    void computeLog(Point &P, Point &Q, mpz_t result);
};

#endif //ECC_CPP_RHOPOLLARD_H
