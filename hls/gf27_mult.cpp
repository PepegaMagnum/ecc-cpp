#include "gf27_mult.hpp"

ap_uint<M27> gf27_mult(ap_uint<M27> a, ap_uint<M27> b) {
#pragma HLS INLINE off
    return gf_mult<M27>(a, b, (ap_uint<M27>)MODLOW27);
}
