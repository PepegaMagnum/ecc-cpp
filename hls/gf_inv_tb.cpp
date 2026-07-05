#include <ap_int.h>
#include <gmp.h>
#include <cstdio>
#include <cstdint>
#include "gf_field.hpp"
#include "math_operations.h"

template <int W>
static ap_uint<W> mpz_to_ap(const mpz_t v) {
    ap_uint<W> r = 0;
    for (int i = 0; i < W; i++) if (mpz_tstbit(v, i)) r[i] = 1;
    return r;
}
template <int W>
static void ap_to_mpz(mpz_t out, ap_uint<W> v) {
    mpz_set_ui(out, 0);
    for (int i = 0; i < W; i++) if (v[i]) mpz_setbit(out, i);
}

int main() {
    printf("GF(2^163) inversion (Itoh-Tsujii)");

    mpz_t fz, ma, ref, bound, one;
    mpz_inits(fz, ma, ref, bound, one, NULL);
    mpz_set_str(fz, "800000000000000000000000000000000000000C9", 16);
    mpz_ui_pow_ui(bound, 2, 163);
    mpz_set_ui(one, 1);

    gmp_randstate_t st; gmp_randinit_mt(st); gmp_randseed_ui(st, 777);

    int nSamples = 2000;
    int err_vs_binInv = 0;   
    int err_identity  = 0;

    for (int t = 0; t < nSamples; t++) {
        mpz_urandomm(ma, st, bound);
        if (mpz_sgn(ma) == 0) { mpz_set_ui(ma, 1); }   // avoid 0

        ap_uint<163> ha  = mpz_to_ap<163>(ma);
        ap_uint<163> hinv = gf163_inv(ha);

        mpz_set(ref, ma);
        binInv(ref, fz, 163);
        ap_uint<163> sinv = mpz_to_ap<163>(ref);

        if (hinv != sinv) {
            if (err_vs_binInv < 5)
                gmp_printf("  MISMATCH vs binInv: a=0x%Zx\n", ma);
            err_vs_binInv++;
        }

        ap_uint<163> prod = gf163_mult(ha, hinv);
        if (prod != (ap_uint<163>)1) {
            if (err_identity < 5)
                gmp_printf("  IDENTITY FAIL: a=0x%Zx\n", ma);
            err_identity++;
        }
    }

    mpz_clears(fz, ma, ref, bound, one, NULL);
    gmp_randclear(st);

    printf("  vs binInv : %d mismatches -> %s\n",
           err_vs_binInv, err_vs_binInv ? "FAIL" : "PASS");
    printf("  a*inv==1  : %d failures  -> %s\n",
           err_identity, err_identity ? "FAIL" : "PASS");

    int e = err_vs_binInv + err_identity;
    printf("\nTOTAL = %d -> %s\n", e, e ? "FAIL" : "PASS");
    return e ? 1 : 0;
}
