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

    Point funcF(Point Xi, Point P, Point Q);
    void funcG(mpz_t result, mpz_t a, Point P, Point Xi);
    void funcH(mpz_t result, mpz_t b, Point P, Point Xi);



public:
    RhoPollard() = default;
    RhoPollard(Curve a_curve, uint32_t a_n) {
        m_curve = std::move(a_curve);
        mpz_set_d(m_n.get_mpz_t(), a_n);

    }
    void computeLog(const Point& P, const Point& Q, mpz_t result, std::mt19937_64 generator);
};

#endif //ECC_CPP_RHOPOLLARD_H
