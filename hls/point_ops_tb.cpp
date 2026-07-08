#include <ap_int.h>
#include <gmp.h>
#include <gmpxx.h>
#include <cstdio>
#include "point_ops.hpp"
#include "Curve.h"
#include "Point.h"
 
void point_add_163(ap_uint<163>, ap_uint<163>, bool,
                   ap_uint<163>, ap_uint<163>, bool,
                   ap_uint<163>,
                   ap_uint<163>&, ap_uint<163>&, bool&);
void point_double_163(ap_uint<163>, ap_uint<163>, bool,
                      ap_uint<163>,
                      ap_uint<163>&, ap_uint<163>&, bool&);
 
static ap_uint<163> mpz_to_ap(const mpz_t v) {
    ap_uint<163> r = 0;
    for (int i = 0; i < 163; i++) if (mpz_tstbit(v, i)) r[i] = 1;
    return r;
}
static ap_uint<163> mpzc_to_ap(const mpz_class& v) { return mpz_to_ap(v.get_mpz_t()); }
 
int main() {
    mpz_class a("1", 16);
    mpz_class b("1", 16);
    mpz_class fz;
    mpz_set_str(fz.get_mpz_t(), "800000000000000000000000000000000000000C9", 16);
    uint32_t m = 163;
 
    mpz_class curveA("1", 16);
    mpz_class curveB("1", 16);
    Curve curve(curveA.get_mpz_t(), curveB.get_mpz_t(), m, fz.get_mpz_t());
 
    Point G("2FE13C0537BBC11ACAA07D793DE4E6D5E5C94EEE8",
            "289070FB05D38FF58321F2E800536D538CCDAA3D9");
    if (!curve.isPointOnCurve(G)) {
        printf("ERROR: base point not on curve");
    }
 
    ap_uint<163> aHW = mpzc_to_ap(curveA);
 
    gmp_randstate_t st; gmp_randinit_mt(st); gmp_randseed_ui(st, 4242);
    mpz_class order; mpz_set_str(order.get_mpz_t(),
        "4000000000000000000020108A2E0CC0D99F8A5EF", 16);
 
    int nTests = 200, errAdd = 0, errDbl = 0;
    for (int t = 0; t < nTests; t++) {
        mpz_class k1, k2;
        mpz_urandomm(k1.get_mpz_t(), st, order.get_mpz_t());
        mpz_urandomm(k2.get_mpz_t(), st, order.get_mpz_t());
        if (k1 == 0) k1 = 1;
        if (k2 == 0) k2 = 1;
 
        Point P = curve.pointMultiplication(G, k1.get_mpz_t());
        Point Q = curve.pointMultiplication(G, k2.get_mpz_t());
 
        // software reference
        Point sAdd = curve.pointAddition(P, Q);
        Point sDbl = curve.pointDoubling(P);
 
        // hardware
        ap_uint<163> rx, ry; bool rinf;
        point_add_163(mpz_to_ap(P.getX()), mpz_to_ap(P.getY()), false,
                      mpz_to_ap(Q.getX()), mpz_to_ap(Q.getY()), false,
                      aHW, rx, ry, rinf);
        // compare add
        if (!(rinf) ) {
            if (rx != mpz_to_ap(sAdd.getX()) || ry != mpz_to_ap(sAdd.getY()))
                errAdd++;
        }
 
        ap_uint<163> dx, dy; bool dinf;
        point_double_163(mpz_to_ap(P.getX()), mpz_to_ap(P.getY()), false,
                         aHW, dx, dy, dinf);
        if (!(dinf)) {
            if (dx != mpz_to_ap(sDbl.getX()) || dy != mpz_to_ap(sDbl.getY()))
                errDbl++;
        }
    }
    gmp_randclear(st);
 
    printf("point_add : %d/%d mismatches -> %s\n", errAdd, nTests, errAdd?"FAIL":"PASS");
    printf("point_double: %d/%d mismatches -> %s\n", errDbl, nTests, errDbl?"FAIL":"PASS");
    int e = errAdd + errDbl;
    printf("TOTAL = %d -> %s\n", e, e?"FAIL":"PASS");
    return e ? 1 : 0;
}
