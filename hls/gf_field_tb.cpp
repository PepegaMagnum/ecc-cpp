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
 
static void ref_mult(mpz_t out, mpz_t a, mpz_t b, mpz_t fz, uint32_t m) {
    binMult(a, b, out, m);
    binReduc(out, fz, m);
}

static void ref_square(mpz_t out, mpz_t a, mpz_t fz, uint32_t m) {
    binSquare(out, a);
    binReduc(out, fz, m);
}
 
static int test_gf7() {
    printf("GF(2^7)\n");
    mpz_t fz, ma, mb, ref;
    mpz_inits(fz, ma, mb, ref, NULL);
    mpz_set_ui(fz, 0x83);
 
    int emul = 0, esqr = 0;
    for (uint32_t a = 0; a < 128; a++) {
        // square
        ap_uint<7> hs = gf27_square((ap_uint<7>)a);
        mpz_set_ui(ma, a);
        ref_square(ref, ma, fz, 7);
        if ((uint32_t)hs != (uint32_t)mpz_get_ui(ref)) esqr++;
 
        // multiply (exhaustive inner loop)
        for (uint32_t b = 0; b < 128; b++) {
            ap_uint<7> hm = gf27_mult((ap_uint<7>)a, (ap_uint<7>)b);
            mpz_set_ui(ma, a); mpz_set_ui(mb, b);
            ref_mult(ref, ma, mb, fz, 7);
            if ((uint32_t)hm != (uint32_t)mpz_get_ui(ref)) emul++;
        }
    }
    mpz_clears(fz, ma, mb, ref, NULL);
    printf("  mult: %d mismatches -> %s\n", emul, emul ? "FAIL" : "PASS");
    printf("  sqr : %d mismatches -> %s\n", esqr, esqr ? "FAIL" : "PASS");
    return emul + esqr;
}
 
static int test_gf163(int n) {
    printf("GF(2^163)\n", n);
    mpz_t fz, ma, mb, ref, bound;
    mpz_inits(fz, ma, mb, ref, bound, NULL);
    mpz_set_str(fz, "800000000000000000000000000000000000000C9", 16);
    mpz_ui_pow_ui(bound, 2, 163);
 
    gmp_randstate_t st; gmp_randinit_mt(st); gmp_randseed_ui(st, 12345);
 
    int emul = 0, esqr = 0;
    for (int t = 0; t < n; t++) {
        mpz_urandomm(ma, st, bound);
        mpz_urandomm(mb, st, bound);
 
        // square
        ap_uint<163> hs = gf163_square(mpz_to_ap<163>(ma));
        ref_square(ref, ma, fz, 163);
        if (hs != mpz_to_ap<163>(ref)) esqr++;
 
        // multiply
        ap_uint<163> hm = gf163_mult(mpz_to_ap<163>(ma), mpz_to_ap<163>(mb));
        ref_mult(ref, ma, mb, fz, 163);
        if (hm != mpz_to_ap<163>(ref)) emul++;
    }
    mpz_clears(fz, ma, mb, ref, bound, NULL);
    gmp_randclear(st);
    printf("  mult: %d mismatches -> %s\n", emul, emul ? "FAIL" : "PASS");
    printf("  sqr : %d mismatches -> %s\n", esqr, esqr ? "FAIL" : "PASS");
    return emul + esqr;
}
 
int main() {
    int e = 0;
    e += test_gf7();
    e += test_gf163(3000);
    printf("\nTOTAL mismatches = %d -> %s\n", e, e ? "FAIL" : "PASS");
    return e ? 1 : 0;
}
