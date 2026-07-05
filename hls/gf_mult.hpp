#ifndef GF_MULT_HPP
#define GF_MULT_HPP

#include <ap_int.h>

static const int M   = 7;
static const int MOD = 0x3;   // x + 1

template <int M>
ap_uint<2 * M> gf_mult_raw(ap_uint<M> a, ap_uint<M> b) {
#pragma HLS INLINE
    ap_uint<2 * M> result  = 0;
    ap_uint<2 * M> shifted = b;

    for (int i = 0; i < M; i++) {
#pragma HLS UNROLL
        if (a[i]) {
            result ^= shifted;       // carryless: XOR instead of add
        }
        shifted <<= 1;
    }
    return result;
}

template <int M>
ap_uint<M> gf_reduce(ap_uint<2 * M> c, ap_uint<M> modlow) {
#pragma HLS INLINE
    for (int p = 2 * M - 2; p >= M; p--) {
#pragma HLS UNROLL
        if (c[p]) {
            ap_uint<2 * M> reduction = (ap_uint<2 * M>)modlow << (p - M);
            c ^= reduction;
            c ^= ((ap_uint<2 * M>)1 << p);   // clear x^p itself
        }
    }
    return c.range(M - 1, 0);
}

template <int M>
ap_uint<M> binMult(ap_uint<M> a, ap_uint<M> b, ap_uint<M> modlow) {
#pragma HLS INLINE
    ap_uint<2 * M> raw = gf_mult_raw<M>(a, b);
    return gf_reduce<M>(raw, modlow);
}

ap_uint<M27> gf_mult(ap_uint<M27> a, ap_uint<M27> b);

#endif // GF_MULT_HPP
