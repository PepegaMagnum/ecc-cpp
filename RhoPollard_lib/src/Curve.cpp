//
// Created by shen on 4.01.2026.
//

#include <utility>

#include "../include/Curve.h"

Curve::Curve(mpz_t a, mpz_t b, uint32_t m, mpz_t fz) {
    mpz_set(m_a.get_mpz_t(),a);
    mpz_set(m_b.get_mpz_t(),b);
    mpz_set(m_fz.get_mpz_t(),fz);
    m_m = m;
}

bool Curve::isPointOnCurve(const Point& p) {
    mpz_t x;
    mpz_t y;
    mpz_class y2;

    mpz_init_set(x, p.getX());
    mpz_init_set(y, p.getY());

    binSquare(y2.get_mpz_t(), y);
    binReduc(y2.get_mpz_t(),m_fz.get_mpz_t(),m_m);
    // std::cout << y2.get_mpz_t() << std::endl;

    mpz_class xy;
    binMult(x, y, xy.get_mpz_t(), m_m);
    binReduc(xy.get_mpz_t(),m_fz.get_mpz_t(),m_m);
    // std::cout << xy.get_mpz_t() << std::endl;

    mpz_class x2;
    binSquare(x2.get_mpz_t(), x);
    binReduc(x2.get_mpz_t(),m_fz.get_mpz_t(),m_m);
    // std::cout << x2.get_mpz_t() << std::endl;

    mpz_class x3;
    binMult(x2.get_mpz_t(), x,x3.get_mpz_t(), m_m);
    binReduc(x3.get_mpz_t(),m_fz.get_mpz_t(),m_m);
    // std::cout << x3.get_mpz_t() << std::endl;

    mpz_class ax2;
    binMult(m_a.get_mpz_t(), x2.get_mpz_t(),ax2.get_mpz_t(), m_m);
    binReduc(ax2.get_mpz_t(),m_fz.get_mpz_t(),m_m);
    // std::cout << ax2.get_mpz_t() << std::endl;

    mpz_class equation;

    mpz_xor(equation.get_mpz_t(), y2.get_mpz_t(), xy.get_mpz_t());
    mpz_xor(equation.get_mpz_t(), equation.get_mpz_t(), x3.get_mpz_t());
    mpz_xor(equation.get_mpz_t(), equation.get_mpz_t(), ax2.get_mpz_t());
    mpz_xor(equation.get_mpz_t(), equation.get_mpz_t(), m_b.get_mpz_t());

    mpz_clears(x, y, NULL);

    if ( mpz_cmp_ui(equation.get_mpz_t(),0) == 0) {

        return true;
    }
    return false;
}

Point Curve::pointNeg(Point& p) {
    mpz_t newY;
    mpz_init(newY);
    mpz_xor(newY, p.getY(), p.getX());
    Point result(p.getX(), newY, p.getIsInfinity());
    mpz_clear(newY);
    return result;
}

void printHex(const char* name, mpz_t a) {
    gmp_printf("%s = 0x%Zx\n", name, a);
}

Point Curve::pointAddition(Point &P, Point &Q) {
    if (P.getIsInfinity() ) return Q;
    if (Q.getIsInfinity() ) return P;

    mpz_class x1(P.getX());
    mpz_class x2(Q.getX());
    mpz_class y1(P.getY());
    mpz_class y2(Q.getY());


    if (mpz_cmp(x1.get_mpz_t(), x2.get_mpz_t()) == 0) {
        if (mpz_cmp(y1.get_mpz_t(), y2.get_mpz_t()) == 0) {
            return pointDoubling(P);
        } else {
            return Point{0, 0, true};    // P == -Q, sum is infinity
        }
    }


    mpz_class lambda;
    mpz_class lambda2;
    mpz_class y1y2;
    mpz_class x1x2inv;
    mpz_class x1x2;
    mpz_class x1x3;

    mpz_class x3;
    mpz_class y3;

    // lambda calculation
    mpz_xor(x1x2.get_mpz_t(), x1.get_mpz_t(), x2.get_mpz_t());
    // printHex("x1x2", x1x2.get_mpz_t());
    mpz_xor(y1y2.get_mpz_t(), y1.get_mpz_t(), y2.get_mpz_t());
    // printHex("y1y2", y1y2.get_mpz_t());
    mpz_set(x1x2inv.get_mpz_t(), x1x2.get_mpz_t());
    binInv(x1x2inv.get_mpz_t(), m_fz.get_mpz_t(), m_m);
    // printHex("x1x2inv", x1x2inv.get_mpz_t());
    binMult(y1y2.get_mpz_t(), x1x2inv.get_mpz_t(), lambda.get_mpz_t(), m_m);
    // printHex("lambda", lambda.get_mpz_t());

    binSquare(lambda2.get_mpz_t(), lambda.get_mpz_t());
    // printHex("lambda2", lambda2.get_mpz_t());

    mpz_set(x3.get_mpz_t(), lambda2.get_mpz_t());
    mpz_xor(x3.get_mpz_t(), x3.get_mpz_t(), lambda.get_mpz_t());
    mpz_xor(x3.get_mpz_t(), x3.get_mpz_t(), x1.get_mpz_t());
    mpz_xor(x3.get_mpz_t(), x3.get_mpz_t(), m_a.get_mpz_t());
    mpz_xor(x3.get_mpz_t(), x3.get_mpz_t(), x2.get_mpz_t());

    mpz_set_d(y3.get_mpz_t(), 0);
    mpz_xor(x1x3.get_mpz_t(), x1.get_mpz_t(),x3.get_mpz_t());
    // printHex("x1 + x3", x1x3.get_mpz_t());

    binMult(lambda.get_mpz_t(), x1x3.get_mpz_t(), y3.get_mpz_t(), 2*m_m);
    // printHex("(x1+x3)Lambda", y3.get_mpz_t());
    mpz_xor(y3.get_mpz_t(), y3.get_mpz_t(), x3.get_mpz_t());

    mpz_xor(y3.get_mpz_t(), y3.get_mpz_t(), y1.get_mpz_t());

    // printHex("x3", x3.get_mpz_t());
    // printHex("y3", y3.get_mpz_t());

    binReduc(x3.get_mpz_t(), m_fz.get_mpz_t(),m_m);
    binReduc(y3.get_mpz_t(), m_fz.get_mpz_t(),m_m);

    return Point {x3.get_mpz_t(), y3.get_mpz_t(), false};;

}

Point Curve::pointDoubling(const Point& P) {
    mpz_class x(P.getX());
    mpz_class y(P.getY());

    mpz_class invX(x.get_mpz_t());
    binInv(invX.get_mpz_t(), m_fz.get_mpz_t(), m_m);
    // std::cout << "Inversion of X" << invX.get_mpz_t() << std::endl;

    mpz_class yInvX;
    mpz_class lambda;
    mpz_class lambda2;

    binMult(y.get_mpz_t(),invX.get_mpz_t(),yInvX.get_mpz_t(), m_m);
    mpz_xor(lambda.get_mpz_t(), x.get_mpz_t(), yInvX.get_mpz_t());
    // printHex("lambda", lambda.get_mpz_t());
    binSquare(lambda2.get_mpz_t(),lambda.get_mpz_t());

    mpz_class x2;

    binSquare(x2.get_mpz_t(),x.get_mpz_t());
    // printHex("x2", x2.get_mpz_t());

    mpz_class x3, y3;
    mpz_class x3Lambda;

    // calculating x3
    mpz_set(x3.get_mpz_t(), lambda2.get_mpz_t());
    mpz_xor(x3.get_mpz_t(), x3.get_mpz_t(), lambda.get_mpz_t());
    mpz_xor(x3.get_mpz_t(), x3.get_mpz_t(), m_a.get_mpz_t());
    // printHex("x3", x3.get_mpz_t());

    //calculating y3
    // printHex("lambda", lambda.get_mpz_t());
    binMult(lambda.get_mpz_t(), x3.get_mpz_t(), x3Lambda.get_mpz_t(), 2*m_m);

    mpz_set(y3.get_mpz_t(), x2.get_mpz_t());

    mpz_xor(y3.get_mpz_t(), x2.get_mpz_t(), x3Lambda.get_mpz_t());
    mpz_xor(y3.get_mpz_t(), y3.get_mpz_t(), x3.get_mpz_t());

    binReduc(x3.get_mpz_t(), m_fz.get_mpz_t(),m_m);
    binReduc(y3.get_mpz_t(), m_fz.get_mpz_t(),m_m);

    return Point{x3.get_mpz_t(), y3.get_mpz_t(), false};
}

Point Curve::pointMultiplication(const Point &P, mpz_t a) {

    Point q {0, 0, true};
    Point p = P;
    mpz_class multSize = mpz_sizeinbase(a, 2);

    for (int i = 0; i < multSize; i++) {
        // std::cout <<"Step: " <<i <<std::endl;
        if (mpz_tstbit(a, i) == 1) {
            q = pointAddition(p,q);
            // q.print();
        }
        p = pointDoubling(p);
    }

    return q;
}