#include <ap_int.h>
#include <gmp.h>
#include <cstdio>
#include <cstdint>
#include "gf27_mult.hpp"
#include "math_operations.h"

ap_uint<163> gf163_mult(ap_uint<163> a, ap_uint<163> b);

template <int W>
static ap_uint<W> mpz_to_ap(const mpz_t v) {
    ap_uint<W> r = 0;
    for (int i = 0; i < W; i++) {
        if (mpz_tstbit(v, i)) r[i] = 1;
    }
    return r;
}

static void ref_gf_mult(mpz_t out, mpz_t a, mpz_t b, mpz_t fz, uint32_t m) {
    binMult(a, b, out, m);
    binReduc(out, fz, m);
}

// ===========================================================================
static int test_gf7() {
    printf("Testing GF(2^7)\n");
    mpz_t fz, ma, mb, ref;
    mpz_inits(fz, ma, mb, ref, NULL);
    mpz_set_ui(fz, 0x83);          // x^7 + x + 1

    int errors = 0;
    for (uint32_t a = 0; a < 128; a++) {
        for (uint32_t b = 0; b < 128; b++) {
            ap_uint<7> hw = gf27_mult((ap_uint<7>)a, (ap_uint<7>)b);

            mpz_set_ui(ma, a);
            mpz_set_ui(mb, b);
            ref_gf_mult(ref, ma, mb, fz, 7);
            uint32_t sw = (uint32_t)mpz_get_ui(ref);

            if ((uint32_t)hw != sw) {
                if (errors < 10)
                    printf("  MISMATCH a=%u b=%u hw=%u sw=%u\n",
                           a, b, (uint32_t)hw, sw);
                errors++;
            }
        }
    }
    mpz_clears(fz, ma, mb, ref, NULL);
    printf("  16384 pairs, mismatches = %d -> %s\n",
           errors, errors ? "FAIL" : "PASS");
    return errors;
}

static int test_gf163(int nSamples) {
    printf("testing GF(2^163)\n",
           nSamples);
    mpz_t fz, ma, mb, ref, bound;
    mpz_inits(fz, ma, mb, ref, bound, NULL);
    // K-163 polynomial: x^163 + x^7 + x^6 + x^3 + 1
    mpz_set_str(fz, "800000000000000000000000000000000000000C9", 16);
    mpz_ui_pow_ui(bound, 2, 163);

    gmp_randstate_t st;
    gmp_randinit_mt(st);
    gmp_randseed_ui(st, 12345);    // fixed seed: reproducible

    int errors = 0;
    for (int t = 0; t < nSamples; t++) {
        mpz_urandomm(ma, st, bound);
        mpz_urandomm(mb, st, bound);

        ap_uint<163> ha = mpz_to_ap<163>(ma);
        ap_uint<163> hb = mpz_to_ap<163>(mb);
        ap_uint<163> hw = gf163_mult(ha, hb);

        ref_gf_mult(ref, ma, mb, fz, 163);
        ap_uint<163> sw = mpz_to_ap<163>(ref);

        if (hw != sw) {
            if (errors < 10)
                gmp_printf("  MISMATCH a=0x%Zx b=0x%Zx\n", ma, mb);
            errors++;
        }
    }
    mpz_clears(fz, ma, mb, ref, bound, NULL);
    gmp_randclear(st);
    printf("  %d samples, mismatches = %d -> %s\n",
           nSamples, errors, errors ? "FAIL" : "PASS");
    return errors;
}

int main() {
    int e = 0;
    e += test_gf7();
    e += test_gf163(5000);
    printf("\nTOTAL mismatches = %d -> %s\n", e, e ? "FAIL" : "PASS");
    return e ? 1 : 0;
}
