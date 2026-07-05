#include "gf27_mult.hpp"

static const int M163 = 163;
static const int MODLOW163 = 0xC9;

ap_uint<M163> gf163_mult(ap_uint<M163> a, ap_uint<M163> b) {
#pragma HLS INLINE off
    return gf_mult<M163>(a, b, (ap_uint<M163>)MODLOW163);
}
