#include "gf_field.hpp"
 
ap_uint<M27> gf27_mult(ap_uint<M27> a, ap_uint<M27> b) {
#pragma HLS INLINE off
    return gf_mult<M27>(a, b, (ap_uint<M27>)MODLOW27);
}
 
ap_uint<M27> gf27_square(ap_uint<M27> a) {
#pragma HLS INLINE off
    return gf_square<M27>(a, (ap_uint<M27>)MODLOW27);
}
ap_uint<M27> gf27_inv(ap_uint<M27> a) {
#pragma HLS INLINE off
    return gf_inv_it7<M27>(a, (ap_uint<M27>)MODLOW27);
}
