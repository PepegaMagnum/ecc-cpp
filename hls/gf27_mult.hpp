
#ifndef GF27_MULT_HPP
#define GF27_MULT_HPP

#include <ap_int.h>

static const int M27      = 7;
static const int MODLOW27 = 0x3;

template <int M>
ap_uint<2 * M> binMultHW(ap_uint<M> a, ap_uint<M> b) {
#pragma HLS INLINE
    ap_uint<2 * M> result  = 0;
    ap_uint<2 * M> shifted = b;

    for (int i = 0; i < M; i++) {
#pragma HLS PIPELINE
        if (a[i]) {
            result ^= shifted;
        }
        shifted <<= 1;
    }
    return result;
}

template <int M>
ap_uint<M> binReducHW(ap_uint<2 * M> c, ap_uint<M> modlow) {
#pragma HLS INLINE
    for (int p = 2 * M - 2; p >= M; p--) {
#pragma HLS PIPELINE
        if (c[p]) {
            ap_uint<2 * M> reduction = (ap_uint<2 * M>)modlow << (p - M);
            c ^= reduction;
            c ^= ((ap_uint<2 * M>)1 << p);
        }
    }
    return c.range(M - 1, 0);
}


template <int M>
ap_uint<M> gf_mult(ap_uint<M> a, ap_uint<M> b, ap_uint<M> modlow) {
#pragma HLS INLINE
    ap_uint<2 * M> raw = binMultHW<M>(a, b);
    return binReducHW<M>(raw, modlow);
}


ap_uint<M27> gf27_mult(ap_uint<M27> a, ap_uint<M27> b);

#endif // GF27_MULT_HPP
