#include "gf_field.hpp"
 
ap_uint<M163> gf163_mult(ap_uint<M163> a, ap_uint<M163> b) {
#pragma HLS INLINE off
    return gf_mult<M163>(a, b, (ap_uint<M163>)MODLOW163);
}
 
ap_uint<M163> gf163_square(ap_uint<M163> a) {
#pragma HLS INLINE off
    return gf_square<M163>(a, (ap_uint<M163>)MODLOW163);
}
 
ap_uint<M163> gf163_inv(ap_uint<M163> a) {
#pragma HLS INLINE off
    return gf_inv_it163<M163>(a, (ap_uint<M163>)MODLOW163);
}
