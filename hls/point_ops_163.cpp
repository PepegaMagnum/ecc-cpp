#include "point_ops.hpp"
 
void point_add_163(ap_uint<163> px, ap_uint<163> py, bool pinf,
                   ap_uint<163> qx, ap_uint<163> qy, bool qinf,
                   ap_uint<163> a_coeff,
                   ap_uint<163> &rx, ap_uint<163> &ry, bool &rinf) {
#pragma HLS INLINE off
#pragma HLS ALLOCATION operation instances=mul limit=1
    hls_ecc::Point<163> P{px, py, pinf};
    hls_ecc::Point<163> Q{qx, qy, qinf};
    hls_ecc::Point<163> R = hls_ecc::point_add<163>(P, Q, a_coeff, (ap_uint<163>)MODLOW163);
    rx = R.x; ry = R.y; rinf = R.inf;
}
 
void point_double_163(ap_uint<163> px, ap_uint<163> py, bool pinf,
                      ap_uint<163> a_coeff,
                      ap_uint<163> &rx, ap_uint<163> &ry, bool &rinf) {
#pragma HLS INLINE off
#pragma HLS ALLOCATION operation instances=mul limit=1
    hls_ecc::Point<163> P{px, py, pinf};
    hls_ecc::Point<163> R = hls_ecc::point_double<163>(P, a_coeff, (ap_uint<163>)MODLOW163);
    rx = R.x; ry = R.y; rinf = R.inf;
}
