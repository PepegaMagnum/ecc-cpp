#ifndef GF_FIELD_HPP
#define GF_FIELD_HPP
 
#include <ap_int.h>
 
static const int M27       = 7;
static const int MODLOW27  = 0x3;
static const int M163      = 163;
static const int MODLOW163 = 0xC9;
 
template <int M>
ap_uint<2 * M> gf_mult_raw(ap_uint<M> a, ap_uint<M> b) {
#pragma HLS INLINE
    ap_uint<2 * M> result  = 0;
    ap_uint<2 * M> shifted = b;
    for (int i = 0; i < M; i++) {
#pragma HLS PIPELINE
        if (a[i]) result ^= shifted;
        shifted <<= 1;
    }
    return result;
}
 
template <int M>
ap_uint<M> gf_reduce(ap_uint<2 * M> c, ap_uint<M> modlow) {
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
    ap_uint<2 * M> raw = gf_mult_raw<M>(a, b);
    return gf_reduce<M>(raw, modlow);
}
 
template <int M>
ap_uint<2 * M> gf_square_raw(ap_uint<M> a) {
#pragma HLS INLINE
    ap_uint<2 * M> spread = 0;
    for (int i = 0; i < M; i++) {
#pragma HLS UNROLL
        spread[2 * i] = a[i];
    }
    return spread;
}
 
template <int M>
ap_uint<M> gf_square_n(ap_uint<M> a, int n, ap_uint<M> modlow) {
#pragma HLS INLINE off
    ap_uint<M> r = a;
    for (int i = 0; i < n; i++) {
#pragma HLS LOOP_TRIPCOUNT max=163
        r = gf_square<M>(r, modlow);
    }
    return r;
}

template <int M>
ap_uint<M> gf_inv_it163(ap_uint<M> a, ap_uint<M> modlow) {
#pragma HLS INLINE off
  if(a == 0) return 0;

  ap_uint<M> b1 = a;
  ap_uint<M> b2  = gf_mult<M>(gf_square_n<M>(b1, 1, modlow),  b1,  modlow);
  ap_uint<M> b4  = gf_mult<M>(gf_square_n<M>(b2, 2, modlow),  b2,  modlow);
  ap_uint<M> b5  = gf_mult<M>(gf_square_n<M>(b4, 1, modlow),  b1,  modlow); 

  ap_uint<M> b10 = gf_mult<M>(gf_square_n<M>(b5, 5, modlow),  b5,  modlow);
  ap_uint<M> b20 = gf_mult<M>(gf_square_n<M>(b10,10, modlow), b10, modlow);
  ap_uint<M> b40 = gf_mult<M>(gf_square_n<M>(b20,20, modlow), b20, modlow);
  ap_uint<M> b80 = gf_mult<M>(gf_square_n<M>(b40,40, modlow), b40, modlow);
  ap_uint<M> b81 = gf_mult<M>(gf_square_n<M>(b80, 1, modlow), b1,  modlow);
  ap_uint<M> b162= gf_mult<M>(gf_square_n<M>(b81,81, modlow), b81, modlow);

  return gf_square<M>(b162, modlow);
}

template <int M>
ap_uint<M> gf_inv_it7(ap_uint<M> a, ap_uint<M> modlow) {
#pragma HLS INLINE off
    if (a == 0) return 0;

    ap_uint<M> b1 = a;                                              // 2^1 - 1
    ap_uint<M> b2 = gf_mult<M>(gf_square_n<M>(b1, 1, modlow), b1, modlow); // 2
    ap_uint<M> b3 = gf_mult<M>(gf_square_n<M>(b2, 1, modlow), b1, modlow); // 3
    ap_uint<M> b6 = gf_mult<M>(gf_square_n<M>(b3, 3, modlow), b3, modlow); // 6

    return gf_square<M>(b6, modlow);   // (a^(2^6 - 1))^2 = a^-1
}

ap_uint<M27>  gf27_mult (ap_uint<M27>  a, ap_uint<M27>  b);
ap_uint<M163> gf163_mult(ap_uint<M163> a, ap_uint<M163> b);
ap_uint<M27>  gf27_square (ap_uint<M27>  a);
ap_uint<M27>  gf27_inv(ap_uint<M27> a);
ap_uint<M163> gf163_square(ap_uint<M163> a);
ap_uint<M163> gf163_inv(ap_uint<M163> a);


#endif // GF_FIELD_HPP
