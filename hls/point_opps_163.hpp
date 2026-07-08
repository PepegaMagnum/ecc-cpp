//
// Created by shen on 8.07.2026.
//

#ifndef POINT_OPS_HPP
#define POINT_OPS_HPP

#include <ap_int.h>
#include "gf_field.hpp"

template <int M>
struct Point {
    ap_uint<M> x;
    ap_uint<M> y;
    bool inf;      // true => point at infinity (identity)
};

template <int M>
ap_uint<M> gf_inv_dispatch(ap_uint<M> a, ap_uint<M> modlow);

// specialization for 163
template <>
inline ap_uint<163> gf_inv_dispatch<163>(ap_uint<163> a, ap_uint<163> modlow) {
    return gf_inv_it163<163>(a, modlow);
}
// specialization for 7
template <>
inline ap_uint<7> gf_inv_dispatch<7>(ap_uint<7> a, ap_uint<7> modlow) {
    return gf_inv_it7<7>(a, modlow);
}

// ---- point doubling: 2*P ----
template <int M>
Point<M> point_double(Point<M> P, ap_uint<M> a_coeff, ap_uint<M> modlow) {
#pragma HLS INLINE off
    Point<M> R;
    if (P.inf || P.x == 0) {        // infinity, or order-2 point -> infinity
        R.x = 0; R.y = 0; R.inf = true;
        return R;
    }
    // lam = x1 + y1 * inv(x1)
    ap_uint<M> inv_x1 = gf_inv_dispatch<M>(P.x, modlow);
    ap_uint<M> lam    = P.x ^ gf_mult<M>(P.y, inv_x1, modlow);
    // x3 = lam^2 + lam + a
    ap_uint<M> lam2   = gf_square<M>(lam, modlow);
    ap_uint<M> x3     = lam2 ^ lam ^ a_coeff;
    // y3 = x1^2 + (lam+1)*x3
    ap_uint<M> x1sq   = gf_square<M>(P.x, modlow);
    ap_uint<M> y3     = x1sq ^ gf_mult<M>(lam ^ (ap_uint<M>)1, x3, modlow);

    R.x = x3; R.y = y3; R.inf = false;
    return R;
}

// ---- point addition: P + Q ----
template <int M>
Point<M> point_add(Point<M> P, Point<M> Q, ap_uint<M> a_coeff, ap_uint<M> modlow) {
#pragma HLS INLINE off
    if (P.inf) return Q;
    if (Q.inf) return P;

    Point<M> R;
    if (P.x == Q.x) {
        // same x: either P == -Q (-> infinity) or P == Q (-> doubling)
        if ((P.y ^ Q.y) == P.x) {          // P == -Q
            R.x = 0; R.y = 0; R.inf = true;
            return R;
        }
        return point_double<M>(P, a_coeff, modlow);   // P == Q
    }
    // lam = (y1 + y2) * inv(x1 + x2)
    ap_uint<M> num   = P.y ^ Q.y;
    ap_uint<M> den   = P.x ^ Q.x;
    ap_uint<M> lam   = gf_mult<M>(num, gf_inv_dispatch<M>(den, modlow), modlow);
    // x3 = lam^2 + lam + x1 + x2 + a
    ap_uint<M> lam2  = gf_square<M>(lam, modlow);
    ap_uint<M> x3    = lam2 ^ lam ^ P.x ^ Q.x ^ a_coeff;
    // y3 = lam*(x1 + x3) + x3 + y1
    ap_uint<M> y3    = gf_mult<M>(lam, P.x ^ x3, modlow) ^ x3 ^ P.y;

    R.x = x3; R.y = y3; R.inf = false;
    return R;
}

void point_add_163(ap_uint<163> px, ap_uint<163> py, bool pinf,
                   ap_uint<163> qx, ap_uint<163> qy, bool qinf,
                   ap_uint<163> a_coeff,
                   ap_uint<163> &rx, ap_uint<163> &ry, bool &rinf);

void point_double_163(ap_uint<163> px, ap_uint<163> py, bool pinf,
                      ap_uint<163> a_coeff,
                      ap_uint<163> &rx, ap_uint<163> &ry, bool &rinf);

#endif // POINT_OPS_HPP

